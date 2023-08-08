/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "scalar.hpp"
#include "row_blend.hpp"

inline uint32_t SkUnsplay(uint32_t ag, uint32_t rb) {
    const uint32_t mask = 0xFF00FF00;
    return (ag & mask) | ((rb & mask) >> 8);
}

inline void SkSplay(uint32_t color, uint32_t *ag, uint32_t *rb) {
    const uint32_t mask = 0x00FF00FF;
    *ag = (color >> 8) & mask;
    *rb = color & mask;
}

inline uint32_t SkPMLerp(uint32_t src, uint32_t dst, unsigned scale) {
    // Two 8-bit blends per two 32-bit registers, with space to make sure the math doesn't collide.
    uint32_t src_ag, src_rb, dst_ag, dst_rb;
    SkSplay(src, &src_ag, &src_rb);
    SkSplay(dst, &dst_ag, &dst_rb);

    const uint32_t ret_ag = src_ag * scale + (256 - scale) * dst_ag;
    const uint32_t ret_rb = src_rb * scale + (256 - scale) * dst_rb;

    return SkUnsplay(ret_ag, ret_rb);
}

/* The following function is the modified version of blit_row_s32_blend,
 * provided in the skia library. Please refer to SkBlitRow_D32.cpp
 * for the unmodified version in the source library. */
void row_blend_scalar(config_t *config,
                      input_t *input,
                      output_t *output) {
    row_blend_config_t *row_blend_config = (row_blend_config_t *)config;
    row_blend_input_t *row_blend_input = (row_blend_input_t *)input;
    row_blend_output_t *row_blend_output = (row_blend_output_t *)output;

    int num_cols = row_blend_config->num_cols;
    int num_rows = row_blend_config->num_rows;

    uint32_t *src = row_blend_input->src;
    uint32_t *dst = row_blend_output->dst;
    uint8_t alpha = row_blend_config->alpha + 1;

    for (int out_y = 0; out_y < num_rows; out_y++) {
        for (int out_x = 0; out_x < num_cols; out_x++) {
            *dst = SkPMLerp(*src, *dst, alpha);
            src++;
            dst++;
        }
    }
}