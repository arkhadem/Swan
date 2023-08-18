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

#include "dc_prediction.hpp"
#include "libwebp.hpp"

/* The following function is the modified version of DC16_NEON,
 * provided in the libwebp library. Please refer to dec_neon.c
 * for the unmodified version in the source library. */
void dc_prediction_neon(config_t *config,
                        input_t *input,
                        output_t *output) {
    dc_prediction_config_t *dc_prediction_config = (dc_prediction_config_t *)config;
    dc_prediction_output_t *dc_prediction_output = (dc_prediction_output_t *)output;
    dc_prediction_input_t *dc_prediction_input = (dc_prediction_input_t *)input;

    int BPS = dc_prediction_config->num_cols;

    for (int block = 0; block < dc_prediction_config->block_count; block++) {
        uint8_t *src = dc_prediction_input->block_src[block];
        uint8_t *dst = dc_prediction_output->block_dst[block];

        uint16x8_t sum_top;
        uint16x8_t sum_left;
        uint8x8_t dc0;

        const uint8x16_t A = vld1q_u8(src - BPS); // top row
        const uint16_t p3 = vaddlvq_u8(A);
        sum_top = vdupq_n_u16(p3);

        sum_left = vdupq_n_u16(0);
        for (int i = 0; i < 16; i += 8) {
            const uint8x8_t L0 = vld1_u8(src + (i + 0) * BPS - 1);
            const uint8x8_t L1 = vld1_u8(src + (i + 1) * BPS - 1);
            const uint8x8_t L2 = vld1_u8(src + (i + 2) * BPS - 1);
            const uint8x8_t L3 = vld1_u8(src + (i + 3) * BPS - 1);
            const uint8x8_t L4 = vld1_u8(src + (i + 4) * BPS - 1);
            const uint8x8_t L5 = vld1_u8(src + (i + 5) * BPS - 1);
            const uint8x8_t L6 = vld1_u8(src + (i + 6) * BPS - 1);
            const uint8x8_t L7 = vld1_u8(src + (i + 7) * BPS - 1);
            const uint16x8_t s0 = vaddl_u8(L0, L1);
            const uint16x8_t s1 = vaddl_u8(L2, L3);
            const uint16x8_t s2 = vaddl_u8(L4, L5);
            const uint16x8_t s3 = vaddl_u8(L6, L7);
            const uint16x8_t s01 = vaddq_u16(s0, s1);
            const uint16x8_t s23 = vaddq_u16(s2, s3);
            const uint16x8_t sum = vaddq_u16(s01, s23);
            sum_left = vaddq_u16(sum_left, sum);
        }

        const uint16x8_t sum = vaddq_u16(sum_left, sum_top);
        dc0 = vrshrn_n_u16(sum, 5);

        const uint8x16_t dc = vdupq_lane_u8(dc0, 0);
        for (int i = 0; i < 16; ++i) {
            vst1q_u8(dst + i * BPS, dc);
        }
    }
}