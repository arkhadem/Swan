/* pngrutil.c - utilities to read a PNG file
 *
 * Copyright (c) 2018 Cosmin Truta
 * Copyright (c) 1998-2002,2004,2006-2018 Glenn Randers-Pehrson
 * Copyright (c) 1996-1997 Andreas Dilger
 * Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h (libpng.hpp of Swan benchmark suite)
 *
 * This file contains routines that are only called from within
 * libpng itself during the course of reading an image.
 */

#include "scalar.hpp"
#include "read_paeth.hpp"

/* The following function is the modified version of png_read_filter_row_paeth_1byte_pixel,
 * provided in the libpng library. Please refer to pngrutil.c
 * for the unmodified version in the source library. */
void read_paeth_scalar(config_t *config,
                       input_t *input,
                       output_t *output) {
    read_paeth_config_t *read_paeth_config = (read_paeth_config_t *)config;
    read_paeth_input_t *read_paeth_input = (read_paeth_input_t *)input;
    read_paeth_output_t *read_paeth_output = (read_paeth_output_t *)output;

    for (int row = 0; row < read_paeth_config->num_rows; row++) {
        png_bytep rp = read_paeth_input->input_buf[row];
        png_bytep pp = read_paeth_input->prev_input_buf[row];
        png_bytep rp_out = read_paeth_output->output_buf[row];

        unsigned int bpp = (32 + 7) >> 3;
        png_bytep rp_end = rp + bpp;

        /* Process the first pixel in the rp completely (this is the same as 'up'
        * because there is only one candidate predictor for the first rp).
        */
        while (rp < rp_end) {
            int a = *rp++ + *pp++;
            *rp_out++ = (png_byte)a;
        }

        /* Remainder */
        rp_end = rp_end + (read_paeth_config->num_cols - bpp);

        while (rp < rp_end) {
            int a, b, c, pa, pb, pc, p;

            c = *(pp - bpp);
            a = *(rp_out - bpp);
            b = *pp++;

            p = b - c;
            pc = a - c;

            pa = p < 0 ? -p : p;
            pb = pc < 0 ? -pc : pc;
            pc = (p + pc) < 0 ? -(p + pc) : p + pc;

            if (pb < pa) {
                pa = pb;
                a = b;
            }
            if (pc < pa)
                a = c;

            a += *rp++;
            *rp_out++ = (png_byte)a;
        }
    }
}