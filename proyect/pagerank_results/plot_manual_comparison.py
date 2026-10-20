from pathlib import Path

import matplotlib
import numpy as np
import pandas as pd


matplotlib.use("Agg")
import matplotlib.pyplot as plt

RESULTS_DIR = Path(__file__).resolve().parent
CSV_PATH = RESULTS_DIR / "manual_comparison.csv"
OUTPUT_PATH = RESULTS_DIR / "manual_comparison_improved.png"


def main() -> None:
    comparison_plot_df = (
        pd.read_csv(CSV_PATH)
        .sort_values("node")
        .reset_index(drop=True)
    )

    x = np.arange(len(comparison_plot_df))
    width = 0.36

    fig, (ax_scores, ax_diff) = plt.subplots(
        2,
        1,
        figsize=(9, 6),
        sharex=True,
        gridspec_kw={"height_ratios": [4, 1.4]},
    )

    custom_bars = ax_scores.bar(
        x - width / 2,
        comparison_plot_df["custom"],
        width=width,
        color="#4C78A8",
        edgecolor="black",
        linewidth=0.8,
        alpha=0.9,
        label="Custom",
        zorder=3,
    )
    networkx_bars = ax_scores.bar(
        x + width / 2,
        comparison_plot_df["networkx"],
        width=width,
        color="#F58518",
        edgecolor="black",
        linewidth=0.8,
        alpha=0.75,
        label="NetworkX",
        zorder=3,
    )

    ax_scores.set_ylabel("PageRank score")
    ax_scores.set_title("Custom PageRank vs NetworkX on the Manual Graph")
    ax_scores.legend()
    ax_scores.grid(True, axis="y", alpha=0.3, zorder=0)

    ax_scores.bar_label(custom_bars, fmt="%.4f", padding=3, fontsize=8)
    ax_scores.bar_label(networkx_bars, fmt="%.4f", padding=3, fontsize=8)

    ax_diff.bar(
        x,
        comparison_plot_df["abs_diff"],
        width=0.5,
        color="#54A24B",
        edgecolor="black",
        linewidth=0.8,
        alpha=0.85,
    )
    ax_diff.set_ylabel("|diff|")
    ax_diff.set_xlabel("Node")
    ax_diff.set_xticks(x)
    ax_diff.set_xticklabels(comparison_plot_df["node"])
    ax_diff.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
    ax_diff.grid(True, axis="y", alpha=0.3)

    plt.tight_layout()
    fig.savefig(OUTPUT_PATH, dpi=200, bbox_inches="tight")
    plt.close(fig)
    print(f"Saved improved comparison plot to: {OUTPUT_PATH}")


if __name__ == "__main__":
    main()
