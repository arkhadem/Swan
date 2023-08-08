/* palette_neon_intrinsics.c - NEON optimised palette expansion functions
 *
 * Copyright (c) 2018-2019 Cosmin Truta
 * Copyright (c) 2017-2018 Arm Holdings. All rights reserved.
 * Written by Richard Townsend <Richard.Townsend@arm.com>, February 2017.
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h (libpng.hpp of Swan benchmark suite)
 */

#include "neon.hpp"
#include <arm_neon.h>
#include <stdint.h>
#include <stdio.h>

#include "expand_palette.hpp"
#include "libpng.hpp"

/* The following function is the modified version of png_do_expand_palette_rgba8_neon,
 * provided in the libpng library. Please refer to palette_neon_intrinsics.c
 * for the unmodified version in the source library. */
void expand_palette_neon(config_t *config,
                         input_t *input,
                         output_t *output) {
    expand_palette_config_t *expand_palette_config = (expand_palette_config_t *)config;
    expand_palette_input_t *expand_palette_input = (expand_palette_input_t *)input;
    expand_palette_output_t *expand_palette_output = (expand_palette_output_t *)output;

    int num_rows = expand_palette_config->num_rows;
    int row_width = expand_palette_config->num_cols;
    png_uint_32 *riffled_palette = (png_uint_32 *)expand_palette_config->riffled_palette;

    const png_int_32 pixels_per_chunk = 4;

    for (int row = 0; row < num_rows; row++) {
        png_bytep ssp = expand_palette_input->input_buf[row] + (size_t)row_width - 1;
        png_bytep ddp = expand_palette_output->output_buf[row] + ((size_t)row_width << 2) - 1;
        ddp = ddp - ((pixels_per_chunk * sizeof(png_uint_32)) - 1);
        for (int i = 0; i < row_width; i += pixels_per_chunk) {
            uint32x4_t cur;
            png_bytep sp = ssp - i;
            png_bytep dp = ddp - (i << 2);
            cur = vld1q_dup_u32(riffled_palette + *(sp - 3));
            cur = vld1q_lane_u32(riffled_palette + *(sp - 2), cur, 1);
            cur = vld1q_lane_u32(riffled_palette + *(sp - 1), cur, 2);
            cur = vld1q_lane_u32(riffled_palette + *(sp - 0), cur, 3);
            vst1q_u32((png_uint_32 *)dp, cur);
        }
    }
}
