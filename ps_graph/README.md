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
| `ps_bigo.py` | `ps_bigo.png` + `ps_bigo.txt` | estimated Big O per algorithm (log-log fit) |
| `ps_audit.py` | `ps_audit.png` + terminal report | checks if `adaptive` actually picks the best algorithm everywhere (red = it picked wrong, with the algorithm it should've used) |

