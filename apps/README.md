# apps

Two small browser tools for building intuition about push_swap, built on a
shared pure-JS stack simulator (`shared/stack_engine.js`, no DOM
dependency — `applyMove`/`applyOp`/`replay`/`isSorted`).

## simulator

Manual click-through: load a stack (or generate a random one), click each
op button (`sa sb ss pa pb ra rb rr rra rrb rrr`), watch the two stacks
update, undo if needed. No server, no build — just open the file.

```
open apps/simulator/index.html   # or: python3 -m http.server, then visit it
```

## visualizer

Generates a permutation via `ps_input`, runs it through your `push_swap`,
and replays the resulting op list move-by-move on a canvas — play/pause,
step, scrub, speed control, zoom, and a "compare algorithms" mode that runs
all four levels on the same input side by side.

Needs a local server (it shells out to `ps_input` and `push_swap`):

```
make push_swap input          # from the toolkit root, if not already built
python3 apps/visualizer/server.py
```

Then open `http://localhost:8910/`.

By default it assumes the same layout as the rest of the toolkit — cloned
inside your push_swap project root, so `../` (two levels up from
`apps/visualizer/`) is your push_swap project. Override with `PUSH_SWAP_PATH`
if you keep it elsewhere:

```
PUSH_SWAP_PATH=/path/to/your/push_swap python3 apps/visualizer/server.py
```

The server only shells out to `ps_input`/`push_swap` with argv lists (never
builds shell strings) and only serves files from `apps/visualizer/` and
`apps/shared/`.
