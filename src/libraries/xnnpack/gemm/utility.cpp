#include "gemm.hpp"

#include "xnnpack.hpp"

#include "swan.hpp"

#include "utility.hpp"

template <typename T>
int gemm_config_init(size_t cache_size,
                     config_t *&config) {

    gemm_config_t *gemm_config = (gemm_config_t *)config;

    // configuration
#ifdef SWAN_SIMULATION
    int layer_count = 1;
#else
    int layer_count = LAYER_COUNT;
#endif

    alloc_1D<gemm_config_t>(1, gemm_config);
    alloc_1D<int>(layer_count, gemm_config->M);
    alloc_1D<int>(layer_count, gemm_config->N);
    alloc_1D<int>(layer_count, gemm_config->K);

    // in/output versions
    size_t input_size = 0;
    size_t output_size = 0;

    gemm_config->layer_count = layer_count;
    for (int i = 0; i < layer_count; i++) {
        gemm_config->M[i] = layer_infos[i][M_IDX];
        gemm_config->N[i] = layer_infos[i][N_IDX];
        gemm_config->K[i] = layer_infos[i][K_IDX];
        input_size += layer_infos[i][K_IDX] * layer_infos[i][M_IDX] * sizeof(T);
        input_size += layer_infos[i][N_IDX] * layer_infos[i][K_IDX] * sizeof(T);
        output_size += layer_infos[i][N_IDX] * layer_infos[i][M_IDX] * sizeof(T);
    }
    gemm_config->granularity = 1;

    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)gemm_config;

    return count;
}

template <typename T>
void gemm_input_init(int count,
                     config_t *config,
                     input_t **&input) {

    gemm_config_t *gemm_config = (gemm_config_t *)config;
    gemm_input_t<T> **gemm_input = (gemm_input_t<T> **)input;

    int layer_count = gemm_config->layer_count;

    // initializing input versions
    alloc_1D<gemm_input_t<T> *>(count, gemm_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<gemm_input_t<T>>(1, gemm_input[i]);

        alloc_1D<T *>(layer_count, gemm_input[i]->input_matrix);
        alloc_1D<T *>(layer_count, gemm_input[i]->bias_vector);
        alloc_1D<T *>(layer_count, gemm_input[i]->weight_matrix);
        init_alloc_1D<T>(layer_count, gemm_input[i]->min);
        init_alloc_1D<T>(layer_count, gemm_input[i]->max);

        for (int layer = 0; layer < layer_count; layer++) {
            int M = layer_infos[layer][M_IDX];
            int N = layer_infos[layer][N_IDX];
            int K = layer_infos[layer][K_IDX];
            init_alloc_1D<T>(K * M, gemm_input[i]->input_matrix[layer]);
            init_alloc_1D<T>(N, gemm_input[i]->bias_vector[layer]);
            init_alloc_1D<T>(N * K, gemm_input[i]->weight_matrix[layer]);
        }
    }

    input = (input_t **)gemm_input;
}

template <typename T>
void gemm_output_init(int count,
                      config_t *config,
                      output_t **&output) {

    gemm_config_t *gemm_config = (gemm_config_t *)config;
    gemm_output_t<T> **gemm_output = (gemm_output_t<T> **)output;

    int layer_count = gemm_config->layer_count;

    // initializing output versions
    alloc_1D<gemm_output_t<T> *>(count, gemm_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<gemm_output_t<T>>(1, gemm_output[i]);

        alloc_1D<T *>(layer_count, gemm_output[i]->output_matrix);
#if CACHE_STATUS == CACHE_STATUS_WARM
        init_alloc_1D<T>(MAX_INOUT_SIZE, gemm_output[i]->inout0, 0, 0);
        init_alloc_1D<T>(MAX_INOUT_SIZE, gemm_output[i]->inout1, 0, 0);
#endif

        for (int layer = 0; layer < layer_count; layer++) {
            int M = layer_infos[layer][M_IDX];
            int N = layer_infos[layer][N_IDX];
            alloc_1D<T>(N * M, gemm_output[i]->output_matrix[layer]);
        }
    }

    output = (output_t **)gemm_output;
}

template <typename T>
void gemm_comparer(config_t *config,
                   output_t *output_scalar,
                   output_t *output_neon) {
    gemm_config_t *gemm_config = (gemm_config_t *)config;
    gemm_output_t<T> *gemm_output_scalar = (gemm_output_t<T> *)output_scalar;
    gemm_output_t<T> *gemm_output_neon = (gemm_output_t<T> *)output_neon;

    int layer_count = gemm_config->layer_count;

#if CACHE_STATUS == CACHE_STATUS_WARM
    int M = layer_infos[layer_count - 1][M_IDX];
    int N = layer_infos[layer_count - 1][N_IDX];
    char obj_name[100];
    sprintf(obj_name, "output_matrix[%d]", layer_count - 1);
    compare_1D<T>(N * M, gemm_output_scalar->output_matrix[layer_count - 1], gemm_output_neon->output_matrix[layer_count - 1], obj_name);
#else
    for (int layer = 0; layer < layer_count; layer++) {
        int M = layer_infos[layer][M_IDX];
        int N = layer_infos[layer][N_IDX];
        char obj_name[100];
        sprintf(obj_name, "output_matrix[%d]", layer);
        compare_1D<T>(N * M, gemm_output_scalar->output_matrix[layer], gemm_output_neon->output_matrix[layer], obj_name);
    }
#endif
}

