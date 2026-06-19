import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('ps_graph_data.txt', sep='\t')

# Worst-case Big O. For each (algorithm, size) we take the MAX ops over all
# disorders -- the worst input arrangement at that size -- then fit. This is
# the textbook definition: Big O is a worst-case bound, so the slope must be
# measured on the worst slice, not an arbitrary fixed one.
#
# It matters here because selection is INPUT-SENSITIVE: it's ~O(n) on sorted
# input (early-exit) AND on reversed input (the min sits at the bottom, one
# rotation away), and only hits Theta(n^2) in the messy middle. So its cost
# vs disorder is a hump that peaks around d~0.5-0.65 -- fixing d=1.0 would
# read it as LINEAR, fixing d=0.5 happens to land near the peak by luck.
# radix and chunk are disorder-OBLIVIOUS (flat across d), so their envelope
# equals any single slice anyway. The max-over-disorder is correct for all.
sub = df[(df['ops_avg'] > 0) & (df['size'] >= 20)]
worst = sub.groupby(['algorithm', 'size'], as_index=False)['ops_avg'].max()

# Bucket edges sit at the midpoints of the ideal log-log slopes measured over
# this size range (n~20..1000): n log n ~ 1.21, n*sqrt(n) = 1.5, n^2 = 2.0.
def bigo_label(k):
    if   k < 1.15: return 'O(n)'
    elif k < 1.35: return 'O(n log n)'
    elif k < 1.75: return 'O(n√n)'
    elif k < 2.3:  return 'O(n²)'
    elif k < 3.1:  return 'O(n³)'
    else:          return f'O(n^{k:.2f})'

fig, ax = plt.subplots(figsize=(8, 5))

rows = []
for algo, g in worst.groupby('algorithm'):
    g = g.sort_values('size')
    x = np.log(g['size'].values.astype(float))
    y = np.log(g['ops_avg'].values.astype(float))

    k, c = np.polyfit(x, y, 1)
    y_pred = k * x + c
    r2 = 1 - np.sum((y - y_pred) ** 2) / np.sum((y - y.mean()) ** 2)

    rows.append((algo, k, bigo_label(k), r2))

    # scatter — worst-case data
    sc = ax.scatter(g['size'], g['ops_avg'], label=f'{algo}  ({bigo_label(k)},  k={k:.3f})', zorder=3)

    # fitted line extended across the full size range
    n_fit = np.linspace(g['size'].min(), g['size'].max(), 200)
    ops_fit = np.exp(c) * n_fit ** k
    ax.plot(n_fit, ops_fit, color=sc.get_facecolor()[0], linestyle='--', linewidth=1, alpha=0.7)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_title('empirical Big O  —  worst case over disorder  (log-log fit, n≥20)')
ax.set_xlabel('n  (log scale)')
ax.set_ylabel('worst-case ops  (log scale)')
ax.legend(fontsize=9)
ax.grid(True, which='both', linestyle=':', alpha=0.4)

plt.tight_layout()
plt.savefig('ps_bigo.png', dpi=150)

# text table
header = f"  empirical Big O  (worst case over disorder, log-log fit, n≥20)\n"
header += f"  {'algorithm':<10}  {'exponent':>8}  {'class':>12}  {'R²':>6}\n"
header += "  " + "-" * 42 + "\n"
lines = [f"  {algo:<10}  {k:>8.3f}  {cls:>12}  {r2:>6.4f}" for algo, k, cls, r2 in rows]

table = header + "\n".join(lines) + "\n"
print("\n" + table)

with open('ps_bigo.txt', 'w') as f:
    f.write(table)
