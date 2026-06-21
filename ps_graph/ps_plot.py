import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('ps_graph_data.txt', sep='\t')
df = df[df['algorithm'] != 'adaptive']  # plot only the base algos, not the dispatcher
ops_df = df

def mark_disorder_crossovers(ax, n):
    sub = ops_df[ops_df['size'] == n]
    series = {}
    for algo, g in sub.groupby('algorithm'):
        g = g.sort_values('disorder')
        ax.plot(g['disorder'], g['ops_avg'], marker='o', label=algo)
        series[algo] = g.set_index('disorder')['ops_avg']

    if 'simple' not in series or 'complex' not in series:
        return

    diff = series['simple'] - series['complex']
    ymax = max(series['simple'].max(), series['complex'].max())
    for i in range(len(diff) - 1):
        if diff.iloc[i] * diff.iloc[i + 1] < 0:
            x1, x2 = diff.index[i], diff.index[i + 1]
            cx = x1 - diff.iloc[i] * (x2 - x1) / (diff.iloc[i + 1] - diff.iloc[i])
            ax.axvline(cx, color='gray', linestyle='--', linewidth=0.8)
            ax.text(cx + 0.02, ymax * 0.92, f'd≈{cx:.2f}', fontsize=7, color='gray')

    ax.set_title(f'ops vs disorder  (n={n})')
    ax.set_xlabel('disorder')
    ax.set_ylabel('ops')
    ax.legend()

def grade_graph(ax, n, thresholds):
    sub = df[df['size'] == n]
    ymax = max(df[df['size'] == n]['ops_avg'].max() * 1.1, thresholds['pass'] * 1.2)

    ax.axhspan(0,                    thresholds['excellent'], color='green',  alpha=0.12, label=f"excellent (<{thresholds['excellent']})")
    ax.axhspan(thresholds['excellent'], thresholds['good'],   color='yellow', alpha=0.15, label=f"good (<{thresholds['good']})")
    ax.axhspan(thresholds['good'],    thresholds['pass'],     color='orange', alpha=0.15, label=f"pass (<{thresholds['pass']})")
    ax.axhspan(thresholds['pass'],    ymax,                   color='red',    alpha=0.12, label='fail')

    for algo, g in sub.groupby('algorithm'):
        g = g.sort_values('disorder')
        ax.plot(g['disorder'], g['ops_avg'], marker='o', label=algo)

    ax.set_ylim(0, ymax)
    ax.set_title(f'challenge targets  (n={n})')
    ax.set_xlabel('disorder')
    ax.set_ylabel('ops')
    ax.legend(fontsize=7)

fig, axes = plt.subplots(5, 2, figsize=(13, 21))
(ax1, ax2), (ax3, ax4), (ax5, ax6), (ax7, ax8), (ax9, ax10) = axes
ax10.set_visible(False)

# --- size graph ---
sub = ops_df[ops_df['disorder'] == 0.5]
series = {}
for algo, g in sub.groupby('algorithm'):
    g = g.sort_values('size')
    ax1.plot(g['size'], g['ops_avg'], marker='o', label=algo)
    series[algo] = g.set_index('size')['ops_avg']

diff = series['simple'] - series['complex']
for i in range(len(diff) - 1):
    if diff.iloc[i] * diff.iloc[i + 1] < 0:
        x1, x2 = diff.index[i], diff.index[i + 1]
        cx = x1 - diff.iloc[i] * (x2 - x1) / (diff.iloc[i + 1] - diff.iloc[i])
        cy = series['complex'].iloc[i] + (series['complex'].iloc[i + 1] - series['complex'].iloc[i]) * (cx - x1) / (x2 - x1)
        ax1.annotate(f'crossover n≈{cx:.0f}', xy=(cx, cy), xytext=(cx + 80, cy + 8000),
                     arrowprops=dict(arrowstyle='->', color='gray'), fontsize=8, color='gray')
        ax1.axvline(cx, color='gray', linestyle='--', linewidth=0.8)
        break

sizes = sorted(sub['size'].unique())
ax1.set_xticks(sizes)
ax1.set_xticklabels([str(s) for s in sizes], rotation=60, ha='right', fontsize=7)
ax1.set_title('ops vs size  (disorder=0.5)')
ax1.set_xlabel('size')
ax1.set_ylabel('ops')
ax1.legend()

# --- disorder graphs ---
mark_disorder_crossovers(ax2, 10)
mark_disorder_crossovers(ax3, 100)
mark_disorder_crossovers(ax4, 500)

# --- challenge target graphs (row 3: official, row 4: extrapolated) ---
grade_graph(ax5, 100, {'pass': 2000, 'good': 1500, 'excellent': 700})
grade_graph(ax6, 500, {'pass': 12000, 'good': 8000, 'excellent': 5500})
grade_graph(ax7, 50,  {'pass': 930,  'good': 730,  'excellent': 290})
grade_graph(ax8, 10,  {'pass': 160,  'good': 135,  'excellent': 35})

ax7.set_title('challenge targets  (n=50)  [extrapolated]')
ax8.set_title('challenge targets  (n=10)  [extrapolated]')

# --- runtime vs size ---
sub = df[df['disorder'] == 0.5]
for algo, g in sub.groupby('algorithm'):
    g = g.sort_values('size')
    ax9.plot(g['size'], g['time_ms_avg'], marker='o', label=algo)

ax9.set_xticks(sizes)
ax9.set_xticklabels([str(s) for s in sizes], rotation=60, ha='right', fontsize=7)
ax9.set_title('runtime vs size  (disorder=0.5)')
ax9.set_xlabel('size')
ax9.set_ylabel('ms')
ax9.legend()

plt.tight_layout()
plt.savefig('ps_plot.png', dpi=150)
