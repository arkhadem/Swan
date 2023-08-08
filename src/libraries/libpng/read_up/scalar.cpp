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
#include "read_up.hpp"

/* The following function is the modified version of png_read_filter_row_up,
 * provided in the libpng library. Please refer to pngrutil.c
 * for the unmodified version in the source library. */
void read_up_scalar(config_t *config,
                    input_t *input,
                    output_t *output) {
    read_up_config_t *read_up_config = (read_up_config_t *)config;
    read_up_input_t *read_up_input = (read_up_input_t *)input;
    read_up_output_t *read_up_output = (read_up_output_t *)output;

    for (int row = 0; row < read_up_config->num_rows; row++) {
        size_t istop = read_up_config->num_cols;
        png_bytep rp = read_up_input->input_buf[row];
        png_bytep pp = read_up_input->prev_input_buf[row];
        png_bytep rp_out = read_up_output->output_buf[row];

        for (int i = 0; i < istop; i++) {
            *rp_out = (png_byte)(((int)(*rp) + (int)(*pp++)) & 0xff);
            rp++;
            rp_out++;
        }
    }
}