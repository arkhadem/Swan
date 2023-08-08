// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "scalar.hpp"
#include "is_audible.hpp"
#include "scalar.hpp"

static inline void Vsvesq(const float *source_p,
                          int source_stride,
                          float *sum_p,
                          uint32_t frames_to_process) {
    while (frames_to_process > 0u) {
        const float sample = *source_p;
        *sum_p += sample * sample;
        source_p += source_stride;
        --frames_to_process;
    }
}

/* The following function is the modified version of IsAudible,
 * provided in the webaudio library. Please refer to audio_context.cc
 * for the unmodified version in the source library. */
void is_audible_scalar(config_t *config,
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
        }

        is_audible_output->return_value[block] = energy > 0;
    }
}