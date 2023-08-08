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

#include "neon.hpp"
#include <arm_neon.h>
#include <stdint.h>

#include "libwebp.hpp"
#include "sharp_update_rgb.hpp"

/* The following function is the modified version of SharpYuvUpdateRGB_NEON,
 * provided in the libwebp library. Please refer to sharpyuv_neon.c
 * for the unmodified version in the source library. */
void sharp_update_rgb_neon(config_t *config,
                           input_t *input,
                           output_t *output) {
    sharp_update_rgb_config_t *sharp_update_rgb_config = (sharp_update_rgb_config_t *)config;
    sharp_update_rgb_input_t *sharp_update_rgb_input = (sharp_update_rgb_input_t *)input;
    sharp_update_rgb_output_t *sharp_update_rgb_output = (sharp_update_rgb_output_t *)output;

    for (int row = 0; row < sharp_update_rgb_config->num_rows; row++) {
        int16_t *src = sharp_update_rgb_input->src[row];
        int16_t *ref = sharp_update_rgb_input->ref[row];
        int16_t *dst = sharp_update_rgb_output->dst[row];

        for (int i = 0; i < sharp_update_rgb_config->num_cols; i += 8, src += 8, ref += 8, dst += 8) {
            const int16x8_t A = vld1q_s16(ref);
            const int16x8_t B = vld1q_s16(src);
            const int16x8_t C = vld1q_s16(dst);
            const int16x8_t D = vsubq_s16(A, B); // diff_uv
            const int16x8_t E = vaddq_s16(C, D); // new_uv
            vst1q_s16(dst, E);
        }
    }
}
