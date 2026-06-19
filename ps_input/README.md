# ps_input

Generates the complete input for push_swap: a number list with a target
disorder and size, with or without flags. Numbers can be random, from
INT_MIN to INT_MAX (value), or start always from 0 (rank).

## Build

```
make
```

(or `make input` from the toolkit root)

## Modes

| Mode | How |
|---|---|
| Interactive | `./ps_input`, answers 4 questions |
| Auto (for scripts) | `./ps_input <size> <disorder> <level> <bench> <use_rank>` |

Interactive asks: size, disorder (0.0=sorted, 1.0=reversed), algorithm
(0=adaptive 1=simple 2=medium 3=complex), bench tag (y/n), rank or value
(r/v). Auto mode takes the same answers as arguments, in the same order,
e.g.:

```
./ps_input 340 0.5 3 n r
```

This is what `ps_graph` uses internally.

Output (just the flags + array, ready to paste after `./push_swap`):

```
--simple --bench 3 5 1 4 2
```

```
./push_swap --simple 3 5 1 4 2
./push_swap --simple 3 5 1 4 2 | wc -l   # count operations
```

## Disorder

Disorder = fraction of pairs that are out of order. 0.0 = sorted, 0.5 =
random, 1.0 = fully reversed.

## Notes

- Very small arrays (n=3) can't hit every exact disorder value — you get
  the closest one.
- Max size is 10000, just because of time, not a hard limit. Selection
  sort at n=10000 takes ~25 min.
