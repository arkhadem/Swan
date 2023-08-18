#include "scalar.hpp"
#include "spmm.hpp"

void spmm_fp32_scalar(config_t *config,
                      input_t *input,
                      output_t *output) {

    spmm_config_t *spmm_config = (spmm_config_t *)config;
    spmm_input_t<float> *spmm_input = (spmm_input_t<float> *)input;
    spmm_output_t<float> *spmm_output = (spmm_output_t<float> *)output;

#if CACHE_STATUS == CACHE_STATUS_WARM
    float *inout0 = spmm_output->inout0;
    float *inout1 = spmm_output->inout1;
#endif

    // number of layers
    int layer_count = spmm_config->layer_count;

    for (int layer = 0; layer < layer_count; layer++) {
        int M = spmm_config->M[layer];
        int N = spmm_config->N[layer];
        float *bias_vector = spmm_input->bias_vector[layer];
        float *weight_value_vector = spmm_input->weight_value_vector[layer];
        int32_t *weight_IDX_vector = spmm_input->weight_IDX_vector[layer];
        uint32_t *weight_NNZ_vector = spmm_input->weight_NNZ_vector[layer];
        float min = spmm_input->min[layer];
        float max = spmm_input->max[layer];

#if CACHE_STATUS == CACHE_STATUS_WARM
        float *input_matrix;
        if (layer == 0) {
            input_matrix = spmm_input->input_matrix[layer];
        } else if (layer % 2) {
            input_matrix = inout0;
        } else {
            input_matrix = inout1;
        }

        float *output_matrix;
        if (layer == layer_count - 1) {
            output_matrix = spmm_output->output_matrix[layer];
        } else if (layer % 2) {
            output_matrix = inout1;
        } else {
            output_matrix = inout0;
        }
#else
        float *input_matrix = spmm_input->input_matrix[layer];
        float *output_matrix = spmm_output->output_matrix[layer];
#endif

        int m = 0;

        while (m + 16 <= M) {
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float acc0 = bias_vector[n];
                float acc1 = acc0;
                float acc2 = acc0;
                float acc3 = acc0;
                float acc4 = acc0;
                float acc5 = acc0;
                float acc6 = acc0;
                float acc7 = acc0;
                float acc8 = acc0;
                float acc9 = acc0;
                float acc10 = acc0;
                float acc11 = acc0;
                float acc12 = acc0;
                float acc13 = acc0;
                float acc14 = acc0;
                float acc15 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    float *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    float w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                    acc8 += (w * *input_addr_temp++);
                    acc9 += (w * *input_addr_temp++);
                    acc10 += (w * *input_addr_temp++);
                    acc11 += (w * *input_addr_temp++);
                    acc12 += (w * *input_addr_temp++);
                    acc13 += (w * *input_addr_temp++);
                    acc14 += (w * *input_addr_temp++);
                    acc15 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;
                acc4 = acc4 > max ? max : acc4;
                acc5 = acc5 > max ? max : acc5;
                acc6 = acc6 > max ? max : acc6;
                acc7 = acc7 > max ? max : acc7;
                acc8 = acc8 > max ? max : acc8;
                acc9 = acc9 > max ? max : acc9;
                acc10 = acc10 > max ? max : acc10;
                acc11 = acc11 > max ? max : acc11;
                acc12 = acc12 > max ? max : acc12;
                acc13 = acc13 > max ? max : acc13;
                acc14 = acc14 > max ? max : acc14;
                acc15 = acc15 > max ? max : acc15;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;
                acc4 = acc4 < min ? min : acc4;
                acc5 = acc5 < min ? min : acc5;
                acc6 = acc6 < min ? min : acc6;
                acc7 = acc7 < min ? min : acc7;
                acc8 = acc8 < min ? min : acc8;
                acc9 = acc9 < min ? min : acc9;
                acc10 = acc10 < min ? min : acc10;
                acc11 = acc11 < min ? min : acc11;
                acc12 = acc12 < min ? min : acc12;
                acc13 = acc13 < min ? min : acc13;
                acc14 = acc14 < min ? min : acc14;
                acc15 = acc15 < min ? min : acc15;

                float *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;
                *output_addr_temp++ = acc4;
                *output_addr_temp++ = acc5;
                *output_addr_temp++ = acc6;
                *output_addr_temp++ = acc7;
                *output_addr_temp++ = acc8;
                *output_addr_temp++ = acc9;
                *output_addr_temp++ = acc10;
                *output_addr_temp++ = acc11;
                *output_addr_temp++ = acc12;
                *output_addr_temp++ = acc13;
                *output_addr_temp++ = acc14;
                *output_addr_temp++ = acc15;

                output_addr += M;
            }

            m += 16;
            input_matrix += 16;
            output_matrix += 16;
        }

        while (m + 8 <= M) {
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float acc0 = bias_vector[n];
                float acc1 = acc0;
                float acc2 = acc0;
                float acc3 = acc0;
                float acc4 = acc0;
                float acc5 = acc0;
                float acc6 = acc0;
                float acc7 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    float *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    float w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;
                acc4 = acc4 > max ? max : acc4;
                acc5 = acc5 > max ? max : acc5;
                acc6 = acc6 > max ? max : acc6;
                acc7 = acc7 > max ? max : acc7;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;
                acc4 = acc4 < min ? min : acc4;
                acc5 = acc5 < min ? min : acc5;
                acc6 = acc6 < min ? min : acc6;
                acc7 = acc7 < min ? min : acc7;

                float *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;
                *output_addr_temp++ = acc4;
                *output_addr_temp++ = acc5;
                *output_addr_temp++ = acc6;
                *output_addr_temp++ = acc7;

                output_addr += M;
            }

            m += 8;
            input_matrix += 8;
            output_matrix += 8;
        }

        while (m + 4 <= M) {
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float acc0 = bias_vector[n];
                float acc1 = acc0;
                float acc2 = acc0;
                float acc3 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    float *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    float w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;

                float *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;

                output_addr += M;
            }

            m += 4;
            input_matrix += 4;
            output_matrix += 4;
        }

        while (m + 2 <= M) {
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float acc0 = bias_vector[n];
                float acc1 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    float *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    float w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;

                float *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;

                output_addr += M;
            }

            m += 2;
            input_matrix += 2;
            output_matrix += 2;
        }

        while (m + 1 <= M) {
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float acc0 = bias_vector[n];

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    float *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    float w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;

                acc0 = acc0 < min ? min : acc0;

                float *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;

                output_addr += M;
            }

            m += 1;
            input_matrix += 1;
            output_matrix += 1;
        }
    }
}

