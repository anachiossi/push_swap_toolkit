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
| [`ps_input/`](ps_input/README.md) | Generates the complete input for push_swap with or without flags, numbers list with target disorder, size. Numbers can be random INT_MIN to INT_MAX (value) or start always from 0 (rank) |
| [`ps_check/`](ps_check/README.md) | Checks if push_swap's output actually sorts the stack. It needs the same input as argv and take the operation list from stdout, so executing your push_swap is a must |
| [`ps_graph/`](ps_graph/README.md) | Execute a compreensive sweep (run push_swap 5x times for each set of size x disorder). Outputs a .txt with all the data. From that some py scripts create graphs to compare your algorithms, calculates empiric big O, audit if your adaptive mode chooses the best algorithm, and a heatmap of which algorithm wins at each case (size, disorder).
| [`apps/`](apps/README.md) | 2 Browser tools: 1 - a simulator, to help you test mentally your code ideas and 2 - sort visualizer to watch your actual code sorting the stack, as fast or slow as you want |

`ps_input` and `ps_check` are made by me, the apps and graph are still new territory for me so I got helped by claude guided.  

## Commands

| Command | Does |
|---|---|
| `make` | builds everything |
| `make run graph` | runs the benchmark + makes the graphs |
| `make clean` / `fclean` | removes build files |
| `make re` | clean rebuild |

Your push_swap must support `--adaptive --simple --medium --complex`.
