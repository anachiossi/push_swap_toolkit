# ps_graph

Runs push_swap many times across different sizes/disorders/algorithms,
then draws graphs of the results.

## Build & run

```
make graph        # from the toolkit root
./ps_graph/ps_graph
python3 ps_graph/ps_plot.py
python3 ps_graph/ps_bigo.py
python3 ps_graph/ps_adapt.py
python3 ps_graph/ps_audit.py
```

Or just `make run-graph` from the toolkit root to do all of it at once.

## Changing what gets tested

Edit `../config.py` (sizes, disorder levels, algorithms, how many runs to
average). Then re-run `make graph`.

## What each plot shows

- **ps_plot.png** — ops vs size/disorder, compared to the 42 subject's
  pass/good/excellent targets.
- **ps_bigo.png** — fits a curve to estimate each algorithm's Big O.
- **ps_adapt.png** — heatmap of which algorithm wins at each (size, disorder).
- **ps_audit.png** — checks if your `adaptive` mode actually picks the
  best algorithm everywhere (red = it picked wrong).

## Note

It never calls push_swap with `--bench` — that flag hides the op list (it
only prints a stats summary), which is what we're counting. Op count comes
from counting push_swap's normal printed lines instead.
