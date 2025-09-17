/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "neon.hpp"
#include <stdio.h>

#include <stdint.h>

#include "convolve_horizontally.hpp"
#include "skia.hpp"

#ifndef SWAN_SIMULATION
/* The following function is the modified version of ConvolveVertically_Neon,
 * provided in the skia library. Please refer to convolver_neon.cc
 * for the unmodified version in the source library. */
void convolve_horizontally_neon(config_t *config,
                                input_t *input,
                                output_t *output) {
    convolve_horizontally_config_t *convolve_horizontally_config = (convolve_horizontally_config_t *)config;
    convolve_horizontally_input_t *convolve_horizontally_input = (convolve_horizontally_input_t *)input;
    convolve_horizontally_output_t *convolve_horizontally_output = (convolve_horizontally_output_t *)output;

    // Loop over each pixel on this row in the output image.
    int num_cols = convolve_horizontally_config->num_cols;
    int num_rows = convolve_horizontally_config->num_rows;
    int filter_length = convolve_horizontally_config->filter_length;

    uint8_t *src_data = convolve_horizontally_input->src_data;
    int16_t *filter_values = convolve_horizontally_input->filter_values;
    uint8_t *out_row = convolve_horizontally_output->out_row;

    for (int out_y = 0; out_y < num_rows; out_y++) {
        uint8_t *out_row_addr = out_row;
        for (int out_x = 0; out_x < num_cols; out_x++) {

            // Compute the first pixel in this row that the filter affects. It will
            // touch |filter_length| pixels (4 bytes each) after this.
            const unsigned char *row_to_filter = &src_data[out_x * 4];

            int16_t *filter_values_addr = filter_values;

            // Apply the filter to the row to get the destination pixel in |accum|.
            int32x4_t accum = vdupq_n_s32(0);
            for (int filter_x = 0; filter_x < (filter_length / 4); filter_x++) {
                // Load 4 coefficients.
                int16x4_t coeffs = vld1_s16(filter_values_addr);
                // Load 4 pixels into a q-register.
                uint8x16_t pixels = vld1q_u8(row_to_filter);

                // Expand to 16-bit channels split across two q-registers.
                int16x8_t p01_16 = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(pixels)));
                int16x8_t p23_16 = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(pixels)));

                // Scale each pixel (each d-register) by its filter coefficients,
                // accumulating into 32-bit.
                accum = vmlal_lane_s16(accum, vget_low_s16(p01_16), coeffs, 0);
                accum = vmlal_lane_s16(accum, vget_high_s16(p01_16), coeffs, 1);
                accum = vmlal_lane_s16(accum, vget_low_s16(p23_16), coeffs, 2);
                accum = vmlal_lane_s16(accum, vget_high_s16(p23_16), coeffs, 3);

                // Advance to next elements.
                row_to_filter += 16;
                filter_values_addr += 4;
            }

            // Bring this value back in range. All of the filter scaling factors
            // are in fixed point with kShiftBits bits of fractional part.
            int16x4_t accum16 = vqshrn_n_s32(accum, 2);

            // Pack and store the new pixel.
            uint8x8_t accum8 = vqmovun_s16(vcombine_s16(accum16, accum16));
            vst1_lane_u32(reinterpret_cast<uint32_t *>(out_row_addr), vreinterpret_u32_u8(accum8), 0);
            out_row_addr += 4;
        }
        src_data += (num_cols + filter_length) * 4;
        out_row += num_cols * 4;
    }
}
#else
void convolve_horizontally_neon(config_t *config,
                                input_t *input,
                                output_t *output) {
    convolve_horizontally_config_t *convolve_horizontally_config = (convolve_horizontally_config_t *)config;
    convolve_horizontally_input_t *convolve_horizontally_input = (convolve_horizontally_input_t *)input;
    convolve_horizontally_output_t *convolve_horizontally_output = (convolve_horizontally_output_t *)output;

    // Loop over each pixel on this row in the output image.
    int num_cols = convolve_horizontally_config->num_cols;
    int num_rows = convolve_horizontally_config->num_rows;
    int filter_length = convolve_horizontally_config->filter_length;

    uint8_t *src_data = convolve_horizontally_input->src_data;
    int16_t *filter_values = convolve_horizontally_input->filter_values;
    uint8_t *out_row = convolve_horizontally_output->out_row;

    for (int out_y = 0; out_y < num_rows; out_y++) {
        uint8_t *out_row_addr = out_row;
        int out_x = 0;

#if SWAN_REG_TYPE >= SWAN_REG_1024_TYPE
        for (; out_x + 8 <= num_cols; out_x += 8) {
            // Compute the first pixel in this row that the filter affects. It will
            // touch |filter_length| pixels (4 bytes each) after this.
            const unsigned char *row_to_filter = &src_data[out_x * 4];
            int16_t *filter_values_addr = filter_values;
            // Apply the filter to the row to get the destination pixel in |accum|.
            int32x32_t accum = vdupoq_n_s32(0);
            int16x32_t pixels0, pixels1;
            pixels1 = vreinterpretqq_s16_u16(vmovlqq_u8(vld1dq_u8(row_to_filter)));
            row_to_filter += 32;
            for (int filter_x = 0; filter_x < filter_length / 8; filter_x++) {
                // Load 1 coefficient.
                int16x4_t coeffs0 = vld1_s16(filter_values_addr);
                int16x4_t coeffs1 = vld1_s16(filter_values_addr + 4);
                filter_values_addr += 8;

                pixels0 = pixels1;
                pixels1 = vreinterpretqq_s16_u16(vmovlqq_u8(vld1dq_u8(row_to_filter)));
                row_to_filter += 32;

                // Scale each pixel (each d-register) by its filter coefficients,
                // accumulating into 32-bit.
                accum = vmlaloq_lane_s16(accum, pixels0, coeffs0, 0);
                accum = vmlaloq_lane_s16(accum, vextqq_s16(pixels0, pixels1, 4), coeffs0, 1);
                accum = vmlaloq_lane_s16(accum, vextqq_s16(pixels0, pixels1, 8), coeffs0, 2);
                accum = vmlaloq_lane_s16(accum, vextqq_s16(pixels0, pixels1, 12), coeffs0, 3);
                accum = vmlaloq_lane_s16(accum, vextqq_s16(pixels0, pixels1, 16), coeffs1, 0);
                accum = vmlaloq_lane_s16(accum, vextqq_s16(pixels0, pixels1, 20), coeffs1, 1);
                accum = vmlaloq_lane_s16(accum, vextqq_s16(pixels0, pixels1, 24), coeffs1, 2);
                accum = vmlaloq_lane_s16(accum, vextqq_s16(pixels0, pixels1, 28), coeffs1, 3);
            }
            // Bring this value back in range. All of the filter scaling factors
            // are in fixed point with kShiftBits bits of fractional part.
            int16x32_t accum16 = vqshrnoq_n_s32(accum, 2);

            // Pack and store the new pixel.
            uint8x32_t accum8 = vqmovunqq_s16(accum16);
            vst1dq_u8(out_row_addr, accum8);
            out_row_addr += 32;

            fake_neon_flusher();
        }
#endif

#if SWAN_REG_TYPE >= SWAN_REG_512_TYPE
        for (; out_x + 4 <= num_cols; out_x += 4) {
            // Compute the first pixel in this row that the filter affects. It will
            // touch |filter_length| pixels (4 bytes each) after this.
            const unsigned char *row_to_filter = &src_data[out_x * 4];
            int16_t *filter_values_addr = filter_values;
            // Apply the filter to the row to get the destination pixel in |accum|.
            int32x16_t accum = vdupqq_n_s32(0);
            int16x16_t pixels0, pixels1;
            pixels1 = vreinterpretdq_s16_u16(vmovldq_u8(vld1q_u8(row_to_filter)));
            row_to_filter += 16;
            for (int filter_x = 0; filter_x < filter_length / 4; filter_x++) {
                // Load 1 coefficient.
                int16x4_t coeffs = vld1_s16(filter_values_addr);

                // Load 4 pixels into a q-register.
                pixels0 = pixels1;
                pixels1 = vreinterpretdq_s16_u16(vmovldq_u8(vld1q_u8(row_to_filter)));
                row_to_filter += 16;

                // Scale each pixel (each d-register) by its filter coefficients,
                // accumulating into 32-bit.
                accum = vmlalqq_lane_s16(accum, pixels0, coeffs, 0);
                accum = vmlalqq_lane_s16(accum, vextdq_s16(pixels0, pixels1, 4), coeffs, 1);
                accum = vmlalqq_lane_s16(accum, vextdq_s16(pixels0, pixels1, 8), coeffs, 2);
                accum = vmlalqq_lane_s16(accum, vextdq_s16(pixels0, pixels1, 12), coeffs, 3);

                // Advance to next elements.
                filter_values_addr += 4;
            }
            // Bring this value back in range. All of the filter scaling factors
            // are in fixed point with kShiftBits bits of fractional part.
            int16x16_t accum16 = vqshrnqq_n_s32(accum, 2);

            // Pack and store the new pixel.
            uint8x16_t accum8 = vqmovundq_s16(accum16);
            vst1q_u8(out_row_addr, accum8);
            out_row_addr += 16;

            fake_neon_flusher();
        }
#endif

#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
        for (; out_x + 2 <= num_cols; out_x += 2) {
            // Compute the first pixel in this row that the filter affects. It will
            // touch |filter_length| pixels (4 bytes each) after this.
            const unsigned char *row_to_filter = &src_data[out_x * 4];
            int16_t *filter_values_addr = filter_values;
            // Apply the filter to the row to get the destination pixel in |accum|.
            int32x8_t accum = vdupdq_n_s32(0);
            int16x8_t pixels0, pixels1, pixels2;
            pixels2 = vreinterpretq_s16_u16(vmovl_u8(vld1_u8(row_to_filter)));
            row_to_filter += 8;
            for (int filter_x = 0; filter_x < filter_length / 4; filter_x++) {
                // Load 1 coefficient.
                int16x4_t coeffs = vld1_s16(filter_values_addr);

                // Load 4 pixels into a q-register.
                pixels0 = pixels2;
                pixels1 = vreinterpretq_s16_u16(vmovl_u8(vld1_u8(row_to_filter)));
                row_to_filter += 8;
                pixels2 = vreinterpretq_s16_u16(vmovl_u8(vld1_u8(row_to_filter)));
                row_to_filter += 8;

                // Scale each pixel (each d-register) by its filter coefficients,
                // accumulating into 32-bit.
                accum = vmlaldq_lane_s16(accum, pixels0, coeffs, 0);
                accum = vmlaldq_lane_s16(accum, vextq_s16(pixels0, pixels1, 4), coeffs, 1);
                accum = vmlaldq_lane_s16(accum, pixels1, coeffs, 2);
                accum = vmlaldq_lane_s16(accum, vextq_s16(pixels1, pixels2, 4), coeffs, 3);

                // Advance to next elements.
                filter_values_addr += 4;
            }
            // Bring this value back in range. All of the filter scaling factors
            // are in fixed point with kShiftBits bits of fractional part.
            int16x8_t accum16 = vqshrndq_n_s32(accum, 2);

            // Pack and store the new pixel.
            uint8x8_t accum8 = vqmovun_s16(accum16);
            vst1_u8(out_row_addr, accum8);
            out_row_addr += 8;

            fake_neon_flusher();
        }
#endif

        for (; out_x < num_cols; out_x++) {
            // Compute the first pixel in this row that the filter affects. It will
            // touch |filter_length| pixels (4 bytes each) after this.
            const unsigned char *row_to_filter = &src_data[out_x * 4];
            int16_t *filter_values_addr = filter_values;

            // Apply the filter to the row to get the destination pixel in |accum|.
            int32x4_t accum = vdupq_n_s32(0);
            for (int filter_x = 0; filter_x < (filter_length / 4); filter_x++) {
                // Load 4 coefficients.
                int16x4_t coeffs = vld1_s16(filter_values_addr);
                // Load 4 pixels into a q-register.
                uint8x16_t pixels = vld1q_u8(row_to_filter);

                // Expand to 16-bit channels split across two q-registers.
                int16x8_t p01_16 = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(pixels)));
                int16x8_t p23_16 = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(pixels)));

                // Scale each pixel (each d-register) by its filter coefficients,
                // accumulating into 32-bit.
                accum = vmlal_lane_s16(accum, vget_low_s16(p01_16), coeffs, 0);
                accum = vmlal_lane_s16(accum, vget_high_s16(p01_16), coeffs, 1);
                accum = vmlal_lane_s16(accum, vget_low_s16(p23_16), coeffs, 2);
                accum = vmlal_lane_s16(accum, vget_high_s16(p23_16), coeffs, 3);

                // Advance to next elements.
                row_to_filter += 16;
                filter_values_addr += 4;
            }

            // Bring this value back in range. All of the filter scaling factors
            // are in fixed point with kShiftBits bits of fractional part.
            int16x4_t accum16 = vqshrn_n_s32(accum, 2);

            // Pack and store the new pixel.
            uint8x8_t accum8 = vqmovun_s16(vcombine_s16(accum16, accum16));
            vst1_lane_u32(reinterpret_cast<uint32_t *>(out_row_addr), vreinterpret_u32_u8(accum8), 0);
            out_row_addr += 4;

#if !defined(NEON2RVV)
            fake_neon_flusher();
#endif
        }

        src_data += (num_cols + filter_length) * 4;
        out_row += num_cols * 4;
    }
}
#endif