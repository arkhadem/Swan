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
#include <arm_neon.h>
#include <stdint.h>
#include <stdio.h>

#include "libwebp.hpp"
#include "swan.hpp"
#include "vertical_filter.hpp"

static void VerticalPredictLine_NEON(const uint8_t *src, const uint8_t *pred,
                                     uint8_t *dst, int length) {
    int i;
    SWAN_ASSERT(length >= 0)
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
static void VerticalPredictLineLeft_NEON(const uint8_t *src, uint8_t *dst, int length) {
    VerticalPredictLine_NEON(src, src - 1, dst, length);
}

static inline void DoVerticalFilter_NEON(const uint8_t *in,
                                         int width, int height, int stride,
                                         int row, int num_rows,
                                         uint8_t *out) {
    const size_t start_offset = row * stride;
    const int last_row = row + num_rows;
    in += start_offset;
    out += start_offset;

    if (row == 0) {
        // Very first top-left pixel is copied.
        out[0] = in[0];
        // Rest of top scan-line is left-predicted.
        VerticalPredictLineLeft_NEON(in + 1, out + 1, width - 1);
        row = 1;
        in += stride;
        out += stride;
    }

    // Filter line-by-line.
    while (row < last_row) {
        VerticalPredictLine_NEON(in, in - stride, out, width);
        ++row;
        in += stride;
        out += stride;
    }
}

/* The following function is the modified version of VerticalFilter_NEON,
 * provided in the libwebp library. Please refer to filters_neon.c
 * for the unmodified version in the source library. */
void vertical_filter_neon(config_t *config,
                          input_t *input,
                          output_t *output) {
    vertical_filter_config_t *vertical_filter_config = (vertical_filter_config_t *)config;
    vertical_filter_input_t *vertical_filter_input = (vertical_filter_input_t *)input;
    vertical_filter_output_t *vertical_filter_output = (vertical_filter_output_t *)output;

    int num_rows = vertical_filter_config->num_rows;
    int num_cols = vertical_filter_config->num_cols;
    int stride = vertical_filter_config->stride;
    uint8_t *out = vertical_filter_output->out;
    uint8_t *in = vertical_filter_input->in;
    DoVerticalFilter_NEON(in, num_cols, num_rows, stride, 0, num_rows, out);
}