kernel_utility_t gemm_fp32_utility = {gemm_config_init<float>, gemm_input_init<float>, gemm_output_init<float>, gemm_comparer<float>};
kernel_utility_t gemm_int32_utility = {gemm_config_init<int32_t>, gemm_input_init<int32_t>, gemm_output_init<int32_t>, gemm_comparer<int32_t>};
#ifndef SWAN_SIMULATION
kernel_utility_t gemm_fp16_utility = {gemm_config_init<__fp16>, gemm_input_init<__fp16>, gemm_output_init<__fp16>, gemm_comparer<__fp16>};
#endif
kernel_utility_t gemm_int16_utility = {gemm_config_init<int16_t>, gemm_input_init<int16_t>, gemm_output_init<int16_t>, gemm_comparer<int16_t>};

#ifdef SWAN_SIMULATION
int layer_infos[156][3] = {
    {127, 256, 128},
    {3136, 128, 64},
    {3136, 128, 128},
    {784, 256, 128},
    {784, 256, 256},
    {196, 512, 256},
    {196, 512, 512},
    {49, 1024, 512},
    {49, 1024, 1024},
    {12544, 16, 32},
    {12544, 96, 16},
    {3136, 24, 96},
    {3136, 144, 24},
    {3136, 24, 144},
    {784, 32, 144},
    {784, 192, 32},
    {784, 32, 192},
    {196, 64, 192},
    {196, 384, 64},
    {196, 64, 384},
    {196, 96, 384},
    {196, 576, 96},
    {196, 96, 576},
    {49, 160, 576},
    {49, 960, 160},
    {49, 160, 960},
    {49, 320, 960},
    {49, 1280, 320},
    {3136, 16, 16},
    {3136, 72, 16},
    {784, 24, 72},
    {784, 88, 24},
    {784, 24, 88},
    {784, 96, 24},
    {196, 40, 96},
    {196, 240, 40},
    {196, 40, 240},
    {196, 120, 40},
    {196, 48, 120},
    {196, 144, 48},
    {196, 48, 144},
    {196, 288, 48},
    {49, 96, 288},
    {49, 576, 96},
    {49, 96, 576},
    {12544, 16, 16},
    {12544, 64, 16},
    {3136, 24, 64},
    {3136, 72, 24},
    {3136, 24, 72},
    {784, 40, 72},
    {784, 120, 40},
    {784, 40, 120},
    {784, 240, 40},
    {196, 80, 240},
    {196, 200, 80},
    {196, 80, 200},
    {196, 184, 80},
    {196, 80, 184},
    {196, 480, 80},
    {196, 112, 480},
    {196, 672, 112},
    {196, 112, 672},
    {49, 160, 672},
    {49, 960, 160},
    {49, 160, 960},
    {3136, 36, 24},
    {784, 120, 36},
    {784, 36, 144},
    {784, 144, 36},
    {784, 72, 144},
    {196, 144, 72},
    {196, 72, 288},
    {196, 288, 72},
    {196, 144, 288},
    {49, 288, 144},
    {49, 144, 576},
    {49, 576, 144},
    {3136, 50, 24},
    {784, 88, 25},
    {784, 25, 100},
    {784, 100, 25},
    {784, 50, 100},
    {196, 100, 50},
    {196, 50, 200},
    {196, 200, 50},
    {196, 100, 200},
    {49, 200, 100},
    {49, 100, 400},
    {49, 400, 100},
    {3136, 60, 24},
    {784, 72, 20},
    {784, 20, 80},
    {784, 80, 20},
    {784, 40, 80},
    {196, 80, 40},
    {196, 40, 160},
    {196, 160, 40},
    {196, 80, 160},
    {49, 160, 80},
    {49, 80, 320},
    {49, 320, 80},
    {3136, 68, 24},
    {784, 62, 17},
    {784, 17, 68},
    {784, 68, 17},
    {784, 34, 68},
    {196, 68, 34},
    {196, 34, 136},
    {196, 136, 34},
    {196, 68, 136},
    {49, 136, 68},
    {49, 68, 272},
    {49, 272, 68},
    {3136, 96, 24},
    {784, 45, 12},
    {784, 12, 48},
    {784, 48, 12},
    {784, 24, 48},
    {196, 48, 24},
    {196, 24, 96},
    {196, 96, 24},
    {196, 48, 96},
    {49, 96, 48},
    {49, 48, 192},
    {49, 192, 48},
    {3136, 24, 24},
    {784, 24, 24},
    {784, 48, 48},
    {196, 48, 48},
    {196, 96, 96},
    {49, 96, 96},
    {49, 1024, 192},
    {3136, 58, 24},
    {784, 58, 24},
    {784, 58, 58},
    {196, 116, 116},
    {196, 232, 232},
    {49, 232, 232},
    {49, 1024, 464},
    {3136, 88, 24},
    {784, 88, 24},
    {784, 88, 88},
    {784, 176, 176},
    {196, 176, 176},
    {196, 352, 352},
    {49, 352, 352},
    {49, 1024, 704},
    {3136, 122, 24},
    {784, 122, 24},
    {784, 122, 122},
    {784, 244, 244},
    {196, 244, 244},
    {196, 488, 488},
    {49, 488, 488},
    {49, 2048, 976}};
