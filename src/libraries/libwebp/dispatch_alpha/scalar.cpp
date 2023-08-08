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
#include "dispatch_alpha.hpp"
#include <stdint.h>

/* The following function is the modified version of DispatchAlpha_C,
 * provided in the libwebp library. Please refer to FILE_NAME
 * for the unmodified version in the source library. */
void dispatch_alpha_scalar(config_t *config,
                           input_t *input,
                           output_t *output) {
    dispatch_alpha_config_t *dispatch_alpha_config = (dispatch_alpha_config_t *)config;
    dispatch_alpha_input_t *dispatch_alpha_input = (dispatch_alpha_input_t *)input;
    dispatch_alpha_output_t *dispatch_alpha_output = (dispatch_alpha_output_t *)output;

    uint32_t alpha_mask = 0xff;
    int height = dispatch_alpha_config->num_rows;
    int width = dispatch_alpha_config->num_cols;
    int alpha_stride = width;
    int src_dst_stride = width << 2;
    uint8_t *alpha = dispatch_alpha_input->alpha;
    uint8_t *src = dispatch_alpha_input->src;
    uint8_t *dst = dispatch_alpha_output->dst;
    uint32_t *src_32 = (uint32_t *)src;
    uint32_t *dst_32 = (uint32_t *)dst;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            const uint32_t alpha_value = alpha[i];
            dst_32[i] = src_32[i];
            dst[4 * i] = alpha_value;
            alpha_mask &= alpha_value;
        }
        alpha += alpha_stride;
        dst += src_dst_stride;
        src_32 += alpha_stride;
        dst_32 += alpha_stride;
    }

    dispatch_alpha_output->return_val[0] = (alpha_mask != 0xff);
}