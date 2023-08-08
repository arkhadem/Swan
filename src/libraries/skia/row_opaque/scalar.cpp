/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "scalar.hpp"
#include "row_opaque.hpp"

inline int upscale_31_to_32(int value) {
    return value + (value >> 4);
}

inline int blend_32(int src, int dst, int scale) {
    return dst + ((src - dst) * scale >> 5);
}

inline uint32_t SkPackARGB32(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    return (a << SK_A32_SHIFT) | (r << SK_R32_SHIFT) |
           (g << SK_G32_SHIFT) | (b << SK_B32_SHIFT);
}

inline uint32_t blend_lcd16_opaque(int srcR, int srcG, int srcB,
                                   uint32_t dst, uint16_t mask,
                                   uint32_t opaqueDst) {
    if (mask == 0) {
        return dst;
    }

    if (0xFFFF == mask) {
        return opaqueDst;
    }

    /*  We want all of these in 5bits, hence the shifts in case one of them
     *  (green) is 6bits.
     */
    int maskR = SkGetPackedR16(mask) >> (SK_R16_BITS - 5);
    int maskG = SkGetPackedG16(mask) >> (SK_G16_BITS - 5);
    int maskB = SkGetPackedB16(mask) >> (SK_B16_BITS - 5);

    // Now upscale them to 0..32, so we can use blend32
    maskR = upscale_31_to_32(maskR);
    maskG = upscale_31_to_32(maskG);
    maskB = upscale_31_to_32(maskB);

    int dstR = SkGetPackedR32(dst);
    int dstG = SkGetPackedG32(dst);
    int dstB = SkGetPackedB32(dst);

    // LCD blitting is only supported if the dst is known/required
    // to be opaque
    return SkPackARGB32(0xFF,
                        blend_32(srcR, dstR, maskR),
                        blend_32(srcG, dstG, maskG),
                        blend_32(srcB, dstB, maskB));
}

void row_opaque_scalar(config_t *config,
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
        int srcR = SkColorGetR(color[out_y]);
        int srcG = SkColorGetG(color[out_y]);
        int srcB = SkColorGetB(color[out_y]);
        for (int out_x = 0; out_x < num_cols; out_x++) {
            dst_dst[out_x] = blend_lcd16_opaque(srcR, srcG, srcB, src_dst[out_x], src_src[out_x], src_opaque[out_y]);
        }
        src_src += num_cols;
        src_dst += num_cols;
        dst_dst += num_cols;
    }
}