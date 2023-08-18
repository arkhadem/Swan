#include "scalar.hpp"
#include "gemm.hpp"
#include "xnnpack.hpp"

void gemm_fp32_scalar(config_t *config,
                      input_t *input,
                      output_t *output) {

    gemm_config_t *gemm_config = (gemm_config_t *)config;
    gemm_input_t<float> *gemm_input = (gemm_input_t<float> *)input;
    gemm_output_t<float> *gemm_output = (gemm_output_t<float> *)output;

#if CACHE_STATUS == CACHE_STATUS_WARM
    float *inout0 = gemm_output->inout0;
    float *inout1 = gemm_output->inout1;
#endif

    // number of layers
    int layer_count = gemm_config->layer_count;

    for (int layer = 0; layer < layer_count; layer++) {
        int M = gemm_config->M[layer];
        int N = gemm_config->N[layer];
        int K = gemm_config->K[layer];
        float *bias_vector = gemm_input->bias_vector[layer];
        float *weight_matrix = gemm_input->weight_matrix[layer];
        float min = gemm_input->min[layer];
        float max = gemm_input->max[layer];

#if CACHE_STATUS == CACHE_STATUS_WARM
        float *input_matrix;
        if (layer == 0) {
            input_matrix = gemm_input->input_matrix[layer];
        } else if (layer % 2) {
            input_matrix = inout0;
        } else {
            input_matrix = inout1;
        }

        float *output_matrix;
        if (layer == layer_count - 1) {
            output_matrix = gemm_output->output_matrix[layer];
        } else if (layer % 2) {
            output_matrix = inout1;
        } else {
            output_matrix = inout0;
        }
#else
        float *input_matrix = gemm_input->input_matrix[layer];
        float *output_matrix = gemm_output->output_matrix[layer];
#endif

        int m = 0;

        while (m + 16 <= M) {
            float *weight_addr = weight_matrix;
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float *intput_addr = input_matrix;

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

                for (int k = 0; k < K; k++) {
                    float *input_addr_temp = intput_addr;
                    float w = *weight_addr++;
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
                    intput_addr += M;
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
            float *weight_addr = weight_matrix;
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float *intput_addr = input_matrix;

                float acc0 = bias_vector[n];
                float acc1 = acc0;
                float acc2 = acc0;
                float acc3 = acc0;
                float acc4 = acc0;
                float acc5 = acc0;
                float acc6 = acc0;
                float acc7 = acc0;

                for (int k = 0; k < K; k++) {
                    float *input_addr_temp = intput_addr;
                    float w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            float *weight_addr = weight_matrix;
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float *intput_addr = input_matrix;

                float acc0 = bias_vector[n];
                float acc1 = acc0;
                float acc2 = acc0;
                float acc3 = acc0;

                for (int k = 0; k < K; k++) {
                    float *input_addr_temp = intput_addr;
                    float w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            float *weight_addr = weight_matrix;
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float *intput_addr = input_matrix;

                float acc0 = bias_vector[n];
                float acc1 = acc0;

                for (int k = 0; k < K; k++) {
                    float *input_addr_temp = intput_addr;
                    float w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            float *weight_addr = weight_matrix;
            float *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                float *intput_addr = input_matrix;

                float acc0 = bias_vector[n];

                for (int k = 0; k < K; k++) {
                    float *input_addr_temp = intput_addr;
                    float w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    intput_addr += M;
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

void gemm_int32_scalar(config_t *config,
                       input_t *input,
                       output_t *output) {

    gemm_config_t *gemm_config = (gemm_config_t *)config;
    gemm_input_t<int32_t> *gemm_input = (gemm_input_t<int32_t> *)input;
    gemm_output_t<int32_t> *gemm_output = (gemm_output_t<int32_t> *)output;

#if CACHE_STATUS == CACHE_STATUS_WARM
    int32_t *inout0 = gemm_output->inout0;
    int32_t *inout1 = gemm_output->inout1;
#endif

    // number of layers
    int layer_count = gemm_config->layer_count;

    for (int layer = 0; layer < layer_count; layer++) {
        int M = gemm_config->M[layer];
        int N = gemm_config->N[layer];
        int K = gemm_config->K[layer];
        int32_t *bias_vector = gemm_input->bias_vector[layer];
        int32_t *weight_matrix = gemm_input->weight_matrix[layer];
        int32_t min = gemm_input->min[layer];
        int32_t max = gemm_input->max[layer];

#if CACHE_STATUS == CACHE_STATUS_WARM
        int32_t *input_matrix;
        if (layer == 0) {
            input_matrix = gemm_input->input_matrix[layer];
        } else if (layer % 2) {
            input_matrix = inout0;
        } else {
            input_matrix = inout1;
        }

        int32_t *output_matrix;
        if (layer == layer_count - 1) {
            output_matrix = gemm_output->output_matrix[layer];
        } else if (layer % 2) {
            output_matrix = inout1;
        } else {
            output_matrix = inout0;
        }
#else
        int32_t *input_matrix = gemm_input->input_matrix[layer];
        int32_t *output_matrix = gemm_output->output_matrix[layer];
#endif

        int m = 0;

        while (m + 16 <= M) {
            int32_t *weight_addr = weight_matrix;
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t *intput_addr = input_matrix;

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

                for (int k = 0; k < K; k++) {
                    int32_t *input_addr_temp = intput_addr;
                    int32_t w = *weight_addr++;
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
                    intput_addr += M;
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
            int32_t *weight_addr = weight_matrix;
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t *intput_addr = input_matrix;

                int32_t acc0 = bias_vector[n];
                int32_t acc1 = acc0;
                int32_t acc2 = acc0;
                int32_t acc3 = acc0;
                int32_t acc4 = acc0;
                int32_t acc5 = acc0;
                int32_t acc6 = acc0;
                int32_t acc7 = acc0;

                for (int k = 0; k < K; k++) {
                    int32_t *input_addr_temp = intput_addr;
                    int32_t w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            int32_t *weight_addr = weight_matrix;
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t *intput_addr = input_matrix;

                int32_t acc0 = bias_vector[n];
                int32_t acc1 = acc0;
                int32_t acc2 = acc0;
                int32_t acc3 = acc0;

                for (int k = 0; k < K; k++) {
                    int32_t *input_addr_temp = intput_addr;
                    int32_t w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            int32_t *weight_addr = weight_matrix;
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t *intput_addr = input_matrix;

                int32_t acc0 = bias_vector[n];
                int32_t acc1 = acc0;

                for (int k = 0; k < K; k++) {
                    int32_t *input_addr_temp = intput_addr;
                    int32_t w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            int32_t *weight_addr = weight_matrix;
            int32_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int32_t *intput_addr = input_matrix;

                int32_t acc0 = bias_vector[n];

                for (int k = 0; k < K; k++) {
                    int32_t *input_addr_temp = intput_addr;
                    int32_t w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    intput_addr += M;
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

void gemm_int16_scalar(config_t *config,
                       input_t *input,
                       output_t *output) {

    gemm_config_t *gemm_config = (gemm_config_t *)config;
    gemm_input_t<int16_t> *gemm_input = (gemm_input_t<int16_t> *)input;
    gemm_output_t<int16_t> *gemm_output = (gemm_output_t<int16_t> *)output;

#if CACHE_STATUS == CACHE_STATUS_WARM
    int16_t *inout0 = gemm_output->inout0;
    int16_t *inout1 = gemm_output->inout1;
#endif

    // number of layers
    int layer_count = gemm_config->layer_count;

    for (int layer = 0; layer < layer_count; layer++) {
        int M = gemm_config->M[layer];
        int N = gemm_config->N[layer];
        int K = gemm_config->K[layer];
        int16_t *bias_vector = gemm_input->bias_vector[layer];
        int16_t *weight_matrix = gemm_input->weight_matrix[layer];
        int16_t min = gemm_input->min[layer];
        int16_t max = gemm_input->max[layer];

#if CACHE_STATUS == CACHE_STATUS_WARM
        int16_t *input_matrix;
        if (layer == 0) {
            input_matrix = gemm_input->input_matrix[layer];
        } else if (layer % 2) {
            input_matrix = inout0;
        } else {
            input_matrix = inout1;
        }

        int16_t *output_matrix;
        if (layer == layer_count - 1) {
            output_matrix = gemm_output->output_matrix[layer];
        } else if (layer % 2) {
            output_matrix = inout1;
        } else {
            output_matrix = inout0;
        }
#else
        int16_t *input_matrix = gemm_input->input_matrix[layer];
        int16_t *output_matrix = gemm_output->output_matrix[layer];
#endif

        int m = 0;

        while (m + 16 <= M) {
            int16_t *weight_addr = weight_matrix;
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t *intput_addr = input_matrix;

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

                for (int k = 0; k < K; k++) {
                    int16_t *input_addr_temp = intput_addr;
                    int16_t w = *weight_addr++;
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
                    intput_addr += M;
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
            int16_t *weight_addr = weight_matrix;
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t *intput_addr = input_matrix;

                int16_t acc0 = bias_vector[n];
                int16_t acc1 = acc0;
                int16_t acc2 = acc0;
                int16_t acc3 = acc0;
                int16_t acc4 = acc0;
                int16_t acc5 = acc0;
                int16_t acc6 = acc0;
                int16_t acc7 = acc0;

                for (int k = 0; k < K; k++) {
                    int16_t *input_addr_temp = intput_addr;
                    int16_t w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            int16_t *weight_addr = weight_matrix;
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t *intput_addr = input_matrix;

                int16_t acc0 = bias_vector[n];
                int16_t acc1 = acc0;
                int16_t acc2 = acc0;
                int16_t acc3 = acc0;

                for (int k = 0; k < K; k++) {
                    int16_t *input_addr_temp = intput_addr;
                    int16_t w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            int16_t *weight_addr = weight_matrix;
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t *intput_addr = input_matrix;

                int16_t acc0 = bias_vector[n];
                int16_t acc1 = acc0;

                for (int k = 0; k < K; k++) {
                    int16_t *input_addr_temp = intput_addr;
                    int16_t w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            int16_t *weight_addr = weight_matrix;
            int16_t *output_addr = output_matrix;

            for (int n = 0; n < N; n++) {
                int16_t *intput_addr = input_matrix;

                int16_t acc0 = bias_vector[n];

                for (int k = 0; k < K; k++) {
                    int16_t *input_addr_temp = intput_addr;
                    int16_t w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    intput_addr += M;
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
void gemm_fp16_scalar(config_t *config,
                      input_t *input,
                      output_t *output) {

    gemm_config_t *gemm_config = (gemm_config_t *)config;
    gemm_input_t<__fp16> *gemm_input = (gemm_input_t<__fp16> *)input;
    gemm_output_t<__fp16> *gemm_output = (gemm_output_t<__fp16> *)output;

#if CACHE_STATUS == CACHE_STATUS_WARM
    __fp16 *inout0 = gemm_output->inout0;
    __fp16 *inout1 = gemm_output->inout1;
#endif

    // number of layers
    int layer_count = gemm_config->layer_count;

    for (int layer = 0; layer < layer_count; layer++) {
        int M = gemm_config->M[layer];
        int N = gemm_config->N[layer];
        int K = gemm_config->K[layer];
        __fp16 *bias_vector = gemm_input->bias_vector[layer];
        __fp16 *weight_matrix = gemm_input->weight_matrix[layer];
        __fp16 min = gemm_input->min[layer];
        __fp16 max = gemm_input->max[layer];

#if CACHE_STATUS == CACHE_STATUS_WARM
        __fp16 *input_matrix;
        if (layer == 0) {
            input_matrix = gemm_input->input_matrix[layer];
        } else if (layer % 2) {
            input_matrix = inout0;
        } else {
            input_matrix = inout1;
        }

        __fp16 *output_matrix;
        if (layer == layer_count - 1) {
            output_matrix = gemm_output->output_matrix[layer];
        } else if (layer % 2) {
            output_matrix = inout1;
        } else {
            output_matrix = inout0;
        }
#else
        __fp16 *input_matrix = gemm_input->input_matrix[layer];
        __fp16 *output_matrix = gemm_output->output_matrix[layer];
#endif

        int m = 0;

        while (m + 16 <= M) {
            _Float16 *weight_addr = (_Float16 *)weight_matrix;
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 *intput_addr = (_Float16 *)input_matrix;

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

                for (int k = 0; k < K; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)intput_addr;
                    _Float16 w = *weight_addr++;
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
                    intput_addr += M;
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
            _Float16 *weight_addr = (_Float16 *)weight_matrix;
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 *intput_addr = (_Float16 *)input_matrix;

                _Float16 acc0 = bias_vector[n];
                _Float16 acc1 = acc0;
                _Float16 acc2 = acc0;
                _Float16 acc3 = acc0;
                _Float16 acc4 = acc0;
                _Float16 acc5 = acc0;
                _Float16 acc6 = acc0;
                _Float16 acc7 = acc0;

                for (int k = 0; k < K; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)intput_addr;
                    _Float16 w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    acc4 += (w * *input_addr_temp++);
                    acc5 += (w * *input_addr_temp++);
                    acc6 += (w * *input_addr_temp++);
                    acc7 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            _Float16 *weight_addr = (_Float16 *)weight_matrix;
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 *intput_addr = (_Float16 *)input_matrix;

                _Float16 acc0 = bias_vector[n];
                _Float16 acc1 = acc0;
                _Float16 acc2 = acc0;
                _Float16 acc3 = acc0;

                for (int k = 0; k < K; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)intput_addr;
                    _Float16 w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    acc2 += (w * *input_addr_temp++);
                    acc3 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            _Float16 *weight_addr = (_Float16 *)weight_matrix;
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 *intput_addr = (_Float16 *)input_matrix;

                _Float16 acc0 = bias_vector[n];
                _Float16 acc1 = acc0;

                for (int k = 0; k < K; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)intput_addr;
                    _Float16 w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    acc1 += (w * *input_addr_temp++);
                    intput_addr += M;
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
            _Float16 *weight_addr = (_Float16 *)weight_matrix;
            _Float16 *output_addr = (_Float16 *)output_matrix;

            for (int n = 0; n < N; n++) {
                _Float16 *intput_addr = (_Float16 *)input_matrix;

                _Float16 acc0 = bias_vector[n];

                for (int k = 0; k < K; k++) {
                    _Float16 *input_addr_temp = (_Float16 *)intput_addr;
                    _Float16 w = *weight_addr++;
                    acc0 += (w * *input_addr_temp++);
                    intput_addr += M;
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
