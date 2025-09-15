/*
 * jccolext-neon.c - colorspace conversion (64-bit Arm Neon)
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

/* RGB -> YCbCr conversion is defined by the following equations:
 *    Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *    Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *    Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 * Avoid floating point arithmetic by using shifted integer constants:
 *    0.29899597 = 19595 * 2^-16
 *    0.58700561 = 38470 * 2^-16
 *    0.11399841 =  7471 * 2^-16
 *    0.16874695 = 11059 * 2^-16
 *    0.33125305 = 21709 * 2^-16
 *    0.50000000 = 32768 * 2^-16
 *    0.41868592 = 27439 * 2^-16
 *    0.08131409 =  5329 * 2^-16
 * These constants are defined in jccolor-neon.c
 *
 * We add the fixed-point equivalent of 0.5 to Cb and Cr, which effectively
 * rounds up or down the result via integer truncation.
 */

#include "neon.hpp"

#include "rgb_to_ycbcr.hpp"

#ifndef SWAN_SIMULATION

/* The following function is the modified version of jsimd_rgb_ycc_convert_neon,
 * provided in the libjpeg-turbo library. Please refer to jccolext-neon.c
 * for the unmodified version in the source library. */
void rgb_to_ycbcr_neon(config_t *config,
                       input_t *input,
                       output_t *output) {
    rgb_to_ycbcr_config_t *rgb_to_ycbcr_config = (rgb_to_ycbcr_config_t *)config;
    rgb_to_ycbcr_input_t *rgb_to_ycbcr_input = (rgb_to_ycbcr_input_t *)input;
    rgb_to_ycbcr_output_t *rgb_to_ycbcr_output = (rgb_to_ycbcr_output_t *)output;

    /* Pointer to RGB(X/A) input data */
    JSAMPROW inptr;
    /* Pointers to Y, Cb, and Cr output data */
    JSAMPROW outptr0;
    JSAMPROW outptr1;
    JSAMPROW outptr2;

    /* Set up conversion constants. */
    const uint16x8_t consts = vld1q_u16(jsimd_rgb_ycc_neon_consts);
    const uint32x4_t scaled_128_5 = vdupq_n_u32((128 << 16) + 32767);

    for (JDIMENSION row = 0; row < rgb_to_ycbcr_config->num_rows; row++) {
        inptr = rgb_to_ycbcr_input->input_buf[row];
        outptr0 = rgb_to_ycbcr_output->output_buf[0][row];
        outptr1 = rgb_to_ycbcr_output->output_buf[1][row];
        outptr2 = rgb_to_ycbcr_output->output_buf[2][row];

        for (JDIMENSION col = 0; col < rgb_to_ycbcr_config->num_cols; col += 16) {

            uint8x16x4_t input_pixels = vld4q_u8(inptr);

            uint16x8_t r_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_RED]));
            uint16x8_t g_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_GREEN]));
            uint16x8_t b_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_BLUE]));
            uint16x8_t r_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_RED]));
            uint16x8_t g_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_GREEN]));
            uint16x8_t b_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_BLUE]));

            /* Compute Y = 0.29900 * R + 0.58700 * G + 0.11400 * B */
            uint32x4_t y_ll = vmull_laneq_u16(vget_low_u16(r_l), consts, 0);
            y_ll = vmlal_laneq_u16(y_ll, vget_low_u16(g_l), consts, 1);
            y_ll = vmlal_laneq_u16(y_ll, vget_low_u16(b_l), consts, 2);
            uint32x4_t y_lh = vmull_laneq_u16(vget_high_u16(r_l), consts, 0);
            y_lh = vmlal_laneq_u16(y_lh, vget_high_u16(g_l), consts, 1);
            y_lh = vmlal_laneq_u16(y_lh, vget_high_u16(b_l), consts, 2);
            uint32x4_t y_hl = vmull_laneq_u16(vget_low_u16(r_h), consts, 0);
            y_hl = vmlal_laneq_u16(y_hl, vget_low_u16(g_h), consts, 1);
            y_hl = vmlal_laneq_u16(y_hl, vget_low_u16(b_h), consts, 2);
            uint32x4_t y_hh = vmull_laneq_u16(vget_high_u16(r_h), consts, 0);
            y_hh = vmlal_laneq_u16(y_hh, vget_high_u16(g_h), consts, 1);
            y_hh = vmlal_laneq_u16(y_hh, vget_high_u16(b_h), consts, 2);

            /* Compute Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128 */
            uint32x4_t cb_ll = scaled_128_5;
            cb_ll = vmlsl_laneq_u16(cb_ll, vget_low_u16(r_l), consts, 3);
            cb_ll = vmlsl_laneq_u16(cb_ll, vget_low_u16(g_l), consts, 4);
            cb_ll = vmlal_laneq_u16(cb_ll, vget_low_u16(b_l), consts, 5);
            uint32x4_t cb_lh = scaled_128_5;
            cb_lh = vmlsl_laneq_u16(cb_lh, vget_high_u16(r_l), consts, 3);
            cb_lh = vmlsl_laneq_u16(cb_lh, vget_high_u16(g_l), consts, 4);
            cb_lh = vmlal_laneq_u16(cb_lh, vget_high_u16(b_l), consts, 5);
            uint32x4_t cb_hl = scaled_128_5;
            cb_hl = vmlsl_laneq_u16(cb_hl, vget_low_u16(r_h), consts, 3);
            cb_hl = vmlsl_laneq_u16(cb_hl, vget_low_u16(g_h), consts, 4);
            cb_hl = vmlal_laneq_u16(cb_hl, vget_low_u16(b_h), consts, 5);
            uint32x4_t cb_hh = scaled_128_5;
            cb_hh = vmlsl_laneq_u16(cb_hh, vget_high_u16(r_h), consts, 3);
            cb_hh = vmlsl_laneq_u16(cb_hh, vget_high_u16(g_h), consts, 4);
            cb_hh = vmlal_laneq_u16(cb_hh, vget_high_u16(b_h), consts, 5);

            /* Compute Cr = 0.50000 * R - 0.41869 * G - 0.08131 * B  + 128 */
            uint32x4_t cr_ll = scaled_128_5;
            cr_ll = vmlal_laneq_u16(cr_ll, vget_low_u16(r_l), consts, 5);
            cr_ll = vmlsl_laneq_u16(cr_ll, vget_low_u16(g_l), consts, 6);
            cr_ll = vmlsl_laneq_u16(cr_ll, vget_low_u16(b_l), consts, 7);
            uint32x4_t cr_lh = scaled_128_5;
            cr_lh = vmlal_laneq_u16(cr_lh, vget_high_u16(r_l), consts, 5);
            cr_lh = vmlsl_laneq_u16(cr_lh, vget_high_u16(g_l), consts, 6);
            cr_lh = vmlsl_laneq_u16(cr_lh, vget_high_u16(b_l), consts, 7);
            uint32x4_t cr_hl = scaled_128_5;
            cr_hl = vmlal_laneq_u16(cr_hl, vget_low_u16(r_h), consts, 5);
            cr_hl = vmlsl_laneq_u16(cr_hl, vget_low_u16(g_h), consts, 6);
            cr_hl = vmlsl_laneq_u16(cr_hl, vget_low_u16(b_h), consts, 7);
            uint32x4_t cr_hh = scaled_128_5;
            cr_hh = vmlal_laneq_u16(cr_hh, vget_high_u16(r_h), consts, 5);
            cr_hh = vmlsl_laneq_u16(cr_hh, vget_high_u16(g_h), consts, 6);
            cr_hh = vmlsl_laneq_u16(cr_hh, vget_high_u16(b_h), consts, 7);

            /* Descale Y values (rounding right shift) and narrow to 16-bit. */
            uint16x8_t y_l = vcombine_u16(vrshrn_n_u32(y_ll, 16), vrshrn_n_u32(y_lh, 16));
            uint16x8_t y_h = vcombine_u16(vrshrn_n_u32(y_hl, 16), vrshrn_n_u32(y_hh, 16));
            /* Descale Cb values (right shift) and narrow to 16-bit. */
            uint16x8_t cb_l = vcombine_u16(vshrn_n_u32(cb_ll, 16), vshrn_n_u32(cb_lh, 16));
            uint16x8_t cb_h = vcombine_u16(vshrn_n_u32(cb_hl, 16), vshrn_n_u32(cb_hh, 16));
            /* Descale Cr values (right shift) and narrow to 16-bit. */
            uint16x8_t cr_l = vcombine_u16(vshrn_n_u32(cr_ll, 16), vshrn_n_u32(cr_lh, 16));
            uint16x8_t cr_h = vcombine_u16(vshrn_n_u32(cr_hl, 16), vshrn_n_u32(cr_hh, 16));
            /* Narrow Y, Cb, and Cr values to 8-bit and store to memory.  Buffer
            * overwrite is permitted up to the next multiple of ALIGN_SIZE bytes.
            */
            vst1q_u8(outptr0, vcombine_u8(vmovn_u16(y_l), vmovn_u16(y_h)));
            vst1q_u8(outptr1, vcombine_u8(vmovn_u16(cb_l), vmovn_u16(cb_h)));
            vst1q_u8(outptr2, vcombine_u8(vmovn_u16(cr_l), vmovn_u16(cr_h)));

            /* Increment pointers. */
            inptr += (16 * RGB_PIXELSIZE);
            outptr0 += 16;
            outptr1 += 16;
            outptr2 += 16;
        }
    }
}

