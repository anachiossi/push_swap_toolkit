# Default assumes this repo is cloned *inside* your push_swap project root,
# so ../ is the push_swap project. Override if you keep it elsewhere:
#   make PUSH_SWAP_PATH=/path/to/your/push_swap
PUSH_SWAP_PATH	?= ..
PUSH_SWAP		:= $(PUSH_SWAP_PATH)/push_swap
PS_INPUT		:= $(CURDIR)/ps_input/ps_input

.PHONY: all push_swap input check graphtpl graph clean fclean re

all: push_swap input check graphtpl

# Rebuilds your push_swap project's actual binary, needed to run real
# benchmarks/visualizations against it (see graph and apps/visualizer).
push_swap:
	$(MAKE) -C $(PUSH_SWAP_PATH)

# ps_input and ps_check link against a fixed, committed push_swap.a/libft.a
# (mine) -- never copied from whoever cloned this repo. Their C source
# calls specific function names/signatures (s, p, r, is_sorted, ft_atoi,
# get_next_line, ...) that can vary from student to student, so swapping
# in an arbitrary push_swap/libft here would just break the build.
input:
	$(MAKE) -C ps_input

check:
	$(MAKE) -C ps_check

# Just templates the ps_graph script -- build step, no run, kept out of
# `make all` because the actual sweep below is the slow part.
graphtpl: push_swap
	$(MAKE) -C ps_graph PUSH_SWAP=$(PUSH_SWAP) PS_GEN=$(PS_INPUT)

# Runs the benchmark sweep + plots. Everything generated (data file, pngs,
# bigo.txt) lands in out/, kept separate from the source/scripts in ps_graph/.
graph: all
	mkdir -p out
	cd out && ../ps_graph/ps_graph && python3 ../ps_graph/ps_plot.py \
		&& python3 ../ps_graph/ps_bigo.py && python3 ../ps_graph/ps_audit.py

clean:
	$(MAKE) -C ps_input clean
	$(MAKE) -C ps_check clean
	rm -f ps_graph/ps_graph
	rm -rf out

fclean: clean
	$(MAKE) -C ps_input fclean
	$(MAKE) -C ps_check fclean

re: fclean all
