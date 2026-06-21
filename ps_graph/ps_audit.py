#!/usr/bin/env python3
"""
ps_audit — does the adaptive dispatcher actually pick the best strategy?

For every (size, disorder) point it compares adaptive's ops against the best
of the OTHER algorithms that actually sorted. (A stub that reports 0 ops at a
non-trivial point didn't really sort, so it's ignored.) Adaptive is "good" if
it is at least as cheap as the best available algorithm; if it is clearly
worse, the cell is RED and labelled with the algorithm it should have used.

  green = adaptive is optimal (or better, e.g. sort_inverted at d=1.0)
  red   = adaptive routed to a worse algorithm -> a zone to fix

Caveat: adaptive and the base algos are averaged over DIFFERENT random inputs
per point, so a tolerance (10%) is applied — only gaps beyond sampling noise
are flagged red. (When adaptive routes to simple for n<=100, its value and
simple's value are the same algorithm on different draws, so they wobble a
little; the tolerance absorbs that.)

Outputs ps_audit.png and a text report:
  1. zones where adaptive is suboptimal (with the algorithm it should use)
  2. STRATEGY MAP — cheapest algorithm per size, by disorder range
  3. WINNER BY DISORDER — best algorithm at each disorder, tallied over sizes
Run:  python3 ps_audit.py
"""

import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

TOL = 0.10        # ignore relative gaps within 10% (sampling noise between runs)
MIN_WASTE = 10    # also require >=10 absolute ops, so tiny small-n noise isn't flagged

df = pd.read_csv('ps_graph_data.txt', sep='\t')
if 'adaptive' not in set(df['algorithm']):
    sys.exit("no 'adaptive' rows yet — run ./ps_graph with adaptive in ALGORITHMS")

sizes = sorted(df['size'].unique())
disorders = sorted(df['disorder'].unique())
ops = df.set_index(['size', 'disorder', 'algorithm'])['ops_avg'].to_dict()
bases = [a for a in df['algorithm'].unique() if a != 'adaptive']

gap = np.full((len(sizes), len(disorders)), np.nan)
pick = {}      # (i, j) -> algorithm adaptive should switch to
zones = []     # (n, d, adaptive_ops, best_ops, best_algo)

for i, n in enumerate(sizes):
    for j, d in enumerate(disorders):
        adp = ops.get((n, d, 'adaptive'))
        if adp is None:
            continue
        cand = {}
        for a in bases:
            v = ops.get((n, d, a))
            if v is None:
                continue
            if v > 0 or n == 1 or d == 0:       # drop stub 0s at non-trivial points
                cand[a] = v
        if not cand:
            continue
        best_algo = min(cand, key=cand.get)
        best = cand[best_algo]
        if best <= 0:                            # trivial point: everyone is 0
            gap[i, j] = 0.0
            continue
        rel = (adp - best) / best
        if rel > TOL and (adp - best) >= MIN_WASTE:
            gap[i, j] = rel
            pick[(i, j)] = best_algo
            zones.append((n, d, adp, best, best_algo))
        else:
            gap[i, j] = 0.0

# ---- heatmap ----
finite = gap[np.isfinite(gap)]
vmax = max(0.5, finite.max() if finite.size else 0.5)
fig, ax = plt.subplots(figsize=(12, 7))
im = ax.imshow(gap, aspect='auto', origin='lower', cmap='RdYlGn_r', vmin=0, vmax=vmax)
ax.set_xticks(range(len(disorders)))
ax.set_xticklabels([f'{d:.2f}' for d in disorders], rotation=45, ha='right', fontsize=7)
ax.set_yticks(range(len(sizes)))
ax.set_yticklabels(sizes, fontsize=7)
ax.set_xlabel('disorder')
ax.set_ylabel('size')
ax.set_title("adaptive audit — green: optimal,  red: adaptive worse than an "
             "available algorithm\n(letter in a red cell = algorithm it should use)")
