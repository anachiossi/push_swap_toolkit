# ps_input

Input generator for push_swap. Given a target size and disorder level,
produces a random permutation with that exact disorder and prints the
push_swap flags + array (e.g. `--simple --bench 3 5 1 4 2`) ready to append
after `./push_swap`. Supports both interactive use and scripted auto mode
(used internally by `ps_graph`).

## Build

```
make
```

or, from the toolkit root: `make input`.

## Modes

### Interactive

Run with no arguments:

```
./ps_input
```

The program asks four questions, in order:

| Prompt | Range | Description |
|---|---|---|
| How many numbers? | 1 – 10000 | size of the input array |
| How much disorder? | 0.0 – 1.0 | 0.0 = sorted, 1.0 = fully reversed |
| Which algorithm? | 0 / 1 / 2 / 3 | 0=adaptive 1=simple 2=medium 3=complex |
| Add tag --bench? | y / n | whether to add `--bench` to the printed command |
| Print rank or value? | r / v | r = ranks [0…n-1], v = random integers |

Example session:

```
How many numbers? (1 - 10000)
5
How much disorder? (0.0 - 1.0)
0.8
Which algorithm?
(adaptive 0 | simple 1| medium 2| complex 3)
1
Add tag --bench? (y / n)
y
Print rank or value?
(r for rank (0 - n-1) | v for value (random int_min - int_max)
v
```

Output (just the push_swap flags and the array — no header/stats line):

```
--simple --bench 3 5 1 4 2
```

Feed the numbers straight to push_swap:

```
./push_swap --simple 3 5 1 4 2
```

To count operations:

```
./push_swap --simple 3 5 1 4 2 | wc -l
```

### Auto mode

Pass all five parameters as arguments — no prompts, designed for scripting.
Each argument takes the same answer you'd type at the matching interactive
prompt:

```
./ps_input <size> <disorder> <level> <bench> <use_rank>
```

| Argument | Type | Values | Description |
|---|---|---|---|
| size | int | 1 – 10000 | array size |
| disorder | float | 0.0 – 1.0 | target disorder level |
| level | int | 0 / 1 / 2 / 3 | algorithm: 0=adaptive 1=simple 2=medium 3=complex |
| bench | char | y / n | y = print benchmark header, n = skip |
| use_rank | char | r / v | r = output ranks [0…n-1], v = output random integers |

Example:

```
./ps_input 340 0.5 3 n r
```

Any value outside the documented ranges, or a `bench`/`use_rank` letter that
isn't `y`/`n`/`r`/`v`, prints the usage line and exits 1.

This is the mode `ps_graph` uses internally. For each data point, ps_graph
calls ps_input in a loop, takes the last line of output (the push_swap
command), strips the binary name and flags with `sed`, and feeds the
numbers directly to push_swap.

## Disorder metric

Disorder is the normalised inversion count:

```
disorder = wrong_pairs / total_pairs
         = wrong_pairs / (n * (n-1) / 2)
```

A pair (i, j) with i < j is *wrong* if `value[i] > value[j]` — that is, they
are out of order. The total number of possible pairs is `n*(n-1)/2`, so the
result is always in [0.0, 1.0].

```
0.0  →  fully sorted, zero inversions
0.5  →  approximately random (half of all pairs inverted)
1.0  →  fully reversed, every pair inverted
```

This definition is independent of the actual values: it only measures the
shape of the permutation.

## How the array is generated

Generation happens in two stages.

### Stage 1 — rank array (`gen_rank.c`, `gen_agents.c`)

The rank array is a permutation of `[0, n-1]` that encodes the sorted order
of the final values. The generator starts from an extreme:

- if target disorder < 0.5 → start ascending (sorted, disorder = 0)
- if target disorder ≥ 0.5 → start descending (reversed, disorder = 1)

Then it adjusts toward the target by repeatedly picking a random adjacent
pair and swapping it only if the swap moves in the right direction:

- **make_chaos** — when current disorder is below target: swaps adjacent
  pairs that are currently in order, adding one inversion per swap
- **make_order** — when current disorder is above target: swaps adjacent
  pairs that are currently inverted, removing one inversion per swap

Both functions count exactly how many swaps are needed (`to_chaos` /
`to_order`) before entering the loop, so the process converges
deterministically. The randomness is only in *which* adjacent position is
picked each iteration.

### Stage 2 — value array (`gen_number.c`)

A sorted array of `n` random integers is generated (uniform in roughly
`[-RAND_MAX/2, RAND_MAX/2]`), then reordered according to the rank array.
This guarantees all values are unique and spread across the integer range,
while the permutation shape (and therefore the disorder) is exactly what
was produced in stage 1.

### rank vs value output

With `use_rank = r`, the program prints the rank array `[0…n-1]`. With
`use_rank = v`, it prints the random integer values instead. Both have the
same permutation shape and therefore the same disorder. ps_graph always uses
ranks because the number of push_swap operations depends only on the
permutation, not the actual values.

## Notes

- **Approximation for small arrays.** With n = 3, the only possible
  inversion counts are 0, 1, 2, 3. You cannot hit exactly disorder = 0.5
  (which would require 1.5 inversions). The generator produces the closest
  achievable value. The benchmark header always prints both the target and
  the actual generated disorder so you can see the difference.

- **Randomness.** The seed is `time(NULL) ^ getpid()`. When ps_graph
  launches multiple ps_input processes in rapid succession within the same
  second, the XOR with the process ID ensures they produce different
  arrays.

- **Size limit.** The maximum of 10000 is a practical limit, not an
  arithmetic one. The inversion count formula `n*(n-1)/2` fits in a 32-bit
  int up to n ≈ 65535. The real constraint is sweep time: selection sort
  (simple) at n = 10000 takes around 25 minutes per run.
