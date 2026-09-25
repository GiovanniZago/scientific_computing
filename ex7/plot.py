import csv
import matplotlib.pyplot as plt

def read_columns(filename):
    with open(filename, newline="") as file:
        rows = list(csv.DictReader(file))
    return {name: [float(row[name]) for row in rows] for name in rows[0]}

if __name__ == "__main__":
    # 1. Running fraction of heads.
    coin = read_columns("coin.csv")
    plt.figure()
    plt.plot(coin["tosses"], coin["fraction_heads"], linewidth=1)
    plt.axhline(0.5, color="red", linestyle="--", label="expected fraction = 0.5")
    plt.xscale("log")
    plt.xlabel("Number of tosses")
    plt.ylabel("Fraction of heads")
    plt.title("Coin tosses and the law of large numbers")
    plt.legend()
    plt.tight_layout()
    plt.savefig("coin.png")
    plt.close()

    # 2. Absolute error in the estimate of pi.
    pi = read_columns("pi.csv")
    plt.figure()
    plt.plot(pi["N"], pi["absolute_error"], "o-")
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Number of points")
    plt.ylabel("Absolute error")
    plt.title("Monte Carlo estimate of pi")
    plt.tight_layout()
    plt.savefig("pi.png")
    plt.close()

    # 3. Histogram of U squared and its analytic density.
    square = read_columns("square_histogram.csv")
    plt.figure()
    plt.bar(square["y"], square["histogram_density"], width=1 / 80,
            alpha=0.6, label="Histogram")
    plt.plot(square["y"], square["analytic_density"], color="red",
            label="1 / (2 sqrt(y))")
    plt.xlabel("y")
    plt.ylabel("Density")
    plt.title("Y = U squared")
    plt.legend()
    plt.tight_layout()
    plt.savefig("square_histogram.png")
    plt.close()

    # 4. Exponential histogram and PDF.
    exponential = read_columns("exponential_histogram.csv")
    plt.figure()
    plt.bar(exponential["y"], exponential["histogram_density"], width=6 / 80,
            alpha=0.6, label="Histogram")
    plt.plot(exponential["y"], exponential["analytic_density"], color="red",
            label="1.5 exp(-1.5 y)")
    plt.xlabel("y")
    plt.ylabel("Density")
    plt.title("Exponential sample")
    plt.legend()
    plt.tight_layout()
    plt.savefig("exponential_histogram.png")
    plt.close()

    # 5. Empirical and exact CDF for the same exponential sample.
    cdf = read_columns("exponential_cdf.csv")
    plt.figure()
    plt.plot(cdf["y"], cdf["empirical_cdf"], label="Empirical CDF")
    plt.plot(cdf["y"], cdf["analytic_cdf"], color="red", linestyle="--",
            label="1 - exp(-1.5 y)")
    plt.xlim(0, 6)
    plt.xlabel("y")
    plt.ylabel("Cumulative probability")
    plt.title("Exponential CDF")
    plt.legend()
    plt.tight_layout()
    plt.savefig("exponential_cdf.png")
    plt.close()
