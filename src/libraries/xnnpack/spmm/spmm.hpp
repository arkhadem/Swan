#ifndef BFE1E2E7_DDA9_476A_9073_22D4442897A4
#define BFE1E2E7_DDA9_476A_9073_22D4442897A4

#include <stdint.h>
#include <stdlib.h>

#include "xnnpack.hpp"

#define MR 32

typedef struct spmm_config_s : config_t {
    int layer_count;
    int *M;
    int *N;
    float sparsity;
} spmm_config_t;

template <typename T>
struct spmm_input_t : input_t {
    T **input_matrix;
    T **bias_vector;
    T **weight_value_vector;
    int32_t **weight_IDX_vector;
    uint32_t **weight_NNZ_vector;
    T *min;
    T *max;
};

template <typename T>
struct spmm_output_t : output_t {
#if CACHE_STATUS == CACHE_STATUS_WARM
    T *inout0;
    T *inout1;
#endif
    T **output_matrix;
};

#endif /* BFE1E2E7_DDA9_476A_9073_22D4442897A4 */
