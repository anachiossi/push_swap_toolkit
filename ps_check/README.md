# ps_check

A standalone port of the bonus checker from the push_swap project itself —
same checking logic, but linked against the *actual compiled* `push_swap.a`
(and `libft.a`) instead of recompiling `stack.c`/`operations_*.c` from
scratch. This is the student's own code, not a generic tool: `src/*.c` here
is the same checker design as `bonus/checker_bonus.c` etc. in the push_swap
project.

## Layout

```
inc/push_swap.h     -- copied from the push_swap project (declares t_stack,
                        t_ps_flags, s/p/r/r_r, is_sorted, disorder, ...)
inc/libft.h          -- copied from the push_swap project
inc/ps_check.h       -- this tool's own header: t_op_count, t_op dispatch
                        table, ckr_* wrappers, read_ops, print_result,
                        print_check_bench
lib/push_swap.a      -- compiled push_swap object files (everything except
                        main.c), copied from the push_swap project
lib/libft.a          -- copied from the push_swap project
src/checker_main_bonus.c    -- main(): parses argv into a stack, calls
                                read_ops, prints OK/KO
src/checker_bonus.c         -- read_ops: dispatch loop reading op tokens
                                from stdin
src/checker_op_ps_bonus.c   -- ckr_sa/sb/ss/pa/pb
src/checker_op_r_bonus.c    -- ckr_ra/rb/rr
src/checker_op_rr_bonus.c   -- ckr_rra/rrb/rrr
src/checker_utils_bonus.c   -- print_result (OK/KO), print_check_bench
```

`lib/` and `inc/push_swap.h`/`inc/libft.h` are committed so a fresh clone
has something to build against immediately, but they're specific to
whichever push_swap they were copied from — every student's `push_swap.h`
differs. The top-level `make check` (see the toolkit root README) refreshes
them from `../push_swap.a`, `../libft.a`, `../push_swap.h` before building,
so they always match *your* push_swap.

## Build

```
make
```

builds against whatever is currently in `lib/`/`inc/`. To make sure those
match your current push_swap build, run `make check` from the toolkit root
instead — it copies the fresh `.a` files and header first.

## Usage

Same shape as push_swap's own bonus checker: give it the starting numbers
as argv, pipe push_swap's op list into stdin.

```
./push_swap --simple 3 1 2 5 4 | ./ps_check 3 1 2 5 4
```

prints `OK` if the resulting stack `a` is sorted and stack `b` is empty,
`KO` otherwise. An unrecognised op token on stdin also prints `Error` (to
stderr) and stops processing further ops, but `print_result` still runs and
checks whatever state was reached.

Pass `--bench` *before* the numbers (same convention push_swap itself uses)
to also print an operation-count summary to stderr:

```
./push_swap --simple 3 1 2 5 4 | ./ps_check --bench 3 1 2 5 4
```

```
[bench] disorder: 30.00%
[bench] total ops: 7
[bench] sa: 1	sb: 0	ss: 0	pa: 2	pb: 2
[bench] ra: 1	rb: 0	rr: 0	rra: 1	rrb: 0	rrr: 0
OK
```

Don't pass `--bench` to push_swap itself here: that flag makes push_swap
run its ops silently (no stdout op list at all, just its own `[bench]`
summary on stderr) — there'd be nothing for `ps_check` to read. Run
push_swap plain and let `ps_check`'s own `--bench` produce the summary
instead.

## What this is *not*

Unlike the earlier scaffold version of this tool, `ps_check` does not sweep
a grid of (size, disorder, algorithm) by itself, and it does not vendor or
reimplement push_swap's logic independently — it trusts the linked
`push_swap.a` for everything except its own `main`/dispatch/print code.
Checking many inputs (the input → push_swap → checker chain across a whole
grid) is a separate next step, meant to be built by hand as a small
terminal pipeline (C/shell, no Python/JS) on top of this single-check unit.
