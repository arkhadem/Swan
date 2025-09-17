/* filter_neon_intrinsics.c - NEON optimised filter functions
 *
 * Copyright (c) 2018 Cosmin Truta
 * Copyright (c) 2014,2016 Glenn Randers-Pehrson
 * Written by James Yu <james.yu at linaro.org>, October 2013.
 * Based on filter_neon.S, written by Mans Rullgard, 2011.
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h (libpng.hpp of Swan benchmark suite)
 */

#include "neon.hpp"

#include <stdint.h>

#include "libpng.hpp"
#include "read_sub.hpp"

/* The following function is the modified version of png_read_filter_row_sub4_neon,
 * provided in the libpng library. Please refer to filter_neon_intrinsics.c
 * for the unmodified version in the source library. */
void read_sub_neon(config_t *config,
                   input_t *input,
                   output_t *output) {
    read_sub_config_t *read_sub_config = (read_sub_config_t *)config;
    read_sub_input_t *read_sub_input = (read_sub_input_t *)input;
    read_sub_output_t *read_sub_output = (read_sub_output_t *)output;

    uint32x2x4_t *temp_pointer;
    uint8x8x4_t vdest;

    for (int row = 0; row < read_sub_config->num_rows; row++) {
        png_bytep rp = read_sub_input->input_buf[row];
        png_bytep rp_out = read_sub_output->output_buf[row];
        png_bytep rp_stop = read_sub_input->input_buf[row] + read_sub_config->num_cols;

#if defined(NEON2RVV)
        #define GET(v, idx) __riscv_vget_v_u8m1x4_u8m1(v, idx)
        #define SET(v, idx, val) v = __riscv_vset_v_u8m1_u8m1x4(v, idx, val)
        SET(vdest, 3, vdup_n_u8(0));
#else
        vdest.val[3] = vdup_n_u8(0);
#endif

        for (; rp < rp_stop; rp += 16, rp_out += 16) {
            uint32x2x4_t vtmp = vld4_u32(png_ptr(uint32_t, rp));
            uint8x8x4_t *vrpt = png_ptr(uint8x8x4_t, &vtmp);
            uint8x8x4_t vrp = *vrpt;
            uint32x2x4_t vdest_val;

#if defined(NEON2RVV)
            SET(vdest, 0, vadd_u8(GET(vdest, 3), GET(vrp, 0)));
            SET(vdest, 1, vadd_u8(GET(vdest, 0), GET(vrp, 1)));
            SET(vdest, 2, vadd_u8(GET(vdest, 1), GET(vrp, 2)));
            SET(vdest, 3, vadd_u8(GET(vdest, 2), GET(vrp, 3)));
            #undef GET
            #undef SET
#else
            vdest.val[0] = vadd_u8(vdest.val[3], vrp.val[0]);
            vdest.val[1] = vadd_u8(vdest.val[0], vrp.val[1]);
            vdest.val[2] = vadd_u8(vdest.val[1], vrp.val[2]);
            vdest.val[3] = vadd_u8(vdest.val[2], vrp.val[3]);
#endif

            vdest_val = png_ldr(uint32x2x4_t, &vdest);
            vst4_lane_u32(png_ptr(uint32_t, rp_out), vdest_val, 0);
        }
    }
}
