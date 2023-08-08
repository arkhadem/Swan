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
#include "dc_prediction.hpp"

static inline void Put16(int v, uint8_t *dst, int BPS) {
    for (int j = 0; j < 16; ++j) {
        uint8_t *my_dst = dst + j * BPS;
        for (int i = 0; i < 16; i++) {
            *my_dst++ = v;
        }
    }
}

/* The following function is the modified version of DC16_C,
 * provided in the libwebp library. Please refer to dec.c
 * for the unmodified version in the source library. */
void dc_prediction_scalar(config_t *config,
                          input_t *input,
                          output_t *output) {
    dc_prediction_config_t *dc_prediction_config = (dc_prediction_config_t *)config;
    dc_prediction_output_t *dc_prediction_output = (dc_prediction_output_t *)output;
    dc_prediction_input_t *dc_prediction_input = (dc_prediction_input_t *)input;

    int BPS = dc_prediction_config->num_cols;

    for (int block = 0; block < dc_prediction_config->block_count; block++) {
        uint8_t *src = dc_prediction_input->block_src[block];
        uint8_t *dst = dc_prediction_output->block_dst[block];

        int DC = 16;
        for (int j = 0; j < 16; ++j) {
            DC += src[-1 + j * BPS] + src[j - BPS];
        }
        Put16(DC >> 5, dst, BPS);
    }
}