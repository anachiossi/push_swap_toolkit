# ============================================================
# ps_graph — paths
# Edit here, then run make to regenerate the script.
# ============================================================

PUSH_SWAP	?=
PS_GEN		?= $(abspath ../ps_input/ps_input)
PS_CHECK	?= $(abspath ../ps_check/ps_check)

SED_FLAGS = \
	-e 's|__PUSH_SWAP__|$(PUSH_SWAP)|g' \
	-e 's|__PS_GEN__|$(PS_GEN)|g' \
	-e 's|__PS_CHECK__|$(PS_CHECK)|g'

all: check-vars ps_graph

check-vars:
	@if [ -z "$(PUSH_SWAP)" ]; then \
		echo "Usage: make PUSH_SWAP=/path/to/your/push_swap"; exit 1; \
	fi

# Always regenerated: the baked-in paths come from make variables
# (PUSH_SWAP/PS_GEN), not the .tpl, so gating on the template alone would
# keep a stale binary path whenever only the variable changes.
ps_graph: ps_graph.tpl
	sed $(SED_FLAGS) $< > $@
	chmod +x $@

clean:
	rm -f ps_graph

re: clean all

.PHONY: all clean re check-vars ps_graph
