// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "neon.hpp"
#include "handle_nan.hpp"

#include <math.h>

/* The following function is the modified version of HandleNaNValues,
 * provided in the webaudio library. Please refer to audio_param_handler.cc
 * for the unmodified version in the source library. */
void handle_nan_neon(config_t *config,
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

        int k = 0;

        uint32x4_t defaults = vreinterpretq_u32_f32(vdupq_n_f32(default_value));
        for (k = 0; k < number_of_values; k += 4) {
            float32x4_t v = vld1q_f32(input_values + k);
            // Returns true (all ones) if v is not NaN
            uint32x4_t is_not_nan = vceqq_f32(v, v);
            // Get the parts that are not NaN
            uint32x4_t result = vandq_u32(is_not_nan, vreinterpretq_u32_f32(v));
            // Replace the parts that are NaN with the default and merge with previous
            // result.  (Note: vbic_u32(x, y) = x and not y)
            result = vorrq_u32(result, vbicq_u32(defaults, is_not_nan));
            vst1q_f32(output_values + k, vreinterpretq_f32_u32(result));
        }

        for (; k < number_of_values; ++k) {
            if (isnan(input_values[k])) {
                output_values[k] = default_value;
            } else {
                output_values[k] = input_values[k];
            }
        }
    }
}