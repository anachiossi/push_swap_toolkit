# ps_input

Generates a random stack of numbers with a chosen size and disorder level,
and prints it ready to feed into push_swap.

## Build

```
make
```

(or `make input` from the toolkit root)

## Interactive mode

```
./ps_input
```

Answers 4 questions: size, disorder (0.0=sorted, 1.0=reversed), algorithm
(0=adaptive 1=simple 2=medium 3=complex), bench tag (y/n), output as ranks
or values (r/v). Prints something like:

```
--simple --bench 3 5 1 4 2
```

Use it like this:

```
./push_swap --simple 3 5 1 4 2
./push_swap --simple 3 5 1 4 2 | wc -l   # count operations
```

## Auto mode (no prompts, for scripts)

```
./ps_input <size> <disorder> <level> <bench:y/n> <use_rank:r/v>
```

Example:

```
./ps_input 340 0.5 3 n r
```

This is what `ps_graph` uses internally.

## Disorder

Disorder = fraction of pairs that are out of order. 0.0 = sorted, 0.5 =
random, 1.0 = fully reversed.

## Notes

- With very small arrays (n=3), exact disorder values like 0.5 aren't
  reachable — you get the closest one.
- Max size is 10000. Not a hard limit, just slow (selection sort at
  n=10000 takes ~25 min).
