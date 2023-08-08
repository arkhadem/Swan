/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "neon.hpp"
#include <arm_neon.h>
#include <stdint.h>

#include "convolve_vertically.hpp"
#include "skia.hpp"

/* The following function is the modified version of ConvolveHorizontally_Neon,
 * provided in the skia library. Please refer to convolver_neon.cc
 * for the unmodified version in the source library. */

// Does vertical convolution to produce one output row. The filter values and
// length are given in the first two parameters. These are applied to each
// of the rows pointed to in the |source_data_rows| array, with each row
// being |pixel_width| wide.
//
// The output must have room for |pixel_width * 4| bytes.
void convolve_vertically_neon(config_t *config,
                              input_t *input,
                              output_t *output) {
    convolve_vertically_config_t *convolve_vertically_config = (convolve_vertically_config_t *)config;
    convolve_vertically_input_t *convolve_vertically_input = (convolve_vertically_input_t *)input;
    convolve_vertically_output_t *convolve_vertically_output = (convolve_vertically_output_t *)output;

    // Loop over each pixel on this row in the output image.
    int num_cols = convolve_vertically_config->num_cols;
    int num_rows = convolve_vertically_config->num_rows;
    int filter_length = convolve_vertically_config->filter_length;

    uint8_t **src_data = (uint8_t **)convolve_vertically_input->src_data;
    int16_t *filter_values = convolve_vertically_input->filter_values;

    for (int out_y = 0; out_y < num_rows; out_y++) {
        uint8_t *out_row = (uint8_t *)convolve_vertically_output->out_row[out_y];

        // Output four pixels per iteration (16 bytes).
        for (int out_x = 0; out_x < num_cols; out_x += 4) {
            // Accumulated result for each pixel. 32 bits per RGBA channel.
            int32x4_t accum0 = vdupq_n_s32(0);
            int32x4_t accum1 = vdupq_n_s32(0);
            int32x4_t accum2 = vdupq_n_s32(0);
            int32x4_t accum3 = vdupq_n_s32(0);

            // Convolve with one filter coefficient per iteration.
            for (int filter_y = 0; filter_y < filter_length; filter_y++) {
                // Load four pixels (16 bytes) together.
                // [8] a3 b3 g3 r3 a2 b2 g2 r2 a1 b1 g1 r1 a0 b0 g0 r0
                uint8x16_t src8 = vld1q_u8(&src_data[filter_y][out_x << 2]);

                int16x8_t src16_01 = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(src8)));
                int16x8_t src16_23 = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(src8)));

                accum0 = vmlal_n_s16(accum0, vget_low_s16(src16_01), filter_values[filter_y]);
                accum1 = vmlal_n_s16(accum1, vget_high_s16(src16_01), filter_values[filter_y]);
                accum2 = vmlal_n_s16(accum2, vget_low_s16(src16_23), filter_values[filter_y]);
                accum3 = vmlal_n_s16(accum3, vget_high_s16(src16_23), filter_values[filter_y]);
            }

            // Shift right for fixed point implementation.
            // Packing 32 bits |accum| to 16 bits per channel (unsigned saturation).
            int16x4_t accum16_0 = vqshrn_n_s32(accum0, 2);
            int16x4_t accum16_1 = vqshrn_n_s32(accum1, 2);
            int16x4_t accum16_2 = vqshrn_n_s32(accum2, 2);
            int16x4_t accum16_3 = vqshrn_n_s32(accum3, 2);

            // [16] a1 b1 g1 r1 a0 b0 g0 r0
            int16x8_t accum16_low = vcombine_s16(accum16_0, accum16_1);
            // [16] a3 b3 g3 r3 a2 b2 g2 r2
            int16x8_t accum16_high = vcombine_s16(accum16_2, accum16_3);

            // Packing 16 bits |accum| to 8 bits per channel (unsigned saturation).
            // [8] a3 b3 g3 r3 a2 b2 g2 r2 a1 b1 g1 r1 a0 b0 g0 r0
            uint8x16_t accum8 = vcombine_u8(vqmovun_s16(accum16_low), vqmovun_s16(accum16_high));

            // Store the convolution result (16 bytes) and advance the pixel pointers.
            vst1q_u8(out_row, accum8);
            out_row += 16;
        }
        src_data += 1;
    }
}
