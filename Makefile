# Usage: make graph PUSH_SWAP=/path/to/your/push_swap
PUSH_SWAP ?=
PS_INPUT := $(CURDIR)/ps_input/ps_input

.PHONY: all input check graph clean

all: graph

input:
	$(MAKE) -C ps_input

check: input
	$(MAKE) -C ps_check
	@if [ -z "$(PUSH_SWAP)" ]; then \
		echo "Usage: make check PUSH_SWAP=/path/to/your/push_swap"; exit 1; \
	fi
	./ps_check/ps_check --push-swap $(PUSH_SWAP) --ps-input $(PS_INPUT)

graph: check
	$(MAKE) -C ps_graph PUSH_SWAP=$(PUSH_SWAP) PS_GEN=$(PS_INPUT)
	./ps_graph/ps_graph
	python3 ps_graph/ps_plot.py
	python3 ps_graph/ps_bigo.py
	python3 ps_graph/ps_adapt.py

clean:
	$(MAKE) -C ps_input clean
	$(MAKE) -C ps_check clean
	rm -f ps_graph/ps_graph ps_graph/ps_graph_data.txt
