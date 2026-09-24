#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <string.h>
#include "parser/parser.h"

int main(void) {
    int N, chunk_size;
    float x, y, a;
    
    // parse config file
    config_option_t co;
    if ((co = read_config_file("conf.conf")) == NULL) {
        printf("Error in reading and loading config file\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        printf("Key: %s\tValue: %s\n", co->key, co->value);

        if (strcmp(co->key, "N") == 0) {
            sscanf(co->value, "%d", &N);
        } else if (strcmp(co->key, "chunk_size") == 0) {
            sscanf(co->value, "%d", &chunk_size);
        } else if (strcmp(co->key, "x") == 0) {
            sscanf(co->value, "%f", &x);
        } else if (strcmp(co->key, "y") == 0) {
            sscanf(co->value, "%f", &y);
        } else if (strcmp(co->key, "a") == 0) {
            sscanf(co->value, "%f", &a);
        } else {
            printf("Unexpected key\n");
            exit(EXIT_FAILURE);
        }

        if (co->prev != NULL) {
            co = co->prev;
        } else {
            break;
        }
    }

    printf("\n\nValues set:\nN = %d, chunk_size = %d, x = %f, y = %f, a = %f\n", N, chunk_size, x, y, a);

    float ref = a * x + y;
    int num_chunks = (N + chunk_size - 1) / chunk_size; // use integer division to floor

    float *x_vec = malloc(N * sizeof(float));
    float *y_vec = malloc(N * sizeof(float));
    float *d_vec = malloc(N * sizeof(float));
    float *partial_chunk_sum = calloc(num_chunks, sizeof(float));

    // initialize vectors
    for (int i = 0; i < N; ++i) {
        x_vec[i] = x;
        y_vec[i] = y;
        d_vec[i] = 0.0f;
    }

    // blockwise vector addition
    for (int cidx = 0; cidx < num_chunks; ++cidx) {
        int start = cidx * chunk_size;
        int end = (cidx + 1) * chunk_size;

        if (end > N) end = N;

        for (int i = start; i < end; ++i) {
            d_vec[i] = a * x_vec[i] + y_vec[i];
            partial_chunk_sum[cidx] += d_vec[i];
        }
    }

    // result check
    float sum = 0;
    int errors = 0;
    for (int i = 0; i < N; ++i) {
        if (fabs(d_vec[i] - ref) > 1e-8f) {
            ++errors;
        }

        sum += d_vec[i];
    }

    // sum all partial sums
    float chunk_sum = 0.0f;
    for (int cidx = 0; cidx < num_chunks; ++cidx) {
        chunk_sum += partial_chunk_sum[cidx];
    }

    // check on sums
    int error_sum = (fabs(sum - chunk_sum) > 1e-8f) ? 1 : 0;

    // print results
    printf("\nErrors found: %d\n", errors);
    
    if (error_sum) {
        printf("Sum check failed: %f (element sum), %f (chunk sum)\n", sum, chunk_sum);
    } else {
        printf("Element-wise and chunk-wise sums are in aggreement\n");
    }

    // free memory
    free(x_vec);
    free(y_vec);
    free(d_vec);
    free(partial_chunk_sum);

    return 0;
}