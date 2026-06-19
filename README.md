# push_swap_toolkit

Tools for testing and visualizing your push_swap project.

## Setup

Clone this **inside your push_swap folder**:

```
your_push_swap/
├── push_swap.h, Makefile, *.c, ...
└── push_swap_toolkit/   <- clone it here
```

Then:

```
cd push_swap_toolkit
make
```

That's it. One `make` builds your push_swap project and every tool.

(Keeping the toolkit somewhere else? Add `PUSH_SWAP_PATH=/path/to/push_swap`
to any `make` command.)

## What's inside

| Folder | What it does |
|---|---|
| [`ps_input/`](ps_input/README.md) | Generates a random stack with a chosen size and disorder level |
| [`ps_check/`](ps_check/README.md) | Checks if push_swap's output actually sorts the stack |
| [`ps_graph/`](ps_graph/README.md) | Benchmarks push_swap and draws graphs |
| [`apps/`](apps/README.md) | Browser tools to watch push_swap sort, step by step |

`ps_input` and `ps_check` are your own code — same as push_swap.c. The rest
(`ps_graph`, the build setup) is just tooling.

## Commands

| Command | Does |
|---|---|
| `make` | builds everything |
| `make run-graph` | runs the benchmark + makes the graphs |
| `make clean` / `fclean` | removes build files |
| `make re` | clean rebuild |

Your push_swap must support `--adaptive --simple --medium --complex`.
