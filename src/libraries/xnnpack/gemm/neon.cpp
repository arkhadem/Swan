#include "neon.hpp"
#include "gemm.hpp"

void gemm_fp32_neon(config_t *config,
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

#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
        while (m + 64 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                const float *input_addr = (const float *)(input_matrix + m);
                const float *weight_addr = (const float *)(weight_matrix + n * K);

#if SWAN_REG_TYPE >= SWAN_REG_1024_TYPE
                float32x32_t vacc0 = vld1oq_dup_f32(bias_addr);
                float32x32_t vacc1 = vld1oq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {
                    const float32x32_t vi0 = vld1oq_f32(input_addr);
                    const float32x32_t vi1 = vld1oq_f32(input_addr + 32);
                    input_addr += M;

                    const float32x32_t vw = vld1oq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmlaoq_f32(vacc0, vi0, vw);
                    vacc1 = vmlaoq_f32(vacc1, vi1, vw);
                }
                const float32x32_t vmax = vld1oq_dup_f32(&max);
                float32x32_t vout0 = vminoq_f32(vacc0, vmax);
                float32x32_t vout1 = vminoq_f32(vacc1, vmax);
                const float32x32_t vmin = vld1oq_dup_f32(&min);
                vout0 = vmaxoq_f32(vout0, vmin);
                vout1 = vmaxoq_f32(vout1, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1oq_f32(output_addr, vout0);
                vst1oq_f32(output_addr + 32, vout1);
#elif SWAN_REG_TYPE >= SWAN_REG_512_TYPE
                float32x16_t vacc0 = vld1qq_dup_f32(bias_addr);
                float32x16_t vacc1 = vld1qq_dup_f32(bias_addr);
                float32x16_t vacc2 = vld1qq_dup_f32(bias_addr);
                float32x16_t vacc3 = vld1qq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x16_t vi0 = vld1qq_f32(input_addr);
                    const float32x16_t vi1 = vld1qq_f32(input_addr + 16);
                    const float32x16_t vi2 = vld1qq_f32(input_addr + 32);
                    const float32x16_t vi3 = vld1qq_f32(input_addr + 48);
                    input_addr += M;

                    const float32x16_t vw = vld1qq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmlaqq_f32(vacc0, vi0, vw);
                    vacc1 = vmlaqq_f32(vacc1, vi1, vw);
                    vacc2 = vmlaqq_f32(vacc2, vi2, vw);
                    vacc3 = vmlaqq_f32(vacc3, vi3, vw);
                }
                const float32x16_t vmax = vld1qq_dup_f32(&max);
                float32x16_t vout0 = vminqq_f32(vacc0, vmax);
                float32x16_t vout1 = vminqq_f32(vacc1, vmax);
                float32x16_t vout2 = vminqq_f32(vacc2, vmax);
                float32x16_t vout3 = vminqq_f32(vacc3, vmax);
                const float32x16_t vmin = vld1qq_dup_f32(&min);
                vout0 = vmaxqq_f32(vout0, vmin);
                vout1 = vmaxqq_f32(vout1, vmin);
                vout2 = vmaxqq_f32(vout2, vmin);
                vout3 = vmaxqq_f32(vout3, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1qq_f32(output_addr, vout0);
                vst1qq_f32(output_addr + 16, vout1);
                vst1qq_f32(output_addr + 32, vout2);
                vst1qq_f32(output_addr + 48, vout3);

#else
                float32x8_t vacc0 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc1 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc2 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc3 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc4 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc5 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc6 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc7 = vld1dq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x8_t vi0 = vld1dq_f32(input_addr);
                    const float32x8_t vi1 = vld1dq_f32(input_addr + 8);
                    const float32x8_t vi2 = vld1dq_f32(input_addr + 16);
                    const float32x8_t vi3 = vld1dq_f32(input_addr + 24);
                    const float32x8_t vi4 = vld1dq_f32(input_addr + 32);
                    const float32x8_t vi5 = vld1dq_f32(input_addr + 40);
                    const float32x8_t vi6 = vld1dq_f32(input_addr + 48);
                    const float32x8_t vi7 = vld1dq_f32(input_addr + 56);
                    input_addr += M;

                    const float32x8_t vw = vld1dq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmladq_f32(vacc0, vi0, vw);
                    vacc1 = vmladq_f32(vacc1, vi1, vw);
                    vacc2 = vmladq_f32(vacc2, vi2, vw);
                    vacc3 = vmladq_f32(vacc3, vi3, vw);
                    vacc4 = vmladq_f32(vacc4, vi4, vw);
                    vacc5 = vmladq_f32(vacc5, vi5, vw);
                    vacc6 = vmladq_f32(vacc6, vi6, vw);
                    vacc7 = vmladq_f32(vacc7, vi7, vw);
                }
                const float32x8_t vmax = vld1dq_dup_f32(&max);
                float32x8_t vout0 = vmindq_f32(vacc0, vmax);
                float32x8_t vout1 = vmindq_f32(vacc1, vmax);
                float32x8_t vout2 = vmindq_f32(vacc2, vmax);
                float32x8_t vout3 = vmindq_f32(vacc3, vmax);
                float32x8_t vout4 = vmindq_f32(vacc4, vmax);
                float32x8_t vout5 = vmindq_f32(vacc5, vmax);
                float32x8_t vout6 = vmindq_f32(vacc6, vmax);
                float32x8_t vout7 = vmindq_f32(vacc7, vmax);
                const float32x8_t vmin = vld1dq_dup_f32(&min);
                vout0 = vmaxdq_f32(vout0, vmin);
                vout1 = vmaxdq_f32(vout1, vmin);
                vout2 = vmaxdq_f32(vout2, vmin);
                vout3 = vmaxdq_f32(vout3, vmin);
                vout4 = vmaxdq_f32(vout4, vmin);
                vout5 = vmaxdq_f32(vout5, vmin);
                vout6 = vmaxdq_f32(vout6, vmin);
                vout7 = vmaxdq_f32(vout7, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1dq_f32(output_addr, vout0);
                vst1dq_f32(output_addr + 8, vout1);
                vst1dq_f32(output_addr + 16, vout2);
                vst1dq_f32(output_addr + 24, vout3);
                vst1dq_f32(output_addr + 32, vout4);
                vst1dq_f32(output_addr + 40, vout5);
                vst1dq_f32(output_addr + 48, vout6);
                vst1dq_f32(output_addr + 56, vout7);
#endif

#ifdef SWAN_SIMULATION
                fake_neon_flusher();
#endif
            }
            m += 64;
        }
#endif

        while (m + 32 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                const float *input_addr = (const float *)(input_matrix + m);
                const float *weight_addr = (const float *)(weight_matrix + n * K);

#if SWAN_REG_TYPE >= SWAN_REG_1024_TYPE
                float32x32_t vacc0 = vld1oq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {
                    const float32x32_t vi0 = vld1oq_f32(input_addr);
                    input_addr += M;

                    const float32x32_t vw = vld1oq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmlaoq_f32(vacc0, vi0, vw);
                }
                const float32x32_t vmax = vld1oq_dup_f32(&max);
                float32x32_t vout0 = vminoq_f32(vacc0, vmax);
                const float32x32_t vmin = vld1oq_dup_f32(&min);
                vout0 = vmaxoq_f32(vout0, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1oq_f32(output_addr, vout0);
#elif SWAN_REG_TYPE >= SWAN_REG_512_TYPE
                float32x16_t vacc0 = vld1qq_dup_f32(bias_addr);
                float32x16_t vacc1 = vld1qq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x16_t vi0 = vld1qq_f32(input_addr);
                    const float32x16_t vi1 = vld1qq_f32(input_addr + 16);
                    input_addr += M;

                    const float32x16_t vw = vld1qq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmlaqq_f32(vacc0, vi0, vw);
                    vacc1 = vmlaqq_f32(vacc1, vi1, vw);
                }
                const float32x16_t vmax = vld1qq_dup_f32(&max);
                float32x16_t vout0 = vminqq_f32(vacc0, vmax);
                float32x16_t vout1 = vminqq_f32(vacc1, vmax);
                const float32x16_t vmin = vld1qq_dup_f32(&min);
                vout0 = vmaxqq_f32(vout0, vmin);
                vout1 = vmaxqq_f32(vout1, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1qq_f32(output_addr, vout0);
                vst1qq_f32(output_addr + 16, vout1);

#elif SWAN_REG_TYPE >= SWAN_REG_256_TYPE
                float32x8_t vacc0 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc1 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc2 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc3 = vld1dq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x8_t vi0 = vld1dq_f32(input_addr);
                    const float32x8_t vi1 = vld1dq_f32(input_addr + 8);
                    const float32x8_t vi2 = vld1dq_f32(input_addr + 16);
                    const float32x8_t vi3 = vld1dq_f32(input_addr + 24);
                    input_addr += M;

                    const float32x8_t vw = vld1dq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmladq_f32(vacc0, vi0, vw);
                    vacc1 = vmladq_f32(vacc1, vi1, vw);
                    vacc2 = vmladq_f32(vacc2, vi2, vw);
                    vacc3 = vmladq_f32(vacc3, vi3, vw);
                }
                const float32x8_t vmax = vld1dq_dup_f32(&max);
                float32x8_t vout0 = vmindq_f32(vacc0, vmax);
                float32x8_t vout1 = vmindq_f32(vacc1, vmax);
                float32x8_t vout2 = vmindq_f32(vacc2, vmax);
                float32x8_t vout3 = vmindq_f32(vacc3, vmax);
                const float32x8_t vmin = vld1dq_dup_f32(&min);
                vout0 = vmaxdq_f32(vout0, vmin);
                vout1 = vmaxdq_f32(vout1, vmin);
                vout2 = vmaxdq_f32(vout2, vmin);
                vout3 = vmaxdq_f32(vout3, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1dq_f32(output_addr, vout0);
                vst1dq_f32(output_addr + 8, vout1);
                vst1dq_f32(output_addr + 16, vout2);
                vst1dq_f32(output_addr + 24, vout3);
#else
                float32x4_t vacc0 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc1 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc2 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc3 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc4 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc5 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc6 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc7 = vld1q_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x4_t vi0 = vld1q_f32(input_addr);
                    const float32x4_t vi1 = vld1q_f32(input_addr + 4);
                    const float32x4_t vi2 = vld1q_f32(input_addr + 8);
                    const float32x4_t vi3 = vld1q_f32(input_addr + 12);
                    const float32x4_t vi4 = vld1q_f32(input_addr + 16);
                    const float32x4_t vi5 = vld1q_f32(input_addr + 20);
                    const float32x4_t vi6 = vld1q_f32(input_addr + 24);
                    const float32x4_t vi7 = vld1q_f32(input_addr + 28);
                    input_addr += M;

                    const float32x4_t vw = vld1q_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmlaq_f32(vacc0, vi0, vw);
                    vacc1 = vmlaq_f32(vacc1, vi1, vw);
                    vacc2 = vmlaq_f32(vacc2, vi2, vw);
                    vacc3 = vmlaq_f32(vacc3, vi3, vw);
                    vacc4 = vmlaq_f32(vacc4, vi4, vw);
                    vacc5 = vmlaq_f32(vacc5, vi5, vw);
                    vacc6 = vmlaq_f32(vacc6, vi6, vw);
                    vacc7 = vmlaq_f32(vacc7, vi7, vw);
                }
                const float32x4_t vmax = vld1q_dup_f32(&max);
                float32x4_t vout0 = vminq_f32(vacc0, vmax);
                float32x4_t vout1 = vminq_f32(vacc1, vmax);
                float32x4_t vout2 = vminq_f32(vacc2, vmax);
                float32x4_t vout3 = vminq_f32(vacc3, vmax);
                float32x4_t vout4 = vminq_f32(vacc4, vmax);
                float32x4_t vout5 = vminq_f32(vacc5, vmax);
                float32x4_t vout6 = vminq_f32(vacc6, vmax);
                float32x4_t vout7 = vminq_f32(vacc7, vmax);
                const float32x4_t vmin = vld1q_dup_f32(&min);
                vout0 = vmaxq_f32(vout0, vmin);
                vout1 = vmaxq_f32(vout1, vmin);
                vout2 = vmaxq_f32(vout2, vmin);
                vout3 = vmaxq_f32(vout3, vmin);
                vout4 = vmaxq_f32(vout4, vmin);
                vout5 = vmaxq_f32(vout5, vmin);
                vout6 = vmaxq_f32(vout6, vmin);
                vout7 = vmaxq_f32(vout7, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1q_f32(output_addr, vout0);
                vst1q_f32(output_addr + 4, vout1);
                vst1q_f32(output_addr + 8, vout2);
                vst1q_f32(output_addr + 12, vout3);
                vst1q_f32(output_addr + 16, vout4);
                vst1q_f32(output_addr + 20, vout5);
                vst1q_f32(output_addr + 24, vout6);
                vst1q_f32(output_addr + 28, vout7);
#endif

#ifdef SWAN_SIMULATION
#if !defined(NEON2RVV)
                fake_neon_flusher();
#endif
#endif
            }
            m += 32;
        }

        while (m + 16 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                const float *input_addr = (const float *)(input_matrix + m);
                const float *weight_addr = (const float *)(weight_matrix + n * K);

#if SWAN_REG_TYPE >= SWAN_REG_512_TYPE
                float32x16_t vacc0 = vld1qq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x16_t vi0 = vld1qq_f32(input_addr);
                    input_addr += M;

                    const float32x16_t vw = vld1qq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmlaqq_f32(vacc0, vi0, vw);
                }
                const float32x16_t vmax = vld1qq_dup_f32(&max);
                float32x16_t vout0 = vminqq_f32(vacc0, vmax);
                const float32x16_t vmin = vld1qq_dup_f32(&min);
                vout0 = vmaxqq_f32(vout0, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1qq_f32(output_addr, vout0);

#elif SWAN_REG_TYPE >= SWAN_REG_256_TYPE
                float32x8_t vacc0 = vld1dq_dup_f32(bias_addr);
                float32x8_t vacc1 = vld1dq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x8_t vi0 = vld1dq_f32(input_addr);
                    const float32x8_t vi1 = vld1dq_f32(input_addr + 8);
                    input_addr += M;

                    const float32x8_t vw = vld1dq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmladq_f32(vacc0, vi0, vw);
                    vacc1 = vmladq_f32(vacc1, vi1, vw);
                }
                const float32x8_t vmax = vld1dq_dup_f32(&max);
                float32x8_t vout0 = vmindq_f32(vacc0, vmax);
                float32x8_t vout1 = vmindq_f32(vacc1, vmax);
                const float32x8_t vmin = vld1dq_dup_f32(&min);
                vout0 = vmaxdq_f32(vout0, vmin);
                vout1 = vmaxdq_f32(vout1, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1dq_f32(output_addr, vout0);
                vst1dq_f32(output_addr + 8, vout1);
#else
                float32x4_t vacc0 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc1 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc2 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc3 = vld1q_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x4_t vi0 = vld1q_f32(input_addr);
                    const float32x4_t vi1 = vld1q_f32(input_addr + 4);
                    const float32x4_t vi2 = vld1q_f32(input_addr + 8);
                    const float32x4_t vi3 = vld1q_f32(input_addr + 12);
                    input_addr += M;

                    const float32x4_t vw = vld1q_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmlaq_f32(vacc0, vi0, vw);
                    vacc1 = vmlaq_f32(vacc1, vi1, vw);
                    vacc2 = vmlaq_f32(vacc2, vi2, vw);
                    vacc3 = vmlaq_f32(vacc3, vi3, vw);
                }
                const float32x4_t vmax = vld1q_dup_f32(&max);
                float32x4_t vout0 = vminq_f32(vacc0, vmax);
                float32x4_t vout1 = vminq_f32(vacc1, vmax);
                float32x4_t vout2 = vminq_f32(vacc2, vmax);
                float32x4_t vout3 = vminq_f32(vacc3, vmax);
                const float32x4_t vmin = vld1q_dup_f32(&min);
                vout0 = vmaxq_f32(vout0, vmin);
                vout1 = vmaxq_f32(vout1, vmin);
                vout2 = vmaxq_f32(vout2, vmin);
                vout3 = vmaxq_f32(vout3, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1q_f32(output_addr, vout0);
                vst1q_f32(output_addr + 4, vout1);
                vst1q_f32(output_addr + 8, vout2);
                vst1q_f32(output_addr + 12, vout3);
#endif
#ifdef SWAN_SIMULATION
#if !defined(NEON2RVV)
                fake_neon_flusher();
#endif
#endif
            }
            m += 16;
        }

        while (m + 8 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                const float *input_addr = (const float *)(input_matrix + m);
                const float *weight_addr = (const float *)(weight_matrix + n * K);

#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
                float32x8_t vacc0 = vld1dq_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x8_t vi0 = vld1dq_f32(input_addr);
                    input_addr += M;

                    const float32x8_t vw = vld1dq_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmladq_f32(vacc0, vi0, vw);
                }
                const float32x8_t vmax = vld1dq_dup_f32(&max);
                float32x8_t vout0 = vmindq_f32(vacc0, vmax);
                const float32x8_t vmin = vld1dq_dup_f32(&min);
                vout0 = vmaxdq_f32(vout0, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1dq_f32(output_addr, vout0);
#else
                float32x4_t vacc0 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc1 = vld1q_dup_f32(bias_addr);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x4_t vi0 = vld1q_f32(input_addr);
                    const float32x4_t vi1 = vld1q_f32(input_addr + 4);
                    input_addr += M;

                    const float32x4_t vw = vld1q_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0 = vmlaq_f32(vacc0, vi0, vw);
                    vacc1 = vmlaq_f32(vacc1, vi1, vw);
                }
                const float32x4_t vmax = vld1q_dup_f32(&max);
                float32x4_t vout0 = vminq_f32(vacc0, vmax);
                float32x4_t vout1 = vminq_f32(vacc1, vmax);
                const float32x4_t vmin = vld1q_dup_f32(&min);
                vout0 = vmaxq_f32(vout0, vmin);
                vout1 = vmaxq_f32(vout1, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1q_f32(output_addr, vout0);
                vst1q_f32(output_addr + 4, vout1);
#endif

#ifdef SWAN_SIMULATION
#if !defined(NEON2RVV)
                fake_neon_flusher();
#endif
#endif
            }
            m += 8;
        }

        while (m + 4 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                float32x4_t vacc0123 = vld1q_dup_f32(bias_addr);

                const float *input_addr = (const float *)(input_matrix + m);
                const float *weight_addr = (const float *)(weight_matrix + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const float32x4_t vi0123 = vld1q_f32(input_addr);
                    input_addr += M;

                    const float32x4_t vw = vld1q_dup_f32(weight_addr);
                    weight_addr += 1;

                    vacc0123 = vmlaq_f32(vacc0123, vi0123, vw);
                }

                const float32x4_t vmax = vld1q_dup_f32(&max);
                float32x4_t vout0123 = vminq_f32(vacc0123, vmax);
                const float32x4_t vmin = vld1q_dup_f32(&min);
                vout0123 = vmaxq_f32(vout0123, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1q_f32(output_addr, vout0123);

#ifdef SWAN_SIMULATION
#if !defined(NEON2RVV)
                fake_neon_flusher();
#endif
#endif
            }
            m += 4;
        }

        for (int n = 0; n < N; n++) {
            for (int m_l = m; m_l < M; m_l++) {
                int input_idx = m_l;
                int weight_idx = n * K;

                float acc = bias_vector[n];
                for (uint32_t k = 0; k < K; k++) {
                    acc += (input_matrix[input_idx] * weight_matrix[weight_idx]);
                    input_idx += M;
                    weight_idx += 1;
                }
                acc = acc < max ? acc : max;
                acc = acc > min ? acc : min;
                output_matrix[n * M + m_l] = acc;
            }
        }
    }
}

