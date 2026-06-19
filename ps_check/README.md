# ps_check

Checks if a push_swap op list actually sorts the stack. Your own bonus
checker code, just linked against your real push_swap instead of
recompiling it.

## Build

```
make
```

(or `make check` from the toolkit root — this also refreshes the copies of
`push_swap.a`/`libft.a`/`push_swap.h` in `lib/`/`inc/` from your project,
so it always matches what you just built)

## Usage

```
./push_swap --simple 3 1 2 5 4 | ./ps_check 3 1 2 5 4
```

Prints `OK` or `KO`.

Want the op-count breakdown? Put `--bench` *before* the numbers, and run
push_swap plain (not with its own `--bench`, or it won't print any ops to
check):

```
./push_swap --simple 3 1 2 5 4 | ./ps_check --bench 3 1 2 5 4
```

## What it doesn't do

It checks one input at a time. Running it across many inputs (a sweep) is
your own next tool to build, by hand.
