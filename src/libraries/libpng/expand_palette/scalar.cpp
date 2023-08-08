/* pngrtran.c - transforms the data in a row for PNG readers
 *
 * Copyright (c) 2018-2019 Cosmin Truta
 * Copyright (c) 1998-2002,2004,2006-2018 Glenn Randers-Pehrson
 * Copyright (c) 1996-1997 Andreas Dilger
 * Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * This file contains functions optionally called by an application
 * in order to tell libpng how to handle data when reading a PNG.
 * Transformations that are used in both reading and writing are
 * in pngtrans.c.
 */

#include "scalar.hpp"
#include "expand_palette.hpp"
#include "libpng.hpp"
#include <stdio.h>

/* Expands a palette row to an RGB or RGBA row depending
 * upon whether you supply trans and num_trans.
 */

/* The following function is the modified version of png_do_expand_palette,
 * provided in the libpng library. Please refer to pngrtran.c
 * for the unmodified version in the source library. */
void expand_palette_scalar(config_t *config,
                           input_t *input,
                           output_t *output) {
    expand_palette_config_t *expand_palette_config = (expand_palette_config_t *)config;
    expand_palette_input_t *expand_palette_input = (expand_palette_input_t *)input;
    expand_palette_output_t *expand_palette_output = (expand_palette_output_t *)output;

    int num_rows = expand_palette_config->num_rows;
    int row_width = expand_palette_config->num_cols;
    png_bytep trans_alpha = expand_palette_config->a_palette;
    png_colorp palette = expand_palette_config->rgb_palette;

    for (int row = 0; row < num_rows; row++) {
        png_bytep sp = expand_palette_input->input_buf[row] + (size_t)row_width - 1;
        png_bytep dp = expand_palette_output->output_buf[row] + ((size_t)row_width << 2) - 1;
        for (int i = 0; i < row_width; i++) {
            *dp-- = trans_alpha[*sp];
            *dp-- = palette[*sp].blue;
            *dp-- = palette[*sp].green;
            *dp-- = palette[*sp].red;
            sp--;
        }
    }
}