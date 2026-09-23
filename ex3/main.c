#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int N = 100;
    float a=32.1, b=5.99;

    printf("Enter N (integer matrix size)\n");
    if ((nread = getline(&line, &len, stdin)) != -1) {
        if (sscanf(line, "%d", &N) == 1) {
            printf("N set to: %d\n", N);
        } else {
            printf("Error: That was not a valid integer.\n");
        }
    }

    printf("Enter a (float)\n");
    if ((nread = getline(&line, &len, stdin)) != -1) {
        if (sscanf(line, "%f", &a) == 1) {
            printf("a set to: %f\n", a);
        } else {
            printf("Error: That was not a valid float.\n");
        }
    }

    printf("Enter b (float)\n");
    if ((nread = getline(&line, &len, stdin)) != -1) {
        if (sscanf(line, "%f", &b) == 1) {
            printf("b set to: %f\n", b);
        } else {
            printf("Error: That was not a valid float.\n");
        }
    }

    // time initialization
    clock_t begin_init = clock();

    // define our data as N pointers to the N (float) rows plus the total N * N float values
    // using calloc guarantees zero-initialization
    int data_size = N * N;
    float* A_data = (float*) calloc(data_size, sizeof(float));
    float* B_data = (float*) calloc(data_size, sizeof(float));
    float* C_data = (float*) calloc(data_size, sizeof(float));
    
    // the addresses of where the rows of matrixes A, B, C start
    float** A_rows = (float**) malloc(N * sizeof(float*));
    float** B_rows = (float**) malloc(N * sizeof(float*));
    float** C_rows = (float**) malloc(N * sizeof(float*));
    
    // set the correct value to each row pointer and initialize the A, B matrixes
    for (int i = 0; i < N; ++i) {
        // each element of A_rows is a pointer to float
        A_rows[i] = (A_data + i * N); 
        B_rows[i] = (B_data + i * N); 
        C_rows[i] = (C_data + i * N); 

        // fill values only on the diagonal
        A_rows[i][i] = a;
        B_rows[i][i] = b;
    }   

    // time initialization 
    clock_t end_init = clock();

    // time mmul
    clock_t begin_mmul = clock();

    // full matrix-matrix multiplication in the efficient way
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            for (int j = 0; j < N; ++j) {
                C_rows[i][j] += A_rows[i][k] * B_rows[k][j];
            }
        }
    }

    // time mmul
    clock_t end_mmul = clock();

    // time check
    clock_t begin_check = clock();

    // check that the sum of the element on the diagonal is N * a * b
    float sum = 0.0, ref = a * b;
    for (int i = 0; i < N; ++i) {
        sum += C_rows[i][i] / ref;
    }
    int check = (fabs(sum - (float) N) < 1e-8) ? 0 : 1;

    // time check
    clock_t end_check = clock();

    // free memory
    free(line);
    free(A_data);
    free(B_data);
    free(C_data);
    free(A_rows);
    free(B_rows);
    free(C_rows);

    // Final report
    double time_init = (double) (end_init - begin_init) / CLOCKS_PER_SEC;
    double time_mmul = (double) (end_mmul - begin_mmul) / CLOCKS_PER_SEC;
    double time_check = (double) (end_check - begin_check) / CLOCKS_PER_SEC;
    printf("\n\nMatrix-matrix multiplication done for N=%d, a=%f, b=%f\n", N, a, b);
    printf("t_init = %f s\n", time_init);
    printf("t_mmul = %f s\n", time_mmul);
    printf("t_check = %f s\n", time_check);

    if (check == 0) {
        printf("Test succeeded!\n");
    }
    else {
        printf("Test failed, Reference: %d, Sum: %f\n", N, sum);
    }
}
