#!/bin/bash

# --- paths (substituted by Makefile) ---
PUSH_SWAP="__PUSH_SWAP__"
PS_GEN="__PS_GEN__"
PS_CHECK="__PS_CHECK__"

# --- sweep parameters, single source of truth in ../config.py ---
eval "$(python3 "$(dirname "$0")/../gen_sweep_vars.py")"

OUTPUT=${1:-ps_graph_data.txt}

if [ ! -x "$PS_CHECK" ]; then
	echo "ERROR: checker not found or not executable at: $PS_CHECK"
	echo "Build it first (make -C ../ps_check, or 'make check' from the toolkit root)."
	exit 1
fi

# colors only when writing to a real terminal (keep escape codes out of pipes/logs)
if [ -t 1 ]; then
	C_OK=$'\033[32m'; C_KO=$'\033[1;31m'; C_DIM=$'\033[2m'; C_RST=$'\033[0m'
else
	C_OK=''; C_KO=''; C_DIM=''; C_RST=''
fi

# printed under a cell that produced a KO: what failed and a paste-ready command
print_ko_report() {
	printf "       ${C_KO}Error in push_swap : input${C_RST}\n"
	printf "       size=%s  disorder=%s  flag=--%s\n" "$1" "$2" "$3"
	printf "       ${C_DIM}copy/paste to debug:${C_RST}  ./push_swap --%s %s\n" "$3" "$4"
}

printf "size\tdisorder\talgorithm\tops_avg\ttime_ms_avg\n" > "$OUTPUT"

total=$(( ${#SIZES[@]} * ${#DISORDERS[@]} * ${#ALGORITHMS[@]} ))
count=0
ko_total=0

echo ""
echo "ps_graph  |  sizes=${#SIZES[@]}  disorders=${#DISORDERS[@]}  algos=${#ALGORITHMS[@]}  runs=${RUNS}"
echo "-------------------------------------------------------"

for size in "${SIZES[@]}"; do
	for disorder in "${DISORDERS[@]}"; do
		for algo_pair in "${ALGORITHMS[@]}"; do
			algo_name="${algo_pair%%:*}"
			algo_level="${algo_pair##*:}"
			count=$((count + 1))

			printf "  [%d/%d]  n=%-5d  d=%-4s  algo=%-8s" \
				"$count" "$total" "$size" "$disorder" "$algo_name"

			ops_sum=0
			ms_sum=0
			valid=0
			ko=0
			ko_input=""

			for run in $(seq 1 $RUNS); do
				nums=$("$PS_GEN" "$size" "$disorder" "$algo_level" n r 2>/dev/null \
					| tail -1 \
					| sed 's|^\./push_swap ||; s/--[a-zA-Z]* //g')
				[ -z "$nums" ] && continue

				t_start=$(date +%s%N)
				ops_out=$(echo $nums | xargs "$PUSH_SWAP" "--${algo_name}" 2>/dev/null)
				status=$?
				t_end=$(date +%s%N)

				[ "$status" -ne 0 ] && continue
				ops=$(echo -n "$ops_out" | grep -c '^')

				# correctness gate: replay the ops through ps_check; drop KO
				# runs so wrong output never pollutes the averages. Run after
				# t_end so the checker's own runtime isn't counted.
				if [ "$(echo "$ops_out" | "$PS_CHECK" $nums 2>/dev/null)" != "OK" ]; then
					ko=$(( ko + 1 ))
					ko_total=$(( ko_total + 1 ))
					[ -z "$ko_input" ] && ko_input="$nums"
					continue
				fi

				ops_sum=$(( ops_sum + ops ))
				ms_sum=$(( ms_sum + (t_end - t_start) / 1000000 ))
				valid=$(( valid + 1 ))
			done

			if [ "$valid" -eq 0 ]; then
				if [ "$ko" -gt 0 ]; then
					printf "  ${C_KO}KO${C_RST}  (all %d run(s) failed to sort)\n" "$ko"
					print_ko_report "$size" "$disorder" "$algo_name" "$ko_input"
				else
					printf "  ${C_KO}ERROR${C_RST}: all runs failed (no output / crash)\n"
				fi
				continue
			fi

			ops_avg=$(( ops_sum / valid ))
			ms_avg=$(( ms_sum / valid ))

			if [ "$ko" -gt 0 ]; then
				printf "  ops_avg: %-8d  time_avg: %-5dms  ${C_KO}KO (%d/%d)${C_RST}\n" \
					"$ops_avg" "$ms_avg" "$ko" "$RUNS"
				print_ko_report "$size" "$disorder" "$algo_name" "$ko_input"
			else
				printf "  ops_avg: %-8d  time_avg: %-5dms  ${C_OK}OK${C_RST}\n" \
					"$ops_avg" "$ms_avg"
			fi
			printf "%s\t%s\t%s\t%s\t%s\n" "$size" "$disorder" "$algo_name" "$ops_avg" "$ms_avg" >> "$OUTPUT"
		done
	done
done

echo "-------------------------------------------------------"
if [ "$ko_total" -gt 0 ]; then
	printf "  done -> %s\n" "$OUTPUT"
	printf "  ${C_KO}WARNING: %d run(s) produced KO — an algorithm is not sorting correctly.${C_RST}\n" "$ko_total"
	echo "     Flagged cells above are NOT trustworthy; fix before trusting the graphs."
	echo ""
	exit 1
fi
printf "  done -> %s   ${C_OK}(all runs verified OK by ps_check)${C_RST}\n" "$OUTPUT"
echo ""
