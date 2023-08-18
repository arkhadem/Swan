// Copyright 2012 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// ARM NEON version of dsp functions and loop filtering.
//
// Authors: Somnath Banerjee (somnath@google.com)
//          Johann Koenig (johannkoenig@google.com)

#include "neon.hpp"

#include <stdint.h>

#include "he_prediction.hpp"
#include "libwebp.hpp"

/* The following function is the modified version of HE16_NEON,
 * provided in the libwebp library. Please refer to dec_neon.c
 * for the unmodified version in the source library. */
void he_prediction_neon(config_t *config,
                        input_t *input,
                        output_t *output) {

    he_prediction_config_t *he_prediction_config = (he_prediction_config_t *)config;
    he_prediction_output_t *he_prediction_output = (he_prediction_output_t *)output;
    he_prediction_input_t *he_prediction_input = (he_prediction_input_t *)input;

    int BPS = he_prediction_config->num_cols;

    for (int block = 0; block < he_prediction_config->block_count; block++) {
        const uint8_t *src = he_prediction_input->block_src[block];
        uint8_t *dst = he_prediction_output->block_dst[block];

        int j;
        for (j = 0; j < 16; ++j) {
            const uint8x16_t left = vld1q_dup_u8(src - 1);
            vst1q_u8(dst, left);
            src += BPS;
            dst += BPS;
        }
    }
}