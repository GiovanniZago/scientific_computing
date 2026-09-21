#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int N = 0;
    float a, x, y;

    printf("Enter N (number of vector entries)\n");
    if ((nread = getline(&line, &len, stdin)) != -1) {
        if (sscanf(line, "%d", &N) == 1) {
            printf("N set to: %d\n", N);
        } else {
            printf("Error: That was not a valid integer.\n");
        }
    }

    printf("Enter a (scalar float coefficient)\n");
    if ((nread = getline(&line, &len, stdin)) != -1) {
        if (sscanf(line, "%f", &a) == 1) {
            printf("a set to: %f\n", a);
        } else {
            printf("Error: That was not a valid float.\n");
        }
    }

    printf("Enter x (float vector element)\n");
    if ((nread = getline(&line, &len, stdin)) != -1) {
        if (sscanf(line, "%f", &x) == 1) {
            printf("x set to: %f\n", x);
        } else {
            printf("Error: That was not a valid float.\n");
        }
    }

    printf("Enter y (float vector element (bias))\n");
    if ((nread = getline(&line, &len, stdin)) != -1) {
        if (sscanf(line, "%f", &y) == 1) {
            printf("y set to: %f\n", y);
        } else {
            printf("Error: That was not a valid float.\n");
        }
    }

    free(line);
    
    // define reference
    float ref = a * x + y;
    printf("Reference value: %f\n", ref);

    // allocate vectors
    float *x_vec = malloc(N * sizeof(float));
    float *y_vec = malloc(N * sizeof(float));
    float *d_vec = malloc(N * sizeof(float));

    for (int i = 0; i < N; ++i) {
        x_vec[i] = x;
        y_vec[i] = y;
    }

    // vector addition
    for (int i = 0; i < N; ++i) {
        d_vec[i] = a * x_vec[i] + y_vec[i];
    }

    // result check
    int errors = 0;
    for (int i = 0; i < N; ++i) {
        if (fabs(d_vec[i] - ref) > 1e-8) {
            ++errors;
        }
    }

    // free memory
    free(x_vec);
    free(y_vec);
    free(d_vec);

    // print results
    printf("Errors found: %d\n", errors);

    return 0;
}