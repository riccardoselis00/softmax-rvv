def plot_stats(metric, *csv_files):
    import pandas as pd
    import matplotlib.pyplot as plt
    from pathlib import Path

    # Flatten inputs
    files = []
    for arg in csv_files:
        if isinstance(arg, (list, tuple, set)):
            files.extend(arg)
        else:
            files.append(arg)

    fig, ax = plt.subplots(figsize=(8, 4))

    for f in files:
        df = pd.read_csv(f)
        needed = {'dimension', 'file', metric}
        missing = [c for c in needed if c not in df.columns]
        if missing:
            raise ValueError(f"{f} is missing columns: {missing}")

        df = df.copy()
        df['dimension'] = pd.to_numeric(df['dimension'], errors='coerce')
        g = (df.dropna(subset=['dimension'])
               .groupby('dimension', as_index=False)[metric].mean()
               .sort_values('dimension'))

        ax.plot(g['dimension'], g[metric], marker='o', label=Path(f).stem)

    ax.set_xlabel('Dimension')
    ax.set_ylabel(metric)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend()
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    import argparse
    p = argparse.ArgumentParser(description="Plot one metric from one or more CSVs.")
    p.add_argument("metric", help="Full metric name, e.g. board.cache_hierarchy.membus.respLayer1.utilization")
    p.add_argument("csvs", nargs="+", help="One or more CSV files (same schema)")
    args = p.parse_args()
    # assumes your plot_stats(metric, *csvs) signature
    plot_stats(args.metric, *args.csvs)