#else

void rgb_to_ycbcr_neon(config_t *config,
                       input_t *input,
                       output_t *output) {
    rgb_to_ycbcr_config_t *rgb_to_ycbcr_config = (rgb_to_ycbcr_config_t *)config;
    rgb_to_ycbcr_input_t *rgb_to_ycbcr_input = (rgb_to_ycbcr_input_t *)input;
    rgb_to_ycbcr_output_t *rgb_to_ycbcr_output = (rgb_to_ycbcr_output_t *)output;

    /* Pointer to RGB(X/A) input data */
    JSAMPROW inptr;
    /* Pointers to Y, Cb, and Cr output data */
    JSAMPROW outptr0;
    JSAMPROW outptr1;
    JSAMPROW outptr2;

    /* Set up conversion constants. */
    const uint16x8_t consts = vld1q_u16(jsimd_rgb_ycc_neon_consts);
    const uint32x4_t scaled_128_5 = vdupq_n_u32((128 << 16) + 32767);
#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
    const uint32x8_t scaleddq_128_5 = vdupdq_n_u32((128 << 16) + 32767);
#endif
#if SWAN_REG_TYPE >= SWAN_REG_512_TYPE
    const uint32x16_t scaledqq_128_5 = vdupqq_n_u32((128 << 16) + 32767);
#endif
#if SWAN_REG_TYPE >= SWAN_REG_1024_TYPE
    const uint32x32_t scaledoq_128_5 = vdupoq_n_u32((128 << 16) + 32767);
#endif

    for (JDIMENSION row = 0; row < rgb_to_ycbcr_config->num_rows; row++) {
        inptr = rgb_to_ycbcr_input->input_buf[row];
        outptr0 = rgb_to_ycbcr_output->output_buf[0][row];
        outptr1 = rgb_to_ycbcr_output->output_buf[1][row];
        outptr2 = rgb_to_ycbcr_output->output_buf[2][row];

        JDIMENSION col = 0;
#if SWAN_REG_TYPE >= SWAN_REG_1024_TYPE
        for (; col + 128 <= rgb_to_ycbcr_config->num_cols; col += 128) {

            uint8x128x4_t input_pixels = vld4oq_u8(inptr);

            uint16x64_t r_l = vmovloq_u8(vgetoq_low_u8(input_pixels.val[RGB_RED]));
            uint16x64_t g_l = vmovloq_u8(vgetoq_low_u8(input_pixels.val[RGB_GREEN]));
            uint16x64_t b_l = vmovloq_u8(vgetoq_low_u8(input_pixels.val[RGB_BLUE]));
            uint16x64_t r_h = vmovloq_u8(vgetoq_high_u8(input_pixels.val[RGB_RED]));
            uint16x64_t g_h = vmovloq_u8(vgetoq_high_u8(input_pixels.val[RGB_GREEN]));
            uint16x64_t b_h = vmovloq_u8(vgetoq_high_u8(input_pixels.val[RGB_BLUE]));

            /* Compute Y = 0.29900 * R + 0.58700 * G + 0.11400 * B */
            uint32x32_t y_ll = vmulloq_laneq_u16(vgetoq_low_u16(r_l), consts, 0);
            y_ll = vmlaloq_laneq_u16(y_ll, vgetoq_low_u16(g_l), consts, 1);
            y_ll = vmlaloq_laneq_u16(y_ll, vgetoq_low_u16(b_l), consts, 2);
            uint32x32_t y_lh = vmulloq_laneq_u16(vgetoq_high_u16(r_l), consts, 0);
            y_lh = vmlaloq_laneq_u16(y_lh, vgetoq_high_u16(g_l), consts, 1);
            y_lh = vmlaloq_laneq_u16(y_lh, vgetoq_high_u16(b_l), consts, 2);
            uint32x32_t y_hl = vmulloq_laneq_u16(vgetoq_low_u16(r_h), consts, 0);
            y_hl = vmlaloq_laneq_u16(y_hl, vgetoq_low_u16(g_h), consts, 1);
            y_hl = vmlaloq_laneq_u16(y_hl, vgetoq_low_u16(b_h), consts, 2);
            uint32x32_t y_hh = vmulloq_laneq_u16(vgetoq_high_u16(r_h), consts, 0);
            y_hh = vmlaloq_laneq_u16(y_hh, vgetoq_high_u16(g_h), consts, 1);
            y_hh = vmlaloq_laneq_u16(y_hh, vgetoq_high_u16(b_h), consts, 2);

            /* Compute Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128 */
            uint32x32_t cb_ll = scaledoq_128_5;
            cb_ll = vmlsloq_laneq_u16(cb_ll, vgetoq_low_u16(r_l), consts, 3);
            cb_ll = vmlsloq_laneq_u16(cb_ll, vgetoq_low_u16(g_l), consts, 4);
            cb_ll = vmlaloq_laneq_u16(cb_ll, vgetoq_low_u16(b_l), consts, 5);
            uint32x32_t cb_lh = scaledoq_128_5;
            cb_lh = vmlsloq_laneq_u16(cb_lh, vgetoq_high_u16(r_l), consts, 3);
            cb_lh = vmlsloq_laneq_u16(cb_lh, vgetoq_high_u16(g_l), consts, 4);
            cb_lh = vmlaloq_laneq_u16(cb_lh, vgetoq_high_u16(b_l), consts, 5);
            uint32x32_t cb_hl = scaledoq_128_5;
            cb_hl = vmlsloq_laneq_u16(cb_hl, vgetoq_low_u16(r_h), consts, 3);
            cb_hl = vmlsloq_laneq_u16(cb_hl, vgetoq_low_u16(g_h), consts, 4);
            cb_hl = vmlaloq_laneq_u16(cb_hl, vgetoq_low_u16(b_h), consts, 5);
            uint32x32_t cb_hh = scaledoq_128_5;
            cb_hh = vmlsloq_laneq_u16(cb_hh, vgetoq_high_u16(r_h), consts, 3);
            cb_hh = vmlsloq_laneq_u16(cb_hh, vgetoq_high_u16(g_h), consts, 4);
            cb_hh = vmlaloq_laneq_u16(cb_hh, vgetoq_high_u16(b_h), consts, 5);

            /* Compute Cr = 0.50000 * R - 0.41869 * G - 0.08131 * B  + 128 */
            uint32x32_t cr_ll = scaledoq_128_5;
            cr_ll = vmlaloq_laneq_u16(cr_ll, vgetoq_low_u16(r_l), consts, 5);
            cr_ll = vmlsloq_laneq_u16(cr_ll, vgetoq_low_u16(g_l), consts, 6);
            cr_ll = vmlsloq_laneq_u16(cr_ll, vgetoq_low_u16(b_l), consts, 7);
            uint32x32_t cr_lh = scaledoq_128_5;
            cr_lh = vmlaloq_laneq_u16(cr_lh, vgetoq_high_u16(r_l), consts, 5);
            cr_lh = vmlsloq_laneq_u16(cr_lh, vgetoq_high_u16(g_l), consts, 6);
            cr_lh = vmlsloq_laneq_u16(cr_lh, vgetoq_high_u16(b_l), consts, 7);
            uint32x32_t cr_hl = scaledoq_128_5;
            cr_hl = vmlaloq_laneq_u16(cr_hl, vgetoq_low_u16(r_h), consts, 5);
            cr_hl = vmlsloq_laneq_u16(cr_hl, vgetoq_low_u16(g_h), consts, 6);
            cr_hl = vmlsloq_laneq_u16(cr_hl, vgetoq_low_u16(b_h), consts, 7);
            uint32x32_t cr_hh = scaledoq_128_5;
            cr_hh = vmlaloq_laneq_u16(cr_hh, vgetoq_high_u16(r_h), consts, 5);
            cr_hh = vmlsloq_laneq_u16(cr_hh, vgetoq_high_u16(g_h), consts, 6);
            cr_hh = vmlsloq_laneq_u16(cr_hh, vgetoq_high_u16(b_h), consts, 7);

            /* Descale Y values (rounding right shift) and narrow to 16-bit. */
            uint16x64_t y_l = vcombineoq_u16(vrshrnoq_n_u32(y_ll, 16), vrshrnoq_n_u32(y_lh, 16));
            uint16x64_t y_h = vcombineoq_u16(vrshrnoq_n_u32(y_hl, 16), vrshrnoq_n_u32(y_hh, 16));
            /* Descale Cb values (right shift) and narrow to 16-bit. */
            uint16x64_t cb_l = vcombineoq_u16(vshrnoq_n_u32(cb_ll, 16), vshrnoq_n_u32(cb_lh, 16));
            uint16x64_t cb_h = vcombineoq_u16(vshrnoq_n_u32(cb_hl, 16), vshrnoq_n_u32(cb_hh, 16));
            /* Descale Cr values (right shift) and narrow to 16-bit. */
            uint16x64_t cr_l = vcombineoq_u16(vshrnoq_n_u32(cr_ll, 16), vshrnoq_n_u32(cr_lh, 16));
            uint16x64_t cr_h = vcombineoq_u16(vshrnoq_n_u32(cr_hl, 16), vshrnoq_n_u32(cr_hh, 16));
            /* Narrow Y, Cb, and Cr values to 8-bit and store to memory.  Buffer
            * overwrite is permitted up to the next multiple of ALIGN_SIZE bytes.
            */
            vst1oq_u8(outptr0, vcombineoq_u8(vmovnoq_u16(y_l), vmovnoq_u16(y_h)));
            vst1oq_u8(outptr1, vcombineoq_u8(vmovnoq_u16(cb_l), vmovnoq_u16(cb_h)));
            vst1oq_u8(outptr2, vcombineoq_u8(vmovnoq_u16(cr_l), vmovnoq_u16(cr_h)));

            /* Increment pointers. */
            inptr += (128 * RGB_PIXELSIZE);
            outptr0 += 128;
            outptr1 += 128;
            outptr2 += 128;
        }
#endif
#if SWAN_REG_TYPE >= SWAN_REG_512_TYPE
        for (; col + 64 <= rgb_to_ycbcr_config->num_cols; col += 64) {

            uint8x64x4_t input_pixels = vld4qq_u8(inptr);

            uint16x32_t r_l = vmovlqq_u8(vgetqq_low_u8(input_pixels.val[RGB_RED]));
            uint16x32_t g_l = vmovlqq_u8(vgetqq_low_u8(input_pixels.val[RGB_GREEN]));
            uint16x32_t b_l = vmovlqq_u8(vgetqq_low_u8(input_pixels.val[RGB_BLUE]));
            uint16x32_t r_h = vmovlqq_u8(vgetqq_high_u8(input_pixels.val[RGB_RED]));
            uint16x32_t g_h = vmovlqq_u8(vgetqq_high_u8(input_pixels.val[RGB_GREEN]));
            uint16x32_t b_h = vmovlqq_u8(vgetqq_high_u8(input_pixels.val[RGB_BLUE]));

            /* Compute Y = 0.29900 * R + 0.58700 * G + 0.11400 * B */
            uint32x16_t y_ll = vmulldq_laneq_u16(vgetqq_low_u16(r_l), consts, 0);
            y_ll = vmlalqq_laneq_u16(y_ll, vgetqq_low_u16(g_l), consts, 1);
            y_ll = vmlalqq_laneq_u16(y_ll, vgetqq_low_u16(b_l), consts, 2);
            uint32x16_t y_lh = vmulldq_laneq_u16(vgetqq_high_u16(r_l), consts, 0);
            y_lh = vmlalqq_laneq_u16(y_lh, vgetqq_high_u16(g_l), consts, 1);
            y_lh = vmlalqq_laneq_u16(y_lh, vgetqq_high_u16(b_l), consts, 2);
            uint32x16_t y_hl = vmulldq_laneq_u16(vgetqq_low_u16(r_h), consts, 0);
            y_hl = vmlalqq_laneq_u16(y_hl, vgetqq_low_u16(g_h), consts, 1);
            y_hl = vmlalqq_laneq_u16(y_hl, vgetqq_low_u16(b_h), consts, 2);
            uint32x16_t y_hh = vmulldq_laneq_u16(vgetqq_high_u16(r_h), consts, 0);
            y_hh = vmlalqq_laneq_u16(y_hh, vgetqq_high_u16(g_h), consts, 1);
            y_hh = vmlalqq_laneq_u16(y_hh, vgetqq_high_u16(b_h), consts, 2);

            /* Compute Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128 */
            uint32x16_t cb_ll = scaledqq_128_5;
            cb_ll = vmlslqq_laneq_u16(cb_ll, vgetqq_low_u16(r_l), consts, 3);
            cb_ll = vmlslqq_laneq_u16(cb_ll, vgetqq_low_u16(g_l), consts, 4);
            cb_ll = vmlalqq_laneq_u16(cb_ll, vgetqq_low_u16(b_l), consts, 5);
            uint32x16_t cb_lh = scaledqq_128_5;
            cb_lh = vmlslqq_laneq_u16(cb_lh, vgetqq_high_u16(r_l), consts, 3);
            cb_lh = vmlslqq_laneq_u16(cb_lh, vgetqq_high_u16(g_l), consts, 4);
            cb_lh = vmlalqq_laneq_u16(cb_lh, vgetqq_high_u16(b_l), consts, 5);
            uint32x16_t cb_hl = scaledqq_128_5;
            cb_hl = vmlslqq_laneq_u16(cb_hl, vgetqq_low_u16(r_h), consts, 3);
            cb_hl = vmlslqq_laneq_u16(cb_hl, vgetqq_low_u16(g_h), consts, 4);
            cb_hl = vmlalqq_laneq_u16(cb_hl, vgetqq_low_u16(b_h), consts, 5);
            uint32x16_t cb_hh = scaledqq_128_5;
            cb_hh = vmlslqq_laneq_u16(cb_hh, vgetqq_high_u16(r_h), consts, 3);
            cb_hh = vmlslqq_laneq_u16(cb_hh, vgetqq_high_u16(g_h), consts, 4);
            cb_hh = vmlalqq_laneq_u16(cb_hh, vgetqq_high_u16(b_h), consts, 5);

            /* Compute Cr = 0.50000 * R - 0.41869 * G - 0.08131 * B  + 128 */
            uint32x16_t cr_ll = scaledqq_128_5;
            cr_ll = vmlalqq_laneq_u16(cr_ll, vgetqq_low_u16(r_l), consts, 5);
            cr_ll = vmlslqq_laneq_u16(cr_ll, vgetqq_low_u16(g_l), consts, 6);
            cr_ll = vmlslqq_laneq_u16(cr_ll, vgetqq_low_u16(b_l), consts, 7);
            uint32x16_t cr_lh = scaledqq_128_5;
            cr_lh = vmlalqq_laneq_u16(cr_lh, vgetqq_high_u16(r_l), consts, 5);
            cr_lh = vmlslqq_laneq_u16(cr_lh, vgetqq_high_u16(g_l), consts, 6);
            cr_lh = vmlslqq_laneq_u16(cr_lh, vgetqq_high_u16(b_l), consts, 7);
            uint32x16_t cr_hl = scaledqq_128_5;
            cr_hl = vmlalqq_laneq_u16(cr_hl, vgetqq_low_u16(r_h), consts, 5);
            cr_hl = vmlslqq_laneq_u16(cr_hl, vgetqq_low_u16(g_h), consts, 6);
            cr_hl = vmlslqq_laneq_u16(cr_hl, vgetqq_low_u16(b_h), consts, 7);
            uint32x16_t cr_hh = scaledqq_128_5;
            cr_hh = vmlalqq_laneq_u16(cr_hh, vgetqq_high_u16(r_h), consts, 5);
            cr_hh = vmlslqq_laneq_u16(cr_hh, vgetqq_high_u16(g_h), consts, 6);
            cr_hh = vmlslqq_laneq_u16(cr_hh, vgetqq_high_u16(b_h), consts, 7);

            /* Descale Y values (rounding right shift) and narrow to 16-bit. */
            uint16x32_t y_l = vcombinedq_u16(vrshrnqq_n_u32(y_ll, 16), vrshrnqq_n_u32(y_lh, 16));
            uint16x32_t y_h = vcombinedq_u16(vrshrnqq_n_u32(y_hl, 16), vrshrnqq_n_u32(y_hh, 16));
            /* Descale Cb values (right shift) and narrow to 16-bit. */
            uint16x32_t cb_l = vcombinedq_u16(vshrnqq_n_u32(cb_ll, 16), vshrnqq_n_u32(cb_lh, 16));
            uint16x32_t cb_h = vcombinedq_u16(vshrnqq_n_u32(cb_hl, 16), vshrnqq_n_u32(cb_hh, 16));
            /* Descale Cr values (right shift) and narrow to 16-bit. */
            uint16x32_t cr_l = vcombinedq_u16(vshrnqq_n_u32(cr_ll, 16), vshrnqq_n_u32(cr_lh, 16));
            uint16x32_t cr_h = vcombinedq_u16(vshrnqq_n_u32(cr_hl, 16), vshrnqq_n_u32(cr_hh, 16));
            /* Narrow Y, Cb, and Cr values to 8-bit and store to memory.  Buffer
            * overwrite is permitted up to the next multiple of ALIGN_SIZE bytes.
            */
            vst1qq_u8(outptr0, vcombinedq_u8(vmovnqq_u16(y_l), vmovnqq_u16(y_h)));
            vst1qq_u8(outptr1, vcombinedq_u8(vmovnqq_u16(cb_l), vmovnqq_u16(cb_h)));
            vst1qq_u8(outptr2, vcombinedq_u8(vmovnqq_u16(cr_l), vmovnqq_u16(cr_h)));

            /* Increment pointers. */
            inptr += (64 * RGB_PIXELSIZE);
            outptr0 += 64;
            outptr1 += 64;
            outptr2 += 64;
        }
#endif
#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
        for (; col + 32 <= rgb_to_ycbcr_config->num_cols; col += 32) {

            uint8x32x4_t input_pixels = vld4dq_u8(inptr);

            uint16x16_t r_l = vmovldq_u8(vgetdq_low_u8(input_pixels.val[RGB_RED]));
            uint16x16_t g_l = vmovldq_u8(vgetdq_low_u8(input_pixels.val[RGB_GREEN]));
            uint16x16_t b_l = vmovldq_u8(vgetdq_low_u8(input_pixels.val[RGB_BLUE]));
            uint16x16_t r_h = vmovldq_u8(vgetdq_high_u8(input_pixels.val[RGB_RED]));
            uint16x16_t g_h = vmovldq_u8(vgetdq_high_u8(input_pixels.val[RGB_GREEN]));
            uint16x16_t b_h = vmovldq_u8(vgetdq_high_u8(input_pixels.val[RGB_BLUE]));

            /* Compute Y = 0.29900 * R + 0.58700 * G + 0.11400 * B */
            uint32x8_t y_ll = vmullq_laneq_u16(vgetdq_low_u16(r_l), consts, 0);
            y_ll = vmlaldq_laneq_u16(y_ll, vgetdq_low_u16(g_l), consts, 1);
            y_ll = vmlaldq_laneq_u16(y_ll, vgetdq_low_u16(b_l), consts, 2);
            uint32x8_t y_lh = vmullq_laneq_u16(vgetdq_high_u16(r_l), consts, 0);
            y_lh = vmlaldq_laneq_u16(y_lh, vgetdq_high_u16(g_l), consts, 1);
            y_lh = vmlaldq_laneq_u16(y_lh, vgetdq_high_u16(b_l), consts, 2);
            uint32x8_t y_hl = vmullq_laneq_u16(vgetdq_low_u16(r_h), consts, 0);
            y_hl = vmlaldq_laneq_u16(y_hl, vgetdq_low_u16(g_h), consts, 1);
            y_hl = vmlaldq_laneq_u16(y_hl, vgetdq_low_u16(b_h), consts, 2);
            uint32x8_t y_hh = vmullq_laneq_u16(vgetdq_high_u16(r_h), consts, 0);
            y_hh = vmlaldq_laneq_u16(y_hh, vgetdq_high_u16(g_h), consts, 1);
            y_hh = vmlaldq_laneq_u16(y_hh, vgetdq_high_u16(b_h), consts, 2);

            /* Compute Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128 */
            uint32x8_t cb_ll = scaleddq_128_5;
            cb_ll = vmlsldq_laneq_u16(cb_ll, vgetdq_low_u16(r_l), consts, 3);
            cb_ll = vmlsldq_laneq_u16(cb_ll, vgetdq_low_u16(g_l), consts, 4);
            cb_ll = vmlaldq_laneq_u16(cb_ll, vgetdq_low_u16(b_l), consts, 5);
            uint32x8_t cb_lh = scaleddq_128_5;
            cb_lh = vmlsldq_laneq_u16(cb_lh, vgetdq_high_u16(r_l), consts, 3);
            cb_lh = vmlsldq_laneq_u16(cb_lh, vgetdq_high_u16(g_l), consts, 4);
            cb_lh = vmlaldq_laneq_u16(cb_lh, vgetdq_high_u16(b_l), consts, 5);
            uint32x8_t cb_hl = scaleddq_128_5;
            cb_hl = vmlsldq_laneq_u16(cb_hl, vgetdq_low_u16(r_h), consts, 3);
            cb_hl = vmlsldq_laneq_u16(cb_hl, vgetdq_low_u16(g_h), consts, 4);
            cb_hl = vmlaldq_laneq_u16(cb_hl, vgetdq_low_u16(b_h), consts, 5);
            uint32x8_t cb_hh = scaleddq_128_5;
            cb_hh = vmlsldq_laneq_u16(cb_hh, vgetdq_high_u16(r_h), consts, 3);
            cb_hh = vmlsldq_laneq_u16(cb_hh, vgetdq_high_u16(g_h), consts, 4);
            cb_hh = vmlaldq_laneq_u16(cb_hh, vgetdq_high_u16(b_h), consts, 5);

            /* Compute Cr = 0.50000 * R - 0.41869 * G - 0.08131 * B  + 128 */
            uint32x8_t cr_ll = scaleddq_128_5;
            cr_ll = vmlaldq_laneq_u16(cr_ll, vgetdq_low_u16(r_l), consts, 5);
            cr_ll = vmlsldq_laneq_u16(cr_ll, vgetdq_low_u16(g_l), consts, 6);
            cr_ll = vmlsldq_laneq_u16(cr_ll, vgetdq_low_u16(b_l), consts, 7);
            uint32x8_t cr_lh = scaleddq_128_5;
            cr_lh = vmlaldq_laneq_u16(cr_lh, vgetdq_high_u16(r_l), consts, 5);
            cr_lh = vmlsldq_laneq_u16(cr_lh, vgetdq_high_u16(g_l), consts, 6);
            cr_lh = vmlsldq_laneq_u16(cr_lh, vgetdq_high_u16(b_l), consts, 7);
            uint32x8_t cr_hl = scaleddq_128_5;
            cr_hl = vmlaldq_laneq_u16(cr_hl, vgetdq_low_u16(r_h), consts, 5);
            cr_hl = vmlsldq_laneq_u16(cr_hl, vgetdq_low_u16(g_h), consts, 6);
            cr_hl = vmlsldq_laneq_u16(cr_hl, vgetdq_low_u16(b_h), consts, 7);
            uint32x8_t cr_hh = scaleddq_128_5;
            cr_hh = vmlaldq_laneq_u16(cr_hh, vgetdq_high_u16(r_h), consts, 5);
            cr_hh = vmlsldq_laneq_u16(cr_hh, vgetdq_high_u16(g_h), consts, 6);
            cr_hh = vmlsldq_laneq_u16(cr_hh, vgetdq_high_u16(b_h), consts, 7);

            /* Descale Y values (rounding right shift) and narrow to 16-bit. */
            uint16x16_t y_l = vcombineq_u16(vrshrndq_n_u32(y_ll, 16), vrshrndq_n_u32(y_lh, 16));
            uint16x16_t y_h = vcombineq_u16(vrshrndq_n_u32(y_hl, 16), vrshrndq_n_u32(y_hh, 16));
            /* Descale Cb values (right shift) and narrow to 16-bit. */
            uint16x16_t cb_l = vcombineq_u16(vshrndq_n_u32(cb_ll, 16), vshrndq_n_u32(cb_lh, 16));
            uint16x16_t cb_h = vcombineq_u16(vshrndq_n_u32(cb_hl, 16), vshrndq_n_u32(cb_hh, 16));
            /* Descale Cr values (right shift) and narrow to 16-bit. */
            uint16x16_t cr_l = vcombineq_u16(vshrndq_n_u32(cr_ll, 16), vshrndq_n_u32(cr_lh, 16));
            uint16x16_t cr_h = vcombineq_u16(vshrndq_n_u32(cr_hl, 16), vshrndq_n_u32(cr_hh, 16));
            /* Narrow Y, Cb, and Cr values to 8-bit and store to memory.  Buffer
            * overwrite is permitted up to the next multiple of ALIGN_SIZE bytes.
            */
            vst1dq_u8(outptr0, vcombineq_u8(vmovndq_u16(y_l), vmovndq_u16(y_h)));
            vst1dq_u8(outptr1, vcombineq_u8(vmovndq_u16(cb_l), vmovndq_u16(cb_h)));
            vst1dq_u8(outptr2, vcombineq_u8(vmovndq_u16(cr_l), vmovndq_u16(cr_h)));

            /* Increment pointers. */
            inptr += (32 * RGB_PIXELSIZE);
            outptr0 += 32;
            outptr1 += 32;
            outptr2 += 32;
        }
#endif
        for (; col < rgb_to_ycbcr_config->num_cols; col += 16) {

            uint8x16x4_t input_pixels = vld4q_u8(inptr);

#if defined(NEON2RVV)
            #define GET(v, idx) __riscv_vget_v_u8m1x4_u8m1(v, idx)
            uint16x8_t r_l = vmovl_u8(vget_low_u8(GET(input_pixels, RGB_RED)));
            uint16x8_t r_h = vmovl_u8(vget_high_u8(GET(input_pixels, RGB_RED)));
            uint16x8_t g_l = vmovl_u8(vget_low_u8(GET(input_pixels, RGB_GREEN)));
            uint16x8_t g_h = vmovl_u8(vget_high_u8(GET(input_pixels, RGB_GREEN)));
            uint16x8_t b_l = vmovl_u8(vget_low_u8(GET(input_pixels, RGB_BLUE)));
            uint16x8_t b_h = vmovl_u8(vget_high_u8(GET(input_pixels, RGB_BLUE)));
            #undef GET
#else
            uint16x8_t r_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_RED]));
            uint16x8_t g_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_GREEN]));
            uint16x8_t b_l = vmovl_u8(vget_low_u8(input_pixels.val[RGB_BLUE]));
            uint16x8_t r_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_RED]));
            uint16x8_t g_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_GREEN]));
            uint16x8_t b_h = vmovl_u8(vget_high_u8(input_pixels.val[RGB_BLUE]));
