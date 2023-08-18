#include "neon.hpp"
#include "spmm.hpp"

void spmm_fp32_neon(config_t *config,
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

        const float32x4_t vmin = vld1q_dup_f32(&min);
        const float32x4_t vmax = vld1q_dup_f32(&max);

        int m = 0;

        while (m + 32 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                float32x4_t vacc0123 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc4567 = vacc0123;
                float32x4_t vacc89AB = vacc0123;
                float32x4_t vaccCDEF = vacc0123;
                float32x4_t vaccGHIJ = vacc0123;
                float32x4_t vaccKLMN = vacc0123;
                float32x4_t vaccOPQR = vacc0123;
                float32x4_t vaccSTUV = vacc0123;

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const float *input_addr = (const float *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const float32x4_t vi0123 = vld1q_f32(input_addr);
                    const float32x4_t vi4567 = vld1q_f32(input_addr + 4);
                    const float32x4_t vi89AB = vld1q_f32(input_addr + 8);
                    const float32x4_t viCDEF = vld1q_f32(input_addr + 12);
                    const float32x4_t viGHIJ = vld1q_f32(input_addr + 16);
                    const float32x4_t viKLMN = vld1q_f32(input_addr + 20);
                    const float32x4_t viOPQR = vld1q_f32(input_addr + 24);
                    const float32x4_t viSTUV = vld1q_f32(input_addr + 28);

                    const float *weight_addr = (const float *)(weight_value_vector + k);
                    const float32x4_t vw = vld1q_dup_f32(weight_addr);

                    const float32x4_t vm0123 = vmulq_f32(vi0123, vw);
                    const float32x4_t vm4567 = vmulq_f32(vi4567, vw);
                    const float32x4_t vm89AB = vmulq_f32(vi89AB, vw);
                    const float32x4_t vmCDEF = vmulq_f32(viCDEF, vw);
                    const float32x4_t vmGHIJ = vmulq_f32(viGHIJ, vw);
                    const float32x4_t vmKLMN = vmulq_f32(viKLMN, vw);
                    const float32x4_t vmOPQR = vmulq_f32(viOPQR, vw);
                    const float32x4_t vmSTUV = vmulq_f32(viSTUV, vw);

                    vacc0123 = vaddq_f32(vacc0123, vm0123);
                    vacc4567 = vaddq_f32(vacc4567, vm4567);
                    vacc89AB = vaddq_f32(vacc89AB, vm89AB);
                    vaccCDEF = vaddq_f32(vaccCDEF, vmCDEF);
                    vaccGHIJ = vaddq_f32(vaccGHIJ, vmGHIJ);
                    vaccKLMN = vaddq_f32(vaccKLMN, vmKLMN);
                    vaccOPQR = vaddq_f32(vaccOPQR, vmOPQR);
                    vaccSTUV = vaddq_f32(vaccSTUV, vmSTUV);
                }

                float32x4_t vout0123 = vminq_f32(vacc0123, vmax);
                float32x4_t vout4567 = vminq_f32(vacc4567, vmax);
                float32x4_t vout89AB = vminq_f32(vacc89AB, vmax);
                float32x4_t voutCDEF = vminq_f32(vaccCDEF, vmax);
                float32x4_t voutGHIJ = vminq_f32(vaccGHIJ, vmax);
                float32x4_t voutKLMN = vminq_f32(vaccKLMN, vmax);
                float32x4_t voutOPQR = vminq_f32(vaccOPQR, vmax);
                float32x4_t voutSTUV = vminq_f32(vaccSTUV, vmax);
                vout0123 = vmaxq_f32(vout0123, vmin);
                vout4567 = vmaxq_f32(vout4567, vmin);
                vout89AB = vmaxq_f32(vout89AB, vmin);
                voutCDEF = vmaxq_f32(voutCDEF, vmin);
                voutGHIJ = vmaxq_f32(voutGHIJ, vmin);
                voutKLMN = vmaxq_f32(voutKLMN, vmin);
                voutOPQR = vmaxq_f32(voutOPQR, vmin);
                voutSTUV = vmaxq_f32(voutSTUV, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1q_f32(output_addr, vout0123);
                vst1q_f32(output_addr + 4, vout4567);
                vst1q_f32(output_addr + 8, vout89AB);
                vst1q_f32(output_addr + 12, voutCDEF);
                vst1q_f32(output_addr + 16, voutGHIJ);
                vst1q_f32(output_addr + 20, voutKLMN);
                vst1q_f32(output_addr + 24, voutOPQR);
                vst1q_f32(output_addr + 28, voutSTUV);
            }
            m += 32;
        }

        while (m + 16 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                float32x4_t vacc0123 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc4567 = vacc0123;
                float32x4_t vacc89AB = vacc0123;
                float32x4_t vaccCDEF = vacc0123;

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const float *input_addr = (const float *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const float32x4_t vi0123 = vld1q_f32(input_addr);
                    const float32x4_t vi4567 = vld1q_f32(input_addr + 4);
                    const float32x4_t vi89AB = vld1q_f32(input_addr + 8);
                    const float32x4_t viCDEF = vld1q_f32(input_addr + 12);

                    const float *weight_addr = (const float *)(weight_value_vector + k);
                    const float32x4_t vw = vld1q_dup_f32(weight_addr);

                    const float32x4_t vm0123 = vmulq_f32(vi0123, vw);
                    const float32x4_t vm4567 = vmulq_f32(vi4567, vw);
                    const float32x4_t vm89AB = vmulq_f32(vi89AB, vw);
                    const float32x4_t vmCDEF = vmulq_f32(viCDEF, vw);

                    vacc0123 = vaddq_f32(vacc0123, vm0123);
                    vacc4567 = vaddq_f32(vacc4567, vm4567);
                    vacc89AB = vaddq_f32(vacc89AB, vm89AB);
                    vaccCDEF = vaddq_f32(vaccCDEF, vmCDEF);
                }

                float32x4_t vout0123 = vminq_f32(vacc0123, vmax);
                float32x4_t vout4567 = vminq_f32(vacc4567, vmax);
                float32x4_t vout89AB = vminq_f32(vacc89AB, vmax);
                float32x4_t voutCDEF = vminq_f32(vaccCDEF, vmax);
                vout0123 = vmaxq_f32(vout0123, vmin);
                vout4567 = vmaxq_f32(vout4567, vmin);
                vout89AB = vmaxq_f32(vout89AB, vmin);
                voutCDEF = vmaxq_f32(voutCDEF, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1q_f32(output_addr, vout0123);
                vst1q_f32(output_addr + 4, vout4567);
                vst1q_f32(output_addr + 8, vout89AB);
                vst1q_f32(output_addr + 12, voutCDEF);
            }
            m += 16;
        }

        while (m + 8 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                float32x4_t vacc0123 = vld1q_dup_f32(bias_addr);
                float32x4_t vacc4567 = vacc0123;

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const float *input_addr = (const float *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const float32x4_t vi0123 = vld1q_f32(input_addr);
                    const float32x4_t vi4567 = vld1q_f32(input_addr + 4);

                    const float *weight_addr = (const float *)(weight_value_vector + k);
                    const float32x4_t vw = vld1q_dup_f32(weight_addr);

                    const float32x4_t vm0123 = vmulq_f32(vi0123, vw);
                    const float32x4_t vm4567 = vmulq_f32(vi4567, vw);

                    vacc0123 = vaddq_f32(vacc0123, vm0123);
                    vacc4567 = vaddq_f32(vacc4567, vm4567);
                }

                float32x4_t vout0123 = vminq_f32(vacc0123, vmax);
                float32x4_t vout4567 = vminq_f32(vacc4567, vmax);
                vout0123 = vmaxq_f32(vout0123, vmin);
                vout4567 = vmaxq_f32(vout4567, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1q_f32(output_addr, vout0123);
                vst1q_f32(output_addr + 4, vout4567);
            }
            m += 8;
        }

        while (m + 4 <= M) {
            for (int n = 0; n < N; n++) {
                const float *bias_addr = bias_vector + n;
                float32x4_t vacc0123 = vld1q_dup_f32(bias_addr);

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const float *input_addr = (const float *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const float32x4_t vi0123 = vld1q_f32(input_addr);

                    const float *weight_addr = (const float *)(weight_value_vector + k);
                    const float32x4_t vw = vld1q_dup_f32(weight_addr);

                    const float32x4_t vm0123 = vmulq_f32(vi0123, vw);

                    vacc0123 = vaddq_f32(vacc0123, vm0123);
                }

                float32x4_t vout0123 = vminq_f32(vacc0123, vmax);
                vout0123 = vmaxq_f32(vout0123, vmin);

                float *output_addr = (float *)(output_matrix + n * M + m);
                vst1q_f32(output_addr, vout0123);
            }
            m += 4;
        }

        for (int n = 0; n < N; n++) {
            for (int m_l = m; m_l < M; m_l++) {
                float acc = bias_vector[n];
                for (uint32_t k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    acc += (input_matrix[weight_IDX_vector[k] * M + m_l] * weight_value_vector[k]);
                }
                acc = acc < max ? acc : max;
                acc = acc > min ? acc : min;
                output_matrix[n * M + m_l] = acc;
            }
        }
    }
}

