// Copyright 2011 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "scalar.hpp"
#include "convolve_horizontally.hpp"
#include <stdio.h>

unsigned char ClampTo8(int a) {
    if ((unsigned)a < 256)
        return a; // Avoid the extra check in the common case.
    if (a < 0)
        return 0;
    return 255;
}

/* The following function is the modified version of ConvolveHorizontally,
 * provided in the skia library. Please refer to convolver.cc
 * for the unmodified version in the source library. */

// Convolves horizontally along a single row. The row data is given in
// |src_data| and continues for the num_cols() of the filter.
void convolve_horizontally_scalar(config_t *config,
                                  input_t *input,
                                  output_t *output) {
    convolve_horizontally_config_t *convolve_horizontally_config = (convolve_horizontally_config_t *)config;
    convolve_horizontally_input_t *convolve_horizontally_input = (convolve_horizontally_input_t *)input;
    convolve_horizontally_output_t *convolve_horizontally_output = (convolve_horizontally_output_t *)output;

    // Loop over each pixel on this row in the output image.
    int num_cols = convolve_horizontally_config->num_cols;
    int num_rows = convolve_horizontally_config->num_rows;
    int filter_length = convolve_horizontally_config->filter_length;
    int shift_value = convolve_horizontally_config->shift_value;

    uint8_t *src_data = convolve_horizontally_input->src_data;
    int16_t *filter_values = convolve_horizontally_input->filter_values;
    uint8_t *out_row = convolve_horizontally_output->out_row;

    for (int out_y = 0; out_y < num_rows; out_y++) {
        for (int out_x = 0; out_x < num_cols; out_x++) {

            // Compute the first pixel in this row that the filter affects. It will
            // touch |filter_length| pixels (4 bytes each) after this.
            const uint8_t *row_to_filter = &src_data[out_x * 4];

            // Apply the filter to the row to get the destination pixel in |accum|.
            int accum[4] = {0};
            for (int filter_x = 0; filter_x < filter_length; filter_x++) {
                int16_t cur_filter = filter_values[filter_x];
                accum[0] += cur_filter * row_to_filter[filter_x * 4 + 0];
                accum[1] += cur_filter * row_to_filter[filter_x * 4 + 1];
                accum[2] += cur_filter * row_to_filter[filter_x * 4 + 2];
                accum[3] += cur_filter * row_to_filter[filter_x * 4 + 3];
            }

            // Bring this value back in range. All of the filter scaling factors
            // are in fixed point with kShiftBits bits of fractional part.
            accum[0] >>= shift_value;
            accum[1] >>= shift_value;
            accum[2] >>= shift_value;
            accum[3] >>= shift_value;

            // Store the new pixel.
            out_row[out_x * 4 + 0] = ClampTo8(accum[0]);
            out_row[out_x * 4 + 1] = ClampTo8(accum[1]);
            out_row[out_x * 4 + 2] = ClampTo8(accum[2]);
            out_row[out_x * 4 + 3] = ClampTo8(accum[3]);
        }
        src_data += (num_cols + filter_length) * 4;
        out_row += num_cols * 4;
    }
}