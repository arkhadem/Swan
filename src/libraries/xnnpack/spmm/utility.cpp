#include "spmm.hpp"

#include "xnnpack.hpp"

#include "swan.hpp"

#include "utility.hpp"

template <typename T>
int spmm_config_init(size_t cache_size,
                     config_t *&config) {

    spmm_config_t *spmm_config = (spmm_config_t *)config;

    // configuration
    int layer_count = LAYER_COUNT;
    float sparsity = 0.8;

    alloc_1D<spmm_config_t>(1, spmm_config);
    alloc_1D<int>(layer_count, spmm_config->M);
    alloc_1D<int>(layer_count, spmm_config->N);

    // in/output versions
    size_t input_size = 0;
    size_t output_size = 0;

    spmm_config->layer_count = layer_count;
    spmm_config->sparsity = sparsity;
    for (int i = 0; i < layer_count; i++) {
        spmm_config->M[i] = layer_infos[i][M_IDX];
        spmm_config->N[i] = layer_infos[i][N_IDX];
        input_size += layer_infos[i][K_IDX] * layer_infos[i][M_IDX] * sizeof(T);
        input_size += sparsity * (layer_infos[i][N_IDX] * layer_infos[i][K_IDX]) * sizeof(T);
        output_size += layer_infos[i][N_IDX] * layer_infos[i][M_IDX] * sizeof(T);
    }
    spmm_config->granularity = 1;

    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)spmm_config;

    return count;
}

template <typename T>
void spmm_input_init(int count,
                     config_t *config,
                     input_t **&input) {

    spmm_config_t *spmm_config = (spmm_config_t *)config;
    spmm_input_t<T> **spmm_input = (spmm_input_t<T> **)input;

    // configuration
    int layer_count = spmm_config->layer_count;
    float sparsity = 0.8;

    // initializing input versions
    alloc_1D<spmm_input_t<T> *>(count, spmm_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<spmm_input_t<T>>(1, spmm_input[i]);

        alloc_1D<T *>(layer_count, spmm_input[i]->input_matrix);
        alloc_1D<T *>(layer_count, spmm_input[i]->bias_vector);
        alloc_1D<T *>(layer_count, spmm_input[i]->weight_value_vector);
        alloc_1D<int32_t *>(layer_count, spmm_input[i]->weight_IDX_vector);
        alloc_1D<uint32_t *>(layer_count, spmm_input[i]->weight_NNZ_vector);
        init_alloc_1D<T>(layer_count, spmm_input[i]->min);
        init_alloc_1D<T>(layer_count, spmm_input[i]->max);

        T *weight_matrix;
        for (int layer = 0; layer < layer_count; layer++) {
            int M = layer_infos[layer][M_IDX];
            int N = layer_infos[layer][N_IDX];
            int K = layer_infos[layer][K_IDX];

            init_alloc_1D<T>(K * M, spmm_input[i]->input_matrix[layer]);
            init_alloc_1D<T>(N, spmm_input[i]->bias_vector[layer]);

            int num_nonzeroes = (float)(N * K) * (1.0000 - sparsity) + 1;

            sparse_init_alloc_1D<T>(N * K, sparsity, weight_matrix);
            alloc_1D<T>(num_nonzeroes, spmm_input[i]->weight_value_vector[layer]);
            alloc_1D<int32_t>(num_nonzeroes, spmm_input[i]->weight_IDX_vector[layer]);
            alloc_1D<uint32_t>(N + 1, spmm_input[i]->weight_NNZ_vector[layer]);

            uint32_t nnz = 0;
            for (uint32_t n = 0; n < N; n++) {
                spmm_input[i]->weight_NNZ_vector[layer][n] = nnz;
                for (uint32_t k = 0; k < K; k++) {
                    if (weight_matrix[n * K + k] != 0) {
                        spmm_input[i]->weight_value_vector[layer][nnz] = weight_matrix[n * K + k];
                        spmm_input[i]->weight_IDX_vector[layer][nnz] = k;
                        nnz++;
                    }
                }
            }
            spmm_input[i]->weight_NNZ_vector[layer][N] = nnz;

            if (nnz != num_nonzeroes) {
                fprintf(stderr, "Wrong calculation for layer %d (M=%d,N=%d,K=%d) in CSR factory. Actual NNZ (%d) != Calculated NNZ (%d)!\n", layer, M, N, K, nnz, num_nonzeroes);
                exit(-1);
            }
        }
    }

    input = (input_t **)spmm_input;
}