void spmm_int32_scalar(config_t *config,
                       input_t *input,
                       output_t *output) {

    spmm_config_t *spmm_config = (spmm_config_t *)config;
    spmm_input_t<int32_t> *spmm_input = (spmm_input_t<int32_t> *)input;
    spmm_output_t<int32_t> *spmm_output = (spmm_output_t<int32_t> *)output;

#if CACHE_STATUS == CACHE_STATUS_WARM
    int32_t *inout0 = spmm_output->inout0;
    int32_t *inout1 = spmm_output->inout1;
#endif

    // number of layers
    int layer_count = spmm_config->layer_count;

    for (int layer = 0; layer < layer_count; layer++) {
        int M = spmm_config->M[layer];
        int N = spmm_config->N[layer];
        int32_t *bias_vector = spmm_input->bias_vector[layer];
        int32_t *weight_value_vector = spmm_input->weight_value_vector[layer];
        int32_t *weight_IDX_vector = spmm_input->weight_IDX_vector[layer];
        uint32_t *weight_NNZ_vector = spmm_input->weight_NNZ_vector[layer];
        int32_t min = spmm_input->min[layer];
        int32_t max = spmm_input->max[layer];

#if CACHE_STATUS == CACHE_STATUS_WARM
        int32_t *input_matrix;
        if (layer == 0) {
            input_matrix = spmm_input->input_matrix[layer];
        } else if (layer % 2) {
            input_matrix = inout0;
        } else {
            input_matrix = inout1;
        }

        int32_t *output_matrix;
        if (layer == layer_count - 1) {
            output_matrix = spmm_output->output_matrix[layer];
        } else if (layer % 2) {
            output_matrix = inout1;
        } else {
            output_matrix = inout0;
        }
#else
        int32_t *input_matrix = spmm_input->input_matrix[layer];
        int32_t *output_matrix = spmm_output->output_matrix[layer];
#endif

        int m = 0;

        while (m + 16 <= M) {
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t acc0 = bias_vector[n];
                int32_t acc1 = acc0;
                int32_t acc2 = acc0;
                int32_t acc3 = acc0;
                int32_t acc4 = acc0;
                int32_t acc5 = acc0;
                int32_t acc6 = acc0;
                int32_t acc7 = acc0;
                int32_t acc8 = acc0;
                int32_t acc9 = acc0;
                int32_t acc10 = acc0;
                int32_t acc11 = acc0;
                int32_t acc12 = acc0;
                int32_t acc13 = acc0;
                int32_t acc14 = acc0;
                int32_t acc15 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int32_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int32_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                    acc8 += (w * *input_addr_temp++);
                    acc9 += (w * *input_addr_temp++);
                    acc10 += (w * *input_addr_temp++);
                    acc11 += (w * *input_addr_temp++);
                    acc12 += (w * *input_addr_temp++);
                    acc13 += (w * *input_addr_temp++);
                    acc14 += (w * *input_addr_temp++);
                    acc15 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;
                acc4 = acc4 > max ? max : acc4;
                acc5 = acc5 > max ? max : acc5;
                acc6 = acc6 > max ? max : acc6;
                acc7 = acc7 > max ? max : acc7;
                acc8 = acc8 > max ? max : acc8;
                acc9 = acc9 > max ? max : acc9;
                acc10 = acc10 > max ? max : acc10;
                acc11 = acc11 > max ? max : acc11;
                acc12 = acc12 > max ? max : acc12;
                acc13 = acc13 > max ? max : acc13;
                acc14 = acc14 > max ? max : acc14;
                acc15 = acc15 > max ? max : acc15;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;
                acc4 = acc4 < min ? min : acc4;
                acc5 = acc5 < min ? min : acc5;
                acc6 = acc6 < min ? min : acc6;
                acc7 = acc7 < min ? min : acc7;
                acc8 = acc8 < min ? min : acc8;
                acc9 = acc9 < min ? min : acc9;
                acc10 = acc10 < min ? min : acc10;
                acc11 = acc11 < min ? min : acc11;
                acc12 = acc12 < min ? min : acc12;
                acc13 = acc13 < min ? min : acc13;
                acc14 = acc14 < min ? min : acc14;
                acc15 = acc15 < min ? min : acc15;

                int32_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;
                *output_addr_temp++ = acc4;
                *output_addr_temp++ = acc5;
                *output_addr_temp++ = acc6;
                *output_addr_temp++ = acc7;
                *output_addr_temp++ = acc8;
                *output_addr_temp++ = acc9;
                *output_addr_temp++ = acc10;
                *output_addr_temp++ = acc11;
                *output_addr_temp++ = acc12;
                *output_addr_temp++ = acc13;
                *output_addr_temp++ = acc14;
                *output_addr_temp++ = acc15;

                output_addr += M;
            }

            m += 16;
            input_matrix += 16;
            output_matrix += 16;
        }

        while (m + 8 <= M) {
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t acc0 = bias_vector[n];
                int32_t acc1 = acc0;
                int32_t acc2 = acc0;
                int32_t acc3 = acc0;
                int32_t acc4 = acc0;
                int32_t acc5 = acc0;
                int32_t acc6 = acc0;
                int32_t acc7 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int32_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int32_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;
                acc4 = acc4 > max ? max : acc4;
                acc5 = acc5 > max ? max : acc5;
                acc6 = acc6 > max ? max : acc6;
                acc7 = acc7 > max ? max : acc7;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;
                acc4 = acc4 < min ? min : acc4;
                acc5 = acc5 < min ? min : acc5;
                acc6 = acc6 < min ? min : acc6;
                acc7 = acc7 < min ? min : acc7;

                int32_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;
                *output_addr_temp++ = acc4;
                *output_addr_temp++ = acc5;
                *output_addr_temp++ = acc6;
                *output_addr_temp++ = acc7;

                output_addr += M;
            }

            m += 8;
            input_matrix += 8;
            output_matrix += 8;
        }

        while (m + 4 <= M) {
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t acc0 = bias_vector[n];
                int32_t acc1 = acc0;
                int32_t acc2 = acc0;
                int32_t acc3 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int32_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int32_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;

                int32_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;

                output_addr += M;
            }

            m += 4;
            input_matrix += 4;
            output_matrix += 4;
        }

        while (m + 2 <= M) {
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t acc0 = bias_vector[n];
                int32_t acc1 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int32_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int32_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;

                int32_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;

                output_addr += M;
            }

            m += 2;
            input_matrix += 2;
            output_matrix += 2;
        }

        while (m + 1 <= M) {
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t acc0 = bias_vector[n];

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int32_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int32_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;

                acc0 = acc0 < min ? min : acc0;

                int32_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;

                output_addr += M;
            }

            m += 1;
            input_matrix += 1;
            output_matrix += 1;
        }
    }
}

