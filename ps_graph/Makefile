# ============================================================
# ps_graph — paths
# Edit here, then run make to regenerate the script.
# ============================================================

PUSH_SWAP	?=
PS_GEN		?= $(abspath ../ps_input/ps_input)

SED_FLAGS = \
	-e 's|__PUSH_SWAP__|$(PUSH_SWAP)|g' \
	-e 's|__PS_GEN__|$(PS_GEN)|g'

all: check-vars ps_graph

check-vars:
	@if [ -z "$(PUSH_SWAP)" ]; then \
		echo "Usage: make PUSH_SWAP=/path/to/your/push_swap"; exit 1; \
	fi

ps_graph: ps_graph.tpl
	sed $(SED_FLAGS) $< > $@
	chmod +x $@

clean:
	rm -f ps_graph

re: clean all

.PHONY: all clean re check-vars
