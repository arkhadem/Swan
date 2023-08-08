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
#include <arm_neon.h>
#include <stdint.h>

#include "libpng.hpp"
#include "read_up.hpp"

/* The following function is the modified version of png_read_filter_row_up_neon,
 * provided in the libpng library. Please refer to filter_neon_intrinsics.c
 * for the unmodified version in the source library. */
void read_up_neon(config_t *config,
                  input_t *input,
                  output_t *output) {
    read_up_config_t *read_up_config = (read_up_config_t *)config;
    read_up_input_t *read_up_input = (read_up_input_t *)input;
    read_up_output_t *read_up_output = (read_up_output_t *)output;

    for (int row = 0; row < read_up_config->num_rows; row++) {
        png_bytep rp = read_up_input->input_buf[row];
        png_bytep rp_stop = read_up_input->input_buf[row] + read_up_config->num_cols;
        png_bytep pp = read_up_input->prev_input_buf[row];
        png_bytep rp_out = read_up_output->output_buf[row];

        for (; rp < rp_stop; rp += 16, pp += 16, rp_out += 16) {
            uint8x16_t qrp = vld1q_u8(rp);
            uint8x16_t qpp = vld1q_u8(pp);
            qrp = vaddq_u8(qrp, qpp);
            vst1q_u8(rp_out, qrp);
        }
    }
}
