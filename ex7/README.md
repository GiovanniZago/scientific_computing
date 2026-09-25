# Exercise 7: random number generators

Compile and run from this directory:

```sh
gcc -std=c11 -Wall -Wextra -O2 main.c -lm -o main.out
./main.out
python3 plot.py
```

The program uses the LCG from `rng.ipynb` for all five exercises. It writes:

| File | Contents |
| --- | --- |
| `coin.csv` | Toss number and running fraction of heads |
| `pi.csv` | Sample size, estimate of pi, and absolute error |
| `square_histogram.csv` | Histogram density and analytic density for `Y = U^2` |
| `exponential_histogram.csv` | Histogram density and analytic exponential density |
| `exponential_cdf.csv` | Sorted sample, empirical CDF, and exact CDF |

`plot.py` reads these files and saves `coin.png`, `pi.png`, `square_histogram.png`, `exponential_histogram.png`, and `exponential_cdf.png`. It requires Matplotlib. Run it from `ex7` after running `main.out`.

The histograms have 80 bins. The exponential histogram covers `0 <= y < 6`; the program reports how many samples lie above that range. The empirical CDF includes the full sample.

With the fixed seeds in `main.c`, the final heads fraction is `0.501010`. The pi errors at `N = 100, 1000, 10000, 100000` are about `0.1016, 0.0256, 0.0032, 0.0036`. The error generally gets smaller, but random sampling does not guarantee a decrease at every step. The squared-uniform density rises sharply near zero, so its first histogram bin is sensitive to bin width. The exponential histogram and CDF can be compared directly with `1.5 exp(-1.5 y)` and `1 - exp(-1.5 y)`.
