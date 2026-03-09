import time
import random
import math
import argparse
import sys
from statistics import mean, stdev
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def exhaustive_knapsack(weights, values, capacity):
    n = len(weights)
    best_value = 0
    best_mask = 0
    for mask in range(1 << n):
        w = 0
        v = 0
        m = mask
        i = 0
        while m:
            if m & 1:
                w += weights[i]
                if w > capacity:
                    v = -1
                    break
                v += values[i]
            i += 1
            m >>= 1
        if v >= 0 and v > best_value:
            best_value = v
            best_mask = mask
    return best_value, best_mask

def run_instance_once(n, seed=None):
    if seed is not None:
        random.seed(seed)
    weights = [random.randint(1, 100) for _ in range(n)]
    values  = [random.randint(1, 100) for _ in range(n)]
    capacity = sum(weights) // 2
    t0 = time.perf_counter()
    best_value, best_mask = exhaustive_knapsack(weights, values, capacity)
    t1 = time.perf_counter()
    return {
        "n": n,
        "weights": weights,
        "values": values,
        "capacity": capacity,
        "time_s": t1 - t0,
        "best_value": best_value
    }

def experiment(n_min=4, n_max=22, reps=3, base_seed=0, csv_out="knapsack_exhaustive_results.csv", max_seconds=600.0):
    rows = []
    for n in range(n_min, n_max + 1):
        times = []
        best_values = []
        print(f"\n=== Running n = {n} (reps={reps}) ===")
        for r in range(reps):
            seed = base_seed + n * 1000 + r
            res = run_instance_once(n, seed=seed)
            times.append(res["time_s"])
            best_values.append(res["best_value"])
            print(f"  rep {r+1}/{reps} -> time: {res['time_s']:.6f}s  best_value: {res['best_value']}")
        avg = mean(times)
        sd = stdev(times) if reps > 1 else 0.0
        rows.append({
            "n": n,
            "subsets": 1 << n,
            "time_mean_s": avg,
            "time_std_s": sd,
            "best_value_mean": mean(best_values)
        })
        if avg > max_seconds:
            print(f"\nMean time {avg:.2f}s for n={n} exceeds max_seconds={max_seconds}s. Stopping experiment.")
            break
    df = pd.DataFrame(rows)
    df.to_csv(csv_out, index=False)
    print(f"\nSaved results to CSV: {csv_out}")
    return df

def fit_exponential_and_predict(df, target_seconds=600.0):
    df2 = df[df["time_mean_s"] > 0].copy()
    if df2.empty:
        raise ValueError("No positive time measurements available for fitting.")
    x = df2["n"].values
    y = df2["time_mean_s"].values
    y = np.maximum(y, 1e-12)
    lny = np.log(y)
    coeffs = np.polyfit(x, lny, 1)
    b = coeffs[0]
    a = coeffs[1]
    if abs(b) < 1e-12:
        n_pred = float("inf")
    else:
        n_pred = (math.log(target_seconds) - a) / b
    model_fn = lambda n_val: math.exp(a + b * n_val)
    return {"a": a, "b": b, "n_pred": n_pred, "model_fn": model_fn}

def plot_results(df, model=None, png_out="knapsack_times.png"):
    if df.empty:
        print("No data to plot.")
        return
    plt.figure(figsize=(8,5))
    plt.errorbar(df["n"], df["time_mean_s"], yerr=df["time_std_s"], fmt='o', capsize=3, label='measured (mean ± sd)')
    if model is not None:
        ns = np.arange(df["n"].min(), df["n"].max() + 1)
        preds = [model(nv) for nv in ns]
        plt.plot(ns, preds, linestyle='--', marker='x', label='exponential fit')
    plt.yscale('log')
    plt.xlabel('n (number of items)')
    plt.ylabel('Time (s) - log scale')
    plt.title('Exhaustive 0/1 Knapsack: time vs n')
    plt.grid(True, which="both", ls=":", linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(png_out, dpi=150)
    print(f"Saved plot to: {png_out}")
    plt.show()

def main(argv=None):
    parser = argparse.ArgumentParser(description="Knapsack exhaustive experiment")
    parser.add_argument("--nmin", type=int, default=4, help="Minimum n")
    parser.add_argument("--nmax", type=int, default=22, help="Maximum n to test")
    parser.add_argument("--reps", type=int, default=3, help="Repetitions per n")
    parser.add_argument("--seed", type=int, default=0, help="Base random seed")
    parser.add_argument("--csv", type=str, default="knapsack_exhaustive_results.csv", help="CSV output file")
    parser.add_argument("--png", type=str, default="knapsack_times.png", help="PNG output file")
    parser.add_argument("--max-time", type=float, default=600.0, help="Stop if mean time for an n exceeds this (seconds)")
    parser.add_argument("--target-time", type=float, default=600.0, help="Target time for prediction (seconds)")

    # Use parse_known_args so kernel-injected args (e.g. -f /path/kernel-...) are ignored.
    if argv is None:
        args, unknown = parser.parse_known_args()
    else:
        args = parser.parse_args(argv)

    print("Knapsack exhaustive experiment")
    print(f"Parameters: nmin={args.nmin}, nmax={args.nmax}, reps={args.reps}, seed={args.seed}")
    print("Warning: exhaustive search grows exponentially; choose nmax accordingly.")

    df = experiment(n_min=args.nmin, n_max=args.nmax, reps=args.reps,
                    base_seed=args.seed, csv_out=args.csv, max_seconds=args.max_time)

    if df.empty:
        print("No results recorded. Exiting.")
        return

    try:
        fit = fit_exponential_and_predict(df, target_seconds=args.target_time)
        a = fit["a"]
        b = fit["b"]
        n_pred = fit["n_pred"]
        print("\nFitted model: ln(time) = a + b * n")
        print(f"  a = {a:.6f}, b = {b:.6f}")
        if math.isfinite(n_pred):
            print(f"Estimated n for T={args.target_time}s => n_pred = {n_pred:.3f} (floor -> {int(math.floor(n_pred))})")
        else:
            print("b is too small; cannot predict n (division by near-zero).")
    except Exception as e:
        print("Could not fit model:", e)
        fit = None

    model_fn = fit["model_fn"] if fit is not None else None
    plot_results(df, model=model_fn, png_out=args.png)

if __name__ == "__main__":
    # pass None so main uses parse_known_args() on sys.argv (robust in notebooks)
    main()