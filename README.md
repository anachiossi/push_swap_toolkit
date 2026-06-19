# push_swap_toolkit

Tool kit for the 2026 version of the 42 school project push_swap.

Meant to be cloned **inside your push_swap project root**, so `../` resolves
to your push_swap project (its `push_swap.h`, `libft.a`, and Makefile). This
matters because every student's `push_swap.h`/ABI differs — the toolkit
rebuilds your push_swap project itself rather than vendoring a fixed copy.

```
your_push_swap_project/
├── push_swap.h, Makefile, *.c, ...   (your project)
└── push_swap_toolkit/                (this repo, cloned here)
```

If you'd rather keep it elsewhere, every `make` target accepts
`PUSH_SWAP_PATH=/path/to/your/push_swap` (default `..`).

## Quick start

```
cd push_swap_toolkit
make
```

One `make` from the toolkit root builds everything: your push_swap project
(binary + `push_swap.a`), `ps_input`, `ps_check`, and templates `ps_graph`.

## Pieces

- **[`ps_input/`](ps_input/README.md)** — input generator. Given a size and
  a target disorder level (0.0 = sorted, 1.0 = fully reversed), produces a
  permutation with that exact disorder, interactively or in scripted auto
  mode. Self-contained, no dependency on your push_swap project tree.
- **[`ps_check/`](ps_check/README.md)** — correctness checker. A port of
  your own bonus checker, linked against your actual compiled
  `push_swap.a` + `libft.a` instead of recompiling the stack/operations
  logic. Checks one input against one op list at a time
  (`./push_swap --simple 3 1 2 5 4 | ./ps_check 3 1 2 5 4` → `OK`/`KO`).
- **[`ps_graph/`](ps_graph/README.md)** — benchmark sweep + plots. Runs a
  grid of (size, disorder, algorithm), averages op count and wall time, and
  produces `ps_plot.png` (op counts vs size/disorder against the 42
  challenge targets), `ps_bigo.png` (empirical Big O fit), `ps_adapt.png`
  (which algorithm wins where), and `ps_audit.png` (whether the adaptive
  dispatcher actually picks the winner).
- **`config.py`** — single source of truth for `ps_graph`'s sweep grid
  (sizes, disorder levels, algorithms, run count).
- **[`apps/`](apps/README.md)** — two browser tools: a manual click-through
  stack simulator, and a `ps_input`-fed visualizer that replays push_swap's
  op list move-by-move (play/step/scrub/zoom, plus a side-by-side
  algorithm-comparison mode).

`ps_input` and `ps_check`'s actual logic (`src/*.c` in each) is the
student's own code, on the same footing as push_swap.c itself — not
something this toolkit's tooling writes for you. `ps_graph`'s sweep/plot
scripts and the Makefile wiring across all three are the tooling layer.

## Top-level `make` targets

| Target | Does |
|---|---|
| `all` (default) | `push_swap input check graph` — builds every executable |
| `push_swap` | runs `make` and `make push_swap.a` in `$(PUSH_SWAP_PATH)` |
| `input` | builds `ps_input` |
| `check` | refreshes `ps_check/lib/`+`ps_check/inc/push_swap.h` from `$(PUSH_SWAP_PATH)`, then builds `ps_check` |
| `graph` | templates `ps_graph/ps_graph` against `$(PUSH_SWAP_PATH)/push_swap` and `ps_input/ps_input` |
| `run-graph` | `all`, then runs the sweep + all four plotting scripts |
| `clean` / `fclean` / `re` | delegate to each subfolder's own Makefile |

## Editing the ps_graph sweep grid

Edit `config.py` (`SIZES`, `DISORDERS`, `ALGORITHMS`, `GRAPH_RUNS`), then
re-run `make graph` (retemplates `ps_graph.tpl` → `ps_graph`, no need to
edit the bash script) and `./ps_graph/ps_graph` (or `make run-graph` to redo
everything in one step).

Your `push_swap` must support the level flags `--adaptive --simple --medium
--complex` (see your project's `main.c`/`parse.c`); the level passed to
`ps_input` just picks which flag to pass to push_swap, it doesn't change
the generated permutation itself.
