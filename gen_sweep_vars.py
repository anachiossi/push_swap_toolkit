#!/usr/bin/env python3
"""Prints config.py's sweep grid as bash array assignments, so ps_graph.tpl
can `eval` it instead of keeping a second, hand-maintained copy."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from config import ALGORITHMS, DISORDERS, GRAPH_RUNS, SIZES

print(f"SIZES=({' '.join(str(s) for s in SIZES)})")
print(f"DISORDERS=({' '.join(str(d) for d in DISORDERS)})")
print(f"ALGORITHMS=({' '.join(f'{name}:{level}' for name, level in ALGORITHMS.items())})")
print(f"RUNS={GRAPH_RUNS}")
