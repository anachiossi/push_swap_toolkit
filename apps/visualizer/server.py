#!/usr/bin/env python3
"""Local-only server: serves the visualizer frontend and runs ps_input + push_swap
to produce an initial array (in rank form) and the resulting op list as JSON.
Never builds shell strings — all subprocess calls use argv lists."""

import json
import os
import re
import subprocess
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse

APP_DIR = Path(__file__).resolve().parent
TOOLKIT_DIR = APP_DIR.parent.parent
# Default assumes the toolkit is cloned inside your push_swap project root
# (same convention as the toolkit's own Makefile PUSH_SWAP_PATH); override
# with the PUSH_SWAP_PATH env var if you keep it elsewhere.
PUSH_SWAP_PATH = Path(os.environ.get("PUSH_SWAP_PATH", TOOLKIT_DIR.parent)).resolve()
PS_GEN = TOOLKIT_DIR / "ps_input" / "ps_input"
PUSH_SWAP = PUSH_SWAP_PATH / "push_swap"

LEVELS = {"adaptive": 0, "simple": 1, "medium": 2, "complex": 3}


def validate(size: int, disorder: float, level: str | None = None):
	if not (1 <= size <= 500):
		raise ValueError("size must be 1-500")
	if not (0.0 <= disorder <= 1.0):
		raise ValueError("disorder must be 0.0-1.0")
	if level is not None and level not in LEVELS:
		raise ValueError("level must be one of " + ", ".join(LEVELS))


def generate_values(size: int, disorder: float):
	# level only affects which --flag ps_input suggests in its output text, not
	# the generated permutation itself, so any level works here.
	# auto-mode args: <size> <disorder> <level> <bench:y/n> <use_rank:r/v>
	gen = subprocess.run(
		[str(PS_GEN), str(size), f"{disorder:.4f}", "0", "n", "r"],
		capture_output=True,
		text=True,
		check=True,
		cwd=TOOLKIT_DIR,
	)
	line = gen.stdout.strip().splitlines()[-1]
	values = [int(tok) for tok in re.findall(r"-?\d+", line)]
	if len(values) != size:
		raise RuntimeError(f"ps_input produced {len(values)} values, expected {size}")
	return values


def run_push_swap(values: list[int], level: str):
	ps = subprocess.run(
		[str(PUSH_SWAP), f"--{level}"] + [str(v) for v in values],
		capture_output=True,
		text=True,
		check=True,
		cwd=PUSH_SWAP_PATH,
	)
	return [line for line in ps.stdout.splitlines() if line]


def run_pipeline(size: int, disorder: float, level: str):
	validate(size, disorder, level)
	values = generate_values(size, disorder)
	ops = run_push_swap(values, level)
	return values, ops


def run_comparison(size: int, disorder: float):
	validate(size, disorder)
	values = generate_values(size, disorder)
	results = {level: run_push_swap(values, level) for level in LEVELS}
	return values, results


class Handler(BaseHTTPRequestHandler):
	def _send_json(self, status, payload):
		body = json.dumps(payload).encode()
		self.send_response(status)
		self.send_header("Content-Type", "application/json")
		self.send_header("Content-Length", str(len(body)))
		self.end_headers()
		self.wfile.write(body)

	def do_POST(self):
		path = urlparse(self.path).path
		length = int(self.headers.get("Content-Length", 0))
		try:
			req = json.loads(self.rfile.read(length) or b"{}")
			if path == "/api/generate":
				values, ops = run_pipeline(
					int(req["size"]), float(req["disorder"]), str(req.get("level", "adaptive"))
				)
				self._send_json(200, {"initial": values, "ops": ops})
			elif path == "/api/compare":
				values, results = run_comparison(int(req["size"]), float(req["disorder"]))
				self._send_json(200, {"initial": values, "results": results})
			else:
				self._send_json(404, {"error": "not found"})
		except subprocess.CalledProcessError as e:
			self._send_json(500, {"error": e.stderr or str(e)})
		except (ValueError, KeyError, RuntimeError) as e:
			self._send_json(400, {"error": str(e)})

	def do_GET(self):
		path = urlparse(self.path).path
		if path == "/":
			path = "/index.html"
		target = (APP_DIR / path.lstrip("/")).resolve()
		shared = (APP_DIR.parent / "shared").resolve()
		if not (str(target).startswith(str(APP_DIR)) or str(target).startswith(str(shared))):
			self._send_json(403, {"error": "forbidden"})
			return
		if path.startswith("/shared/"):
			target = (APP_DIR.parent / path.lstrip("/")).resolve()
		if not target.is_file():
			self._send_json(404, {"error": "not found"})
			return
		ctype = {
			".html": "text/html",
			".js": "application/javascript",
			".css": "text/css",
		}.get(target.suffix, "application/octet-stream")
		body = target.read_bytes()
		self.send_response(200)
		self.send_header("Content-Type", ctype)
		self.send_header("Content-Length", str(len(body)))
		self.end_headers()
		self.wfile.write(body)

	def log_message(self, fmt, *args):
		pass


if __name__ == "__main__":
	if not PS_GEN.exists():
		raise SystemExit(f"ps_input not found at {PS_GEN}, run `make input` in the toolkit root first")
	if not PUSH_SWAP.exists():
		raise SystemExit(f"push_swap not found at {PUSH_SWAP}, run `make push_swap` in the toolkit root "
			"first (or set PUSH_SWAP_PATH if your push_swap project isn't at ../)")
	port = 8910
	print(f"visualizer at http://localhost:{port}/")
	ThreadingHTTPServer(("localhost", port), Handler).serve_forever()
