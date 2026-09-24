import pandas as pd
import matplotlib.pyplot as plt

if __name__ == "__main__":
    x_dict = {
        0: 0.1, 
        1: 0.5,
        2: 0.9,
        3: 0.99
    }

    df = pd.read_csv("out.csv")

    for i in x_dict:
        dfx = df[df.i == i]

        plt.figure(figsize = (11, 10))

        plt.subplot(2, 1, 1)
        plt.plot(dfx.L, dfx.err_rel_direct, linestyle = "--", marker = "o", label = "Bonnet/direct")
        plt.plot(dfx.L, dfx.err_rel_back, linestyle = "--", marker = "x", label = "Miller/backward")
        plt.grid()
        plt.yscale("log")
        plt.xlabel(r"$L$")
        plt.ylabel("Relative Error")
        plt.xlim([dfx.L.min(), dfx.L.max()])
        plt.title(f"P_L(x={x_dict[i]}) vs GNU GSL Reference")
        plt.legend(title="Method")

        plt.subplot(2, 1, 2)
        plt.plot(dfx.L, dfx.err_abs_direct, linestyle = "--", marker = "o", label = "Bonnet/direct")
        plt.plot(dfx.L, dfx.err_abs_back, linestyle = "--", marker = "x", label = "Miller/backward")
        plt.grid()
        plt.yscale("log")
        plt.xlabel(r"$L$")
        plt.ylabel("Absolute Error")
        plt.xlim([dfx.L.min(), dfx.L.max()])
        plt.title(f"P_L(x={x_dict[i]}) vs GNU GSL Reference")
        plt.legend(title="Method")

        plt.savefig(f"plot_{i}.png")

