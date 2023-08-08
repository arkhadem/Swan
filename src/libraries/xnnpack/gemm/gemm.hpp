#ifndef C2371007_63A6_4C04_8307_0A99502CD7C1
#define C2371007_63A6_4C04_8307_0A99502CD7C1

#include <stdint.h>
#include <stdlib.h>

#include "xnnpack.hpp"

#define MR 32

typedef struct gemm_config_s : config_t {
    int layer_count;
    int *M;
    int *N;
    int *K;
} gemm_config_t;

template <typename T>
struct gemm_input_t : input_t {
    T **input_matrix;
    T **bias_vector;
    T **weight_matrix;
    T *min;
    T *max;
};

template <typename T>
struct gemm_output_t : output_t {
#if CACHE_STATUS == CACHE_STATUS_WARM
    T *inout0;
    T *inout1;
#endif
    T **output_matrix;
};

#endif /* C2371007_63A6_4C04_8307_0A99502CD7C1 */
