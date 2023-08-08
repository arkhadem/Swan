
/*
 * jdcolor.c
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * Modified 2011 by Guido Vollbeding.
 * libjpeg-turbo Modifications:
 * Copyright 2009 Pierre Ossman <ossman@cendio.se> for Cendio AB
 * Copyright (C) 2009, 2011-2012, 2014-2015, D. R. Commander.
 * Copyright (C) 2013, Linaro Limited.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 * This file contains output colorspace conversion routines.
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

#include "scalar.hpp"
#include "rgb_to_gray.hpp"

/* The following function is the modified version of rgb_gray_convert,
 * provided in the libjpeg_turboturbo library. Please refer to jdcolor.c
 * for the unmodified version in the source library. */
void rgb_to_gray_scalar(config_t *config,
                        input_t *input,
                        output_t *output) {
    rgb_to_gray_config_t *rgb_to_gray_config = (rgb_to_gray_config_t *)config;
    rgb_to_gray_input_t *rgb_to_gray_input = (rgb_to_gray_input_t *)input;
    rgb_to_gray_output_t *rgb_to_gray_output = (rgb_to_gray_output_t *)output;

    JLONG *ctab = rgb_to_gray_config->cconvert->rgb_y_tab;
    unsigned int num_rows = rgb_to_gray_config->num_rows;
    unsigned int num_cols = rgb_to_gray_config->num_cols;

    JSAMPROW outptr;
    JSAMPROW inptr;

    for (JDIMENSION row = 0; row < num_rows; row++) {
        inptr = rgb_to_gray_input->input_buf[row];
        outptr = rgb_to_gray_output->output_buf[row];

        for (JDIMENSION col = 0; col < num_cols; col++) {
            int r = inptr[0];
            int g = inptr[1];
            int b = inptr[2];
            /* Y */
            outptr[col] = (JSAMPLE)((ctab[r + R_Y_OFF] + ctab[g + G_Y_OFF] + ctab[b + B_Y_OFF]) >> SCALEBITS);
            inptr += RGB_PIXELSIZE;
        }
    }
}