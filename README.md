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

**Not keeping the toolkit inside your push_swap project?** Edit the
`PUSH_SWAP_PATH` line at the top of the `Makefile` and point it at your
push_swap folder. (If the toolkit *is* inside the project, set it to `..`.)

`make graph` also needs Python 3 with a few packages (pandas, numpy,
matplotlib):

```
pip install -r requirements.txt
```

## What's inside

| Folder | What it does |
|---|---|
| [`ps_input/`](ps_input/README.md) | Generates the complete input for push_swap, with or without flags: a number list with a target disorder and size. Numbers can be random, from INT_MIN to INT_MAX (value), or start always from 0 (rank) |
| [`ps_check/`](ps_check/README.md) | Checks if push_swap's output actually sorts the stack. It needs the same input as argv and takes the operation list from stdout, so executing your push_swap is a must |
| [`ps_graph/`](ps_graph/README.md) | Executes a comprehensive sweep (runs push_swap 5 times for each size x disorder combination). Outputs a .txt with all the data. From that, some py scripts create graphs to compare your algorithms, calculate the empirical big O, and audit if your adaptive mode chooses the best algorithm for each case. |
| [`apps/`](apps/README.md) | 2 browser tools: 1 - a simulator, to help you mentally test your code ideas, and 2 - a sort visualizer to watch your actual code sorting the stack, as fast or slow as you want |

`ps_input` and `ps_check` are made by me. The apps and ps_graph are still new territory for me, so I got help from Claude as a guide.  

`ps_input` and `ps_check` link against my own committed `push_swap.a`/
`libft.a` (in their `lib/` folders) — that's fixed and the same for
everyone who clones this repo, never swapped for yours. 

| Command | Does |
|---|---|
| `make` | builds everything |
| `make graph` | runs the benchmark + makes the graphs |
| `make clean` / `fclean` | removes build files |
| `make re` | clean rebuild |

Your push_swap must support `--adaptive --simple --medium --complex`.
