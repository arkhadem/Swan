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

#include <stdint.h>

#include "libwebp.hpp"
#include "sharp_filter_row.hpp"

/* The following function is the modified version of SharpYuvFilterRow16_NEON,
 * provided in the libwebp library. Please refer to sharpyuv_neon.c
 * for the unmodified version in the source library. */
void sharp_filter_row_neon(config_t *config,
                           input_t *input,
                           output_t *output) {
    sharp_filter_row_config_t *sharp_filter_row_config = (sharp_filter_row_config_t *)config;
    sharp_filter_row_input_t *sharp_filter_row_input = (sharp_filter_row_input_t *)input;
    sharp_filter_row_output_t *sharp_filter_row_output = (sharp_filter_row_output_t *)output;

    const int max_y = (1 << sharp_filter_row_config->bit_depth) - 1;
    const int16x8_t max = vdupq_n_s16(max_y);
    const int16x8_t zero = vdupq_n_s16(0);

    for (int row = 0; row < sharp_filter_row_config->num_rows; row++) {
        int16_t *A = sharp_filter_row_input->A[row];
        int16_t *B = sharp_filter_row_input->B[row];
        uint16_t *best_y = sharp_filter_row_input->best_y[row];
        uint16_t *out = sharp_filter_row_output->out[row];

        for (int i = 0; i + 8 <= sharp_filter_row_config->num_cols; i += 8) {
            const int16x8_t a0 = vld1q_s16(A + i + 0);
            const int16x8_t a1 = vld1q_s16(A + i + 1);
            const int16x8_t b0 = vld1q_s16(B + i + 0);
            const int16x8_t b1 = vld1q_s16(B + i + 1);
            const int16x8_t a0b1 = vaddq_s16(a0, b1);
            const int16x8_t a1b0 = vaddq_s16(a1, b0);
            const int16x8_t a0a1b0b1 = vaddq_s16(a0b1, a1b0); // A0+A1+B0+B1
            const int16x8_t a0b1_2 = vaddq_s16(a0b1, a0b1);   // 2*(A0+B1)
            const int16x8_t a1b0_2 = vaddq_s16(a1b0, a1b0);   // 2*(A1+B0)
            const int16x8_t c0 = vshrq_n_s16(vaddq_s16(a0b1_2, a0a1b0b1), 3);
            const int16x8_t c1 = vshrq_n_s16(vaddq_s16(a1b0_2, a0a1b0b1), 3);
            const int16x8_t e0 = vrhaddq_s16(c1, a0);
            const int16x8_t e1 = vrhaddq_s16(c0, a1);
            const int16x8x2_t f = vzipq_s16(e0, e1);
            const int16x8_t g0 = vreinterpretq_s16_u16(vld1q_u16(best_y + 2 * i + 0));
            const int16x8_t g1 = vreinterpretq_s16_u16(vld1q_u16(best_y + 2 * i + 8));
#if defined(NEON2RVV)
            #define GET(v, idx) __riscv_vget_v_i16m1x2_i16m1(v, idx)
            const int16x8_t h0 = vaddq_s16(g0, GET(f, 0));
            const int16x8_t h1 = vaddq_s16(g1, GET(f, 1));
            #undef GET
#else
            const int16x8_t h0 = vaddq_s16(g0, f.val[0]);
            const int16x8_t h1 = vaddq_s16(g1, f.val[1]);
#endif
            const int16x8_t i0 = vmaxq_s16(vminq_s16(h0, max), zero);
            const int16x8_t i1 = vmaxq_s16(vminq_s16(h1, max), zero);
            vst1q_u16(out + 2 * i + 0, vreinterpretq_u16_s16(i0));
            vst1q_u16(out + 2 * i + 8, vreinterpretq_u16_s16(i1));
        }
    }
}
