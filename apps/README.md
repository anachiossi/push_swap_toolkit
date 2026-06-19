# apps

Two browser tools to watch push_swap sort, step by step.

## simulator

Click through ops by hand. No setup needed:

```
open apps/simulator/index.html
```

## visualizer

Generates a stack, runs push_swap, and replays the moves with play/pause,
step, zoom, and an algorithm-comparison view.

```
make push_swap input          # from the toolkit root, if not built yet
python3 apps/visualizer/server.py
```

Open `http://localhost:8910/`.

Keeping the toolkit outside your push_swap folder? Run it with
`PUSH_SWAP_PATH=/path/to/push_swap python3 apps/visualizer/server.py`.
