# push_swap_toolkit

Tool kit for the 2026 version of the 42 school project push_swap.

Works against **your own** compiled `push_swap` binary — nothing here
bundles, reuses, or replaces your project's source.

## Two tiers

**Terminal tools (yours to write):**

- **`ps_input/`** (formerly `ps_gen`) — input generator. Given a size and a
  target disorder level (0.0 = sorted, 1.0 = fully reversed), produces a
  permutation with that exact disorder. Self-contained, no dependency on a
  push_swap project tree. Already built.
- **`ps_check/`** — correctness sweep. Runs your `push_swap` across a grid
  of (size, disorder, algorithm) and independently replays each printed op
  list to verify the final stack is sorted and `b` is empty. Currently a
  scaffold (struct layout + signatures + TODOs in `src/`) — see
  `ps_check/README.md` for what each function needs to do and why it must
  stay independent of your bonus checker.

**Generated analysis (built already):**

- **`ps_graph/`** — benchmark sweep + plots. Runs the same grid, averages
  op count and wall time over several samples per point, and produces
  `ps_plot.png` (ops vs size/disorder, 42 challenge targets), `ps_bigo.png`
  (power-law fit / Big O), and `ps_adapt.png` (which algorithm wins where).
- **`config.py`** — single source of truth for `ps_graph`'s sweep grid
  (sizes, disorder levels, algorithms, run count). `ps_check` keeps its own
  copy of the grid in C (see `ps_check/src/sweep.c`) — sync by hand if you
  want identical coverage.
- The simulator and visualizer apps live separately, in the local project
  (`~/42/cursus/push_swap/apps/`) — not part of this shareable toolkit.

## Setup

```
git clone git@github.com:anachiossi/push_swap_toolkit.git
cd push_swap_toolkit
make input          # builds ps_input
```

## Run

```
make check PUSH_SWAP=/path/to/your/push_swap   # correctness sweep only (once ps_check is implemented)
make graph PUSH_SWAP=/path/to/your/push_swap   # check, then benchmark + plot
```

`make graph` always runs `check` first and stops if any combination
produces a wrong result — trust the graphs only once the sweep is clean.

Your `push_swap` must support the level flags `--adaptive --simple --medium
--complex` (see your project's `main.c`/`parse.c`); the level passed to
`ps_input` just picks which flag to pass to push_swap, it doesn't change
the generated permutation itself.

## Editing the ps_graph sweep grid

Edit `config.py` (`SIZES`, `DISORDERS`, `ALGORITHMS`, `GRAPH_RUNS`), then
re-run `make graph`. `ps_graph.tpl` picks the grid up automatically via
`gen_sweep_vars.py` — no need to edit the bash script.
