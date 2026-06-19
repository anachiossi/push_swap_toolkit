# Default assumes this repo is cloned *inside* your push_swap project root,
# so ../ is the push_swap project. Override if you keep it elsewhere:
#   make PUSH_SWAP_PATH=/path/to/your/push_swap
PUSH_SWAP_PATH	?= ..
PUSH_SWAP		:= $(PUSH_SWAP_PATH)/push_swap
PS_INPUT		:= $(CURDIR)/ps_input/ps_input

.PHONY: all push_swap input check graph run-graph clean fclean re

all: push_swap input check graph

# Rebuilds your push_swap project (binary + push_swap.a) from its own
# Makefile -- every student's push_swap.h differs, so this can't be
# skipped or vendored once and reused.
push_swap:
	$(MAKE) -C $(PUSH_SWAP_PATH)
	$(MAKE) -C $(PUSH_SWAP_PATH) push_swap.a

input:
	$(MAKE) -C ps_input

# Refreshes ps_check's lib/inc copies from your just-built push_swap project,
# then builds ps_check against them.
check: push_swap
	cp $(PUSH_SWAP_PATH)/push_swap.a ps_check/lib/push_swap.a
	cp $(PUSH_SWAP_PATH)/libft.a ps_check/lib/libft.a
	cp $(PUSH_SWAP_PATH)/push_swap.h ps_check/inc/push_swap.h
	$(MAKE) -C ps_check

graph: push_swap
	$(MAKE) -C ps_graph PUSH_SWAP=$(PUSH_SWAP) PS_GEN=$(PS_INPUT)

# Runs the benchmark sweep + plots (separate from `all` since it's slow).
run-graph: all
	./ps_graph/ps_graph
	python3 ps_graph/ps_plot.py
	python3 ps_graph/ps_bigo.py
	python3 ps_graph/ps_adapt.py

clean:
	$(MAKE) -C ps_input clean
	$(MAKE) -C ps_check clean
	rm -f ps_graph/ps_graph ps_graph/ps_graph_data.txt

fclean: clean
	$(MAKE) -C ps_input fclean
	$(MAKE) -C ps_check fclean

re: fclean all
