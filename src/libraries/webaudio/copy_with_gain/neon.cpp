/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "neon.hpp"
#include "copy_with_gain.hpp"
#include "neon.hpp"
#include <arm_neon.h>

static inline void Vsmul(const float *source_p,
                         int source_stride,
                         const float *scale,
                         float *dest_p,
                         int dest_stride,
                         uint32_t frames_to_process) {
    int n = frames_to_process;

    if (source_stride == 1 && dest_stride == 1) {
        float k = *scale;
        int tail_frames = n % 4;
        const float *end_p = dest_p + n - tail_frames;

        while (dest_p < end_p) {
            float32x4_t source = vld1q_f32(source_p);
            vst1q_f32(dest_p, vmulq_n_f32(source, k));

            source_p += 4;
            dest_p += 4;
        }
        n = tail_frames;
    }

    const float k = *scale;
    while (n > 0u) {
        *dest_p = k * *source_p;
        source_p += source_stride;
        dest_p += dest_stride;
        --n;
    }
}

/* The following function is the modified version of CopyWithGainFrom,
 * provided in the webaudio library. Please refer to audio_bus.cc
 * for the unmodified version in the source library. */
void copy_with_gain_neon(config_t *config,
                         input_t *input,
                         output_t *output) {
    copy_with_gain_config_t *copy_with_gain_config = (copy_with_gain_config_t *)config;
    copy_with_gain_input_t *copy_with_gain_input = (copy_with_gain_input_t *)input;
    copy_with_gain_output_t *copy_with_gain_output = (copy_with_gain_output_t *)output;

    uint32_t data_size = copy_with_gain_config->data_size;
    uint32_t block_count = copy_with_gain_config->block_count;
    uint32_t number_of_channels = copy_with_gain_config->number_of_channels;
    float gain = copy_with_gain_config->gain;

    for (int block = 0; block < block_count; block++) {
        float **sources = copy_with_gain_input->sources[block];
        float **destinations = copy_with_gain_output->destinations[block];

        for (unsigned channel_index = 0; channel_index < number_of_channels; ++channel_index) {
            Vsmul(sources[channel_index], 1, &gain, destinations[channel_index], 1, data_size);
        }
    }
}