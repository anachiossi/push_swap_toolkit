"""Single source of truth for the ps_graph sweep grid.

ps_check is a separate, user-written C tool and does not read this file --
keep its own grid constants in sync by hand if you want the same coverage."""

SIZES = [1, 5, 10, 20, 35, 50, 65, 80, 100, 150, 200, 300, 500, 750, 1000]

DISORDERS = [round(i * 0.05, 2) for i in range(21)]  # 0.0 .. 1.0 step 0.05

# name -> level passed to ps_input / push_swap --<name>
# Drop any algorithm you haven't implemented yet -- the sweep, audit and bigo
# all derive their algorithm set from whatever is listed here / shows up in the
# data, so a partial set (e.g. no "complex") just works.
ALGORITHMS = {"adaptive": 0, "simple": 1, "medium": 2, "complex": 3}

# Two algorithms whose crossover ps_plot annotates (dashed line + label). Point
# it at the pair you care about; if either name isn't in ALGORITHMS the
# annotation is skipped instead of crashing the plot.
CROSSOVER_PAIR = ("simple", "complex")

GRAPH_RUNS = 5  # samples averaged per (size, disorder, algorithm) point
