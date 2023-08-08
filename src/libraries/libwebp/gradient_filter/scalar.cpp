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
#include "gradient_filter.hpp"

static inline void GradientPredictLine_C(const uint8_t *src, const uint8_t *pred,
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

static inline int GradientPredictor_C(uint8_t a, uint8_t b, uint8_t c) {
    const int g = a + b - c;
    return ((g & ~0xff) == 0) ? g : (g < 0) ? 0
                                            : 255; // clip to 8bit
}

static inline void DoGradientFilter_C(const uint8_t *in,
                                      int width, int height, int stride,
                                      int row, int num_rows,
                                      int inverse, uint8_t *out) {

    const uint8_t *preds;
    const size_t start_offset = row * stride;
    const int last_row = row + num_rows;
    in += start_offset;
    out += start_offset;
    preds = inverse ? out : in;

    // left prediction for top scan-line
    if (row == 0) {
        out[0] = in[0];
        GradientPredictLine_C(in + 1, preds, out + 1, width - 1, inverse);
        row = 1;
        preds += stride;
        in += stride;
        out += stride;
    }

    // Filter line-by-line.
    while (row < last_row) {
        int w;
        // leftmost pixel: predict from above.
        GradientPredictLine_C(in, preds - stride, out, 1, inverse);
        for (w = 1; w < width; ++w) {
            const int pred = GradientPredictor_C(preds[w - 1],
                                                 preds[w - stride],
                                                 preds[w - stride - 1]);
            out[w] = (uint8_t)(in[w] + (inverse ? pred : -pred));
        }
        ++row;
        preds += stride;
        in += stride;
        out += stride;
    }
}

/* The following function is the modified version of GradientFilter_C,
 * provided in the libwebp library. Please refer to filters.c
 * for the unmodified version in the source library. */
void gradient_filter_scalar(config_t *config,
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

    DoGradientFilter_C(in, num_cols, num_rows, stride, 0, num_rows, 0, out);
}
