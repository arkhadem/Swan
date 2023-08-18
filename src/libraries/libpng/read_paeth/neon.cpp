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
#include "read_paeth.hpp"

static uint8x8_t paeth(uint8x8_t a, uint8x8_t b, uint8x8_t c) {
    uint8x8_t d, e;
    uint16x8_t p1, pa, pb, pc;

    p1 = vaddl_u8(a, b);    /* a + b */
    pc = vaddl_u8(c, c);    /* c * 2 */
    pa = vabdl_u8(b, c);    /* pa */
    pb = vabdl_u8(a, c);    /* pb */
    pc = vabdq_u16(p1, pc); /* pc */

    p1 = vcleq_u16(pa, pb); /* pa <= pb */
    pa = vcleq_u16(pa, pc); /* pa <= pc */
    pb = vcleq_u16(pb, pc); /* pb <= pc */

    p1 = vandq_u16(p1, pa); /* pa <= pb && pa <= pc */

    d = vmovn_u16(pb);
    e = vmovn_u16(p1);

    d = vbsl_u8(d, b, c);
    e = vbsl_u8(e, a, d);

    return e;
}

/* The following function is the modified version of png_read_filter_row_paeth4_neon,
 * provided in the libpng library. Please refer to filter_neon_intrinsics.c
 * for the unmodified version in the source library. */
void read_paeth_neon(config_t *config,
                     input_t *input,
                     output_t *output) {
    read_paeth_config_t *read_paeth_config = (read_paeth_config_t *)config;
    read_paeth_input_t *read_paeth_input = (read_paeth_input_t *)input;
    read_paeth_output_t *read_paeth_output = (read_paeth_output_t *)output;

    for (int row = 0; row < read_paeth_config->num_rows; row++) {
        png_bytep rp = read_paeth_input->input_buf[row];
        png_bytep rp_stop = read_paeth_input->input_buf[row] + read_paeth_config->num_cols;
        png_bytep pp = read_paeth_input->prev_input_buf[row];
        png_bytep rp_out = read_paeth_output->output_buf[row];

        uint8x8_t vlast = vdup_n_u8(0);
        uint8x8x4_t vdest;
        vdest.val[3] = vdup_n_u8(0);

        for (; rp < rp_stop; rp += 16, pp += 16, rp_out += 16) {
            uint32x2x4_t vtmp;
            uint8x8x4_t *vrpt, *vppt;
            uint8x8x4_t vrp, vpp;
            uint32x2x4_t *temp_pointer;
            uint32x2x4_t vdest_val;

            vtmp = vld4_u32(png_ptr(uint32_t, rp));
            vrpt = png_ptr(uint8x8x4_t, &vtmp);
            vrp = *vrpt;
            vtmp = vld4_u32(png_ptrc(uint32_t, pp));
            vppt = png_ptr(uint8x8x4_t, &vtmp);
            vpp = *vppt;

            vdest.val[0] = paeth(vdest.val[3], vpp.val[0], vlast);
            vdest.val[0] = vadd_u8(vdest.val[0], vrp.val[0]);
            vdest.val[1] = paeth(vdest.val[0], vpp.val[1], vpp.val[0]);
            vdest.val[1] = vadd_u8(vdest.val[1], vrp.val[1]);
            vdest.val[2] = paeth(vdest.val[1], vpp.val[2], vpp.val[1]);
            vdest.val[2] = vadd_u8(vdest.val[2], vrp.val[2]);
            vdest.val[3] = paeth(vdest.val[2], vpp.val[3], vpp.val[2]);
            vdest.val[3] = vadd_u8(vdest.val[3], vrp.val[3]);

            vlast = vpp.val[3];

            vdest_val = png_ldr(uint32x2x4_t, &vdest);
            vst4_lane_u32(png_ptr(uint32_t, rp_out), vdest_val, 0);
        }
    }
}
