# ps_graph

Benchmark sweep + plots. Runs your `push_swap` across a grid of
(size, disorder, algorithm), averages op count and wall time over several
samples per point, and produces plots analysing how it scales and which
algorithm wins where. Unlike `ps_input`/`ps_check`, this is tooling/analysis
rather than learning code — it's fine for this to be fully AI-built.

## Build

```
make PUSH_SWAP=/path/to/your/push_swap PS_GEN=/path/to/ps_input
```

or, from the toolkit root: `make graph`, which fills in both paths
automatically (`../push_swap` and `ps_input/ps_input`). This templates
`ps_graph.tpl` → `ps_graph` via `sed` — edit `ps_graph.tpl`, not the
generated `ps_graph` script directly.

## Run

```
./ps_graph [output_file]      # default: ps_graph_data.txt
python3 ps_plot.py
python3 ps_bigo.py
python3 ps_adapt.py
python3 ps_audit.py
```

or `make run-graph` from the toolkit root, which does `make all` then all
of the above in one shot. The sweep itself can take a while depending on
`config.py`'s grid (selection sort at large n is slow — see `ps_input`'s
README for the size-limit rationale).

## Sweep grid

Single source of truth: `../config.py` (`SIZES`, `DISORDERS`, `ALGORITHMS`,
`GRAPH_RUNS`), read via `../gen_sweep_vars.py`, which prints it as bash
array assignments that `ps_graph.tpl` `eval`s. Edit `config.py`, re-run
`make graph` to retemplate, then `./ps_graph` — no need to touch the bash
script itself.

## Sweep mechanics

For each `(size, disorder, algorithm)` point, `GRAPH_RUNS` times:

1. Run `ps_input <size> <disorder> <algo_level> n r` (no bench tag, rank
   output) to get a permutation with that exact disorder.
2. Strip the leading `./push_swap`/flags from `ps_input`'s last output line
   with `sed`, leaving just the numbers.
3. Run the real `push_swap --<algo_name>` on those numbers — **plain, no
   `--bench`**. Op count comes from counting lines of push_swap's actual
   printed op list (`grep -c '^'`), not from a `--bench` summary: that flag
   makes push_swap print stats to *stderr* instead of the op list, which is
   the wrong shape for this sweep (an earlier version of this script passed
   `--bench` and then discarded stderr, which silently zeroed every point).
4. Wall time is measured around that same call with `date +%s%N`.

A nonzero exit status from push_swap counts the run as a failure and skips
it. An empty op list with exit 0 (already-sorted input, or n=1) legitimately
counts as 0 ops — that's not a failure.

Results are averaged over the `GRAPH_RUNS` samples and written to
`ps_graph_data.txt` (`size  disorder  algorithm  ops_avg  time_ms_avg`,
tab-separated).

## Plots

All read `ps_graph_data.txt`, written by `./ps_graph`.

- **`ps_plot.py` → `ps_plot.png`** — ops vs disorder (per size) with
  simple/complex crossover points marked, ops vs size (at disorder=0.5)
  with its own crossover annotation, three rows of 42-subject
  challenge-target bands (excellent/good/pass/fail, official at n=100/500,
  extrapolated at n=10/50), and runtime vs size.
- **`ps_bigo.py` → `ps_bigo.png` + `ps_bigo.txt`** — log-log fit per
  algorithm (disorder=0.5, n≥20), labels the empirical complexity class
  (O(n), O(n log n), O(n²), O(n³), or a raw exponent) with R².
- **`ps_adapt.py` → `ps_adapt.png`** — heatmap of which base algorithm
  (simple/medium/complex) has the lowest op count at each (size, disorder),
  plus a text report of winner zones by size and by disorder.
- **`ps_audit.py` → `ps_audit.png` + stdout report** — checks whether the
  `adaptive` dispatcher actually routes to the best *available* base
  algorithm at each point. Adaptive and the base algorithms are sampled on
  different random draws per point, so a 10% relative tolerance (and a
  10-op absolute floor) absorbs sampling noise; gaps beyond that are
  flagged red and labelled with the algorithm adaptive should have used.
  Also prints an empirical selection/radix threshold per size, derived from
  where simple's and complex's op counts cross over. Requires `adaptive` to
  be in `config.py`'s `ALGORITHMS` (exits early otherwise).
