#!/usr/bin/env python3
"""Reads the sweep settings from config.py and prints them as bash variables.
The ps_graph script runs this and uses the result, so the settings live in one
place (config.py) instead of being copied into two files you'd have to update
by hand."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from config import ALGORITHMS, DISORDERS, GRAPH_RUNS, SIZES

print(f"SIZES=({' '.join(str(s) for s in SIZES)})")
print(f"DISORDERS=({' '.join(str(d) for d in DISORDERS)})")
print(f"ALGORITHMS=({' '.join(f'{name}:{level}' for name, level in ALGORITHMS.items())})")
print(f"RUNS={GRAPH_RUNS}")
