// Copyright 2022 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// Speed-critical functions for Sharp YUV.
//
// Author: Skal (pascal.massimino@gmail.com)

#include "scalar.hpp"
#include "sharp_update_rgb.hpp"

/* The following function is the modified version of SharpYuvUpdateRGB_C,
 * provided in the libwebp library. Please refer to sharpyuv_dsp.c
 * for the unmodified version in the source library. */
void sharp_update_rgb_scalar(config_t *config,
                             input_t *input,
                             output_t *output) {
    sharp_update_rgb_config_t *sharp_update_rgb_config = (sharp_update_rgb_config_t *)config;
    sharp_update_rgb_input_t *sharp_update_rgb_input = (sharp_update_rgb_input_t *)input;
    sharp_update_rgb_output_t *sharp_update_rgb_output = (sharp_update_rgb_output_t *)output;

    for (int row = 0; row < sharp_update_rgb_config->num_rows; row++) {
        int16_t *src = sharp_update_rgb_input->src[row];
        int16_t *ref = sharp_update_rgb_input->ref[row];
        int16_t *dst_i = sharp_update_rgb_input->dst[row];
        int16_t *dst_o = sharp_update_rgb_output->dst[row];

        for (int i = 0; i < sharp_update_rgb_config->num_cols; ++i, src++, ref++, dst_i++, dst_o++) {
            int diff_uv = *ref - *src;
            *dst_o = *dst_i + diff_uv;
        }
    }
}