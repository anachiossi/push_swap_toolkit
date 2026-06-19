import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('ps_graph_data.txt', sep='\t')

sub = df[(df['disorder'] == 0.5) & (df['ops_avg'] > 0) & (df['size'] >= 20)]

def bigo_label(k):
    if   k < 1.15: return 'O(n)'
    elif k < 1.65: return 'O(n log n)'
    elif k < 2.3:  return 'O(n²)'
    elif k < 3.1:  return 'O(n³)'
    else:          return f'O(n^{k:.2f})'

fig, ax = plt.subplots(figsize=(8, 5))

rows = []
for algo, g in sub.groupby('algorithm'):
    g = g.sort_values('size')
    x = np.log(g['size'].values.astype(float))
    y = np.log(g['ops_avg'].values.astype(float))

    k, c = np.polyfit(x, y, 1)
    y_pred = k * x + c
    r2 = 1 - np.sum((y - y_pred) ** 2) / np.sum((y - y.mean()) ** 2)

    rows.append((algo, k, bigo_label(k), r2))

    # scatter — actual data
    sc = ax.scatter(g['size'], g['ops_avg'], label=f'{algo}  ({bigo_label(k)},  k={k:.3f})', zorder=3)

    # fitted line extended across the full size range
    n_fit = np.linspace(g['size'].min(), g['size'].max(), 200)
    ops_fit = np.exp(c) * n_fit ** k
    ax.plot(n_fit, ops_fit, color=sc.get_facecolor()[0], linestyle='--', linewidth=1, alpha=0.7)

ax.set_xscale('log')
ax.set_yscale('log')
ax.set_title('empirical Big O  —  log-log fit  (disorder=0.5, n≥20)')
ax.set_xlabel('n  (log scale)')
ax.set_ylabel('ops  (log scale)')
ax.legend(fontsize=9)
ax.grid(True, which='both', linestyle=':', alpha=0.4)

plt.tight_layout()
plt.savefig('ps_bigo.png', dpi=150)

# text table
header = f"  empirical Big O  (log-log fit, disorder=0.5, n≥20)\n"
header += f"  {'algorithm':<10}  {'exponent':>8}  {'class':>12}  {'R²':>6}\n"
header += "  " + "-" * 42 + "\n"
lines = [f"  {algo:<10}  {k:>8.3f}  {cls:>12}  {r2:>6.4f}" for algo, k, cls, r2 in rows]

table = header + "\n".join(lines) + "\n"
print("\n" + table)

with open('ps_bigo.txt', 'w') as f:
    f.write(table)
