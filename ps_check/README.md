# ps_check

Checks if push_swap's output actually sorts the stack. It needs the same
input as argv and takes the operation list from stdout, so executing your
push_swap is a must. My own bonus checker code, just linked against my
real push_swap instead of recompiling it.

## Build

```
make
```

(or `make check` from the toolkit root)

Links against the `push_swap.a`/`libft.a`/`push_swap.h` committed in
`lib/`/`inc/` — mine, fixed, the same for everyone who clones this repo.
It's never swapped for your own push_swap: the checker's C calls specific
function names/signatures that can vary from student to student, so
mixing in someone else's would just break the build.

## Commands

| Command | Does |
|---|---|
| `./push_swap --simple 3 1 2 5 4 \| ./ps_check 3 1 2 5 4` | prints `OK` or `KO` |
| `./push_swap --simple 3 1 2 5 4 \| ./ps_check --bench 3 1 2 5 4` | also prints an op-count breakdown |

`--bench` goes *before* the numbers. And run push_swap plain, not with its
own `--bench` — that flag hides the op list, so there'd be nothing to
check.

## What it doesn't do

It checks one input at a time. Running it across many inputs (a sweep) is
my own next tool to build, by hand.
