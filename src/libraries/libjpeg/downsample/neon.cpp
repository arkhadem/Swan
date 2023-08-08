/*
 * jcsample-neon.c - downsampling (Arm Neon)
 *
 * Copyright (C) 2020, Arm Limited.  All Rights Reserved.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "neon.hpp"
#include <arm_neon.h>
#include <stdint.h>

#include "downsample.hpp"
#include "libjpeg.hpp"

/* Downsample pixel values of a single component.
 * This version handles the standard case of 2:1 horizontal and 2:1 vertical,
 * without smoothing.
 */
/* The following function is the modified version of jsimd_h2v2_downsample_neon,
 * provided in the libjpeg-turbo library. Please refer to jcsample-neon.c
 * for the unmodified version in the source library. */
void downsample_neon(config_t *config,
                     input_t *input,
                     output_t *output) {
    downsample_config_t *downsample_config = (downsample_config_t *)config;
    downsample_input_t *downsample_input = (downsample_input_t *)input;
    downsample_output_t *downsample_output = (downsample_output_t *)output;

    JSAMPROW inptr0;
    JSAMPROW inptr1;
    JSAMPROW outptr;
    /* Load bias pattern (alternating every pixel.) */
    /* { 1, 2, 1, 2, 1, 2, 1, 2 } */
    const uint16x8_t bias = vreinterpretq_u16_u32(vdupq_n_u32(0x00020001));

    for (JDIMENSION row = 0; row < downsample_config->output_num_rows; row++) {
        outptr = downsample_output->output_buf[row];
        inptr0 = downsample_input->input_buf[2 * row];
        inptr1 = downsample_input->input_buf[2 * row + 1];
        for (JDIMENSION col = 0; col < downsample_config->output_num_cols; col += 8) {
            uint8x16_t pixels_r0 = vld1q_u8(inptr0);
            uint8x16_t pixels_r1 = vld1q_u8(inptr1);
            /* Add adjacent pixel values in row 0, widen to 16-bit, and add bias. */
            uint16x8_t samples_u16 = vpadalq_u8(bias, pixels_r0);
            /* Add adjacent pixel values in row 1, widen to 16-bit, and accumulate. */
            samples_u16 = vpadalq_u8(samples_u16, pixels_r1);
            /* Divide total by 4 and narrow to 8-bit. */
            uint8x8_t samples_u8 = vshrn_n_u16(samples_u16, 2);
            /* Store samples to memory and increment pointers. */
            vst1_u8(outptr, samples_u8);

            inptr0 += 16;
            inptr1 += 16;
            outptr += 8;
        }
    }
}