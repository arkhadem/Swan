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
#include "ve_prediction.hpp"

/* The following function is the modified version of VE16_C,
 * provided in the libwebp library. Please refer to dec.c
 * for the unmodified version in the source library. */
void ve_prediction_scalar(config_t *config,
                          input_t *input,
                          output_t *output) {
    ve_prediction_config_t *ve_prediction_config = (ve_prediction_config_t *)config;
    ve_prediction_output_t *ve_prediction_output = (ve_prediction_output_t *)output;
    ve_prediction_input_t *ve_prediction_input = (ve_prediction_input_t *)input;

    int BPS = ve_prediction_config->num_cols;

    for (int block = 0; block < ve_prediction_config->block_count; block++) {
        uint8_t *src = ve_prediction_input->block_src[block];
        uint8_t *dst = ve_prediction_output->block_dst[block];

        for (int j = 0; j < 16; ++j) {
            uint8_t *my_src = src - BPS;
            uint8_t *my_dst = dst + j * BPS;
            for (int i = 0; i < 16; i++) {
                *my_dst++ = *my_src++;
            }
        }
    }
}