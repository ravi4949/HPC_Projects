
import csv
import matplotlib.pyplot as plt

points = []
pi_values = []
errors = []

with open("results.csv", "r") as file:
    reader = csv.DictReader(file)

    for row in reader:
        points.append(int(row["Points"]))
        pi_values.append(float(row["Estimated Pi"]))
        errors.append(float(row["Absolute Error"]))

plt.figure()
plt.semilogx(points, pi_values, marker="o")
plt.axhline(3.141592653589793, linestyle="--", label="Actual Pi")
plt.xlabel("Number of random points")
plt.ylabel("Estimated Pi")
plt.title("Monte Carlo Estimation of Pi")
plt.legend()
plt.grid(True)
plt.savefig("pi_estimate.png")
plt.show()

plt.figure()
plt.loglog(points, errors, marker="o")
plt.xlabel("Number of random points")
plt.ylabel("Absolute Error")
plt.title("Error versus Sample Size")
plt.grid(True)
plt.savefig("pi_error.png")
plt.show()