void spmm_int32_neon(config_t *config,
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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const int32_t *input_addr = (const int32_t *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const int32x4_t vi0123 = vld1q_s32(input_addr);
                    const int32x4_t vi4567 = vld1q_s32(input_addr + 4);
                    const int32x4_t vi89AB = vld1q_s32(input_addr + 8);
                    const int32x4_t viCDEF = vld1q_s32(input_addr + 12);
                    const int32x4_t viGHIJ = vld1q_s32(input_addr + 16);
                    const int32x4_t viKLMN = vld1q_s32(input_addr + 20);
                    const int32x4_t viOPQR = vld1q_s32(input_addr + 24);
                    const int32x4_t viSTUV = vld1q_s32(input_addr + 28);

                    const int32_t *weight_addr = (const int32_t *)(weight_value_vector + k);
                    const int32x4_t vw = vld1q_dup_s32(weight_addr);

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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const int32_t *input_addr = (const int32_t *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const int32x4_t vi0123 = vld1q_s32(input_addr);
                    const int32x4_t vi4567 = vld1q_s32(input_addr + 4);
                    const int32x4_t vi89AB = vld1q_s32(input_addr + 8);
                    const int32x4_t viCDEF = vld1q_s32(input_addr + 12);

                    const int32_t *weight_addr = (const int32_t *)(weight_value_vector + k);
                    const int32x4_t vw = vld1q_dup_s32(weight_addr);

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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const int32_t *input_addr = (const int32_t *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const int32x4_t vi0123 = vld1q_s32(input_addr);
                    const int32x4_t vi4567 = vld1q_s32(input_addr + 4);

                    const int32_t *weight_addr = (const int32_t *)(weight_value_vector + k);
                    const int32x4_t vw = vld1q_dup_s32(weight_addr);

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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const int32_t *input_addr = (const int32_t *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const int32x4_t vi0123 = vld1q_s32(input_addr);

                    const int32_t *weight_addr = (const int32_t *)(weight_value_vector + k);
                    const int32x4_t vw = vld1q_dup_s32(weight_addr);

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
                for (uint32_t k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    acc += (input_matrix[weight_IDX_vector[k] * M + m_l] * weight_value_vector[k]);
                }
                acc = acc < max ? acc : max;
                acc = acc > min ? acc : min;
                output_matrix[n * M + m_l] = acc;
            }
        }
    }
}

void spmm_int16_neon(config_t *config,
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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const int16_t *input_addr = (const int16_t *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const int16x8_t vi0123 = vld1q_s16(input_addr);
                    const int16x8_t vi4567 = vld1q_s16(input_addr + 8);
                    const int16x8_t vi89AB = vld1q_s16(input_addr + 16);
                    const int16x8_t viCDEF = vld1q_s16(input_addr + 24);

                    const int16_t *weight_addr = (const int16_t *)(weight_value_vector + k);
                    const int16x8_t vw = vld1q_dup_s16(weight_addr);

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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const int16_t *input_addr = (const int16_t *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const int16x8_t vi0123 = vld1q_s16(input_addr);
                    const int16x8_t vi4567 = vld1q_s16(input_addr + 8);

                    const int16_t *weight_addr = (const int16_t *)(weight_value_vector + k);
                    const int16x8_t vw = vld1q_dup_s16(weight_addr);

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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const int16_t *input_addr = (const int16_t *)(input_matrix + weight_IDX_vector[k] * M + m);
                    const int16x8_t vi0123 = vld1q_s16(input_addr);

                    const int16_t *weight_addr = (const int16_t *)(weight_value_vector + k);
                    const int16x8_t vw = vld1q_dup_s16(weight_addr);

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
                for (uint32_t k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    acc += (input_matrix[weight_IDX_vector[k] * M + m_l] * weight_value_vector[k]);
                }
                acc = acc < max ? acc : max;
                acc = acc > min ? acc : min;
                output_matrix[n * M + m_l] = acc;
            }
        }
    }
}