void gemm_int32_neon(config_t *config,
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

        const int32x4_t vmin = vld1q_dup_s32(&min);
        const int32x4_t vmax = vld1q_dup_s32(&max);

        int m = 0;

        while (m + 32 <= M) {
            for (int n = 0; n < N; n++) {
                const int32_t *bias_addr = bias_vector + n;
                int32x4_t vacc0123 = vld1q_dup_s32(bias_addr);
                int32x4_t vacc4567 = vacc0123;
                int32x4_t vacc89AB = vacc0123;
                int32x4_t vaccCDEF = vacc0123;
                int32x4_t vaccGHIJ = vacc0123;
                int32x4_t vaccKLMN = vacc0123;
                int32x4_t vaccOPQR = vacc0123;
                int32x4_t vaccSTUV = vacc0123;

                const int32_t *input_addr = (const int32_t *)(input_matrix + m);
                const int32_t *weight_addr = (const int32_t *)(weight_matrix + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const int32x4_t vi0123 = vld1q_s32(input_addr);
                    const int32x4_t vi4567 = vld1q_s32(input_addr + 4);
                    const int32x4_t vi89AB = vld1q_s32(input_addr + 8);
                    const int32x4_t viCDEF = vld1q_s32(input_addr + 12);
                    const int32x4_t viGHIJ = vld1q_s32(input_addr + 16);
                    const int32x4_t viKLMN = vld1q_s32(input_addr + 20);
                    const int32x4_t viOPQR = vld1q_s32(input_addr + 24);
                    const int32x4_t viSTUV = vld1q_s32(input_addr + 28);
                    input_addr += M;

                    const int32x4_t vw = vld1q_dup_s32(weight_addr);
                    weight_addr += 1;

                    const int32x4_t vm0123 = vmulq_s32(vi0123, vw);
                    const int32x4_t vm4567 = vmulq_s32(vi4567, vw);
                    const int32x4_t vm89AB = vmulq_s32(vi89AB, vw);
                    const int32x4_t vmCDEF = vmulq_s32(viCDEF, vw);
                    const int32x4_t vmGHIJ = vmulq_s32(viGHIJ, vw);
                    const int32x4_t vmKLMN = vmulq_s32(viKLMN, vw);
                    const int32x4_t vmOPQR = vmulq_s32(viOPQR, vw);
                    const int32x4_t vmSTUV = vmulq_s32(viSTUV, vw);

                    vacc0123 = vaddq_s32(vacc0123, vm0123);
                    vacc4567 = vaddq_s32(vacc4567, vm4567);
                    vacc89AB = vaddq_s32(vacc89AB, vm89AB);
                    vaccCDEF = vaddq_s32(vaccCDEF, vmCDEF);
                    vaccGHIJ = vaddq_s32(vaccGHIJ, vmGHIJ);
                    vaccKLMN = vaddq_s32(vaccKLMN, vmKLMN);
                    vaccOPQR = vaddq_s32(vaccOPQR, vmOPQR);
                    vaccSTUV = vaddq_s32(vaccSTUV, vmSTUV);
                }

                int32x4_t vout0123 = vminq_s32(vacc0123, vmax);
                int32x4_t vout4567 = vminq_s32(vacc4567, vmax);
                int32x4_t vout89AB = vminq_s32(vacc89AB, vmax);
                int32x4_t voutCDEF = vminq_s32(vaccCDEF, vmax);
                int32x4_t voutGHIJ = vminq_s32(vaccGHIJ, vmax);
                int32x4_t voutKLMN = vminq_s32(vaccKLMN, vmax);
                int32x4_t voutOPQR = vminq_s32(vaccOPQR, vmax);
                int32x4_t voutSTUV = vminq_s32(vaccSTUV, vmax);
                vout0123 = vmaxq_s32(vout0123, vmin);
                vout4567 = vmaxq_s32(vout4567, vmin);
                vout89AB = vmaxq_s32(vout89AB, vmin);
                voutCDEF = vmaxq_s32(voutCDEF, vmin);
                voutGHIJ = vmaxq_s32(voutGHIJ, vmin);
                voutKLMN = vmaxq_s32(voutKLMN, vmin);
                voutOPQR = vmaxq_s32(voutOPQR, vmin);
                voutSTUV = vmaxq_s32(voutSTUV, vmin);

                int32_t *output_addr = (int32_t *)(output_matrix + n * M + m);
                vst1q_s32(output_addr, vout0123);
                vst1q_s32(output_addr + 4, vout4567);
                vst1q_s32(output_addr + 8, vout89AB);
                vst1q_s32(output_addr + 12, voutCDEF);
                vst1q_s32(output_addr + 16, voutGHIJ);
                vst1q_s32(output_addr + 20, voutKLMN);
                vst1q_s32(output_addr + 24, voutOPQR);
                vst1q_s32(output_addr + 28, voutSTUV);
            }
            m += 32;
        }

        while (m + 16 <= M) {
            for (int n = 0; n < N; n++) {
                const int32_t *bias_addr = bias_vector + n;
                int32x4_t vacc0123 = vld1q_dup_s32(bias_addr);
                int32x4_t vacc4567 = vacc0123;
                int32x4_t vacc89AB = vacc0123;
                int32x4_t vaccCDEF = vacc0123;

                const int32_t *input_addr = (const int32_t *)(input_matrix + m);
                const int32_t *weight_addr = (const int32_t *)(weight_matrix + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const int32x4_t vi0123 = vld1q_s32(input_addr);
                    const int32x4_t vi4567 = vld1q_s32(input_addr + 4);
                    const int32x4_t vi89AB = vld1q_s32(input_addr + 8);
                    const int32x4_t viCDEF = vld1q_s32(input_addr + 12);
                    input_addr += M;

                    const int32x4_t vw = vld1q_dup_s32(weight_addr);
                    weight_addr += 1;

                    const int32x4_t vm0123 = vmulq_s32(vi0123, vw);
                    const int32x4_t vm4567 = vmulq_s32(vi4567, vw);
                    const int32x4_t vm89AB = vmulq_s32(vi89AB, vw);
                    const int32x4_t vmCDEF = vmulq_s32(viCDEF, vw);

                    vacc0123 = vaddq_s32(vacc0123, vm0123);
                    vacc4567 = vaddq_s32(vacc4567, vm4567);
                    vacc89AB = vaddq_s32(vacc89AB, vm89AB);
                    vaccCDEF = vaddq_s32(vaccCDEF, vmCDEF);
                }

                int32x4_t vout0123 = vminq_s32(vacc0123, vmax);
                int32x4_t vout4567 = vminq_s32(vacc4567, vmax);
                int32x4_t vout89AB = vminq_s32(vacc89AB, vmax);
                int32x4_t voutCDEF = vminq_s32(vaccCDEF, vmax);
                vout0123 = vmaxq_s32(vout0123, vmin);
                vout4567 = vmaxq_s32(vout4567, vmin);
                vout89AB = vmaxq_s32(vout89AB, vmin);
                voutCDEF = vmaxq_s32(voutCDEF, vmin);

                int32_t *output_addr = (int32_t *)(output_matrix + n * M + m);
                vst1q_s32(output_addr, vout0123);
                vst1q_s32(output_addr + 4, vout4567);
                vst1q_s32(output_addr + 8, vout89AB);
                vst1q_s32(output_addr + 12, voutCDEF);
            }
            m += 16;
        }

        while (m + 8 <= M) {
            for (int n = 0; n < N; n++) {
                const int32_t *bias_addr = bias_vector + n;
                int32x4_t vacc0123 = vld1q_dup_s32(bias_addr);
                int32x4_t vacc4567 = vacc0123;

                const int32_t *input_addr = (const int32_t *)(input_matrix + m);
                const int32_t *weight_addr = (const int32_t *)(weight_matrix + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const int32x4_t vi0123 = vld1q_s32(input_addr);
                    const int32x4_t vi4567 = vld1q_s32(input_addr + 4);
                    input_addr += M;

                    const int32x4_t vw = vld1q_dup_s32(weight_addr);
                    weight_addr += 1;

                    const int32x4_t vm0123 = vmulq_s32(vi0123, vw);
                    const int32x4_t vm4567 = vmulq_s32(vi4567, vw);

                    vacc0123 = vaddq_s32(vacc0123, vm0123);
                    vacc4567 = vaddq_s32(vacc4567, vm4567);
                }

                int32x4_t vout0123 = vminq_s32(vacc0123, vmax);
                int32x4_t vout4567 = vminq_s32(vacc4567, vmax);
                vout0123 = vmaxq_s32(vout0123, vmin);
                vout4567 = vmaxq_s32(vout4567, vmin);

                int32_t *output_addr = (int32_t *)(output_matrix + n * M + m);
                vst1q_s32(output_addr, vout0123);
                vst1q_s32(output_addr + 4, vout4567);
            }
            m += 8;
        }

        while (m + 4 <= M) {
            for (int n = 0; n < N; n++) {
                const int32_t *bias_addr = bias_vector + n;
                int32x4_t vacc0123 = vld1q_dup_s32(bias_addr);

                const int32_t *input_addr = (const int32_t *)(input_matrix + m);
                const int32_t *weight_addr = (const int32_t *)(weight_matrix + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const int32x4_t vi0123 = vld1q_s32(input_addr);
                    input_addr += M;

                    const int32x4_t vw = vld1q_dup_s32(weight_addr);
                    weight_addr += 1;

                    const int32x4_t vm0123 = vmulq_s32(vi0123, vw);

                    vacc0123 = vaddq_s32(vacc0123, vm0123);
                }

                int32x4_t vout0123 = vminq_s32(vacc0123, vmax);
                vout0123 = vmaxq_s32(vout0123, vmin);

                int32_t *output_addr = (int32_t *)(output_matrix + n * M + m);
                vst1q_s32(output_addr, vout0123);
            }
            m += 4;
        }

        for (int n = 0; n < N; n++) {
            for (int m_l = m; m_l < M; m_l++) {
                int32_t acc = bias_vector[n];
                int input_idx = m_l;
                int weight_idx = n * K;

                for (uint32_t k = 0; k < K; k++) {
                    acc += (input_matrix[input_idx] * weight_matrix[weight_idx]);
                    input_idx += M;
                    weight_idx += 1;
                }
                acc = acc < max ? acc : max;
                acc = acc > min ? acc : min;
                output_matrix[n * M + m_l] = acc;
            }
        }
    }
}

