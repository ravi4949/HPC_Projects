import csv
import math
import matplotlib.pyplot as plt

def read_csv(path):
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        return [row for row in reader]

serial_rows = read_csv("serial_results.csv")
points = [int(r["num_points"]) for r in serial_rows]
pi_estimates = [float(r["pi_estimate"]) for r in serial_rows]
errors = [float(r["error"]) for r in serial_rows]

plt.figure(figsize=(7, 5))
plt.semilogx(points, pi_estimates, marker="o", label="Estimated pi")
plt.axhline(y=math.pi, color="red", linestyle="--", label="True pi")
plt.xlabel("Number of points (log scale)")
plt.ylabel("Estimated pi")
plt.title("Number of Points vs. Estimated Pi")
plt.legend()
plt.grid(True, which="both", ls=":")
plt.tight_layout()
plt.savefig("plot_points_vs_pi.png", dpi=150)
plt.close()

plt.figure(figsize=(7, 5))
plt.loglog(points, errors, marker="o", color="darkorange")
plt.xlabel("Number of points (log scale)")
plt.ylabel("Absolute error (log scale)")
plt.title("Number of Points vs. Error")
plt.grid(True, which="both", ls=":")
plt.tight_layout()
plt.savefig("plot_points_vs_error.png", dpi=150)
plt.close()

thread_rows = read_csv("thread_scaling_results.csv")
thread_rows.sort(key=lambda r: int(r["num_threads"]))

threads = [int(r["num_threads"]) for r in thread_rows]
times = [float(r["time_seconds"]) for r in thread_rows]

fixed_n = thread_rows[0]["num_points"]
serial_time_match = next(
    (float(r["time_seconds"]) for r in serial_rows if r["num_points"] == fixed_n),
    None,
)
baseline_time = serial_time_match if serial_time_match is not None else times[0]

speedup = [baseline_time / t for t in times]
efficiency = [s / n for s, n in zip(speedup, threads)]

plt.figure(figsize=(7, 5))
plt.plot(threads, times, marker="o", color="green")
plt.xlabel("Number of threads")
plt.ylabel("Execution time (seconds)")
plt.title(f"Number of Threads vs. Execution Time (N={fixed_n})")
plt.grid(True, ls=":")
plt.tight_layout()
plt.savefig("plot_threads_vs_time.png", dpi=150)
plt.close()

plt.figure(figsize=(7, 5))
plt.plot(threads, speedup, marker="o", color="purple", label="Actual speedup")
plt.plot(threads, threads, linestyle="--", color="gray", label="Ideal (linear) speedup")
plt.xlabel("Number of threads")
plt.ylabel("Speedup (T_serial / T_parallel)")
plt.title(f"Number of Threads vs. Speedup (N={fixed_n})")
plt.legend()
plt.grid(True, ls=":")
plt.tight_layout()
plt.savefig("plot_threads_vs_speedup.png", dpi=150)
plt.close()

plt.figure(figsize=(7, 5))
plt.plot(threads, efficiency, marker="o", color="teal")
plt.axhline(y=1.0, color="gray", linestyle="--", label="Ideal efficiency = 1.0")
plt.xlabel("Number of threads")
plt.ylabel("Efficiency (Speedup / Threads)")
plt.title(f"Number of Threads vs. Efficiency (N={fixed_n})")
plt.ylim(0, 1.2)
plt.legend()
plt.grid(True, ls=":")
plt.tight_layout()
plt.savefig("plot_threads_vs_efficiency.png", dpi=150)
plt.close()

print("Saved plots:")
print("  plot_points_vs_pi.png")
print("  plot_points_vs_error.png")
print("  plot_threads_vs_time.png")
print("  plot_threads_vs_speedup.png")
print("  plot_threads_vs_efficiency.png")