#ifndef SWAN_SIMULATION
void spmm_fp16_neon(config_t *config,
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

        const float16x8_t vmax = vreinterpretq_f16_u16(vld1q_dup_u16(&max));
        const float16x8_t vmin = vreinterpretq_f16_u16(vld1q_dup_u16(&min));

        const __fp16 *input_f16 = (const __fp16 *)input_matrix;
        const __fp16 *bias_f16 = (const __fp16 *)bias_vector;
        const __fp16 *weights_f16 = (const __fp16 *)weight_value_vector;
        __fp16 *output_f16 = (__fp16 *)output_matrix;

        int m = 0;

        while (m + 32 <= M) {
            for (int n = 0; n < N; n++) {
                const __fp16 *bias_addr = bias_f16 + n;
                float16x8_t vacc0123 = vld1q_dup_f16(bias_addr);
                float16x8_t vacc4567 = vacc0123;
                float16x8_t vacc89AB = vacc0123;
                float16x8_t vaccCDEF = vacc0123;

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const __fp16 *input_addr = (const __fp16 *)(input_f16 + weight_IDX_vector[k] * M + m);
                    const float16x8_t vi0123 = vld1q_f16(input_addr);
                    const float16x8_t vi4567 = vld1q_f16(input_addr + 8);
                    const float16x8_t vi89AB = vld1q_f16(input_addr + 16);
                    const float16x8_t viCDEF = vld1q_f16(input_addr + 24);

                    const __fp16 *weight_addr = (const __fp16 *)(weights_f16 + k);
                    const float16x8_t vw = vld1q_dup_f16(weight_addr);

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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const __fp16 *input_addr = (const __fp16 *)(input_f16 + weight_IDX_vector[k] * M + m);
                    const float16x8_t vi0123 = vld1q_f16(input_addr);
                    const float16x8_t vi4567 = vld1q_f16(input_addr + 8);

                    const __fp16 *weight_addr = (const __fp16 *)(weights_f16 + k);
                    const float16x8_t vw = vld1q_dup_f16(weight_addr);

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

                uint32_t max_k = weight_NNZ_vector[n + 1];
                for (uint32_t k = weight_NNZ_vector[n]; k < max_k; k++) {

                    const __fp16 *input_addr = (const __fp16 *)(input_f16 + weight_IDX_vector[k] * M + m);
                    const float16x8_t vi0123 = vld1q_f16(input_addr);

                    const __fp16 *weight_addr = (const __fp16 *)(weights_f16 + k);
                    const float16x8_t vw = vld1q_dup_f16(weight_addr);

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
                for (uint32_t k = weight_NNZ_vector[n]; k < weight_NNZ_vector[n + 1]; k++) {
                    acc += (input_matrix[weight_IDX_vector[k] * M + m_l] * weight_value_vector[k]);
                }
                acc = acc < max ? acc : max;
                acc = acc > min ? acc : min;
                output_matrix[n * M + m_l] = acc;
            }
        }
    }
}
#endif