#!/bin/bash

# --- paths (substituted by Makefile) ---
PUSH_SWAP="__PUSH_SWAP__"
PS_GEN="__PS_GEN__"

# --- sweep parameters, single source of truth in ../config.py ---
eval "$(python3 "$(dirname "$0")/../gen_sweep_vars.py")"

OUTPUT=${1:-ps_graph_data.txt}

printf "size\tdisorder\talgorithm\tops_avg\ttime_ms_avg\n" > "$OUTPUT"

total=$(( ${#SIZES[@]} * ${#DISORDERS[@]} * ${#ALGORITHMS[@]} ))
count=0

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

			for run in $(seq 1 $RUNS); do
				nums=$("$PS_GEN" "$size" "$disorder" "$algo_level" 0 1 2>/dev/null \
					| tail -1 \
					| sed 's|^\./push_swap ||; s/--[a-zA-Z]* //g')
				[ -z "$nums" ] && continue

				t_start=$(date +%s%N)
				bench_out=$(echo $nums | xargs "$PUSH_SWAP" "--${algo_name}" "--bench" 2>/dev/null)
				t_end=$(date +%s%N)

				ops=$(echo "$bench_out" | grep "\[bench\] total ops:" | awk '{print $NF}')
				[ -z "$ops" ] && continue

				ops_sum=$(( ops_sum + ops ))
				ms_sum=$(( ms_sum + (t_end - t_start) / 1000000 ))
				valid=$(( valid + 1 ))
			done

			if [ "$valid" -eq 0 ]; then
				echo "  ERROR: all runs failed"
				continue
			fi

			ops_avg=$(( ops_sum / valid ))
			ms_avg=$(( ms_sum / valid ))

			printf "  ops_avg: %-8d  time_avg: %dms\n" "$ops_avg" "$ms_avg"
			printf "%s\t%s\t%s\t%s\t%s\n" "$size" "$disorder" "$algo_name" "$ops_avg" "$ms_avg" >> "$OUTPUT"
		done
	done
done

echo "-------------------------------------------------------"
echo "  done -> $OUTPUT"
echo ""
