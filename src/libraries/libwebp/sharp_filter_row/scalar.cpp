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
#include "sharp_filter_row.hpp"

static uint16_t clip(int v, int max) {
    return (v < 0) ? 0 : (v > max) ? max
                                   : (uint16_t)v;
}

/* The following function is the modified version of SharpYuvFilterRow_C,
 * provided in the libwebp library. Please refer to sharpyuv_dsp.c
 * for the unmodified version in the source library. */
void sharp_filter_row_scalar(config_t *config,
                             input_t *input,
                             output_t *output) {
    sharp_filter_row_config_t *sharp_filter_row_config = (sharp_filter_row_config_t *)config;
    sharp_filter_row_input_t *sharp_filter_row_input = (sharp_filter_row_input_t *)input;
    sharp_filter_row_output_t *sharp_filter_row_output = (sharp_filter_row_output_t *)output;

    const int max_y = (1 << sharp_filter_row_config->bit_depth) - 1;

    for (int row = 0; row < sharp_filter_row_config->num_rows; row++) {
        int16_t *A = sharp_filter_row_input->A[row];
        int16_t *B = sharp_filter_row_input->B[row];
        uint16_t *best_y = sharp_filter_row_input->best_y[row];
        uint16_t *out = sharp_filter_row_output->out[row];

        for (int i = 0; i < sharp_filter_row_config->num_cols; ++i, ++A, ++B) {
            const int v0 = (A[0] * 9 + A[1] * 3 + B[0] * 3 + B[1] + 8) >> 4;
            const int v1 = (A[1] * 9 + A[0] * 3 + B[1] * 3 + B[0] + 8) >> 4;
            out[2 * i + 0] = clip(best_y[2 * i + 0] + v0, max_y);
            out[2 * i + 1] = clip(best_y[2 * i + 1] + v1, max_y);
        }
    }
}