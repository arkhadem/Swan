// Copyright 2017 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// Utilities for processing transparent channel, NEON version.
//
// Author: Skal (pascal.massimino@gmail.com)

#include "neon.hpp"

#include <stdint.h>

#include "dispatch_alpha.hpp"
#include "libwebp.hpp"

/* The following function is the modified version of DispatchAlpha_NEON,
 * provided in the libwebp library. Please refer to alpha_processing_neon.c
 * for the unmodified version in the source library. */
void dispatch_alpha_neon(config_t *config,
                         input_t *input,
                         output_t *output) {
    dispatch_alpha_config_t *dispatch_alpha_config = (dispatch_alpha_config_t *)config;
    dispatch_alpha_input_t *dispatch_alpha_input = (dispatch_alpha_input_t *)input;
    dispatch_alpha_output_t *dispatch_alpha_output = (dispatch_alpha_output_t *)output;

    uint32_t alpha_mask = 0xffu;
    uint8x8_t mask8 = vdup_n_u8(0xff);
    uint32_t tmp[2];
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
        // We don't know if alpha is first or last in dst[] (depending on rgbA/Argb
        // mode). So we must be sure dst[4*i + 8 - 1] is writable for the store.
        // Hence the test with 'width - 1' instead of just 'width'.
        int i = 0;
        for (; i + 8 <= width - 1; i += 8) {
            uint8x8x4_t rgbX = vld4_u8((const uint8_t *)(src + 4 * i));
            const uint8x8_t alphas = vld1_u8(alpha + i);
#if defined(NEON2RVV)
            #define SET(v, idx, val) v = __riscv_vset_v_u8m1_u8m1x4(v, idx, val)
            SET(rgbX, 0, alphas);
#else
            rgbX.val[0] = alphas;
#endif
            vst4_u8((uint8_t *)(dst + 4 * i), rgbX);
            mask8 = vand_u8(mask8, alphas);
        }
        for (; i < width; ++i) {
            const uint32_t alpha_value = alpha[i];
            dst_32[i] = src_32[i];
            dst[4 * i] = alpha_value;
            alpha_mask &= alpha_value;
        }
        alpha += alpha_stride;
        src += src_dst_stride;
        dst += src_dst_stride;
        src_32 += alpha_stride;
        dst_32 += alpha_stride;
    }
    vst1_u8((uint8_t *)tmp, mask8);
    alpha_mask *= 0x01010101;
    alpha_mask &= tmp[0];
    alpha_mask &= tmp[1];
    dispatch_alpha_output->return_val[0] = (alpha_mask != 0xffffffffu);
}