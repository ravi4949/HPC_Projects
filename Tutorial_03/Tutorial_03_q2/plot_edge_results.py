import csv
import matplotlib.pyplot as plt

def read_csv(path):
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        return [row for row in reader]

serial_rows = read_csv("serial_edge_results.csv")
serial_time = float(serial_rows[0]["time_seconds"])

parallel_rows = read_csv("parallel_edge_results.csv")
parallel_rows.sort(key=lambda r: int(r["num_threads"]))

threads = [int(r["num_threads"]) for r in parallel_rows]
times = [float(r["time_seconds"]) for r in parallel_rows]
speedup = [serial_time / t for t in times]
efficiency = [s / n for s, n in zip(speedup, threads)]

plt.figure(figsize=(7, 5))
plt.plot(threads, times, marker="o", color="green")
plt.xlabel("Number of threads")
plt.ylabel("Execution time (seconds)")
plt.title("Number of Threads vs. Parallel Execution Time")
plt.grid(True, ls=":")
plt.tight_layout()
plt.savefig("plot_edge_threads_vs_time.png", dpi=150)
plt.close()

plt.figure(figsize=(7, 5))
plt.plot(threads, speedup, marker="o", color="purple", label="Actual speedup")
plt.plot(threads, threads, linestyle="--", color="gray", label="Ideal (linear) speedup")
plt.xlabel("Number of threads")
plt.ylabel("Speedup (T_serial / T_parallel)")
plt.title("Number of Threads vs. Speedup")
plt.legend()
plt.grid(True, ls=":")
plt.tight_layout()
plt.savefig("plot_edge_threads_vs_speedup.png", dpi=150)
plt.close()

plt.figure(figsize=(7, 5))
plt.plot(threads, efficiency, marker="o", color="teal")
plt.axhline(y=1.0, color="gray", linestyle="--", label="Ideal efficiency = 1.0")
plt.xlabel("Number of threads")
plt.ylabel("Efficiency (Speedup / Threads)")
plt.title("Number of Threads vs. Efficiency")
plt.ylim(0, 1.2)
plt.legend()
plt.grid(True, ls=":")
plt.tight_layout()
plt.savefig("plot_edge_threads_vs_efficiency.png", dpi=150)
plt.close()

print("Saved plots:")
print("  plot_edge_threads_vs_time.png")
print("  plot_edge_threads_vs_speedup.png")
print("  plot_edge_threads_vs_efficiency.png")