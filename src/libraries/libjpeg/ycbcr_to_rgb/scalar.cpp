/*
 * jdcolext.c
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2009, 2011, 2015, 2023, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 * This file contains output colorspace conversion routines.
 */

#include "scalar.hpp"
#include "ycbcr_to_rgb.hpp"
#include <stdio.h>

/* The following function is the modified version of ycc_rgb_convert_internal,
 * provided in the libjpeg-turbo library. Please refer to jdcolext.c
 * for the unmodified version in the source library. */
void ycbcr_to_rgb_scalar(config_t *config,
                         input_t *input,
                         output_t *output) {
    ycbcr_to_rgb_config_t *ycbcr_to_rgb_config = (ycbcr_to_rgb_config_t *)config;
    ycbcr_to_rgb_input_t *ycbcr_to_rgb_input = (ycbcr_to_rgb_input_t *)input;
    ycbcr_to_rgb_output_t *ycbcr_to_rgb_output = (ycbcr_to_rgb_output_t *)output;

    JSAMPLE *range_limit = ycbcr_to_rgb_config->sample_range_limit;
    int *Crrtab = ycbcr_to_rgb_config->cconvert->Cr_r_tab;
    int *Cbbtab = ycbcr_to_rgb_config->cconvert->Cb_b_tab;
    JLONG *Crgtab = ycbcr_to_rgb_config->cconvert->Cr_g_tab;
    JLONG *Cbgtab = ycbcr_to_rgb_config->cconvert->Cb_g_tab;

    JSAMPROW outptr;
    JSAMPROW inptr0;
    JSAMPROW inptr1;
    JSAMPROW inptr2;

    for (JDIMENSION row = 0; row < ycbcr_to_rgb_config->num_rows; row++) {
        inptr0 = ycbcr_to_rgb_input->input_buf[0][row];
        inptr1 = ycbcr_to_rgb_input->input_buf[1][row];
        inptr2 = ycbcr_to_rgb_input->input_buf[2][row];
        outptr = ycbcr_to_rgb_output->output_buf[row];

        for (JDIMENSION col = 0; col < ycbcr_to_rgb_config->num_cols; col++) {
            int y = inptr0[col];
            int cb = inptr1[col];
            int cr = inptr2[col];
            /* Range-limiting is essential due to noise introduced by DCT losses. */
            outptr[RGB_RED] = range_limit[y + Crrtab[cr]];
            outptr[RGB_GREEN] = range_limit[y + ((int)RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr], SCALEBITS))];
            outptr[RGB_BLUE] = range_limit[y + Cbbtab[cb]];
            /* Set unused byte to MAXJSAMPLE so it can be interpreted as an opaque */
            /* alpha channel value */
            outptr[RGB_ALPHA] = MAXJSAMPLE;
            outptr += RGB_PIXELSIZE;
        }
    }
}