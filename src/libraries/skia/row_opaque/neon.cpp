/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "neon.hpp"
#include <arm_neon.h>
#include <stdint.h>

#include "row_opaque.hpp"
#include "skia.hpp"

// int8((int16(src) - int16(dst)) >> 5)
inline uint8x8_t blend_32_neon(uint8x8_t src, uint8x8_t dst, uint16x8_t scale) {
    int16x8_t src_wide, dst_wide;

    src_wide = vreinterpretq_s16_u16(vmovl_u8(src));
    dst_wide = vreinterpretq_s16_u16(vmovl_u8(dst));

    src_wide = (src_wide - dst_wide) * vreinterpretq_s16_u16(scale);

    dst_wide += vshrq_n_s16(src_wide, 5);

    return vmovn_u16(vreinterpretq_u16_s16(dst_wide));
}

/* The following function is the modified version of blit_row_lcd16_opaque,
 * provided in the skia library. Please refer to SkBlitter_ARGB32.cpp
 * for the unmodified version in the source library. */
void row_opaque_neon(config_t *config,
                     input_t *input,
                     output_t *output) {
    row_opaque_config_t *row_opaque_config = (row_opaque_config_t *)config;
    row_opaque_input_t *row_opaque_input = (row_opaque_input_t *)input;
    row_opaque_output_t *row_opaque_output = (row_opaque_output_t *)output;

    int num_cols = row_opaque_config->num_cols;
    int num_rows = row_opaque_config->num_rows;

    uint32_t *color = row_opaque_input->color;
    uint16_t *src_src = row_opaque_input->src_src;
    uint32_t *src_dst = row_opaque_input->src_dst;
    uint32_t *src_opaque = row_opaque_input->src_opaque;
    uint32_t *dst_dst = row_opaque_output->dst_dst;

    for (int out_y = 0; out_y < num_rows; out_y++) {
        int colR = SkColorGetR(color[out_y]);
        int colG = SkColorGetG(color[out_y]);
        int colB = SkColorGetB(color[out_y]);

        uint8x8_t vcolR = vdup_n_u8(colR);
        uint8x8_t vcolG = vdup_n_u8(colG);
        uint8x8_t vcolB = vdup_n_u8(colB);
        // Convert RGBA in a uint32_t to 4 uint8_ts
        uint8x8_t vopqDstA = vdup_n_u8(SkGetPackedA32(src_opaque[out_y]));
        uint8x8_t vopqDstR = vdup_n_u8(SkGetPackedR32(src_opaque[out_y]));
        uint8x8_t vopqDstG = vdup_n_u8(SkGetPackedG32(src_opaque[out_y]));
        uint8x8_t vopqDstB = vdup_n_u8(SkGetPackedB32(src_opaque[out_y]));

        for (int out_x = 0; out_x < num_cols; out_x += 8) {
            uint8x8x4_t vdst;
            uint16x8_t vmask;
            uint16x8_t vmaskR, vmaskG, vmaskB;
            uint8x8_t vsel_trans, vsel_opq;

            vdst = vld4_u8((uint8_t *)src_dst);
            vmask = vld1q_u16(src_src);

            // Prepare compare masks
            vsel_trans = vmovn_u16(vceqq_u16(vmask, vdupq_n_u16(0)));
            vsel_opq = vmovn_u16(vceqq_u16(vmask, vdupq_n_u16(0xFFFF)));

            // Get all the color masks on 5 bits
            vmaskR = vshrq_n_u16(vmask, SK_R16_SHIFT);
            vmaskG = vshrq_n_u16(vshlq_n_u16(vmask, SK_R16_BITS),
                                 SK_B16_BITS + SK_R16_BITS + 1);
            vmaskB = vmask & vdupq_n_u16(SK_B16_MASK);

            // Upscale to 0..32
            vmaskR = vmaskR + vshrq_n_u16(vmaskR, 4);
            vmaskG = vmaskG + vshrq_n_u16(vmaskG, 4);
            vmaskB = vmaskB + vshrq_n_u16(vmaskB, 4);

            vdst.val[NEON_A] = vbsl_u8(vsel_trans, vdst.val[NEON_A], vdup_n_u8(0xFF));
            vdst.val[NEON_R] = blend_32_neon(vcolR, vdst.val[NEON_R], vmaskR);
            vdst.val[NEON_G] = blend_32_neon(vcolG, vdst.val[NEON_G], vmaskG);
            vdst.val[NEON_B] = blend_32_neon(vcolB, vdst.val[NEON_B], vmaskB);

            vdst.val[NEON_A] = vbsl_u8(vsel_opq, vopqDstA, vdst.val[NEON_A]);
            vdst.val[NEON_R] = vbsl_u8(vsel_opq, vopqDstR, vdst.val[NEON_R]);
            vdst.val[NEON_G] = vbsl_u8(vsel_opq, vopqDstG, vdst.val[NEON_G]);
            vdst.val[NEON_B] = vbsl_u8(vsel_opq, vopqDstB, vdst.val[NEON_B]);

            vst4_u8((uint8_t *)dst_dst, vdst);

            src_src += 8;
            src_dst += 8;
            dst_dst += 8;
        }
    }
}
