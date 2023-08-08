// Copyright 2013 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// Utilities for processing transparent channel.
//
// Author: Skal (pascal.massimino@gmail.com)

#include "scalar.hpp"
#include "apply_alpha_multiply.hpp"

// (x * a * 32897) >> 23 is bit-wise equivalent to (int)(x * a / 255.)
// for all 8bit x or a. For bit-wise equivalence to (int)(x * a / 255. + .5),
// one can use instead: (x * a * 65793 + (1 << 23)) >> 24
#define MULTIPLIER(a) ((a)*32897U)
#define PREMULTIPLY(x, m) (((x) * (m)) >> 23)

/* The following function is the modified version of ApplyAlphaMultiply_C,
 * provided in the libwebp library. Please refer to alpha_processing.c
 * for the unmodified version in the source library. */
void apply_alpha_multiply_scalar(config_t *config,
                                 input_t *input,
                                 output_t *output) {
    apply_alpha_multiply_config_t *apply_alpha_multiply_config = (apply_alpha_multiply_config_t *)config;
    apply_alpha_multiply_input_t *apply_alpha_multiply_input = (apply_alpha_multiply_input_t *)input;
    apply_alpha_multiply_output_t *apply_alpha_multiply_output = (apply_alpha_multiply_output_t *)output;

    int h = apply_alpha_multiply_config->num_rows;
    int w = apply_alpha_multiply_config->num_cols;
    bool alpha_first = apply_alpha_multiply_config->alpha_first;
    int stride = w << 2;
    uint8_t *rgba_in = apply_alpha_multiply_input->rgba_in;
    uint8_t *rgba_out = apply_alpha_multiply_output->rgba_out;

    while (h-- > 0) {
        uint8_t *const rgb_in = rgba_in + (alpha_first ? 1 : 0);
        uint8_t *const rgb_out = rgba_out + (alpha_first ? 1 : 0);
        const uint8_t *const alpha_in = rgba_in + (alpha_first ? 0 : 3);
        uint8_t *const alpha_out = rgba_out + (alpha_first ? 0 : 3);
        int i;
        for (i = 0; i < w; ++i) {
            const uint32_t a = alpha_in[4 * i];
            alpha_out[4 * i] = alpha_in[4 * i];
            if (a != 0xff) {
                const uint32_t mult = MULTIPLIER(a);
                rgb_out[4 * i + 0] = PREMULTIPLY(rgb_in[4 * i + 0], mult);
                rgb_out[4 * i + 1] = PREMULTIPLY(rgb_in[4 * i + 1], mult);
                rgb_out[4 * i + 2] = PREMULTIPLY(rgb_in[4 * i + 2], mult);
            }
        }
        rgba_in += stride;
        rgba_out += stride;
    }
}