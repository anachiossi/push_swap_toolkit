# ps_graph

Executes a comprehensive sweep (runs push_swap several times for each size
x disorder combination). Outputs a .txt with all the data. From that, some
py scripts create graphs to compare your algorithms, calculate the
empirical big O, and audit if your adaptive mode chooses the best
algorithm.

Needs `pandas`, `numpy`, `matplotlib`: `pip install -r ../requirements.txt`.

## Build & run
From the toolkit root - builds, runs the sweep, makes all the plots

```
make graph
```

That's the whole thing in one command. Everything it generates (the data
file, the plots, `ps_bigo.txt`) lands in `out/` at the toolkit root, not in
this folder — `ps_graph/` stays just the scripts.

To run the steps yourself instead, from the toolkit root (the scripts
write to whatever the current directory is, so `cd` into `out/` first):

```
mkdir -p out && cd out
../ps_graph/ps_graph
python3 ../ps_graph/ps_plot.py
python3 ../ps_graph/ps_bigo.py
python3 ../ps_graph/ps_audit.py
```

## Changing what gets tested

Edit `../config.py` (sizes, disorder levels, algorithms, how many runs to
average), then re-run `make graph`.

## What each script outputs (into `out/`)

| Script | Output | What it shows |
|---|---|---|
| `ps_graph` | `ps_graph_data.txt` | raw sweep data: size, disorder, algorithm, avg ops, avg time |
| `ps_plot.py` | `ps_plot.png` | ops vs size/disorder, against the 42 subject's pass/good/excellent target zones |
| `ps_bigo.py` | `ps_bigo.png` + `ps_bigo.txt` | estimated Big O per algorithm (log-log fit) — see below |
| `ps_audit.py` | `ps_audit.png` + terminal report | checks if `adaptive` actually picks the best algorithm everywhere (red = it picked wrong, with the algorithm it should've used) |

## How the Big O is measured (`ps_bigo.py`)

The exponent is the slope of an `ops vs size` line in log-log space. The
decisions behind it, and why:

**Worst case over disorder.** Big O is a worst-case bound, so for each
`(algorithm, size)` we take the **max ops across all disorders**, then fit.
We deliberately do *not* fit a single fixed disorder — that only measures the
cost *at* that disorder, which equals the true class only if that slice
happens to be the worst arrangement.

**Why not just use d=1.0 (reversed) as "the" worst case.** The worst
arrangement is algorithm-specific, not always reversed:

- **simple / selection is input-sensitive**: ~`O(n)` when sorted (early-exit)
  *and* when reversed (the min sits at the bottom, one rotation away via the
  shorter-direction `min_to_top`), and only `Θ(n²)` in the messy middle. Its
  cost vs disorder is a hump peaking around `d ≈ 0.5–0.65`, so measuring at
  `d=1.0` would wrongly read it as **linear**.
- **complex / radix is disorder-oblivious**: identical ops at every disorder
  (move count depends on the rank bits, not the arrangement) → worst = any.
- **medium / chunk is near-oblivious** too.

Max-over-disorder is the one rule that's correct for all three at once.

**Classes this produces** (and the algorithm behind each flag):

| flag | algorithm | class |
|---|---|---|
| `--simple` | selection sort | `O(n²)` |
| `--medium` | chunk sort (√n chunks) | `O(n√n)` — strictly between the other two, since `log n < √n < n` |
| `--complex` | radix sort | `O(n log n)` |
| `--adaptive` | dispatcher | worst case bounded to `O(n log n)` — it routes *away* from selection's `n²` regime at large n. Its single-slope fit is looser (lower R²) because it's a **splice** of algorithms, not one curve. |

**Classifier buckets** sit at the midpoints of the ideal log-log slopes over
this size range (`n ≈ 20..1000`): `n log n ≈ 1.21`, `n√n = 1.5`, `n² = 2.0`.
An earlier version had no `n√n` slot, so chunk sort (`k ≈ 1.37`) got rounded
down into the `n log n` bin and looked linearithmic. Note the exponent alone
is a weak label — two different curves can share an average slope (e.g.
adaptive `~1.30` vs medium `~1.36`) — so the real class comes from curve
*shape*, not just the slope.