void gemm_int16_neon(config_t *config,
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

        const int16x8_t vmin = vld1q_dup_s16(&min);
        const int16x8_t vmax = vld1q_dup_s16(&max);

        int m = 0;

        while (m + 32 <= M) {
            for (int n = 0; n < N; n++) {
                const int16_t *bias_addr = bias_vector + n;
                int16x8_t vacc0123 = vld1q_dup_s16(bias_addr);
                int16x8_t vacc4567 = vacc0123;
                int16x8_t vacc89AB = vacc0123;
                int16x8_t vaccCDEF = vacc0123;

                const int16_t *input_addr = (const int16_t *)(input_matrix + m);
                const int16_t *weight_addr = (const int16_t *)(weight_matrix + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const int16x8_t vi0123 = vld1q_s16(input_addr);
                    const int16x8_t vi4567 = vld1q_s16(input_addr + 8);
                    const int16x8_t vi89AB = vld1q_s16(input_addr + 16);
                    const int16x8_t viCDEF = vld1q_s16(input_addr + 24);
                    input_addr += M;

                    const int16x8_t vw = vld1q_dup_s16(weight_addr);
                    weight_addr += 1;

                    const int16x8_t vm0123 = vmulq_s16(vi0123, vw);
                    const int16x8_t vm4567 = vmulq_s16(vi4567, vw);
                    const int16x8_t vm89AB = vmulq_s16(vi89AB, vw);
                    const int16x8_t vmCDEF = vmulq_s16(viCDEF, vw);

                    vacc0123 = vaddq_s16(vacc0123, vm0123);
                    vacc4567 = vaddq_s16(vacc4567, vm4567);
                    vacc89AB = vaddq_s16(vacc89AB, vm89AB);
                    vaccCDEF = vaddq_s16(vaccCDEF, vmCDEF);
                }

                int16x8_t vout0123 = vminq_s16(vacc0123, vmax);
                int16x8_t vout4567 = vminq_s16(vacc4567, vmax);
                int16x8_t vout89AB = vminq_s16(vacc89AB, vmax);
                int16x8_t voutCDEF = vminq_s16(vaccCDEF, vmax);
                vout0123 = vmaxq_s16(vout0123, vmin);
                vout4567 = vmaxq_s16(vout4567, vmin);
                vout89AB = vmaxq_s16(vout89AB, vmin);
                voutCDEF = vmaxq_s16(voutCDEF, vmin);

                int16_t *output_addr = (int16_t *)(output_matrix + n * M + m);
                vst1q_s16(output_addr, vout0123);
                vst1q_s16(output_addr + 8, vout4567);
                vst1q_s16(output_addr + 16, vout89AB);
                vst1q_s16(output_addr + 24, voutCDEF);
            }
            m += 32;
        }

        while (m + 16 <= M) {
            for (int n = 0; n < N; n++) {
                const int16_t *bias_addr = bias_vector + n;
                int16x8_t vacc0123 = vld1q_dup_s16(bias_addr);
                int16x8_t vacc4567 = vacc0123;

                const int16_t *input_addr = (const int16_t *)(input_matrix + m);
                const int16_t *weight_addr = (const int16_t *)(weight_matrix + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const int16x8_t vi0123 = vld1q_s16(input_addr);
                    const int16x8_t vi4567 = vld1q_s16(input_addr + 8);
                    input_addr += M;

                    const int16x8_t vw = vld1q_dup_s16(weight_addr);
                    weight_addr += 1;

                    const int16x8_t vm0123 = vmulq_s16(vi0123, vw);
                    const int16x8_t vm4567 = vmulq_s16(vi4567, vw);

                    vacc0123 = vaddq_s16(vacc0123, vm0123);
                    vacc4567 = vaddq_s16(vacc4567, vm4567);
                }

                int16x8_t vout0123 = vminq_s16(vacc0123, vmax);
                int16x8_t vout4567 = vminq_s16(vacc4567, vmax);
                vout0123 = vmaxq_s16(vout0123, vmin);
                vout4567 = vmaxq_s16(vout4567, vmin);

                int16_t *output_addr = (int16_t *)(output_matrix + n * M + m);
                vst1q_s16(output_addr, vout0123);
                vst1q_s16(output_addr + 8, vout4567);
            }
            m += 16;
        }

        while (m + 8 <= M) {
            for (int n = 0; n < N; n++) {
                const int16_t *bias_addr = bias_vector + n;
                int16x8_t vacc0123 = vld1q_dup_s16(bias_addr);

                const int16_t *input_addr = (const int16_t *)(input_matrix + m);
                const int16_t *weight_addr = (const int16_t *)(weight_matrix + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const int16x8_t vi0123 = vld1q_s16(input_addr);
                    input_addr += M;

                    const int16x8_t vw = vld1q_dup_s16(weight_addr);
                    weight_addr += 1;

                    const int16x8_t vm0123 = vmulq_s16(vi0123, vw);

                    vacc0123 = vaddq_s16(vacc0123, vm0123);
                }

                int16x8_t vout0123 = vminq_s16(vacc0123, vmax);
                vout0123 = vmaxq_s16(vout0123, vmin);

                int16_t *output_addr = (int16_t *)(output_matrix + n * M + m);
                vst1q_s16(output_addr, vout0123);
            }
            m += 8;
        }

        for (int n = 0; n < N; n++) {
            for (int m_l = m; m_l < M; m_l++) {
                int16_t acc = bias_vector[n];
                int input_idx = m_l;
                int weight_idx = n * K;

                for (uint32_t k = 0; k < K; k++) {
                    acc += (input_matrix[input_idx] * weight_matrix[weight_idx]);
                    input_idx += M;
                    weight_idx += 1;
                }
                acc = acc < max ? acc : max;
                acc = acc > min ? acc : min;
                output_matrix[n * M + m_l] = acc;
            }
        }
    }
}