for (i, j), a in pick.items():
    ax.text(j, i, a[0].upper(), ha='center', va='center',
            fontsize=7, fontweight='bold', color='black')
fig.colorbar(im, ax=ax, label='ops over best (fraction, 0 = optimal)')
plt.tight_layout()
plt.savefig('ps_audit.png', dpi=150)

# ---- report 1: where adaptive is leaving ops on the table ----
checked = int(np.isfinite(gap).sum())
print()
print("  1) ADAPTIVE CHECK  |  is the dispatcher already picking the best?")
print("  " + "=" * 64)
print(f"     points checked: {checked}    suboptimal: {len(zones)}   (tol {int(TOL * 100)}%)")
if not zones:
    print("     adaptive picks the best everywhere ✓")
else:
    print("     below: points where adaptive is clearly beaten, and by which algo")
    print(f"\n     {'size':>5} {'disord':>7} {'adaptive':>9} {'best':>8} {'should use':>11}   waste")
    print("     " + "-" * 60)
    for n, d, adp, best, a in zones:
        print(f"     {n:>5} {d:>7.2f} {int(adp):>9} {int(best):>8} {a:>11}"
              f"   +{int(adp - best)} ({(adp / best - 1) * 100:.0f}%)")
print()


# ---- helper: cheapest base algorithm at one (size, disorder) point ----
# Two algorithms within SPLIT_TOL of each other are treated as a tie (so run-to-
# run noise doesn't flip the winner), and the tie is broken toward the simpler
# algorithm. Trivial all-sorted points (best == 0) report no winner.
SPLIT_TOL = 0.03
NAME_ORDER = {'simple': 0, 'medium': 1, 'complex': 2}


def winner_at(n, d):
    cand = {}
    for a in bases:
        v = ops.get((n, d, a))
        if v is not None and (v > 0 or n == 1 or d == 0):
            cand[a] = v
    if not cand:
        return None
    best = min(cand.values())
    if best <= 0:
        return None
    tied = [a for a, v in cand.items() if (v - best) / best <= SPLIT_TOL]
    return min(tied, key=lambda a: NAME_ORDER.get(a, 9))


# ---- report 2: strategy map (what the dispatcher SHOULD route to) ----
# For each size we walk disorder low->high and group consecutive points by their
# cheapest algorithm, giving contiguous "use ALGO while disorder is in d lo-hi"
# bands. This is the raw material for writing the dispatcher's rules.
def segments(n):
    runs = []
    for d in disorders:
        w = winner_at(n, d)
        if w is None:
            continue
        if runs and runs[-1][0] == w:
            runs[-1][2] = d
        else:
            runs.append([w, d, d])
    return runs


print("  2) STRATEGY MAP  |  cheapest algorithm per size, by disorder range")
print("  " + "=" * 64)
print(f"     read as: at this size, use ALGO while disorder is in that range")
print(f"     (ties within {int(SPLIT_TOL * 100)}% folded together)")
print("     " + "-" * 60)
for n in sizes:
    runs = segments(n)
    if not runs:
        continue
    parts = []
    for algo, lo, hi in runs:
        rng = f"d={lo:.2f}" if lo == hi else f"d {lo:.2f}-{hi:.2f}"
        parts.append(f"{algo} ({rng})")
    print(f"     n={n:>5} :  " + "   ".join(parts))
print()


# ---- report 3: winner by disorder, tallied across all sizes ----
print("  3) WINNER BY DISORDER  |  best algorithm at each disorder, over all sizes")
print("  " + "=" * 64)
print("     each row: how many of the tested sizes each algorithm wins at that disorder")
print("     " + "-" * 60)
for d in disorders:
    tally = {}
    for n in sizes:
        w = winner_at(n, d)
        if w:
            tally[w] = tally.get(w, 0) + 1
    if not tally:
        continue
    total = sum(tally.values())
    summary = ",  ".join(f"{a} {c}/{total}"
                         for a, c in sorted(tally.items(), key=lambda x: -x[1]))
    print(f"     d={d:>5.2f} :  {summary}")
print()
