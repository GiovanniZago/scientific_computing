#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* The same LCG used throughout rng.ipynb. */
static double uniform(uint32_t *state) {
    *state = 1664525u * (*state) + 1013904223u;
    return *state / 4294967296.0;
}

static int compare_doubles(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

int main(void) {
    const int n = 200000;
    const int bins = 80;
    const double lambda = 1.5;
    const double exp_max = 6.0;
    uint32_t state;
    FILE *file;
    int i;

    // 1. Coin tosses: the running fraction should approach 0.5.
    file = fopen("coin.csv", "w");
    if (file == NULL) return 1;
    fprintf(file, "tosses,fraction_heads\n");
    state = 123456789u;
    int heads = 0;
    for (i = 1; i <= 100000; i++) {
        if (uniform(&state) < 0.5) heads++;
        fprintf(file, "%d,%.10f\n", i, (double)heads / i);
    }
    fclose(file);
    printf("Coin tosses: %d / 100000 heads = %.6f\n", heads, heads / 100000.0);

    // 2. Estimate pi with two LCG streams, as in the notebook.
    file = fopen("pi.csv", "w");
    if (file == NULL) return 1;
    fprintf(file, "N,pi_estimate,absolute_error\n");
    uint32_t state_x = 123u, state_y = 987654321u;
    int inside = 0;
    for (i = 1; i <= 100000; i++) {
        double x = uniform(&state_x);
        double y = uniform(&state_y);
        if (x * x + y * y <= 1.0) inside++;
        if (i == 100 || i == 1000 || i == 10000 || i == 100000) {
            double estimate = 4.0 * inside / i;
            double error = fabs(estimate - acos(-1.0));
            fprintf(file, "%d,%.10f,%.10f\n", i, estimate, error);
            printf("Pi with N=%d: %.6f (error %.6f)\n", i, estimate, error);
        }
    }
    fclose(file);

    // 3. Histogram of Y=U^2, compared with f(y)=1/(2 sqrt(y)).
    int square_bins[80] = {0};
    state = 13579u;
    for (i = 0; i < n; i++) {
        double u = uniform(&state);
        double y = u * u;
        int bin = (int)(y * bins);
        square_bins[bin]++;
    }
    file = fopen("square_histogram.csv", "w");
    if (file == NULL) return 1;
    fprintf(file, "y,histogram_density,analytic_density\n");
    for (i = 0; i < bins; i++) {
        double width = 1.0 / bins;
        double center = (i + 0.5) * width;
        fprintf(file, "%.8f,%.8f,%.8f\n", center,
                square_bins[i] / (n * width), 1.0 / (2.0 * sqrt(center)));
    }
    fclose(file);

    // 4 and 5. Exponential histogram and empirical CDF from one sample.
    double *sample = malloc((size_t)n * sizeof(double));
    if (sample == NULL) return 1;
    int exp_bins[80] = {0};
    int above_range = 0;
    state = 246813579u;
    for (i = 0; i < n; i++) {
        double u = uniform(&state);
        double y = -log(1.0 - u) / lambda;
        sample[i] = y;
        if (y < exp_max) {
            int bin = (int)(y * bins / exp_max);
            exp_bins[bin]++;
        } else {
            above_range++;
        }
    }

    file = fopen("exponential_histogram.csv", "w");
    if (file == NULL) { free(sample); return 1; }
    fprintf(file, "y,histogram_density,analytic_density\n");
    for (i = 0; i < bins; i++) {
        double width = exp_max / bins;
        double center = (i + 0.5) * width;
        fprintf(file, "%.8f,%.8f,%.8f\n", center,
                exp_bins[i] / (n * width), lambda * exp(-lambda * center));
    }
    fclose(file);

    qsort(sample, (size_t)n, sizeof(double), compare_doubles);
    file = fopen("exponential_cdf.csv", "w");
    if (file == NULL) { free(sample); return 1; }
    fprintf(file, "y,empirical_cdf,analytic_cdf\n");
    for (i = 0; i < n; i++) {
        fprintf(file, "%.10f,%.10f,%.10f\n", sample[i],
                (double)(i + 1) / n, 1.0 - exp(-lambda * sample[i]));
    }
    fclose(file);
    free(sample);

    printf("Exponential values beyond y=6: %d / %d\n", above_range, n);
    return 0;
}
