// Copyright 2011 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "scalar.hpp"
#include "convolve_vertically.hpp"

/* The following function is the modified version of ConvolveVertically,
 * provided in the skia library. Please refer to convolver.cc
 * for the unmodified version in the source library. */

// Does vertical convolution to produce one output row. The filter values and
// length are given in the first two parameters. These are applied to each
// of the rows pointed to in the |source_data_rows| array, with each row
// being |pixel_width| wide.
//
// The output must have room for |pixel_width * 4| bytes.
void convolve_vertically_scalar(config_t *config,
                                input_t *input,
                                output_t *output) {
    convolve_vertically_config_t *convolve_vertically_config = (convolve_vertically_config_t *)config;
    convolve_vertically_input_t *convolve_vertically_input = (convolve_vertically_input_t *)input;
    convolve_vertically_output_t *convolve_vertically_output = (convolve_vertically_output_t *)output;

    // Loop over each pixel on this row in the output image.
    int num_cols = convolve_vertically_config->num_cols;
    int num_rows = convolve_vertically_config->num_rows;
    int filter_length = convolve_vertically_config->filter_length;

    uint8_t **src_data = (uint8_t **)convolve_vertically_input->src_data;
    int16_t *filter_values = convolve_vertically_input->filter_values;

    for (int out_y = 0; out_y < num_rows; out_y++) {

        uint8_t *out_row = (uint8_t *)convolve_vertically_output->out_row[out_y];
        // We go through each column in the output and do a vertical convolution,
        // generating one output pixel each time.
        for (int out_x = 0; out_x < num_cols; out_x++) {
            // Compute the number of bytes over in each row that the current column
            // we're convolving starts at. The pixel will cover the next 4 bytes.
            int byte_offset = out_x * 4;

            // Apply the filter to one column of pixels.
            int accum[4] = {0};
            for (int filter_y = 0; filter_y < filter_length; filter_y++) {
                int16_t cur_filter = filter_values[filter_y];
                accum[0] += cur_filter * src_data[filter_y][byte_offset + 0];
                accum[1] += cur_filter * src_data[filter_y][byte_offset + 1];
                accum[2] += cur_filter * src_data[filter_y][byte_offset + 2];
                accum[3] += cur_filter * src_data[filter_y][byte_offset + 3];
            }

            // Bring this value back in range. All of the filter scaling factors
            // are in fixed point with kShiftBits bits of precision.
            accum[0] >>= 2;
            accum[1] >>= 2;
            accum[2] >>= 2;
            accum[3] >>= 2;

            // Store the new pixel.
            out_row[byte_offset + 0] = ClampTo8(accum[0]);
            out_row[byte_offset + 1] = ClampTo8(accum[1]);
            out_row[byte_offset + 2] = ClampTo8(accum[2]);
            out_row[byte_offset + 3] = ClampTo8(accum[3]);
        }
        src_data += 1;
    }
}