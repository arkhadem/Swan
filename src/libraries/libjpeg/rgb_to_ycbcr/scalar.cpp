/*
 * jccolext.c
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2009-2012, 2015, 2022, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 * This file contains input colorspace conversion routines.
 */

#include "scalar.hpp"
#include "rgb_to_ycbcr.hpp"

#define RANGE_LIMIT(value) (value)

/*
 * Convert some rows of samples to the JPEG colorspace.
 *
 * Note that we change from the application's interleaved-pixel format
 * to our internal noninterleaved, one-plane-per-component format.
 * The input buffer is therefore three times as wide as the output buffer.
 *
 * A starting row offset is provided only for the output buffer.  The caller
 * can easily adjust the passed input_buf value to accommodate any row
 * offset required on that side.
 */

/* The following function is the modified version of rgb_ycc_convert_internal,
 * provided in the libjpeg-turbo library. Please refer to jccolext.c
 * for the unmodified version in the source library. */
void rgb_to_ycbcr_scalar(config_t *config,
                         input_t *input,
                         output_t *output) {
    rgb_to_ycbcr_config_t *rgb_to_ycbcr_config = (rgb_to_ycbcr_config_t *)config;
    rgb_to_ycbcr_input_t *rgb_to_ycbcr_input = (rgb_to_ycbcr_input_t *)input;
    rgb_to_ycbcr_output_t *rgb_to_ycbcr_output = (rgb_to_ycbcr_output_t *)output;

    JLONG *ctab = rgb_to_ycbcr_config->cconvert->rgb_ycc_tab;

    JSAMPROW inptr;
    JSAMPROW outptr0;
    JSAMPROW outptr1;
    JSAMPROW outptr2;

    unsigned int num_rows = rgb_to_ycbcr_config->num_rows;
    unsigned int num_cols = rgb_to_ycbcr_config->num_cols;

    for (JDIMENSION row = 0; row < num_rows; row++) {
        inptr = rgb_to_ycbcr_input->input_buf[row];
        outptr0 = rgb_to_ycbcr_output->output_buf[0][row];
        outptr1 = rgb_to_ycbcr_output->output_buf[1][row];
        outptr2 = rgb_to_ycbcr_output->output_buf[2][row];
        for (JDIMENSION col = 0; col < num_cols; col++) {
            int r = RANGE_LIMIT(inptr[RGB_RED]);
            int g = RANGE_LIMIT(inptr[RGB_GREEN]);
            int b = RANGE_LIMIT(inptr[RGB_BLUE]);
            inptr += RGB_PIXELSIZE;
            /* If the inputs are 0..MAXJSAMPLE, the outputs of these equations
            * must be too; we do not need an explicit range-limiting operation.
            * Hence the value being shifted is never negative, and we don't
            * need the general RIGHT_SHIFT macro.
            */
            /* Y */
            outptr0[col] = (JSAMPLE)((ctab[r + R_Y_OFF] + ctab[g + G_Y_OFF] + ctab[b + B_Y_OFF]) >> SCALEBITS);
            /* Cb */
            outptr1[col] = (JSAMPLE)((ctab[r + R_CB_OFF] + ctab[g + G_CB_OFF] + ctab[b + B_CB_OFF]) >> SCALEBITS);
            /* Cr */
            outptr2[col] = (JSAMPLE)((ctab[r + R_CR_OFF] + ctab[g + G_CR_OFF] + ctab[b + B_CR_OFF]) >> SCALEBITS);
        }
    }
}