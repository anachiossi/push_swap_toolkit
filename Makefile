# Path to your push_swap project (the folder with its Makefile + binary).
# EDIT THIS LINE to point at your project. 
# If you keep this toolkit inside your push_swap project, set "..".
# Or set the absolute path to the push_swap project folder.
PUSH_SWAP_PATH	?= ..
PUSH_SWAP		:= $(PUSH_SWAP_PATH)/push_swap
PS_INPUT		:= $(CURDIR)/ps_input/ps_input
# Generated output (data file, pngs, bigo.txt) lands one level up from the
# toolkit -- inside your push_swap project, at the same level as push_swap_toolkit.
OUT				:= $(abspath $(CURDIR)/../out)

.PHONY: all push_swap input check graphtpl graph clean fclean re

all: push_swap input check graphtpl

push_swap:
	$(MAKE) -C $(PUSH_SWAP_PATH)

input:
	$(MAKE) -C ps_input

check:
	$(MAKE) -C ps_check

# Just templates the ps_graph script -- build step, no run, kept out of
# `make all` because the actual sweep below is the slow part.
graphtpl: push_swap
	$(MAKE) -C ps_graph PUSH_SWAP=$(PUSH_SWAP) PS_GEN=$(PS_INPUT)

# Runs the benchmark sweep + plots. Everything generated lands in $(OUT) -- one
# level up, inside the push_swap project, kept out of the toolkit repo entirely.
# Scripts are called by absolute path so the cd into $(OUT) doesn't break them.
graph: all
	mkdir -p $(OUT)
	cd $(OUT) && $(CURDIR)/ps_graph/ps_graph \
		&& python3 $(CURDIR)/ps_graph/ps_plot.py \
		&& python3 $(CURDIR)/ps_graph/ps_bigo.py \
		&& python3 $(CURDIR)/ps_graph/ps_audit.py

clean:
	$(MAKE) -C ps_input clean
	$(MAKE) -C ps_check clean
	rm -f ps_graph/ps_graph
	rm -rf $(OUT)

fclean: clean
	$(MAKE) -C ps_input fclean
	$(MAKE) -C ps_check fclean

re: fclean all
