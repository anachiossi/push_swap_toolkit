# ps_graph

Executes a comprehensive sweep (runs push_swap several times for each size
x disorder combination). Outputs a .txt with all the data. From that, some
py scripts create graphs to compare your algorithms, calculate the
empirical big O, audit if your adaptive mode chooses the best algorithm,
and draw a heatmap of which algorithm wins at each case (size, disorder).

Needs `pandas`, `numpy`, `matplotlib`: `pip install -r ../requirements.txt`.

## Build & run

```
make graph        # from the toolkit root - builds, runs the sweep, makes all the plots
```

That's the whole thing in one command. To run the steps yourself instead:

```
./ps_graph/ps_graph
python3 ps_graph/ps_plot.py
python3 ps_graph/ps_bigo.py
python3 ps_graph/ps_adapt.py
python3 ps_graph/ps_audit.py
```

## Changing what gets tested

Edit `../config.py` (sizes, disorder levels, algorithms, how many runs to
average), then re-run `make graph`.

## What each plot shows

| Plot | Shows |
|---|---|
| `ps_plot.png` | ops vs size/disorder, against the 42 subject's pass/good/excellent targets |
| `ps_bigo.png` | fitted curve, estimating each algorithm's Big O |
| `ps_adapt.png` | heatmap of which algorithm wins at each (size, disorder) |
| `ps_audit.png` | checks if `adaptive` actually picks the best algorithm everywhere (red = it picked wrong) |

## Note

It never calls push_swap with `--bench` — that flag hides the op list (it
only prints a stats summary), which is what we're counting. Op count comes
from counting push_swap's normal printed lines instead.
