/*
 *  Copyright (c) 2015 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "scalar.hpp"
#include "sad.hpp"

#define abs(a, b) ((a) < (b) ? ((b) - (a)) : ((a) - (b)))

/* Sum the difference between every corresponding element of the buffers. */
static inline unsigned int sad(const uint8_t *src_ptr, int src_stride,
                               const uint8_t *ref_ptr, int ref_stride,
                               int width, int height) {
    int y, x;
    unsigned int sad = 0;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++)
            sad += abs(src_ptr[x], ref_ptr[x]);

        src_ptr += src_stride;
        ref_ptr += ref_stride;
    }
    return sad;
}

/* The following function is the modified version of vpx_sad##m##x##n##_c macro with m = n = 8,
 * provided in the libvpx library. Please refer to sad.c for the unmodified version in the source library. */
void sad_scalar(config_t *config,
                input_t *input,
                output_t *output) {
    sad_config_t *sad_config = (sad_config_t *)config;
    sad_input_t *sad_input = (sad_input_t *)input;
    sad_output_t *sad_output = (sad_output_t *)output;

    int src_stride = sad_config->src_stride;
    int ref_stride = sad_config->ref_stride;
    uint32_t *return_val = sad_output->return_val;

    for (int block = 0; block < sad_config->block_count; block++) {
        const uint8_t *src_ptr = sad_input->src_ptr[block];
        const uint8_t *ref_ptr = sad_input->ref_ptr[block];

        return_val[block] = sad(src_ptr, src_stride, ref_ptr, ref_stride, 8, 8);
    }
}