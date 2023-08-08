/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "neon.hpp"
#include <arm_neon.h>
#include <stdint.h>

#include "row_blend.hpp"
#include "skia.hpp"

/* The following function is the modified version of blit_row_s32_blend,
 * provided in the skia library. Please refer to SkBlitRow_D32.cpp
 * for the unmodified version in the source library. */
void row_blend_neon(config_t *config,
                    input_t *input,
                    output_t *output) {
    row_blend_config_t *row_blend_config = (row_blend_config_t *)config;
    row_blend_input_t *row_blend_input = (row_blend_input_t *)input;
    row_blend_output_t *row_blend_output = (row_blend_output_t *)output;

    int num_cols = row_blend_config->num_cols;
    int num_rows = row_blend_config->num_rows;

    uint32_t *src = row_blend_input->src;
    uint32_t *dst = row_blend_output->dst;

    uint16_t src_scale = row_blend_config->alpha + 1;
    uint16_t dst_scale = 256 - src_scale;

    for (int out_y = 0; out_y < num_rows; out_y++) {
        for (int out_x = 0; out_x < num_cols; out_x += 2) {
            uint8x8_t vsrc, vdst, vres;
            uint16x8_t vsrc_wide, vdst_wide;

            vsrc = vreinterpret_u8_u32(vld1_u32(src));
            vdst = vreinterpret_u8_u32(vld1_u32(dst));

            vsrc_wide = vmovl_u8(vsrc);
            vsrc_wide = vmulq_u16(vsrc_wide, vdupq_n_u16(src_scale));

            vdst_wide = vmull_u8(vdst, vdup_n_u8(dst_scale));

            vdst_wide += vsrc_wide;
            vres = vshrn_n_u16(vdst_wide, 8);

            vst1_u32(dst, vreinterpret_u32_u8(vres));

            src += 2;
            dst += 2;
        }
    }
}
