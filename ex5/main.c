#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>

double lp_direct(int L, double x) {
    if (L == 0) {
        return 1.0;
    }
    
    if (L == 1) {
        return x;
    }

    int M = L - 1;

    double c1 = (2.0 * M + 1.0) / (M + 1.0);
    double c2 = (-1.0) * M / (M + 1.0);

    double t1 = c1 * x * lp_direct(M, x);
    double t2 = c2 * lp_direct(M - 1, x);

    return t1 + t2;
}

double lp_backward(int M, int L, double x) {
    if (M < L) {
        printf("Invalid parameters: M is supposed to be greater than L.\n");
        exit(EXIT_FAILURE);
    }

    if (L == M + 1) {
        return 0.0;
    }

    if (L == M) {
        return 1.0;
    }

    int N = L + 1;

    double c1 = (2.0 * N + 1.0) / N;
    double c2 = (-1.0) * (N + 1.0) / N;

    double t1 = c1 * x * lp_backward(M, N, x);
    double t2 = c2 * lp_backward(M, N + 1, x);

    return t1 + t2;
}

int main(void) {
    double x = 1.5;
    int L = 5, M = 50;

    // direct recurrence
    double res_direct = lp_direct(L, x);
    printf("P_%d(%f) = %f\n", L, x, res_direct);

    // backward recurrence
    double res_back = lp_backward(M, L, x);
    double c_norm = lp_backward(M, 0, x);
    res_back = 
    printf("P_%d(%f) = %f\n", L, x, res_back);

    return 0;
}