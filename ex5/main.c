#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <gsl/gsl_sf_legendre.h>

double lpDirectRec(int L, double x) {
    if (L == 0) {
        return 1.0;
    }
    
    if (L == 1) {
        return x;
    }

    int M = L - 1;

    double c1 = (2.0 * M + 1.0) / (M + 1.0);
    double c2 = (-1.0) * M / (M + 1.0);

    double t1 = c1 * x * lpDirectRec(M, x);
    double t2 = c2 * lpDirectRec(M - 1, x);

    return t1 + t2;
}

double lpDirect(int L, double x) {
    double p_m = 1.0, p = x, p_n = 0.0;

    for (int i = 1; i < L; ++i) {
        double c1 = (2.0 * i + 1.0) / (i + 1.0);
        double c2 = (-1.0) * i / (i + 1.0);

        p_n = c1 * x * p + c2 * p_m;
        
        p_m = p;
        p = p_n;
    } 

    return p;
}

double lpBackward(int M, int L, double x) {
    if (M < L) {
        printf("Invalid M value. M should be greater than L (M > L).\n");
        exit(EXIT_FAILURE);
    }

    // intialize values
    double p = 0.0, p_n = 1.0, p_nn = 0.0;
    double p_l = 0.0;

    for (int i = M; i >= 1; --i) {
        double c1 = (2.0 * i + 1.0) / i;
        double c2 = (-1.0) * (i + 1.0) / i;

        p = c1 * x * p_n + c2 * p_nn;
        
        if (i == L + 1) {
            p_l = p;
        }

        p_nn = p_n;
        p_n = p;
    }

    return p_l / p;
}

int main(void) {
    double x = 0.5;
    int L = 5;
    
    // GNU GSL reference
    double res_gsl = gsl_sf_legendre_Pl(L, x);
    printf("(GSL) P_%d(%f) = %f\n", L, x, res_gsl);

    // direct recurrence
    double res_direct = lpDirect(L, x);
    printf("(Bonnet/Direct) P_%d(%f) = %f\n", L, x, res_direct);

    // backward recurrence
    int M = 500;
    double res_back = lpBackward(M, L, x);
    printf("(Miller/backward) P_%d(%f) = %f\n", L, x, res_back);

    return 0;
}