#else
int layer_infos[156][3] = {
    {12544, 64, 32},
    {3136, 128, 64},
    {3136, 128, 128},
    {784, 256, 128},
    {784, 256, 256},
    {196, 512, 256},
    {196, 512, 512},
    {49, 1024, 512},
    {49, 1024, 1024},
    {12544, 16, 32},
    {12544, 96, 16},
    {3136, 24, 96},
    {3136, 144, 24},
    {3136, 24, 144},
    {784, 32, 144},
    {784, 192, 32},
    {784, 32, 192},
    {196, 64, 192},
    {196, 384, 64},
    {196, 64, 384},
    {196, 96, 384},
    {196, 576, 96},
    {196, 96, 576},
    {49, 160, 576},
    {49, 960, 160},
    {49, 160, 960},
    {49, 320, 960},
    {49, 1280, 320},
    {3136, 16, 16},
    {3136, 72, 16},
    {784, 24, 72},
    {784, 88, 24},
    {784, 24, 88},
    {784, 96, 24},
    {196, 40, 96},
    {196, 240, 40},
    {196, 40, 240},
    {196, 120, 40},
    {196, 48, 120},
    {196, 144, 48},
    {196, 48, 144},
    {196, 288, 48},
    {49, 96, 288},
    {49, 576, 96},
    {49, 96, 576},
    {12544, 16, 16},
    {12544, 64, 16},
    {3136, 24, 64},
    {3136, 72, 24},
    {3136, 24, 72},
    {784, 40, 72},
    {784, 120, 40},
    {784, 40, 120},
    {784, 240, 40},
    {196, 80, 240},
    {196, 200, 80},
    {196, 80, 200},
    {196, 184, 80},
    {196, 80, 184},
    {196, 480, 80},
    {196, 112, 480},
    {196, 672, 112},
    {196, 112, 672},
    {49, 160, 672},
    {49, 960, 160},
    {49, 160, 960},
    {3136, 36, 24},
    {784, 120, 36},
    {784, 36, 144},
    {784, 144, 36},
    {784, 72, 144},
    {196, 144, 72},
    {196, 72, 288},
    {196, 288, 72},
    {196, 144, 288},
    {49, 288, 144},
    {49, 144, 576},
    {49, 576, 144},
    {3136, 50, 24},
    {784, 88, 25},
    {784, 25, 100},
    {784, 100, 25},
    {784, 50, 100},
    {196, 100, 50},
    {196, 50, 200},
    {196, 200, 50},
    {196, 100, 200},
    {49, 200, 100},
    {49, 100, 400},
    {49, 400, 100},
    {3136, 60, 24},
    {784, 72, 20},
    {784, 20, 80},
    {784, 80, 20},
    {784, 40, 80},
    {196, 80, 40},
    {196, 40, 160},
    {196, 160, 40},
    {196, 80, 160},
    {49, 160, 80},
    {49, 80, 320},
    {49, 320, 80},
    {3136, 68, 24},
    {784, 62, 17},
    {784, 17, 68},
    {784, 68, 17},
    {784, 34, 68},
    {196, 68, 34},
    {196, 34, 136},
    {196, 136, 34},
    {196, 68, 136},
    {49, 136, 68},
    {49, 68, 272},
    {49, 272, 68},
    {3136, 96, 24},
    {784, 45, 12},
    {784, 12, 48},
    {784, 48, 12},
    {784, 24, 48},
    {196, 48, 24},
    {196, 24, 96},
    {196, 96, 24},
    {196, 48, 96},
    {49, 96, 48},
    {49, 48, 192},
    {49, 192, 48},
    {3136, 24, 24},
    {784, 24, 24},
    {784, 48, 48},
    {196, 48, 48},
    {196, 96, 96},
    {49, 96, 96},
    {49, 1024, 192},
    {3136, 58, 24},
    {784, 58, 24},
    {784, 58, 58},
    {196, 116, 116},
    {196, 232, 232},
    {49, 232, 232},
    {49, 1024, 464},
    {3136, 88, 24},
    {784, 88, 24},
    {784, 88, 88},
    {784, 176, 176},
    {196, 176, 176},
    {196, 352, 352},
    {49, 352, 352},
    {49, 1024, 704},
    {3136, 122, 24},
    {784, 122, 24},
    {784, 122, 122},
    {784, 244, 244},
    {196, 244, 244},
    {196, 488, 488},
    {49, 488, 488},
    {49, 2048, 976}};
#endif