// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "scalar.hpp"
#include "handle_nan.hpp"
#include <math.h>

/* The following function is the modified version of HandleNaNValues,
 * provided in the webaudio library. Please refer to audio_param_handler.cc
 * for the unmodified version in the source library. */
void handle_nan_scalar(config_t *config,
                       input_t *input,
                       output_t *output) {
    handle_nan_config_t *handle_nan_config = (handle_nan_config_t *)config;
    handle_nan_input_t *handle_nan_input = (handle_nan_input_t *)input;
    handle_nan_output_t *handle_nan_output = (handle_nan_output_t *)output;

    unsigned number_of_values = handle_nan_config->number_of_values;
    float default_value = handle_nan_config->default_value;
    uint32_t block_count = handle_nan_config->block_count;

    for (unsigned block = 0; block < block_count; ++block) {
        float *input_values = handle_nan_input->input_values[block];
        float *output_values = handle_nan_output->output_values[block];

        for (int k = 0; k < number_of_values; ++k) {
            if (isnan(input_values[k])) {
                output_values[k] = default_value;
            } else {
                output_values[k] = input_values[k];
            }
        }
    }
}