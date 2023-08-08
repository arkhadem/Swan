// Copyright 2011 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// Spatial prediction using various filters
//
// Author: Urvang (urvang@google.com)

#include "scalar.hpp"
#include "vertical_filter.hpp"

static inline void VerticalPredictLine_C(const uint8_t *src, const uint8_t *pred,
                                         uint8_t *dst, int length, int inverse) {
    int i;
    if (inverse) {
        for (i = 0; i < length; ++i)
            dst[i] = (uint8_t)(src[i] + pred[i]);
    } else {
        for (i = 0; i < length; ++i)
            dst[i] = (uint8_t)(src[i] - pred[i]);
    }
}

static inline void DoVerticalFilter_C(const uint8_t *in,
                                      int width, int height, int stride,
                                      int row, int num_rows,
                                      int inverse, uint8_t *out) {
    const uint8_t *preds;
    const size_t start_offset = row * stride;
    const int last_row = row + num_rows;
    in += start_offset;
    out += start_offset;
    preds = inverse ? out : in;

    if (row == 0) {
        // Very first top-left pixel is copied.
        out[0] = in[0];
        // Rest of top scan-line is left-predicted.
        VerticalPredictLine_C(in + 1, preds, out + 1, width - 1, inverse);
        row = 1;
        in += stride;
        out += stride;
    } else {
        // We are starting from in-between. Make sure 'preds' points to prev row.
        preds -= stride;
    }

    // Filter line-by-line.
    while (row < last_row) {
        VerticalPredictLine_C(in, preds, out, width, inverse);
        ++row;
        preds += stride;
        in += stride;
        out += stride;
    }
}

/* The following function is the modified version of VerticalFilter_C,
 * provided in the libwebp library. Please refer to filters.c
 * for the unmodified version in the source library. */
void vertical_filter_scalar(config_t *config,
                            input_t *input,
                            output_t *output) {
    vertical_filter_config_t *vertical_filter_config = (vertical_filter_config_t *)config;
    vertical_filter_input_t *vertical_filter_input = (vertical_filter_input_t *)input;
    vertical_filter_output_t *vertical_filter_output = (vertical_filter_output_t *)output;

    int num_rows = vertical_filter_config->num_rows;
    int num_cols = vertical_filter_config->num_cols;
    int stride = vertical_filter_config->stride;
    uint8_t *in = vertical_filter_input->in;
    uint8_t *out = vertical_filter_output->out;

    DoVerticalFilter_C(in, num_cols, num_rows, stride, 0, num_rows, 0, out);
}