template <typename T>
void spmm_output_init(int count,
                      config_t *config,
                      output_t **&output) {

    spmm_config_t *spmm_config = (spmm_config_t *)config;
    spmm_output_t<T> **spmm_output = (spmm_output_t<T> **)output;

    // configuration
    int layer_count = spmm_config->layer_count;

    // initializing output versions
    alloc_1D<spmm_output_t<T> *>(count, spmm_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<spmm_output_t<T>>(1, spmm_output[i]);

        alloc_1D<T *>(layer_count, spmm_output[i]->output_matrix);
#if CACHE_STATUS == CACHE_STATUS_WARM
        init_alloc_1D<T>(MAX_INOUT_SIZE, spmm_output[i]->inout0, 0, 0);
        init_alloc_1D<T>(MAX_INOUT_SIZE, spmm_output[i]->inout1, 0, 0);
#endif

        for (int layer = 0; layer < layer_count; layer++) {
            int M = layer_infos[layer][M_IDX];
            int N = layer_infos[layer][N_IDX];
            alloc_1D<T>(N * M, spmm_output[i]->output_matrix[layer]);
        }
    }

    output = (output_t **)spmm_output;
}

template <typename T>
void spmm_comparer(config_t *config,
                   output_t *output_scalar,
                   output_t *output_neon) {
    spmm_config_t *spmm_config = (spmm_config_t *)config;
    spmm_output_t<T> *spmm_output_scalar = (spmm_output_t<T> *)output_scalar;
    spmm_output_t<T> *spmm_output_neon = (spmm_output_t<T> *)output_neon;

    int layer_count = spmm_config->layer_count;

#if CACHE_STATUS == CACHE_STATUS_WARM
    int M = layer_infos[layer_count - 1][M_IDX];
    int N = layer_infos[layer_count - 1][N_IDX];
    char obj_name[100];
    sprintf(obj_name, "output_matrix[%d]", layer_count - 1);
    compare_1D<T>(N * M, spmm_output_scalar->output_matrix[layer_count - 1], spmm_output_neon->output_matrix[layer_count - 1], obj_name);
#else
    for (int layer = 0; layer < layer_count; layer++) {
        int M = layer_infos[layer][M_IDX];
        int N = layer_infos[layer][N_IDX];
        char obj_name[100];
        sprintf(obj_name, "output_matrix[%d]", layer);
        compare_1D<T>(N * M, spmm_output_scalar->output_matrix[layer], spmm_output_neon->output_matrix[layer], obj_name);
    }
#endif
}

kernel_utility_t spmm_fp32_utility = {spmm_config_init<float>, spmm_input_init<float>, spmm_output_init<float>, spmm_comparer<float>};
kernel_utility_t spmm_int32_utility = {spmm_config_init<int32_t>, spmm_input_init<int32_t>, spmm_output_init<int32_t>, spmm_comparer<int32_t>};
#ifndef SWAN_SIMULATION
kernel_utility_t spmm_fp16_utility = {spmm_config_init<__fp16>, spmm_input_init<__fp16>, spmm_output_init<__fp16>, spmm_comparer<__fp16>};
#endif
kernel_utility_t spmm_int16_utility = {spmm_config_init<int16_t>, spmm_input_init<int16_t>, spmm_output_init<int16_t>, spmm_comparer<int16_t>};