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
#include "sum_from.hpp"

static inline void Vadd(const float *source1p,
                        int source_stride1,
                        const float *source2p,
                        int source_stride2,
                        float *dest_p,
                        int dest_stride,
                        uint32_t frames_to_process) {
    while (frames_to_process > 0u) {
        *dest_p = *source1p + *source2p;
        source1p += source_stride1;
        source2p += source_stride2;
        dest_p += dest_stride;
        --frames_to_process;
    }
}

/* The following function is the modified version of SumFrom,
 * provided in the webaudio library. Please refer to audio_channel.cc
 * for the unmodified version in the source library. */
void sum_from_scalar(config_t *config,
                     input_t *input,
                     output_t *output) {
    sum_from_config_t *sum_from_config = (sum_from_config_t *)config;
    sum_from_input_t *sum_from_input = (sum_from_input_t *)input;
    sum_from_output_t *sum_from_output = (sum_from_output_t *)output;

    uint32_t data_size = sum_from_config->data_size;
    uint32_t block_count = sum_from_config->block_count;
    float **source1 = sum_from_input->source1;
    float **source2 = sum_from_input->source2;
    float **destination = sum_from_output->destination;

    for (unsigned block = 0; block < block_count; ++block) {
        Vadd(source1[block], 1, source2[block], 1, destination[block], 1, data_size);
    }
}