void spmm_int16_scalar(config_t *config,
                       input_t *input,
                       output_t *output) {

    spmm_config_t *spmm_config = (spmm_config_t *)config;
    spmm_input_t<int16_t> *spmm_input = (spmm_input_t<int16_t> *)input;
    spmm_output_t<int16_t> *spmm_output = (spmm_output_t<int16_t> *)output;

#if CACHE_STATUS == CACHE_STATUS_WARM
    int16_t *inout0 = spmm_output->inout0;
    int16_t *inout1 = spmm_output->inout1;
#endif

    // number of layers
    int layer_count = spmm_config->layer_count;

    for (int layer = 0; layer < layer_count; layer++) {
        int M = spmm_config->M[layer];
        int N = spmm_config->N[layer];
        int16_t *bias_vector = spmm_input->bias_vector[layer];
        int16_t *weight_value_vector = spmm_input->weight_value_vector[layer];
        int32_t *weight_IDX_vector = spmm_input->weight_IDX_vector[layer];
        uint32_t *weight_NNZ_vector = spmm_input->weight_NNZ_vector[layer];
        int16_t min = spmm_input->min[layer];
        int16_t max = spmm_input->max[layer];

#if CACHE_STATUS == CACHE_STATUS_WARM
        int16_t *input_matrix;
        if (layer == 0) {
            input_matrix = spmm_input->input_matrix[layer];
        } else if (layer % 2) {
            input_matrix = inout0;
        } else {
            input_matrix = inout1;
        }

        int16_t *output_matrix;
        if (layer == layer_count - 1) {
            output_matrix = spmm_output->output_matrix[layer];
        } else if (layer % 2) {
            output_matrix = inout1;
        } else {
            output_matrix = inout0;
        }
#else
        int16_t *input_matrix = spmm_input->input_matrix[layer];
        int16_t *output_matrix = spmm_output->output_matrix[layer];
#endif

        int m = 0;

        while (m + 16 <= M) {
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t acc0 = bias_vector[n];
                int16_t acc1 = acc0;
                int16_t acc2 = acc0;
                int16_t acc3 = acc0;
                int16_t acc4 = acc0;
                int16_t acc5 = acc0;
                int16_t acc6 = acc0;
                int16_t acc7 = acc0;
                int16_t acc8 = acc0;
                int16_t acc9 = acc0;
                int16_t acc10 = acc0;
                int16_t acc11 = acc0;
                int16_t acc12 = acc0;
                int16_t acc13 = acc0;
                int16_t acc14 = acc0;
                int16_t acc15 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int16_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int16_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                    acc8 += (w * *input_addr_temp++);
                    acc9 += (w * *input_addr_temp++);
                    acc10 += (w * *input_addr_temp++);
                    acc11 += (w * *input_addr_temp++);
                    acc12 += (w * *input_addr_temp++);
                    acc13 += (w * *input_addr_temp++);
                    acc14 += (w * *input_addr_temp++);
                    acc15 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;
                acc4 = acc4 > max ? max : acc4;
                acc5 = acc5 > max ? max : acc5;
                acc6 = acc6 > max ? max : acc6;
                acc7 = acc7 > max ? max : acc7;
                acc8 = acc8 > max ? max : acc8;
                acc9 = acc9 > max ? max : acc9;
                acc10 = acc10 > max ? max : acc10;
                acc11 = acc11 > max ? max : acc11;
                acc12 = acc12 > max ? max : acc12;
                acc13 = acc13 > max ? max : acc13;
                acc14 = acc14 > max ? max : acc14;
                acc15 = acc15 > max ? max : acc15;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;
                acc4 = acc4 < min ? min : acc4;
                acc5 = acc5 < min ? min : acc5;
                acc6 = acc6 < min ? min : acc6;
                acc7 = acc7 < min ? min : acc7;
                acc8 = acc8 < min ? min : acc8;
                acc9 = acc9 < min ? min : acc9;
                acc10 = acc10 < min ? min : acc10;
                acc11 = acc11 < min ? min : acc11;
                acc12 = acc12 < min ? min : acc12;
                acc13 = acc13 < min ? min : acc13;
                acc14 = acc14 < min ? min : acc14;
                acc15 = acc15 < min ? min : acc15;

                int16_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;
                *output_addr_temp++ = acc4;
                *output_addr_temp++ = acc5;
                *output_addr_temp++ = acc6;
                *output_addr_temp++ = acc7;
                *output_addr_temp++ = acc8;
                *output_addr_temp++ = acc9;
                *output_addr_temp++ = acc10;
                *output_addr_temp++ = acc11;
                *output_addr_temp++ = acc12;
                *output_addr_temp++ = acc13;
                *output_addr_temp++ = acc14;
                *output_addr_temp++ = acc15;

                output_addr += M;
            }

            m += 16;
            input_matrix += 16;
            output_matrix += 16;
        }

        while (m + 8 <= M) {
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t acc0 = bias_vector[n];
                int16_t acc1 = acc0;
                int16_t acc2 = acc0;
                int16_t acc3 = acc0;
                int16_t acc4 = acc0;
                int16_t acc5 = acc0;
                int16_t acc6 = acc0;
                int16_t acc7 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int16_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int16_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;
                acc4 = acc4 > max ? max : acc4;
                acc5 = acc5 > max ? max : acc5;
                acc6 = acc6 > max ? max : acc6;
                acc7 = acc7 > max ? max : acc7;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;
                acc4 = acc4 < min ? min : acc4;
                acc5 = acc5 < min ? min : acc5;
                acc6 = acc6 < min ? min : acc6;
                acc7 = acc7 < min ? min : acc7;

                int16_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;
                *output_addr_temp++ = acc4;
                *output_addr_temp++ = acc5;
                *output_addr_temp++ = acc6;
                *output_addr_temp++ = acc7;

                output_addr += M;
            }

            m += 8;
            input_matrix += 8;
            output_matrix += 8;
        }

        while (m + 4 <= M) {
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t acc0 = bias_vector[n];
                int16_t acc1 = acc0;
                int16_t acc2 = acc0;
                int16_t acc3 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int16_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int16_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;

                int16_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;

                output_addr += M;
            }

            m += 4;
            input_matrix += 4;
            output_matrix += 4;
        }

        while (m + 2 <= M) {
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t acc0 = bias_vector[n];
                int16_t acc1 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int16_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int16_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;

                int16_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;

                output_addr += M;
            }

            m += 2;
            input_matrix += 2;
            output_matrix += 2;
        }

        while (m + 1 <= M) {
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t acc0 = bias_vector[n];

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    int16_t *input_addr_temp = input_matrix + weight_IDX_vector[k] * M;
                    int16_t w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;

                acc0 = acc0 < min ? min : acc0;

                int16_t *output_addr_temp = output_addr;

                *output_addr_temp++ = acc0;

                output_addr += M;
            }

            m += 1;
            input_matrix += 1;
            output_matrix += 1;
        }
    }
}