#endif

            /* Compute Y = 0.29900 * R + 0.58700 * G + 0.11400 * B */
            uint32x4_t y_ll = vmull_laneq_u16(vget_low_u16(r_l), consts, 0);
            y_ll = vmlal_laneq_u16(y_ll, vget_low_u16(g_l), consts, 1);
            y_ll = vmlal_laneq_u16(y_ll, vget_low_u16(b_l), consts, 2);
            uint32x4_t y_lh = vmull_laneq_u16(vget_high_u16(r_l), consts, 0);
            y_lh = vmlal_laneq_u16(y_lh, vget_high_u16(g_l), consts, 1);
            y_lh = vmlal_laneq_u16(y_lh, vget_high_u16(b_l), consts, 2);
            uint32x4_t y_hl = vmull_laneq_u16(vget_low_u16(r_h), consts, 0);
            y_hl = vmlal_laneq_u16(y_hl, vget_low_u16(g_h), consts, 1);
            y_hl = vmlal_laneq_u16(y_hl, vget_low_u16(b_h), consts, 2);
            uint32x4_t y_hh = vmull_laneq_u16(vget_high_u16(r_h), consts, 0);
            y_hh = vmlal_laneq_u16(y_hh, vget_high_u16(g_h), consts, 1);
            y_hh = vmlal_laneq_u16(y_hh, vget_high_u16(b_h), consts, 2);

            /* Compute Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128 */
            uint32x4_t cb_ll = scaled_128_5;
            cb_ll = vmlsl_laneq_u16(cb_ll, vget_low_u16(r_l), consts, 3);
            cb_ll = vmlsl_laneq_u16(cb_ll, vget_low_u16(g_l), consts, 4);
            cb_ll = vmlal_laneq_u16(cb_ll, vget_low_u16(b_l), consts, 5);
            uint32x4_t cb_lh = scaled_128_5;
            cb_lh = vmlsl_laneq_u16(cb_lh, vget_high_u16(r_l), consts, 3);
            cb_lh = vmlsl_laneq_u16(cb_lh, vget_high_u16(g_l), consts, 4);
            cb_lh = vmlal_laneq_u16(cb_lh, vget_high_u16(b_l), consts, 5);
            uint32x4_t cb_hl = scaled_128_5;
            cb_hl = vmlsl_laneq_u16(cb_hl, vget_low_u16(r_h), consts, 3);
            cb_hl = vmlsl_laneq_u16(cb_hl, vget_low_u16(g_h), consts, 4);
            cb_hl = vmlal_laneq_u16(cb_hl, vget_low_u16(b_h), consts, 5);
            uint32x4_t cb_hh = scaled_128_5;
            cb_hh = vmlsl_laneq_u16(cb_hh, vget_high_u16(r_h), consts, 3);
            cb_hh = vmlsl_laneq_u16(cb_hh, vget_high_u16(g_h), consts, 4);
            cb_hh = vmlal_laneq_u16(cb_hh, vget_high_u16(b_h), consts, 5);

            /* Compute Cr = 0.50000 * R - 0.41869 * G - 0.08131 * B  + 128 */
            uint32x4_t cr_ll = scaled_128_5;
            cr_ll = vmlal_laneq_u16(cr_ll, vget_low_u16(r_l), consts, 5);
            cr_ll = vmlsl_laneq_u16(cr_ll, vget_low_u16(g_l), consts, 6);
            cr_ll = vmlsl_laneq_u16(cr_ll, vget_low_u16(b_l), consts, 7);
            uint32x4_t cr_lh = scaled_128_5;
            cr_lh = vmlal_laneq_u16(cr_lh, vget_high_u16(r_l), consts, 5);
            cr_lh = vmlsl_laneq_u16(cr_lh, vget_high_u16(g_l), consts, 6);
            cr_lh = vmlsl_laneq_u16(cr_lh, vget_high_u16(b_l), consts, 7);
            uint32x4_t cr_hl = scaled_128_5;
            cr_hl = vmlal_laneq_u16(cr_hl, vget_low_u16(r_h), consts, 5);
            cr_hl = vmlsl_laneq_u16(cr_hl, vget_low_u16(g_h), consts, 6);
            cr_hl = vmlsl_laneq_u16(cr_hl, vget_low_u16(b_h), consts, 7);
            uint32x4_t cr_hh = scaled_128_5;
            cr_hh = vmlal_laneq_u16(cr_hh, vget_high_u16(r_h), consts, 5);
            cr_hh = vmlsl_laneq_u16(cr_hh, vget_high_u16(g_h), consts, 6);
            cr_hh = vmlsl_laneq_u16(cr_hh, vget_high_u16(b_h), consts, 7);

            /* Descale Y values (rounding right shift) and narrow to 16-bit. */
            uint16x8_t y_l = vcombine_u16(vrshrn_n_u32(y_ll, 16), vrshrn_n_u32(y_lh, 16));
            uint16x8_t y_h = vcombine_u16(vrshrn_n_u32(y_hl, 16), vrshrn_n_u32(y_hh, 16));
            /* Descale Cb values (right shift) and narrow to 16-bit. */
            uint16x8_t cb_l = vcombine_u16(vshrn_n_u32(cb_ll, 16), vshrn_n_u32(cb_lh, 16));
            uint16x8_t cb_h = vcombine_u16(vshrn_n_u32(cb_hl, 16), vshrn_n_u32(cb_hh, 16));
            /* Descale Cr values (right shift) and narrow to 16-bit. */
            uint16x8_t cr_l = vcombine_u16(vshrn_n_u32(cr_ll, 16), vshrn_n_u32(cr_lh, 16));
            uint16x8_t cr_h = vcombine_u16(vshrn_n_u32(cr_hl, 16), vshrn_n_u32(cr_hh, 16));
            /* Narrow Y, Cb, and Cr values to 8-bit and store to memory.  Buffer
            * overwrite is permitted up to the next multiple of ALIGN_SIZE bytes.
            */
            vst1q_u8(outptr0, vcombine_u8(vmovn_u16(y_l), vmovn_u16(y_h)));
            vst1q_u8(outptr1, vcombine_u8(vmovn_u16(cb_l), vmovn_u16(cb_h)));
            vst1q_u8(outptr2, vcombine_u8(vmovn_u16(cr_l), vmovn_u16(cr_h)));

            /* Increment pointers. */
            inptr += (16 * RGB_PIXELSIZE);
            outptr0 += 16;
            outptr1 += 16;
            outptr2 += 16;
        }
    }
}

#endif