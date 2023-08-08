/*
 * jcsample.c
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * libjpeg-turbo Modifications:
 * Copyright 2009 Pierre Ossman <ossman@cendio.se> for Cendio AB
 * Copyright (C) 2014, MIPS Technologies, Inc., California.
 * Copyright (C) 2015, 2019, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 * This file contains downsampling routines.
 *
 * Downsampling input data is counted in "row groups".  A row group
 * is defined to be max_v_samp_factor pixel rows of each component,
 * from which the downsampler produces v_samp_factor sample rows.
 * A single row group is processed in each call to the downsampler module.
 *
 * The downsampler is responsible for edge-expansion of its output data
 * to fill an integral number of DCT blocks horizontally.  The source buffer
 * may be modified if it is helpful for this purpose (the source buffer is
 * allocated wide enough to correspond to the desired output width).
 * The caller (the prep controller) is responsible for vertical padding.
 *
 * The downsampler may request "context rows" by setting need_context_rows
 * during startup.  In this case, the input arrays will contain at least
 * one row group's worth of pixels above and below the passed-in data;
 * the caller will create dummy rows at image top and bottom by replicating
 * the first or last real pixel row.
 *
 * An excellent reference for image resampling is
 *   Digital Image Warping, George Wolberg, 1990.
 *   Pub. by IEEE Computer Society Press, Los Alamitos, CA. ISBN 0-8186-8944-7.
 *
 * The downsampling algorithm used here is a simple average of the source
 * pixels covered by the output pixel.  The hi-falutin sampling literature
 * refers to this as a "box filter".  In general the characteristics of a box
 * filter are not very good, but for the specific cases we normally use (1:1
 * and 2:1 ratios) the box is equivalent to a "triangle filter" which is not
 * nearly so bad.  If you intend to use other sampling ratios, you'd be well
 * advised to improve this code.
 *
 * A simple input-smoothing capability is provided.  This is mainly intended
 * for cleaning up color-dithered GIF input files (if you find it inadequate,
 * we suggest using an external filtering program such as pnmconvol).  When
 * enabled, each input pixel P is replaced by a weighted sum of itself and its
 * eight neighbors.  P's weight is 1-8*SF and each neighbor's weight is SF,
 * where SF = (smoothing_factor / 1024).
 * Currently, smoothing is only supported for 2h2v sampling factors.
 */

#include "scalar.hpp"
#include "downsample.hpp"

/* Downsample pixel values of a single component.
 * This version handles the standard case of 2:1 horizontal and 2:1 vertical,
 * without smoothing.
 */
/* The following function is the modified version of h2v2_downsample,
 * provided in the libjpeg-turbo library. Please refer to jcsample.c
 * for the unmodified version in the source library. */
void downsample_scalar(config_t *config,
                       input_t *input,
                       output_t *output) {
    downsample_config_t *downsample_config = (downsample_config_t *)config;
    downsample_input_t *downsample_input = (downsample_input_t *)input;
    downsample_output_t *downsample_output = (downsample_output_t *)output;
    JSAMPROW inptr0;
    JSAMPROW inptr1;
    JSAMPROW outptr;
    int bias;

    unsigned int output_num_rows = downsample_config->output_num_rows;
    unsigned int output_num_cols = downsample_config->output_num_cols;

    for (JDIMENSION row = 0; row < output_num_rows; row++) {
        outptr = downsample_output->output_buf[row];
        inptr0 = downsample_input->input_buf[2 * row];
        inptr1 = downsample_input->input_buf[2 * row + 1];
        bias = 1; /* bias = 1,2,1,2,... for successive samples */
        for (JDIMENSION col = 0; col < output_num_cols; col++) {
            *outptr++ = (JSAMPLE)((inptr0[0] + inptr0[1] + inptr1[0] + inptr1[1] + bias) >> 2);
            bias ^= 3; /* 1=>2, 2=>1 */
            inptr0 += 2;
            inptr1 += 2;
        }
    }
}