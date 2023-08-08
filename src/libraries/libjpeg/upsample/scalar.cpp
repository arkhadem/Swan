
/*
 * jdmrgext.c
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2011, 2015, 2020, 2023, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 * This file contains code for merged upsampling/color conversion.
 */

#include "scalar.hpp"
#include "upsample.hpp"

/* The following function is the modified version of h2v2_merged_upsample_internal,
 * provided in the libjpeg-turbo library. Please refer to jdmrgext.c
 * for the unmodified version in the source library. */
void upsample_scalar(config_t *config,
                     input_t *input,
                     output_t *output) {
    upsample_config_t *upsample_config = (upsample_config_t *)config;
    upsample_input_t *upsample_input = (upsample_input_t *)input;
    upsample_output_t *upsample_output = (upsample_output_t *)output;

    int y, cred, cgreen, cblue;
    int cb, cr;
    /* copy these pointers into  if possible */
    JSAMPLE *range_limit = upsample_config->sample_range_limit;
    int *Crrtab = upsample_config->cconvert->Cr_r_tab;
    int *Cbbtab = upsample_config->cconvert->Cb_b_tab;
    JLONG *Crgtab = upsample_config->cconvert->Cr_g_tab;
    JLONG *Cbgtab = upsample_config->cconvert->Cb_g_tab;

    for (JDIMENSION row = 0; row < upsample_config->input_num_rows; row++) {

        JSAMPROW inptr00 = upsample_input->input_buf[0][row * 2];
        JSAMPROW inptr01 = upsample_input->input_buf[0][row * 2 + 1];
        JSAMPROW inptr1 = upsample_input->input_buf[1][row];
        JSAMPROW inptr2 = upsample_input->input_buf[2][row];
        JSAMPROW outptr0 = upsample_output->output_buf[0];
        JSAMPROW outptr1 = upsample_output->output_buf[1];
        /* Loop for each group of output pixels */
        JDIMENSION col;
        for (col = upsample_config->output_num_cols >> 1; col > 0; col--) {
            /* Do the chroma part of the calculation */
            cb = *inptr1++;
            cr = *inptr2++;
            cred = Crrtab[cr];
            cgreen = (int)RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr], SCALEBITS);
            cblue = Cbbtab[cb];
            /* Fetch 4 Y values and emit 4 pixels */
            y = *inptr00++;
            outptr0[RGB_RED] = range_limit[y + cred];
            outptr0[RGB_GREEN] = range_limit[y + cgreen];
            outptr0[RGB_BLUE] = range_limit[y + cblue];
            outptr0[RGB_ALPHA] = MAXJSAMPLE;
            outptr0 += RGB_PIXELSIZE;
            y = *inptr00++;
            outptr0[RGB_RED] = range_limit[y + cred];
            outptr0[RGB_GREEN] = range_limit[y + cgreen];
            outptr0[RGB_BLUE] = range_limit[y + cblue];
            outptr0[RGB_ALPHA] = MAXJSAMPLE;
            outptr0 += RGB_PIXELSIZE;
            y = *inptr01++;
            outptr1[RGB_RED] = range_limit[y + cred];
            outptr1[RGB_GREEN] = range_limit[y + cgreen];
            outptr1[RGB_BLUE] = range_limit[y + cblue];
            outptr1[RGB_ALPHA] = MAXJSAMPLE;
            outptr1 += RGB_PIXELSIZE;
            y = *inptr01++;
            outptr1[RGB_RED] = range_limit[y + cred];
            outptr1[RGB_GREEN] = range_limit[y + cgreen];
            outptr1[RGB_BLUE] = range_limit[y + cblue];
            outptr1[RGB_ALPHA] = MAXJSAMPLE;
            outptr1 += RGB_PIXELSIZE;
        }
        /* If image width is odd, do the last output column separately */
        if (upsample_config->output_num_cols & 1) {
            cb = *inptr1;
            cr = *inptr2;
            cred = Crrtab[cr];
            cgreen = (int)RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr], SCALEBITS);
            cblue = Cbbtab[cb];
            y = *inptr00;
            outptr0[RGB_RED] = range_limit[y + cred];
            outptr0[RGB_GREEN] = range_limit[y + cgreen];
            outptr0[RGB_BLUE] = range_limit[y + cblue];
            outptr0[RGB_ALPHA] = MAXJSAMPLE;
            y = *inptr01;
            outptr1[RGB_RED] = range_limit[y + cred];
            outptr1[RGB_GREEN] = range_limit[y + cgreen];
            outptr1[RGB_BLUE] = range_limit[y + cblue];
            outptr1[RGB_ALPHA] = MAXJSAMPLE;
        }
    }
}