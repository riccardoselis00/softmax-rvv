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
