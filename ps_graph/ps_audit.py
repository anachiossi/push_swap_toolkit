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
are flagged red. (When adaptive routes to selection for n<=100, its value and
simple's value are the same algorithm on different draws, so they wobble a
little; the tolerance absorbs that.)

Outputs ps_audit.png and a text report of the zones to change.
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

# ---- report ----
checked = int(np.isfinite(gap).sum())
print()
print("  ps_audit  |  adaptive vs best available strategy")
print("  " + "=" * 50)
print(f"  points checked: {checked}    suboptimal: {len(zones)}   (tol {int(TOL * 100)}%)")
if not zones:
    print("  adaptive picks the best everywhere ✓")
else:
    print(f"\n  {'size':>5} {'disord':>7} {'adaptive':>9} {'best':>8} {'use':>9}   waste")
    print("  " + "-" * 54)
    for n, d, adp, best, a in zones:
        print(f"  {n:>5} {d:>7.2f} {int(adp):>9} {int(best):>8} {a:>9}"
              f"   +{int(adp - best)} ({(adp / best - 1) * 100:.0f}%)")
print()


# ---- ideal selection/radix thresholds per size (straight from the data) ----
# selection wins at the disorder extremes; this finds, for each size, how far
# in those extremes reach. 't' is the value to use in the dispatcher's rule
# "selection if (d < t || d > 1 - t) else radix".
step = float(np.min(np.diff(disorders))) if len(disorders) > 1 else 0.05


def selection_band(n):
    lo = None
    for d in disorders:
        s, c = ops.get((n, d, 'simple')), ops.get((n, d, 'complex'))
        if s is not None and c is not None and s <= c:
            lo = d
        else:
            break
    hi = None
    for d in reversed(disorders):
        s, c = ops.get((n, d, 'simple')), ops.get((n, d, 'complex'))
        if s is not None and c is not None and s <= c:
            hi = d
        else:
            break
    return lo, hi


print("  ideal selection/radix split per size  (from simple-vs-complex crossover)")
print(f"  {'n':>5}   {'selection wins':>24}   {'dispatcher t':>12}")
print("  " + "-" * 50)
for n in sizes:
    if ops.get((n, disorders[0], 'simple')) is None:
        continue
    lo, hi = selection_band(n)
    if lo is not None and hi is not None and lo >= hi:
        print(f"  {n:>5}   {'all disorders':>24}   {'-> simple':>12}")
    elif lo is None and hi is None:
        print(f"  {n:>5}   {'none':>24}   {'-> radix':>12}")
    else:
        desc = "  or  ".join(x for x in
                             [f"d<={lo:.2f}" if lo is not None else "",
                              f"d>={hi:.2f}" if hi is not None else ""] if x)
        cand = ([lo + step] if lo is not None else []) \
            + ([1 - hi + step] if hi is not None else [])
        print(f"  {n:>5}   {desc:>24}   t={round(max(cand), 2):.2f}")
print()


# ---- absolute winners (which base algorithm wins outright) ----
def best_base_at(n, d):
    cand = {}
    for a in bases:
        v = ops.get((n, d, a))
        if v is not None and (v > 0 or n == 1 or d == 0):
            cand[a] = v
    return min(cand, key=cand.get) if cand else None


def tally_str(tally):
    total = sum(tally.values())
    return ",  ".join(f"{a} {c}/{total}"
                      for a, c in sorted(tally.items(), key=lambda x: -x[1]))


print("  absolute winner per size  (base algo winning the most disorders)")
print("  " + "-" * 50)
for n in sizes:
    tally = {}
    for d in disorders:
        w = best_base_at(n, d)
        if w:
            tally[w] = tally.get(w, 0) + 1
    if tally:
        print(f"  n={n:>5} :  {tally_str(tally)}")
print()

print("  absolute winner per disorder  (base algo winning the most sizes)")
print("  " + "-" * 50)
for d in disorders:
    tally = {}
    for n in sizes:
        w = best_base_at(n, d)
        if w:
            tally[w] = tally.get(w, 0) + 1
    if tally:
        print(f"  d={d:>5.2f} :  {tally_str(tally)}")
print()
