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

#include "scalar.hpp"
#include "copy_with_sample.hpp"
#include "scalar.hpp"

static inline void Vmul(const float *source1p,
                        int source_stride1,
                        const float *source2p,
                        int source_stride2,
                        float *dest_p,
                        int dest_stride,
                        uint32_t frames_to_process) {
    while (frames_to_process > 0u) {
        *dest_p = *source1p * *source2p;
        source1p += source_stride1;
        source2p += source_stride2;
        dest_p += dest_stride;
        --frames_to_process;
    }
}

/* The following function is the modified version of CopyWithSampleAccurateGainValuesFrom,
 * provided in the webaudio library. Please refer to audio_bus.cc
 * for the unmodified version in the source library. */
void copy_with_sample_scalar(config_t *config,
                             input_t *input,
                             output_t *output) {
    copy_with_sample_config_t *copy_with_sample_config = (copy_with_sample_config_t *)config;
    copy_with_sample_input_t *copy_with_sample_input = (copy_with_sample_input_t *)input;
    copy_with_sample_output_t *copy_with_sample_output = (copy_with_sample_output_t *)output;

    uint32_t data_size = copy_with_sample_config->data_size;
    uint32_t block_count = copy_with_sample_config->block_count;
    uint32_t number_of_channels = copy_with_sample_config->number_of_channels;
    float *gain = copy_with_sample_input->gain;

    for (int block = 0; block < block_count; block++) {
        float **sources = copy_with_sample_input->sources[block];
        float **destinations = copy_with_sample_output->destinations[block];

        for (unsigned channel_index = 0; channel_index < number_of_channels; ++channel_index) {
            Vmul(sources[channel_index], 1, gain, 1, destinations[channel_index], 1, data_size);
        }
    }
}