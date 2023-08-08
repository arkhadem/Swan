// Copyright 2010 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// Speed-critical decoding functions, default plain-C implementations.
//
// Author: Skal (pascal.massimino@gmail.com)

#include "scalar.hpp"
#include "tm_prediction.hpp"

/* The following function is the modified version of TM16_C,
 * provided in the libwebp library. Please refer to dec.c
 * for the unmodified version in the source library. */
void tm_prediction_scalar(config_t *config,
                          input_t *input,
                          output_t *output) {
    tm_prediction_config_t *tm_prediction_config = (tm_prediction_config_t *)config;
    tm_prediction_output_t *tm_prediction_output = (tm_prediction_output_t *)output;
    tm_prediction_input_t *tm_prediction_input = (tm_prediction_input_t *)input;

    int BPS = tm_prediction_config->num_cols;

    for (int block = 0; block < tm_prediction_config->block_count; block++) {
        uint8_t *src = tm_prediction_input->block_src[block];
        uint8_t *dst = tm_prediction_output->block_dst[block];

        uint8_t *VP8kclip1 = &clip1[255];

        uint8_t *top = src - BPS;
        uint8_t *clip0 = VP8kclip1 - top[-1];
        for (int y = 0; y < 16; ++y) {
            uint8_t *clip = clip0 + src[-1];
            for (int x = 0; x < 16; ++x) {
                dst[x] = clip[top[x]];
            }
            src += BPS;
            dst += BPS;
        }
    }
}