#ifndef SWAN_SIMULATION
void gemm_fp16_neon(config_t *config,
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

        const float16x8_t vmax = vreinterpretq_f16_u16(vld1q_dup_u16(&max));
        const float16x8_t vmin = vreinterpretq_f16_u16(vld1q_dup_u16(&min));

        const __fp16 *input_f16 = (const __fp16 *)input_matrix;
        const __fp16 *bias_f16 = (const __fp16 *)bias_vector;
        const __fp16 *weight_f16 = (const __fp16 *)weight_matrix;
        __fp16 *output_f16 = (__fp16 *)output_matrix;

        int m = 0;

        while (m + 32 <= M) {
            for (int n = 0; n < N; n++) {
                const __fp16 *bias_addr = bias_f16 + n;
                float16x8_t vacc0123 = vld1q_dup_f16(bias_addr);
                float16x8_t vacc4567 = vacc0123;
                float16x8_t vacc89AB = vacc0123;
                float16x8_t vaccCDEF = vacc0123;

                const __fp16 *input_addr = (const __fp16 *)(input_f16 + m);
                const __fp16 *weight_addr = (const __fp16 *)(weight_f16 + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const float16x8_t vi0123 = vld1q_f16(input_addr);
                    const float16x8_t vi4567 = vld1q_f16(input_addr + 8);
                    const float16x8_t vi89AB = vld1q_f16(input_addr + 16);
                    const float16x8_t viCDEF = vld1q_f16(input_addr + 24);
                    input_addr += M;

                    const float16x8_t vw = vld1q_dup_f16(weight_addr);
                    weight_addr += 1;

                    const float16x8_t vm0123 = vmulq_f16(vi0123, vw);
                    const float16x8_t vm4567 = vmulq_f16(vi4567, vw);
                    const float16x8_t vm89AB = vmulq_f16(vi89AB, vw);
                    const float16x8_t vmCDEF = vmulq_f16(viCDEF, vw);

                    vacc0123 = vaddq_f16(vacc0123, vm0123);
                    vacc4567 = vaddq_f16(vacc4567, vm4567);
                    vacc89AB = vaddq_f16(vacc89AB, vm89AB);
                    vaccCDEF = vaddq_f16(vaccCDEF, vmCDEF);
                }

                float16x8_t vout0123 = vminq_f16(vacc0123, vmax);
                float16x8_t vout4567 = vminq_f16(vacc4567, vmax);
                float16x8_t vout89AB = vminq_f16(vacc89AB, vmax);
                float16x8_t voutCDEF = vminq_f16(vaccCDEF, vmax);
                vout0123 = vmaxq_f16(vout0123, vmin);
                vout4567 = vmaxq_f16(vout4567, vmin);
                vout89AB = vmaxq_f16(vout89AB, vmin);
                voutCDEF = vmaxq_f16(voutCDEF, vmin);

                __fp16 *output_addr = (__fp16 *)(output_f16 + n * M + m);
                vst1q_f16(output_addr, vout0123);
                vst1q_f16(output_addr + 8, vout4567);
                vst1q_f16(output_addr + 16, vout89AB);
                vst1q_f16(output_addr + 24, voutCDEF);
            }
            m += 32;
        }

        while (m + 16 <= M) {
            for (int n = 0; n < N; n++) {
                const __fp16 *bias_addr = bias_f16 + n;
                float16x8_t vacc0123 = vld1q_dup_f16(bias_addr);
                float16x8_t vacc4567 = vacc0123;

                const __fp16 *input_addr = (const __fp16 *)(input_f16 + m);
                const __fp16 *weight_addr = (const __fp16 *)(weight_f16 + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const float16x8_t vi0123 = vld1q_f16(input_addr);
                    const float16x8_t vi4567 = vld1q_f16(input_addr + 8);
                    input_addr += M;

                    const float16x8_t vw = vld1q_dup_f16(weight_addr);
                    weight_addr += 1;

                    const float16x8_t vm0123 = vmulq_f16(vi0123, vw);
                    const float16x8_t vm4567 = vmulq_f16(vi4567, vw);

                    vacc0123 = vaddq_f16(vacc0123, vm0123);
                    vacc4567 = vaddq_f16(vacc4567, vm4567);
                }

                float16x8_t vout0123 = vminq_f16(vacc0123, vmax);
                float16x8_t vout4567 = vminq_f16(vacc4567, vmax);
                vout0123 = vmaxq_f16(vout0123, vmin);
                vout4567 = vmaxq_f16(vout4567, vmin);

                __fp16 *output_addr = (__fp16 *)(output_f16 + n * M + m);
                vst1q_f16(output_addr, vout0123);
                vst1q_f16(output_addr + 8, vout4567);
            }
            m += 16;
        }

        while (m + 8 <= M) {
            for (int n = 0; n < N; n++) {
                const __fp16 *bias_addr = bias_f16 + n;
                float16x8_t vacc0123 = vld1q_dup_f16(bias_addr);

                const __fp16 *input_addr = (const __fp16 *)(input_f16 + m);
                const __fp16 *weight_addr = (const __fp16 *)(weight_f16 + n * K);

                for (uint32_t k = 0; k < K; k++) {

                    const float16x8_t vi0123 = vld1q_f16(input_addr);
                    input_addr += M;

                    const float16x8_t vw = vld1q_dup_f16(weight_addr);
                    weight_addr += 1;

                    const float16x8_t vm0123 = vmulq_f16(vi0123, vw);

                    vacc0123 = vaddq_f16(vacc0123, vm0123);
                }

                float16x8_t vout0123 = vminq_f16(vacc0123, vmax);
                vout0123 = vmaxq_f16(vout0123, vmin);

                __fp16 *output_addr = (__fp16 *)(output_f16 + n * M + m);
                vst1q_f16(output_addr, vout0123);
            }
            m += 8;
        }

        for (int n = 0; n < N; n++) {
            for (int m_l = m; m_l < M; m_l++) {
                __fp16 acc = bias_vector[n];
                int input_idx = m_l;
                int weight_idx = n * K;

                for (uint32_t k = 0; k < K; k++) {
                    acc += (input_matrix[input_idx] * weight_matrix[weight_idx]);
                    input_idx += M;
                    weight_idx += 1;
                }
                acc = acc < max ? acc : max;
                acc = acc > min ? acc : min;
                output_matrix[n * M + m_l] = acc;
            }
        }
    }
}
#endif