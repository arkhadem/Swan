// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "neon.hpp"
#include "is_audible.hpp"
#include "neon.hpp"
#include <stdio.h>

#ifndef SWAN_SIMULATION
static inline void Vsvesq(const float *source_p,
                          int source_stride,
                          float *sum_p,
                          uint32_t frames_to_process) {
    int n = frames_to_process;

    if (source_stride == 1) {
        int tail_frames = n % 4;
        const float *end_p = source_p + n - tail_frames;

        float32x4_t four_sum = vdupq_n_f32(0);
        while (source_p < end_p) {
            float32x4_t source = vld1q_f32(source_p);
            four_sum = vmlaq_f32(four_sum, source, source);
            source_p += 4;
        }
        float32x2_t two_sum = vadd_f32(vget_low_f32(four_sum), vget_high_f32(four_sum));

        float group_sum[2];
        vst1_f32(group_sum, two_sum);
        *sum_p += group_sum[0] + group_sum[1];

        n = tail_frames;
    }

    while (n > 0u) {
        const float sample = *source_p;
        *sum_p += sample * sample;
        source_p += source_stride;
        --n;
    }
}

#else
static inline void Vsvesq(const float *source_p,
                          int source_stride,
                          float *sum_p,
                          uint32_t frames_to_process) {
    int n = frames_to_process;

    if (source_stride == 1) {
        int tail_frames = n % 4;
        const float *end_p = source_p + n;

#if SWAN_REG_TYPE == SWAN_REG_1024_TYPE
        float32x32_t thirtytwo_sum = vdupoq_n_f32(0);
        while (source_p + 32 <= end_p) {
            float32x32_t source = vld1oq_f32(source_p);
            thirtytwo_sum = vmlaoq_f32(thirtytwo_sum, source, source);
            source_p += 32;
        }
#endif
#if SWAN_REG_TYPE > SWAN_REG_512_TYPE
        float32x16_t sixteen_sum = vaddqq_f32(vgetoq_low_f32(thirtytwo_sum), vgetoq_high_f32(thirtytwo_sum));
#elif SWAN_REG_TYPE == SWAN_REG_512_TYPE
        float32x16_t sixteen_sum = vdupqq_n_f32(0);
#endif
#if SWAN_REG_TYPE >= SWAN_REG_512_TYPE
        while (source_p + 16 <= end_p) {
            float32x16_t source = vld1qq_f32(source_p);
            sixteen_sum = vmlaqq_f32(sixteen_sum, source, source);
            source_p += 16;
        }
#endif
#if SWAN_REG_TYPE > SWAN_REG_256_TYPE
        float32x8_t eight_sum = vadddq_f32(vgetqq_low_f32(sixteen_sum), vgetqq_high_f32(sixteen_sum));
#elif SWAN_REG_TYPE == SWAN_REG_256_TYPE
        float32x8_t eight_sum = vdupdq_n_f32(0);
#endif
#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
        while (source_p + 8 <= end_p) {
            float32x8_t source = vld1dq_f32(source_p);
            eight_sum = vmladq_f32(eight_sum, source, source);
            source_p += 8;
        }
#endif
#if SWAN_REG_TYPE > SWAN_REG_128_TYPE
        float32x4_t four_sum = vaddq_f32(vgetdq_low_f32(eight_sum), vgetdq_high_f32(eight_sum));
#elif SWAN_REG_TYPE == SWAN_REG_128_TYPE
        float32x4_t four_sum = vdupq_n_f32(0);
#endif
        while (source_p + 4 <= end_p) {
            float32x4_t source = vld1q_f32(source_p);
            four_sum = vmlaq_f32(four_sum, source, source);
            source_p += 4;
        }
        float32x2_t two_sum = vadd_f32(vget_low_f32(four_sum), vget_high_f32(four_sum));
        float group_sum[2];
        vst1_f32(group_sum, two_sum);
        *sum_p += group_sum[0] + group_sum[1];

        n = tail_frames;
    }

    while (n > 0u) {
        const float sample = *source_p;
        *sum_p += sample * sample;
        source_p += source_stride;
        --n;
    }
}
#endif

/* The following function is the modified version of IsAudible,
 * provided in the webaudio library. Please refer to audio_context.cc
 * for the unmodified version in the source library. */
void is_audible_neon(config_t *config,
                     input_t *input,
                     output_t *output) {
    is_audible_config_t *is_audible_config = (is_audible_config_t *)config;
    is_audible_input_t *is_audible_input = (is_audible_input_t *)input;
    is_audible_output_t *is_audible_output = (is_audible_output_t *)output;
    // Compute the energy in each channel and sum up the energy in each channel
    // for the total energy.
    uint32_t data_size = is_audible_config->data_size;
    uint32_t block_count = is_audible_config->block_count;
    uint32_t number_of_channels = is_audible_config->number_of_channels;

    for (int block = 0; block < block_count; block++) {
        float **data = is_audible_input->data[block];

        float energy = 0;
        for (uint32_t k = 0; k < number_of_channels; ++k) {
            const float *my_data = data[k];
            float channel_energy = 0;
            Vsvesq(my_data, 1, &channel_energy, data_size);
            energy += channel_energy;
#ifdef SWAN_SIMULATION
            fake_neon_flusher();
#endif
        }

        is_audible_output->return_value[block] = energy > 0;
    }
}