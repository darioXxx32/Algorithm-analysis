#!/usr/bin/env python3
"""
Plot generator for Adam Optimizer Analysis project.
Generates publication-quality plots for the IEEE report.

Usage: python plot_results.py
       (run from the Adam-Analysis directory)
"""
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import os
import numpy as np

# Use a clean style suitable for IEEE papers
matplotlib.rcParams.update({
    'font.size': 11,
    'font.family': 'serif',
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'legend.fontsize': 10,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'figure.dpi': 300,
    'savefig.dpi': 300,
    'savefig.bbox': 'tight',
})

COLORS = {'adam': '#2196F3', 'sgdm': '#FF9800', 'amsgrad': '#4CAF50'}
LABELS = {'adam': 'Adam', 'sgdm': 'SGD+Momentum', 'amsgrad': 'AMSGrad'}
OUTDIR = 'results/plots'
os.makedirs(OUTDIR, exist_ok=True)

# =====================================================================
# 1. Quadratic 2D — Objective + Gradient Norm
# =====================================================================
def plot_quadratic_2d():
    fig, axes = plt.subplots(1, 3, figsize=(15, 4.5))

    for opt, color in COLORS.items():
        path = f'results/csv/quadratic_2d_{opt}.csv'
        if not os.path.exists(path):
            continue
        df = pd.read_csv(path)

        axes[0].plot(df['iteration'], df['objective'], label=LABELS[opt], color=color, linewidth=1.5)
        axes[1].plot(df['iteration'], df['grad_norm'], label=LABELS[opt], color=color, linewidth=1.5)
        axes[2].plot(df['iteration'], df['dist_to_opt'], label=LABELS[opt], color=color, linewidth=1.5)

    axes[0].set_yscale('log')
    axes[0].set_title('Objective $f(\\mathbf{x})$ vs Iterations')
    axes[0].set_xlabel('Iteration')
    axes[0].set_ylabel('Objective (log scale)')
    axes[0].legend()
    axes[0].grid(True, alpha=0.3)

    axes[1].set_yscale('log')
    axes[1].set_title('Gradient Norm $\\|\\nabla f\\|$ vs Iterations')
    axes[1].set_xlabel('Iteration')
    axes[1].set_ylabel('$\\|\\nabla f\\|$ (log scale)')
    axes[1].legend()
    axes[1].grid(True, alpha=0.3)

    axes[2].set_yscale('log')
    axes[2].set_title('Distance to Optimum')
    axes[2].set_xlabel('Iteration')
    axes[2].set_ylabel('$\\|\\mathbf{x} - \\mathbf{x}^*\\|$ (log scale)')
    axes[2].legend()
    axes[2].grid(True, alpha=0.3)

    fig.suptitle('Experiment 1: Quadratic Function $f(\\mathbf{x}) = \\sum x_i^2$ (2D)', fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(f'{OUTDIR}/quadratic_2d_comparison.png')
    plt.close()
    print(f'Saved {OUTDIR}/quadratic_2d_comparison.png')


# =====================================================================
# 2. Quadratic 10D
# =====================================================================
def plot_quadratic_10d():
    fig, axes = plt.subplots(1, 3, figsize=(15, 4.5))

    for opt, color in COLORS.items():
        path = f'results/csv/quadratic_10d_{opt}.csv'
        if not os.path.exists(path):
            continue
        df = pd.read_csv(path)

        axes[0].plot(df['iteration'], df['objective'], label=LABELS[opt], color=color, linewidth=1.5)
        axes[1].plot(df['iteration'], df['grad_norm'], label=LABELS[opt], color=color, linewidth=1.5)
        axes[2].plot(df['iteration'], df['dist_to_opt'], label=LABELS[opt], color=color, linewidth=1.5)

    for ax in axes:
        ax.set_yscale('log')
        ax.set_xlabel('Iteration')
        ax.legend()
        ax.grid(True, alpha=0.3)

    axes[0].set_title('Objective $f(\\mathbf{x})$ vs Iterations')
    axes[0].set_ylabel('Objective (log scale)')
    axes[1].set_title('Gradient Norm $\\|\\nabla f\\|$ vs Iterations')
    axes[1].set_ylabel('$\\|\\nabla f\\|$ (log scale)')
    axes[2].set_title('Distance to Optimum')
    axes[2].set_ylabel('$\\|\\mathbf{x} - \\mathbf{x}^*\\|$ (log scale)')

    fig.suptitle('Experiment 1b: Quadratic Function $f(\\mathbf{x}) = \\sum x_i^2$ (10D)', fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(f'{OUTDIR}/quadratic_10d_comparison.png')
    plt.close()
    print(f'Saved {OUTDIR}/quadratic_10d_comparison.png')


# =====================================================================
# 3. Rosenbrock — Objective + Trajectory + Gradient Norm
# =====================================================================
def plot_rosenbrock():
    fig, axes = plt.subplots(1, 3, figsize=(15, 4.5))

    for opt, color in COLORS.items():
        path = f'results/csv/rosenbrock_{opt}.csv'
        if not os.path.exists(path):
            continue
        df = pd.read_csv(path)

        axes[0].plot(df['iteration'], df['objective'], label=LABELS[opt], color=color, linewidth=1.5)
        axes[1].plot(df['iteration'], df['grad_norm'], label=LABELS[opt], color=color, linewidth=1.5)

        # Trajectory in x-y space
        x_col = 'x0' if 'x0' in df.columns else 'x'
        y_col = 'x1' if 'x1' in df.columns else 'y'
        if x_col in df.columns and y_col in df.columns:
            axes[2].plot(df[x_col], df[y_col], label=LABELS[opt], color=color, linewidth=1.2,
                         marker='o', markersize=1.5, markevery=max(1, len(df)//30))

    axes[2].plot(1.0, 1.0, 'r*', markersize=15, label='Global Min (1,1)', zorder=10)

    axes[0].set_yscale('log')
    axes[0].set_title('Objective $f(x,y)$ vs Iterations')
    axes[0].set_xlabel('Iteration')
    axes[0].set_ylabel('Objective (log scale)')
    axes[0].legend()
    axes[0].grid(True, alpha=0.3)

    axes[1].set_yscale('log')
    axes[1].set_title('Gradient Norm $\\|\\nabla f\\|$ vs Iterations')
    axes[1].set_xlabel('Iteration')
    axes[1].set_ylabel('$\\|\\nabla f\\|$ (log scale)')
    axes[1].legend()
    axes[1].grid(True, alpha=0.3)

    axes[2].set_title('Parameter Trajectory in $(x, y)$ Space')
    axes[2].set_xlabel('$x$')
    axes[2].set_ylabel('$y$')
    axes[2].legend(fontsize=8)
    axes[2].grid(True, alpha=0.3)

    fig.suptitle('Experiment 2: Rosenbrock Function $f(x,y) = (1-x)^2 + 100(y-x^2)^2$', fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(f'{OUTDIR}/rosenbrock_comparison.png')
    plt.close()
    print(f'Saved {OUTDIR}/rosenbrock_comparison.png')


# =====================================================================
# 4. Rastrigin
# =====================================================================
def plot_rastrigin():
    fig, axes = plt.subplots(1, 3, figsize=(15, 4.5))

    for opt, color in COLORS.items():
        path = f'results/csv/rastrigin_{opt}.csv'
        if not os.path.exists(path):
            continue
        df = pd.read_csv(path)

        axes[0].plot(df['iteration'], df['objective'], label=LABELS[opt], color=color, linewidth=1.5)
        axes[1].plot(df['iteration'], df['grad_norm'], label=LABELS[opt], color=color, linewidth=1.5)

        x_col = 'x0' if 'x0' in df.columns else 'x'
        y_col = 'x1' if 'x1' in df.columns else 'y'
        if x_col in df.columns and y_col in df.columns:
            axes[2].plot(df[x_col], df[y_col], label=LABELS[opt], color=color, linewidth=1.2,
                         marker='o', markersize=1.5, markevery=max(1, len(df)//30))

    axes[2].plot(0.0, 0.0, 'r*', markersize=15, label='Global Min (0,0)', zorder=10)

    axes[0].set_yscale('log')
    axes[0].set_title('Objective vs Iterations')
    axes[0].set_xlabel('Iteration')
    axes[0].set_ylabel('Objective (log scale)')
    axes[0].legend()
    axes[0].grid(True, alpha=0.3)

    axes[1].set_yscale('log')
    axes[1].set_title('Gradient Norm vs Iterations')
    axes[1].set_xlabel('Iteration')
    axes[1].set_ylabel('$\\|\\nabla f\\|$ (log scale)')
    axes[1].legend()
    axes[1].grid(True, alpha=0.3)

    axes[2].set_title('Parameter Trajectory')
    axes[2].set_xlabel('$x$')
    axes[2].set_ylabel('$y$')
    axes[2].legend(fontsize=8)
    axes[2].grid(True, alpha=0.3)

    fig.suptitle('Experiment 3: Rastrigin Function (2D)', fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(f'{OUTDIR}/rastrigin_comparison.png')
    plt.close()
    print(f'Saved {OUTDIR}/rastrigin_comparison.png')


# =====================================================================
# 5. Reddi Counterexample
# =====================================================================
def plot_reddi():
    fig, ax = plt.subplots(1, 1, figsize=(8, 5))

    for opt, color in [('adam', COLORS['adam']), ('amsgrad', COLORS['amsgrad'])]:
        path = f'results/csv/reddi_{opt}.csv'
        if not os.path.exists(path):
            continue
        df = pd.read_csv(path)
        ax.plot(df['iteration'], df['x'], label=LABELS[opt], color=color, linewidth=1.5)

    ax.axhline(y=-1.0, color='red', linestyle='--', linewidth=1.2, label='Optimal $x^* = -1$')
    ax.axhline(y=1.0, color='gray', linestyle=':', linewidth=1, label='$x = 1$ (Divergence)')

    ax.set_title("Reddi's 1D Counterexample: Adam vs AMSGrad", fontsize=13)
    ax.set_xlabel('Iteration')
    ax.set_ylabel('Parameter $x$')
    ax.legend()
    ax.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(f'{OUTDIR}/reddi_counterexample.png')
    plt.close()
    print(f'Saved {OUTDIR}/reddi_counterexample.png')


# =====================================================================
# 6. Learning Rate Sensitivity
# =====================================================================
def plot_lr_sweep():
    path = 'results/csv/lr_sweep_rosenbrock.csv'
    if not os.path.exists(path):
        return

    df = pd.read_csv(path)

    fig, axes = plt.subplots(1, 3, figsize=(15, 4.5))

    # Iterations to converge
    axes[0].semilogx(df['learning_rate'], df['final_iter'], 'o-', color=COLORS['adam'], linewidth=1.5, markersize=6)
    axes[0].set_title('Iterations to Convergence')
    axes[0].set_xlabel('Learning Rate $\\alpha$')
    axes[0].set_ylabel('Iterations')
    axes[0].grid(True, alpha=0.3)

    # Final objective
    axes[1].loglog(df['learning_rate'], df['final_objective'], 's-', color=COLORS['adam'], linewidth=1.5, markersize=6)
    axes[1].set_title('Final Objective Value')
    axes[1].set_xlabel('Learning Rate $\\alpha$')
    axes[1].set_ylabel('$f(\\mathbf{x}_{final})$')
    axes[1].grid(True, alpha=0.3)

    # Execution time
    axes[2].semilogx(df['learning_rate'], df['elapsed_ms'], 'D-', color=COLORS['adam'], linewidth=1.5, markersize=6)
    axes[2].set_title('Execution Time')
    axes[2].set_xlabel('Learning Rate $\\alpha$')
    axes[2].set_ylabel('Time (ms)')
    axes[2].grid(True, alpha=0.3)

    fig.suptitle('Learning Rate Sensitivity: Adam on Rosenbrock', fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(f'{OUTDIR}/lr_sweep_rosenbrock.png')
    plt.close()
    print(f'Saved {OUTDIR}/lr_sweep_rosenbrock.png')

    # Also plot individual trajectories
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))
    lr_colors = {0.001: '#E91E63', 0.01: '#2196F3', 0.1: '#4CAF50'}
    for lr, color in lr_colors.items():
        fname = f'results/csv/rosenbrock_adam_lr{lr:.4f}.csv'
        if not os.path.exists(fname):
            continue
        df_lr = pd.read_csv(fname)
        axes[0].plot(df_lr['iteration'], df_lr['objective'], label=f'$\\alpha = {lr}$', color=color, linewidth=1.5)

        x_col = 'x0' if 'x0' in df_lr.columns else 'x'
        y_col = 'x1' if 'x1' in df_lr.columns else 'y'
        if x_col in df_lr.columns and y_col in df_lr.columns:
            axes[1].plot(df_lr[x_col], df_lr[y_col], label=f'$\\alpha = {lr}$', color=color, linewidth=1.2,
                         marker='o', markersize=1.5, markevery=max(1, len(df_lr)//20))

    axes[0].set_yscale('log')
    axes[0].set_title('Objective vs Iterations')
    axes[0].set_xlabel('Iteration')
    axes[0].set_ylabel('Objective (log scale)')
    axes[0].legend()
    axes[0].grid(True, alpha=0.3)

    axes[1].plot(1.0, 1.0, 'r*', markersize=15, label='Global Min (1,1)', zorder=10)
    axes[1].set_title('Parameter Trajectory')
    axes[1].set_xlabel('$x$')
    axes[1].set_ylabel('$y$')
    axes[1].legend(fontsize=9)
    axes[1].grid(True, alpha=0.3)

    fig.suptitle('Adam: Effect of Learning Rate on Rosenbrock', fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(f'{OUTDIR}/lr_comparison_rosenbrock.png')
    plt.close()
    print(f'Saved {OUTDIR}/lr_comparison_rosenbrock.png')


# =====================================================================
# 7. Summary bar chart
# =====================================================================
def plot_summary():
    path = 'results/csv/summary.csv'
    if not os.path.exists(path):
        return

    df = pd.read_csv(path)
    # Filter to main experiments (not reddi since it has different metrics)
    main_funcs = ['quadratic_2d', 'quadratic_10d', 'rosenbrock', 'rastrigin']
    df_main = df[df['function'].isin(main_funcs)]

    fig, axes = plt.subplots(1, 2, figsize=(14, 5))

    # Iterations to converge
    funcs = df_main['function'].unique()
    x = np.arange(len(funcs))
    width = 0.25
    opt_names = ['Adam', 'SGD+Momentum', 'AMSGrad']

    for i, opt in enumerate(opt_names):
        vals = []
        for func in funcs:
            row = df_main[(df_main['optimizer'] == opt) & (df_main['function'] == func)]
            vals.append(row['final_iter'].values[0] if len(row) > 0 else 0)
        bars = axes[0].bar(x + i*width, vals, width, label=opt,
                           color=list(COLORS.values())[i], alpha=0.85)

    axes[0].set_title('Iterations to Convergence')
    axes[0].set_xticks(x + width)
    axes[0].set_xticklabels([f.replace('_', '\n') for f in funcs], fontsize=9)
    axes[0].set_ylabel('Iterations')
    axes[0].legend()
    axes[0].grid(True, alpha=0.3, axis='y')

    # Execution time
    for i, opt in enumerate(opt_names):
        vals = []
        for func in funcs:
            row = df_main[(df_main['optimizer'] == opt) & (df_main['function'] == func)]
            vals.append(row['elapsed_ms'].values[0] if len(row) > 0 else 0)
        axes[1].bar(x + i*width, vals, width, label=opt,
                    color=list(COLORS.values())[i], alpha=0.85)

    axes[1].set_title('Execution Time')
    axes[1].set_xticks(x + width)
    axes[1].set_xticklabels([f.replace('_', '\n') for f in funcs], fontsize=9)
    axes[1].set_ylabel('Time (ms)')
    axes[1].legend()
    axes[1].grid(True, alpha=0.3, axis='y')

    fig.suptitle('Summary: Optimizer Comparison Across Test Functions', fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(f'{OUTDIR}/summary_comparison.png')
    plt.close()
    print(f'Saved {OUTDIR}/summary_comparison.png')


# =====================================================================
# MAIN
# =====================================================================
if __name__ == '__main__':
    print("Generating plots...")
    plot_quadratic_2d()
    plot_quadratic_10d()
    plot_rosenbrock()
    plot_rastrigin()
    plot_reddi()
    plot_lr_sweep()
    plot_summary()
    print("\nAll plots generated.")
