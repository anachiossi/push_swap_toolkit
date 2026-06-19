# ps_input

Input generator for push_swap. Given a target size and disorder level, produces a random array and prints a ready-to-run push_swap command. Supports both interactive use and scripted auto mode.

## Build

```
cd ps_input && make
```

## Modes

### Interactive

Run with no arguments:

```
./ps_input
```

The program asks four questions:

| Prompt | Range | Description |
|---|---|---|
| How many numbers? | 1 – 10000 | size of the input array |
| How much disorder? | 0.0 – 1.0 | 0.0 = sorted, 1.0 = fully reversed |
| Which algorithm? | adaptive / simple / medium / complex | sets the `--flag` in the output command |
| Do you need benchmarks? | y / n | whether to print size and disorder stats before the command |

Example session:

```
How many numbers? (1 - 10000)
5
How much disorder? (0.0 - 1.0)
0.8
Which algorithm?
(adaptive | simple | medium | complex)
simple
Do you need benchmarks? (y / n)
y
```

Output:

```
size               : 5
disorder target    : 0.8000
disorder generated : 0.8000
./push_swap --simple --bench 3 5 1 4 2
```

Copy the last line and run it:

```
./push_swap --simple 3 5 1 4 2
```

To count operations:

```
./push_swap --simple 3 5 1 4 2 | wc -l
```

### Auto mode

Pass all five parameters as arguments — no prompts, designed for scripting:

```
./ps_input <size> <disorder> <level> <bench> <use_rank>
```

| Argument | Type | Values | Description |
|---|---|---|---|
| size | int | 1 – 10000 | array size |
| disorder | float | 0.0 – 1.0 | target disorder level |
| level | int | 0 / 1 / 2 / 3 | algorithm: 0=adaptive 1=simple 2=medium 3=complex |
| bench | int | 0 / 1 | 1 = print benchmark header, 0 = skip |
| use_rank | int | 0 / 1 | 1 = output ranks [0…n-1], 0 = output random integers |

Example:

```
./ps_input 100 0.5 1 0 1
```

This is the mode `ps_graph` uses internally. For each data point, ps_graph calls ps_input in a loop, takes the last line of output (the push_swap command), strips the binary name and flags with `sed`, and feeds the numbers directly to push_swap.

## Disorder metric

Disorder is the normalised inversion count:

```
disorder = wrong_pairs / total_pairs
         = wrong_pairs / (n * (n-1) / 2)
```

A pair (i, j) with i < j is *wrong* if `value[i] > value[j]` — that is, they are out of order. The total number of possible pairs is `n*(n-1)/2`, so the result is always in [0.0, 1.0].

```
0.0  →  fully sorted, zero inversions
0.5  →  approximately random (half of all pairs inverted)
1.0  →  fully reversed, every pair inverted
```

This definition is independent of the actual values: it only measures the shape of the permutation.

## How the array is generated

Generation happens in two stages.

### Stage 1 — rank array

The rank array is a permutation of `[0, n-1]` that encodes the sorted order of the final values. The generator starts from an extreme:

- if target disorder < 0.5 → start ascending (sorted, disorder = 0)
- if target disorder ≥ 0.5 → start descending (reversed, disorder = 1)

Then it adjusts toward the target by repeatedly picking a random adjacent pair and swapping it only if the swap moves in the right direction:

- **make_chaos** — when current disorder is below target: swaps adjacent pairs that are currently in order, adding one inversion per swap
- **make_order** — when current disorder is above target: swaps adjacent pairs that are currently inverted, removing one inversion per swap

Both functions count exactly how many swaps are needed (`to_chaos` or `to_order`) before entering the loop, so the process converges deterministically. The randomness is only in *which* adjacent position is picked each iteration.

### Stage 2 — value array

A sorted array of `n` random integers is generated (uniform in roughly `[-RAND_MAX/2, RAND_MAX/2]`), then reordered according to the rank array. This guarantees all values are unique and spread across the integer range, while the permutation shape (and therefore the disorder) is exactly what was produced in stage 1.

### rank vs value output

With `use_rank = 1`, the program prints the rank array `[0…n-1]`. With `use_rank = 0`, it prints the random integer values instead. Both have the same permutation shape and therefore the same disorder. ps_graph always uses ranks because the number of push_swap operations depends only on the permutation, not the actual values.

## Notes

- **Approximation for small arrays.** With n = 3, the only possible inversion counts are 0, 1, 2, 3. You cannot hit exactly disorder = 0.5 (which would require 1.5 inversions). The generator produces the closest achievable value. The benchmark header always prints both the target and the actual generated disorder so you can see the difference.

- **Randomness.** The seed is `time(NULL) ^ getpid()`. When ps_graph launches multiple ps_input processes in rapid succession within the same second, the XOR with the process ID ensures they produce different arrays.

- **Size limit.** The maximum of 10000 is a practical limit, not an arithmetic one. The inversion count formula `n*(n-1)/2` fits in a 32-bit int up to n ≈ 65535. The real constraint is sweep time: selection sort (simple) at n = 10000 takes around 25 minutes per run.