#ifndef SWAN_SIMULATION
void spmm_fp16_scalar(config_t *config,
                      input_t *input,
                      output_t *output) {

    spmm_config_t *spmm_config = (spmm_config_t *)config;
    spmm_input_t<__fp16> *spmm_input = (spmm_input_t<__fp16> *)input;
    spmm_output_t<__fp16> *spmm_output = (spmm_output_t<__fp16> *)output;

#if CACHE_STATUS == CACHE_STATUS_WARM
    __fp16 *inout0 = spmm_output->inout0;
    __fp16 *inout1 = spmm_output->inout1;
#endif

    // number of layers
    int layer_count = spmm_config->layer_count;

    for (int layer = 0; layer < layer_count; layer++) {
        int M = spmm_config->M[layer];
        int N = spmm_config->N[layer];
        __fp16 *bias_vector = spmm_input->bias_vector[layer];
        __fp16 *weight_value_vector = spmm_input->weight_value_vector[layer];
        int32_t *weight_IDX_vector = spmm_input->weight_IDX_vector[layer];
        uint32_t *weight_NNZ_vector = spmm_input->weight_NNZ_vector[layer];
        __fp16 min = spmm_input->min[layer];
        __fp16 max = spmm_input->max[layer];

#if CACHE_STATUS == CACHE_STATUS_WARM
        __fp16 *input_matrix;
        if (layer == 0) {
            input_matrix = spmm_input->input_matrix[layer];
        } else if (layer % 2) {
            input_matrix = inout0;
        } else {
            input_matrix = inout1;
        }

        __fp16 *output_matrix;
        if (layer == layer_count - 1) {
            output_matrix = spmm_output->output_matrix[layer];
        } else if (layer % 2) {
            output_matrix = inout1;
        } else {
            output_matrix = inout0;
        }
#else
        __fp16 *input_matrix = spmm_input->input_matrix[layer];
        __fp16 *output_matrix = spmm_output->output_matrix[layer];
#endif

        int m = 0;

        while (m + 16 <= M) {
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 acc0 = bias_vector[n];
                _Float16 acc1 = acc0;
                _Float16 acc2 = acc0;
                _Float16 acc3 = acc0;
                _Float16 acc4 = acc0;
                _Float16 acc5 = acc0;
                _Float16 acc6 = acc0;
                _Float16 acc7 = acc0;
                _Float16 acc8 = acc0;
                _Float16 acc9 = acc0;
                _Float16 acc10 = acc0;
                _Float16 acc11 = acc0;
                _Float16 acc12 = acc0;
                _Float16 acc13 = acc0;
                _Float16 acc14 = acc0;
                _Float16 acc15 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)input_matrix + weight_IDX_vector[k] * M;
                    _Float16 w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                    acc8 += (w * *input_addr_temp++);
                    acc9 += (w * *input_addr_temp++);
                    acc10 += (w * *input_addr_temp++);
                    acc11 += (w * *input_addr_temp++);
                    acc12 += (w * *input_addr_temp++);
                    acc13 += (w * *input_addr_temp++);
                    acc14 += (w * *input_addr_temp++);
                    acc15 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;
                acc4 = acc4 > max ? max : acc4;
                acc5 = acc5 > max ? max : acc5;
                acc6 = acc6 > max ? max : acc6;
                acc7 = acc7 > max ? max : acc7;
                acc8 = acc8 > max ? max : acc8;
                acc9 = acc9 > max ? max : acc9;
                acc10 = acc10 > max ? max : acc10;
                acc11 = acc11 > max ? max : acc11;
                acc12 = acc12 > max ? max : acc12;
                acc13 = acc13 > max ? max : acc13;
                acc14 = acc14 > max ? max : acc14;
                acc15 = acc15 > max ? max : acc15;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;
                acc4 = acc4 < min ? min : acc4;
                acc5 = acc5 < min ? min : acc5;
                acc6 = acc6 < min ? min : acc6;
                acc7 = acc7 < min ? min : acc7;
                acc8 = acc8 < min ? min : acc8;
                acc9 = acc9 < min ? min : acc9;
                acc10 = acc10 < min ? min : acc10;
                acc11 = acc11 < min ? min : acc11;
                acc12 = acc12 < min ? min : acc12;
                acc13 = acc13 < min ? min : acc13;
                acc14 = acc14 < min ? min : acc14;
                acc15 = acc15 < min ? min : acc15;

                _Float16 *output_addr_temp = (_Float16 *)output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;
                *output_addr_temp++ = acc4;
                *output_addr_temp++ = acc5;
                *output_addr_temp++ = acc6;
                *output_addr_temp++ = acc7;
                *output_addr_temp++ = acc8;
                *output_addr_temp++ = acc9;
                *output_addr_temp++ = acc10;
                *output_addr_temp++ = acc11;
                *output_addr_temp++ = acc12;
                *output_addr_temp++ = acc13;
                *output_addr_temp++ = acc14;
                *output_addr_temp++ = acc15;

                output_addr += M;
            }

            m += 16;
            input_matrix += 16;
            output_matrix += 16;
        }

        while (m + 8 <= M) {
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 acc0 = bias_vector[n];
                _Float16 acc1 = acc0;
                _Float16 acc2 = acc0;
                _Float16 acc3 = acc0;
                _Float16 acc4 = acc0;
                _Float16 acc5 = acc0;
                _Float16 acc6 = acc0;
                _Float16 acc7 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)input_matrix + weight_IDX_vector[k] * M;
                    _Float16 w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;
                acc4 = acc4 > max ? max : acc4;
                acc5 = acc5 > max ? max : acc5;
                acc6 = acc6 > max ? max : acc6;
                acc7 = acc7 > max ? max : acc7;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;
                acc4 = acc4 < min ? min : acc4;
                acc5 = acc5 < min ? min : acc5;
                acc6 = acc6 < min ? min : acc6;
                acc7 = acc7 < min ? min : acc7;

                _Float16 *output_addr_temp = (_Float16 *)output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;
                *output_addr_temp++ = acc4;
                *output_addr_temp++ = acc5;
                *output_addr_temp++ = acc6;
                *output_addr_temp++ = acc7;

                output_addr += M;
            }

            m += 8;
            input_matrix += 8;
            output_matrix += 8;
        }

        while (m + 4 <= M) {
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 acc0 = bias_vector[n];
                _Float16 acc1 = acc0;
                _Float16 acc2 = acc0;
                _Float16 acc3 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)input_matrix + weight_IDX_vector[k] * M;
                    _Float16 w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;
                acc2 = acc2 > max ? max : acc2;
                acc3 = acc3 > max ? max : acc3;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;
                acc2 = acc2 < min ? min : acc2;
                acc3 = acc3 < min ? min : acc3;

                _Float16 *output_addr_temp = (_Float16 *)output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;
                *output_addr_temp++ = acc2;
                *output_addr_temp++ = acc3;

                output_addr += M;
            }

            m += 4;
            input_matrix += 4;
            output_matrix += 4;
        }

        while (m + 2 <= M) {
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 acc0 = bias_vector[n];
                _Float16 acc1 = acc0;

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)input_matrix + weight_IDX_vector[k] * M;
                    _Float16 w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;
                acc1 = acc1 > max ? max : acc1;

                acc0 = acc0 < min ? min : acc0;
                acc1 = acc1 < min ? min : acc1;

                _Float16 *output_addr_temp = (_Float16 *)output_addr;

                *output_addr_temp++ = acc0;
                *output_addr_temp++ = acc1;

                output_addr += M;
            }

            m += 2;
            input_matrix += 2;
            output_matrix += 2;
        }

        while (m + 1 <= M) {
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 acc0 = bias_vector[n];

                for (int k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)input_matrix + weight_IDX_vector[k] * M;
                    _Float16 w = weight_value_vector[k];
                    acc0 += (w * *input_addr_temp++);
                }

                acc0 = acc0 > max ? max : acc0;

                acc0 = acc0 < min ? min : acc0;

                _Float16 *output_addr_temp = (_Float16 *)output_addr;

                *output_addr_temp++ = acc0;

                output_addr += M;
            }

            m += 1;
            input_matrix += 1;
            output_matrix += 1;
        }
    }
}
#endif