#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <string.h>
#include <hdf5.h>
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

    int num_chunks = (N + chunk_size - 1) / chunk_size; // use integer division to floor

    float *x_vec = malloc(N * sizeof(float));
    float *y_vec = malloc(N * sizeof(float));
    float *d_vec = malloc(N * sizeof(float));
    double *partial_chunk_sum = calloc(num_chunks, sizeof(double));
    double sum = 0.0f;

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
            // compute vector addition
            d_vec[i] = a * x_vec[i] + y_vec[i];

            // update sums
            partial_chunk_sum[cidx] += d_vec[i];
            sum += d_vec[i];
        }
    }

    // result check
    float ref = a * x + y;
    int errors = 0;
    for (int i = 0; i < N; ++i) {
        if (fabs(d_vec[i] - ref) > 1e-8f) {
            ++errors;
        }
    }

    // sum all partial sums
    double chunk_sum = 0.0f;
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

    // write vector chunks and partial sums to hdf5 file
    hid_t file_id, chunks_dataspace_id, sums_dataspace_id, chunks_id, sums_id;
    herr_t file_status, chunks_status, sums_status;

    // create file
    file_id = H5Fcreate("chunks.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // dump chunks dataset
    // prepare the chunks data to be dumped
    // deal with padding to divide the d vector into equally-sized chunks
    float *chunks_data = calloc(num_chunks * chunk_size, sizeof(float));
    memcpy(chunks_data, d_vec, N * sizeof(float));

    // dump vector chunks
    hsize_t chunks_dims[2] = {num_chunks, chunk_size};
    chunks_dataspace_id = H5Screate_simple(2, chunks_dims, NULL);
    chunks_id = H5Dcreate2(file_id, "/chunks", H5T_IEEE_F32LE, chunks_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    chunks_status = H5Dwrite(chunks_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, chunks_data);
    
    // dump partial sums 
    hsize_t sums_dims[1] = {num_chunks};
    sums_dataspace_id = H5Screate_simple(1, sums_dims, NULL);
    sums_id = H5Dcreate2(file_id, "/partial_sums", H5T_IEEE_F64LE, sums_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sums_status = H5Dwrite(sums_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, partial_chunk_sum);
    
    // close file
    chunks_status = H5Dclose(chunks_id);
    sums_status = H5Dclose(sums_id);
    H5Sclose(chunks_dataspace_id);
    H5Sclose(sums_dataspace_id);
    file_status = H5Fclose(file_id);

    // free memory
    free(x_vec);
    free(y_vec);
    free(d_vec);
    free(partial_chunk_sum);

    return 0;
}