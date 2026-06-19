# ps_check

Correctness sweep for a push_swap binary — **yours to implement**, same as
push_swap itself. This is a scaffold: signatures, struct layout, and TODOs
in `src/`, no working logic.

## Why it has to be independent

Your project's own bonus checker (`bonus/checker_bonus.c`) is part of the
graded bonus and may share assumptions — or bugs — with the rest of your
push_swap. If `ps_check` reused it (or your `operations_*.c` files) to
perform the moves, a bug in those files would be invisible to the checker
too. `ps_check` is only independent if its move logic is a fresh
implementation built from the subject's definitions, not from your code.
It is allowed (and expected) to:

- run your compiled `push_swap` as a subprocess and read its stdout — that
  part is unavoidable, it's what's being tested;
- run `ps_input` the same way, to generate inputs.

It should not:

- `#include` or link against any of push_swap's own `.c` files;
- reuse your bonus checker's logic.

## What to implement

See the `TODO` comments in `src/`:

- **`args.c`** — parse `--push-swap PATH --ps-input PATH`.
- **`proc.c`** — `run_capture`: fork/exec a child, capture its stdout as a
  string.
- **`replay.c`** — `apply_move`/`apply_op`/`is_sorted`: the 9 push_swap
  operations against a `t_state`, and the sorted/empty-`b` check.
- **`sweep.c`** — `run_one_check`: generate → run → replay → verify one
  (size, disorder, algorithm) point. `run_sweep`: iterate the whole grid.

## Build & run

```
make
./ps_check --push-swap /path/to/push_swap --ps-input ../ps_input/ps_input
```

Exit code 0 = all checks passed, non-zero = at least one failure (the
top-level `Makefile`'s `make graph` depends on this to stop before
trusting any benchmark graphs).
