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
#include "he_prediction.hpp"

/* The following function is the modified version of HE16_C,
 * provided in the libwebp library. Please refer to dec.c
 * for the unmodified version in the source library. */
void he_prediction_scalar(config_t *config,
                          input_t *input,
                          output_t *output) {
    he_prediction_config_t *he_prediction_config = (he_prediction_config_t *)config;
    he_prediction_output_t *he_prediction_output = (he_prediction_output_t *)output;
    he_prediction_input_t *he_prediction_input = (he_prediction_input_t *)input;

    int BPS = he_prediction_config->num_cols;

    for (int block = 0; block < he_prediction_config->block_count; block++) {
        const uint8_t *src = he_prediction_input->block_src[block];
        uint8_t *dst = he_prediction_output->block_dst[block];

        for (int j = 16; j > 0; --j) {
            uint8_t *my_dst = dst;
            for (int i = 0; i < 16; i++) {
                *my_dst++ = src[-1];
            }
            src += BPS;
            dst += BPS;
        }
    }
}