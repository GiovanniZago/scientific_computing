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
    int L_max = 50, M = 500;
    double x[] = {0.1, 0.5, 0.9, 0.99};
    int N_points = sizeof(x) / sizeof(x[0]);

    // open file
    FILE* fptr = fopen("out.csv", "w");
    if (fptr == NULL) {
        printf("Error in opening file\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fptr, "i,L,res_gsl,res_direct,err_abs_direct,err_rel_direct,res_back,err_abs_back,err_rel_back\n");

    for (int i = 0; i < N_points; ++i) {
        for (int L = 0; L <= L_max; ++L) {
            // reference
            double res_gsl = gsl_sf_legendre_Pl(L, x[i]);

            // direct
            double res_direct = lpDirect(L, x[i]);
            double err_abs_direct = fabs(res_direct - res_gsl);
            double err_rel_direct = err_abs_direct / fabs(res_gsl);
            
            // backward
            double res_back = lpBackward(M, L, x[i]);
            double err_abs_back = fabs(res_back - res_gsl);
            double err_rel_back = err_abs_back / fabs(res_gsl);

            fprintf(fptr,
                    "%d,%d,%.7lf,%.7lf,%.7e,%.7e,%.7lf,%.7e,%.7e\n",
                    i,
                    L,
                    res_gsl,
                    res_direct,
                    err_abs_direct,
                    err_rel_direct,
                    res_back,
                    err_abs_back,
                    err_rel_back);
        }
    }
    
    fclose(fptr);

    return 0;
}