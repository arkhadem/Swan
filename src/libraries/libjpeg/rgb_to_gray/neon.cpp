
/*
 * jcgryext-neon.c - grayscale colorspace conversion (Arm Neon)
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

/* RGB -> Grayscale conversion is defined by the following equation:
 *    Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *
 * Avoid floating point arithmetic by using shifted integer constants:
 *    0.29899597 = 19595 * 2^-16
 *    0.58700561 = 38470 * 2^-16
 *    0.11399841 =  7471 * 2^-16
 * These constants are defined in jcgray-neon.c
 *
 * This is the same computation as the RGB -> Y portion of RGB -> YCbCr.
 */

#include "neon.hpp"

#include "rgb_to_gray.hpp"

/* RGB -> Grayscale conversion constants */

#define F_0_298 19595
#define F_0_587 38470
#define F_0_113 7471

/* The following function is the modified version of jsimd_rgb_gray_convert_neon,
 * provided in the libjpeg-turbo library. Please refer to jcgryext-neon.c
 * for the unmodified version in the source library. */
void rgb_to_gray_neon(config_t *config,
                      input_t *input,
                      output_t *output) {
    rgb_to_gray_config_t *rgb_to_gray_config = (rgb_to_gray_config_t *)config;
    rgb_to_gray_input_t *rgb_to_gray_input = (rgb_to_gray_input_t *)input;
    rgb_to_gray_output_t *rgb_to_gray_output = (rgb_to_gray_output_t *)output;
    JSAMPROW inptr;
    JSAMPROW outptr;

    for (JDIMENSION row = 0; row < rgb_to_gray_config->num_rows; row++) {
        inptr = rgb_to_gray_input->input_buf[row];
        outptr = rgb_to_gray_output->output_buf[row];

        for (JDIMENSION col = 0; col < rgb_to_gray_config->num_cols; col += 16) {

            uint8x16x4_t input_pixels = vld4q_u8(inptr);
            uint16x8_t r_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_RED]));
            uint16x8_t r_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_RED]));
            uint16x8_t g_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_GREEN]));
            uint16x8_t g_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_GREEN]));
            uint16x8_t b_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_BLUE]));
            uint16x8_t b_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_BLUE]));

            /* Compute Y = 0.29900 * R + 0.58700 * G + 0.11400 * B */
            uint32x4_t y_ll = vmull_n_u16(vget_low_u16(r_l), F_0_298);
            uint32x4_t y_lh = vmull_n_u16(vget_high_u16(r_l), F_0_298);
            uint32x4_t y_hl = vmull_n_u16(vget_low_u16(r_h), F_0_298);
            uint32x4_t y_hh = vmull_n_u16(vget_high_u16(r_h), F_0_298);
            y_ll = vmlal_n_u16(y_ll, vget_low_u16(g_l), F_0_587);
            y_lh = vmlal_n_u16(y_lh, vget_high_u16(g_l), F_0_587);
            y_hl = vmlal_n_u16(y_hl, vget_low_u16(g_h), F_0_587);
            y_hh = vmlal_n_u16(y_hh, vget_high_u16(g_h), F_0_587);
            y_ll = vmlal_n_u16(y_ll, vget_low_u16(b_l), F_0_113);
            y_lh = vmlal_n_u16(y_lh, vget_high_u16(b_l), F_0_113);
            y_hl = vmlal_n_u16(y_hl, vget_low_u16(b_h), F_0_113);
            y_hh = vmlal_n_u16(y_hh, vget_high_u16(b_h), F_0_113);

            /* Descale Y values (rounding right shift) and narrow to 16-bit. */
            uint16x8_t y_l = vcombine_u16(vrshrn_n_u32(y_ll, 16), vrshrn_n_u32(y_lh, 16));
            uint16x8_t y_h = vcombine_u16(vrshrn_n_u32(y_hl, 16), vrshrn_n_u32(y_hh, 16));

            /* Narrow Y values to 8-bit and store to memory.  Buffer overwrite is */
            vst1q_u8(outptr, vcombine_u8(vmovn_u16(y_l), vmovn_u16(y_h)));

            /* Increment pointers. */
            inptr += (16 * RGB_PIXELSIZE);
            outptr += 16;
        }
    }
}

#undef F_0_298
#undef F_0_587
#undef F_0_113