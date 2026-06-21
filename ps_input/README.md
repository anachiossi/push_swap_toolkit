*This project has been created as part of the 42 curriculum by anade-mo.*

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
| Interactive | `./ps_input`, answers 4 questions on terminal|
| Auto (for scripts) | `./ps_input <size> <disorder> <level> <bench> <use_rank>` |

Interactive asks: 
 1. size (0 - 10000)
 2. disorder (0.0=sorted, 1.0=reversed)
 3. algorithm (0=adaptive 1=simple 2=medium 3=complex)
 4. bench tag (y/n)
 5. rank or value (0 - size -1) or (random INT_MIN to INT_MAX) (r/v)
 
 Auto mode takes the same answers as arguments, in the same order,
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
