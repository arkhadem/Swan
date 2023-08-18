// Copyright 2017 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// NEON variant of alpha filters
//
// Author: Skal (pascal.massimino@gmail.com)

#include "neon.hpp"

#include <stdint.h>

#include "gradient_filter.hpp"
#include "libwebp.hpp"
#include "swan.hpp"

#define U8_TO_S16(A) vreinterpretq_s16_u16(vmovl_u8(A))
#define LOAD_U8_TO_S16(A) U8_TO_S16(vld1_u8(A))

static void GradientPredictLine_NEON(const uint8_t *src, const uint8_t *pred,
                                     uint8_t *dst, int length) {
    int i;
    SWAN_ASSERT(length >= 0);
    for (i = 0; i + 16 <= length; i += 16) {
        const uint8x16_t A = vld1q_u8(&src[i]);
        const uint8x16_t B = vld1q_u8(&pred[i]);
        const uint8x16_t C = vsubq_u8(A, B);
        vst1q_u8(&dst[i], C);
    }
    for (; i < length; ++i)
        dst[i] = src[i] - pred[i];
}

// Special case for left-based prediction (when preds==dst-1 or preds==src-1).
static void GradientPredictLineLeft_NEON(const uint8_t *src, uint8_t *dst, int length) {
    GradientPredictLine_NEON(src, src - 1, dst, length);
}

static inline int GradientPredictor_C(uint8_t a, uint8_t b, uint8_t c) {
    const int g = a + b - c;
    return ((g & ~0xff) == 0) ? g : (g < 0) ? 0
                                            : 255; // clip to 8bit
}

static void GradientPredictDirect_NEON(const uint8_t *const row,
                                       const uint8_t *const top,
                                       uint8_t *const out, int length) {
    int i;
    for (i = 0; i + 8 <= length; i += 8) {
        const uint8x8_t A = vld1_u8(&row[i - 1]);
        const uint8x8_t B = vld1_u8(&top[i + 0]);
        const int16x8_t C = vreinterpretq_s16_u16(vaddl_u8(A, B));
        const int16x8_t D = LOAD_U8_TO_S16(&top[i - 1]);
        const uint8x8_t E = vqmovun_s16(vsubq_s16(C, D));
        const uint8x8_t F = vld1_u8(&row[i + 0]);
        vst1_u8(&out[i], vsub_u8(F, E));
    }
    for (; i < length; ++i) {
        out[i] = row[i] - GradientPredictor_C(row[i - 1], top[i], top[i - 1]);
    }
}

static inline void DoGradientFilter_NEON(const uint8_t *in,
                                         int width, int height,
                                         int stride,
                                         int row, int num_rows,
                                         uint8_t *out) {
    const size_t start_offset = row * stride;
    const int last_row = row + num_rows;
    in += start_offset;
    out += start_offset;

    // left prediction for top scan-line
    if (row == 0) {
        out[0] = in[0];
        GradientPredictLineLeft_NEON(in + 1, out + 1, width - 1);
        row = 1;
        in += stride;
        out += stride;
    }

    // Filter line-by-line.
    while (row < last_row) {
        out[0] = in[0] - in[-stride];
        GradientPredictDirect_NEON(in + 1, in + 1 - stride, out + 1, width - 1);
        ++row;
        in += stride;
        out += stride;
    }
}

/* The following function is the modified version of GradientFilter_NEON,
 * provided in the libwebp library. Please refer to filters_neon.c
 * for the unmodified version in the source library. */
void gradient_filter_neon(config_t *config,
                          input_t *input,
                          output_t *output) {
    gradient_filter_config_t *gradient_filter_config = (gradient_filter_config_t *)config;
    gradient_filter_input_t *gradient_filter_input = (gradient_filter_input_t *)input;
    gradient_filter_output_t *gradient_filter_output = (gradient_filter_output_t *)output;

    int num_rows = gradient_filter_config->num_rows;
    int num_cols = gradient_filter_config->num_cols;
    int stride = gradient_filter_config->stride;
    uint8_t *in = gradient_filter_input->in;
    uint8_t *out = gradient_filter_output->out;

    DoGradientFilter_NEON(in, num_cols, num_rows, stride, 0, num_rows, out);
}
