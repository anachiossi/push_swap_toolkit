(function () {
	let snapshots = [];
	let n = 0;
	let cursor = 0;
	let playing = false;
	let rafId = null;
	let lastFrameTime = 0;
	let cellSize = 26;

	const elA = document.getElementById("stack-a");
	const elB = document.getElementById("stack-b");
	const scrub = document.getElementById("scrub");
	const counter = document.getElementById("counter");
	const currentOp = document.getElementById("current-op");
	const meta = document.getElementById("meta");
	const playBtn = document.getElementById("play-pause");
	const speed = document.getElementById("speed");
	const speedLabel = document.getElementById("speed-label");
	const zoom = document.getElementById("zoom");
	const opsLine = document.getElementById("ops-line");

	function formatRate(mps) {
		if (mps >= 1000) return `${Math.round(mps / 1000)}k moves/s`;
		return `${Math.round(mps)} moves/s`;
	}

	function updateSpeedLabel() {
		speedLabel.textContent = formatRate(movesPerSecond());
	}

	function rankColor(value, n) {
		// value is a rank 0..n-1 -> hue sweep, low = cool, high = warm
		const hue = n > 1 ? (value / (n - 1)) * 280 : 0;
		return `hsl(${hue}, 65%, 60%)`;
	}

	// browsers kill the canvas (context loss, renders blank) if the backing
	// store gets too large — cap actual pixel dimensions and let CSS stretch
	// the display size instead of growing the buffer without bound.
	const MAX_CANVAS_DIM = 16384;

	function drawStack(canvas, arr) {
		const dpr = window.devicePixelRatio || 1;
		const w = Math.max(arr.length * cellSize, 1);
		const h = cellSize;
		canvas.style.width = w + "px";
		canvas.style.height = h + "px";
		const scale = Math.min(dpr, MAX_CANVAS_DIM / w, MAX_CANVAS_DIM / h);
		canvas.width = Math.max(Math.floor(w * scale), 1);
		canvas.height = Math.max(Math.floor(h * scale), 1);
		const ctx = canvas.getContext("2d");
		ctx.setTransform(scale, 0, 0, scale, 0, 0);
		ctx.clearRect(0, 0, w, h);
		const gap = Math.min(2, cellSize * 0.1);
		// shrink the font so the widest label (n-1) still fits inside one
		// cell — otherwise multi-digit numbers (n=100, n=500) overflow into
		// neighboring cells and look mangled.
		const maxDigits = Math.max(String(Math.max(n - 1, 0)).length, 1);
		const fontSize = Math.min(cellSize * 0.42, ((cellSize - gap) * 0.82) / (maxDigits * 0.62));
		ctx.font = `600 ${fontSize}px ui-monospace, monospace`;
		ctx.textAlign = "center";
		ctx.textBaseline = "middle";
		const showText = cellSize >= 12 && fontSize >= 6;
		arr.forEach((value, i) => {
			const x = i * cellSize;
			ctx.fillStyle = rankColor(value, n);
			ctx.fillRect(x, 0, cellSize - gap, cellSize - gap);
			if (i === 0) {
				ctx.strokeStyle = canvas.id === "stack-b" ? "#c08fe0" : "#5fd0a0";
				ctx.lineWidth = Math.min(2, cellSize * 0.15);
				ctx.strokeRect(x + 1, 1, cellSize - gap - 2, cellSize - gap - 2);
			}
			if (showText) {
				ctx.fillStyle = "#0c0c0e";
				ctx.fillText(String(value), x + (cellSize - gap) / 2, (cellSize - gap) / 2);
			}
		});
	}

	function renderAt(i) {
		cursor = i;
		const snap = snapshots[i];
		drawStack(elA, snap.state.a);
		drawStack(elB, snap.state.b);
		counter.textContent = `${i} / ${snapshots.length - 1}`;
		currentOp.textContent = snap.op ? snap.op : "";
		scrub.value = i;
	}

	function stop() {
		playing = false;
		playBtn.textContent = "▶ play";
		if (rafId) cancelAnimationFrame(rafId);
		rafId = null;
	}

	// speed slider (1..200) maps to moves/second on an exponential curve so
	// each step of the slider feels like a similar relative speed change,
	// from "watch every move" (~2/s) to "finish instantly" (~20000/s).
	const MIN_MPS = 2;
	const MAX_MPS = 20000;
	function movesPerSecond() {
		const v = Number(speed.value) / 200; // 0..1
		return MIN_MPS * Math.pow(MAX_MPS / MIN_MPS, v);
	}

	function play() {
		if (cursor >= snapshots.length - 1) return;
		playing = true;
		playBtn.textContent = "⏸ pause";
		lastFrameTime = performance.now();
		let carry = 0;
		const frame = (now) => {
			if (!playing) return;
			const dt = (now - lastFrameTime) / 1000;
			lastFrameTime = now;
			carry += dt * movesPerSecond();
			const steps = Math.floor(carry);
			if (steps > 0) {
				carry -= steps;
				const next = Math.min(cursor + steps, snapshots.length - 1);
				renderAt(next);
				if (next >= snapshots.length - 1) {
					stop();
					return;
				}
			}
			rafId = requestAnimationFrame(frame);
		};
		rafId = requestAnimationFrame(frame);
	}

	document.getElementById("gen-form").addEventListener("submit", async (e) => {
		e.preventDefault();
		stop();
		const size = Number(document.getElementById("f-size").value);
		const disorder = Number(document.getElementById("f-disorder").value);
		const level = document.getElementById("f-level").value;
		meta.textContent = "generating...";
		try {
			const res = await fetch("/api/generate", {
				method: "POST",
				headers: { "Content-Type": "application/json" },
				body: JSON.stringify({ size, disorder, level }),
			});
			const data = await res.json();
			if (!res.ok) throw new Error(data.error || "generation failed");
			n = size;
			snapshots = replay(data.initial, data.ops);
			scrub.max = snapshots.length - 1;
			meta.textContent = `n=${size}, disorder=${disorder}, level=${level} — ${data.ops.length} ops, ${snapshots.length - 1} physical moves`;
			opsLine.value = data.ops.join(" ");
			renderAt(0);
		} catch (err) {
			meta.textContent = "error: " + err.message;
		}
	});

	document.getElementById("compare-btn").addEventListener("click", async () => {
		const size = Number(document.getElementById("f-size").value);
		const disorder = Number(document.getElementById("f-disorder").value);
		const btn = document.getElementById("compare-btn");
		btn.disabled = true;
		btn.textContent = "comparing...";
		try {
			const res = await fetch("/api/compare", {
				method: "POST",
				headers: { "Content-Type": "application/json" },
				body: JSON.stringify({ size, disorder }),
			});
			const data = await res.json();
			if (!res.ok) throw new Error(data.error || "comparison failed");
			addCompareRun(size, disorder, data.results);
		} catch (err) {
			meta.textContent = "error: " + err.message;
		} finally {
			btn.disabled = false;
			btn.textContent = "Compare algorithms";
		}
	});

	function addCompareRun(size, disorder, results) {
		const container = document.getElementById("compare-results");
		const run = document.createElement("div");
		run.className = "compare-run";

		const meta = document.createElement("p");
		meta.className = "compare-run-meta";
		meta.textContent = `n=${size}, disorder=${disorder} — ${new Date().toLocaleTimeString()}`;
		run.appendChild(meta);

		const grid = document.createElement("div");
		grid.className = "compare-grid";
		const entries = Object.entries(results); // [algo, ops[]]
		const minTotal = Math.min(...entries.map(([, ops]) => ops.length));
		for (const [algo, ops] of entries) {
			const box = document.createElement("div");
			box.className = "compare-box" + (ops.length === minTotal ? " best" : "");
			const title = document.createElement("div");
			title.className = "compare-box-title";
			title.innerHTML = `<span class="algo">${algo}</span><span class="total">${ops.length} ops</span>`;
			const ta = document.createElement("textarea");
			ta.className = "muted-scroll";
			ta.readOnly = true;
			ta.rows = 1;
			ta.wrap = "off";
			ta.value = ops.join(" ");
			box.appendChild(title);
			box.appendChild(ta);
			grid.appendChild(box);
		}
		run.appendChild(grid);
		container.prepend(run);
	}

	document.getElementById("step-fwd").addEventListener("click", () => {
		stop();
		if (cursor < snapshots.length - 1) renderAt(cursor + 1);
	});

	document.getElementById("step-back").addEventListener("click", () => {
		stop();
		if (cursor > 0) renderAt(cursor - 1);
	});

	playBtn.addEventListener("click", () => {
		if (playing) stop();
		else play();
	});

	document.getElementById("replay-btn").addEventListener("click", () => {
		stop();
		if (snapshots.length) renderAt(0);
		play();
	});

	scrub.addEventListener("input", () => {
		stop();
		renderAt(Number(scrub.value));
	});

	zoom.addEventListener("input", () => {
		cellSize = Number(zoom.value);
		if (snapshots.length) renderAt(cursor);
	});

	speed.addEventListener("input", updateSpeedLabel);
	updateSpeedLabel();
})();
