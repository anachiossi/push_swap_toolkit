*This project has been created as part of the 42 curriculum by anade-mo.*

# apps

2 browser tools: 1 - a simulator, to help you mentally test your code
ideas, and 2 - a sort visualizer to watch your actual code sorting the
stack, as fast or slow as you want.

## Commands

| Command | Does |
|---|---|
| `open apps/simulator/index.html` | opens the simulator, no setup needed |
| `make push_swap input` | builds what the visualizer needs (run from the toolkit root, if not built yet) |
| `python3 apps/visualizer/server.py` | starts the visualizer at `http://localhost:8910/` |

Keeping the toolkit outside your push_swap folder? Run the visualizer with
`PUSH_SWAP_PATH=/path/to/push_swap python3 apps/visualizer/server.py`.
