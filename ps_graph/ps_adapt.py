import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.colors import ListedColormap

df = pd.read_csv('ps_graph_data.txt', sep='\t')

algos     = [a for a in ['simple', 'medium', 'complex'] if a in df['algorithm'].values]
sizes     = sorted(df['size'].unique())
disorders = sorted(df['disorder'].unique())

pivot = df[df['algorithm'].isin(algos)].pivot_table(
    index=['size', 'disorder'], columns='algorithm', values='ops_avg'
)

def best(row):
    valid = row[algos].dropna()
    return valid.idxmin() if not valid.empty else None

pivot['winner'] = pivot.apply(best, axis=1)

algo_idx = {a: i for i, a in enumerate(algos)}
colors   = {'simple': '#4fc3f7', 'medium': '#81c784', 'complex': '#ffb74d'}

def winner_zones(n):
    pts = []
    for d in disorders:
        try:
            w = pivot.loc[(n, d), 'winner']
            if pd.notna(w):
                pts.append((d, w))
        except KeyError:
            pass
    zones = []
    for d, w in pts:
        if zones and zones[-1][2] == w:
            zones[-1] = (zones[-1][0], d, w)
        else:
            zones.append((d, d, w))
    return zones

def fmt_zones(zones):
    parts = []
    for i, (d0, d1, w) in enumerate(zones):
        if len(zones) == 1:
            parts.append(f'{w}: always')
        elif i == 0:
            parts.append(f'{w}: d≤{d1:.2f}')
        elif i == len(zones) - 1:
            parts.append(f'{w}: d≥{d0:.2f}')
        else:
            parts.append(f'{w}: {d0:.2f}–{d1:.2f}')
    return '  |  '.join(parts)

# --- report ---
print()
print("  ps_adapt  |  empirical algorithm selection zones")
print("  " + "=" * 50)

print(f"\n  winner by size  (d=0.5):")
prev = None
for n in sizes:
    try:
        w = pivot.loc[(n, 0.5), 'winner']
    except KeyError:
        continue
    if w != prev:
        print(f"    n={n}: → {w}")
        prev = w

print(f"\n  winner by disorder per size:")
print(f"  {'n':>6}  winner zones")
print("  " + "-" * 60)
for n in sizes:
    zones = winner_zones(n)
    print(f"  {n:>6}  {fmt_zones(zones)}")

print()

# --- heatmap ---
grid = np.full((len(sizes), len(disorders)), np.nan)
for i, n in enumerate(sizes):
    for j, d in enumerate(disorders):
        try:
            w = pivot.loc[(n, d), 'winner']
            if pd.notna(w):
                grid[i, j] = algo_idx[w]
        except KeyError:
            pass

palette = [colors[a] for a in algos]
cmap    = ListedColormap(palette)

fig, ax = plt.subplots(figsize=(10, 6))
ax.imshow(grid, aspect='auto', origin='lower', cmap=cmap, vmin=0, vmax=len(algos) - 1)

ax.set_xticks(range(len(disorders)))
ax.set_xticklabels([f'{d:.2f}' for d in disorders], rotation=45, ha='right', fontsize=7)
ax.set_yticks(range(len(sizes)))
ax.set_yticklabels(sizes, fontsize=7)
ax.set_xlabel('disorder')
ax.set_ylabel('size')
ax.set_title('best algorithm per (size, disorder)')

patches = [mpatches.Patch(color=colors[a], label=a) for a in algos]
ax.legend(handles=patches, loc='upper left')

plt.tight_layout()
plt.savefig('ps_adapt.png', dpi=150)
