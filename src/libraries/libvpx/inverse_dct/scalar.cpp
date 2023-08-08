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
#include "inverse_dct.hpp"

static inline tran_high_t check_range(tran_high_t input) {
    return input;
}

#define WRAPLOW(x) ((int32_t)check_range(x))

static inline tran_high_t dct_const_round_shift(tran_high_t input) {
    tran_high_t rv = ROUND_POWER_OF_TWO(input, DCT_CONST_BITS);
    return (tran_high_t)rv;
}

void idct8_c(const tran_low_t *input, tran_low_t *output) {
    int16_t step1[8], step2[8];
    tran_high_t temp1, temp2;

    // stage 1
    step1[0] = (int16_t)input[0];
    step1[2] = (int16_t)input[4];
    step1[1] = (int16_t)input[2];
    step1[3] = (int16_t)input[6];
    temp1 = (int16_t)input[1] * cospi_28_64 - (int16_t)input[7] * cospi_4_64;
    temp2 = (int16_t)input[1] * cospi_4_64 + (int16_t)input[7] * cospi_28_64;
    step1[4] = WRAPLOW(dct_const_round_shift(temp1));
    step1[7] = WRAPLOW(dct_const_round_shift(temp2));
    temp1 = (int16_t)input[5] * cospi_12_64 - (int16_t)input[3] * cospi_20_64;
    temp2 = (int16_t)input[5] * cospi_20_64 + (int16_t)input[3] * cospi_12_64;
    step1[5] = WRAPLOW(dct_const_round_shift(temp1));
    step1[6] = WRAPLOW(dct_const_round_shift(temp2));

    // stage 2
    temp1 = (step1[0] + step1[2]) * cospi_16_64;
    temp2 = (step1[0] - step1[2]) * cospi_16_64;
    step2[0] = WRAPLOW(dct_const_round_shift(temp1));
    step2[1] = WRAPLOW(dct_const_round_shift(temp2));
    temp1 = step1[1] * cospi_24_64 - step1[3] * cospi_8_64;
    temp2 = step1[1] * cospi_8_64 + step1[3] * cospi_24_64;
    step2[2] = WRAPLOW(dct_const_round_shift(temp1));
    step2[3] = WRAPLOW(dct_const_round_shift(temp2));
    step2[4] = WRAPLOW(step1[4] + step1[5]);
    step2[5] = WRAPLOW(step1[4] - step1[5]);
    step2[6] = WRAPLOW(-step1[6] + step1[7]);
    step2[7] = WRAPLOW(step1[6] + step1[7]);

    // stage 3
    step1[0] = WRAPLOW(step2[0] + step2[3]);
    step1[1] = WRAPLOW(step2[1] + step2[2]);
    step1[2] = WRAPLOW(step2[1] - step2[2]);
    step1[3] = WRAPLOW(step2[0] - step2[3]);
    step1[4] = step2[4];
    temp1 = (step2[6] - step2[5]) * cospi_16_64;
    temp2 = (step2[5] + step2[6]) * cospi_16_64;
    step1[5] = WRAPLOW(dct_const_round_shift(temp1));
    step1[6] = WRAPLOW(dct_const_round_shift(temp2));
    step1[7] = step2[7];

    // stage 4
    output[0] = WRAPLOW(step1[0] + step1[7]);
    output[1] = WRAPLOW(step1[1] + step1[6]);
    output[2] = WRAPLOW(step1[2] + step1[5]);
    output[3] = WRAPLOW(step1[3] + step1[4]);
    output[4] = WRAPLOW(step1[3] - step1[4]);
    output[5] = WRAPLOW(step1[2] - step1[5]);
    output[6] = WRAPLOW(step1[1] - step1[6]);
    output[7] = WRAPLOW(step1[0] - step1[7]);
}

static inline uint8_t clip_pixel(int val) {
    return (val > 255) ? 255 : (val < 0) ? 0
                                         : val;
}

static inline uint8_t clip_pixel_add(uint8_t dest, tran_high_t trans) {
    trans = WRAPLOW(trans);
    return clip_pixel(dest + (int)trans);
}

/* The following function is the modified version of vpx_idct8x8_64_add_c,
 * provided in the libvpx library. Please refer to inv_txfm.c
 * for the unmodified version in the source library. */
void inverse_dct_scalar(config_t *config,
                        input_t *input,
                        output_t *output) {
    inverse_dct_config_t *inverse_dct_config = (inverse_dct_config_t *)config;
    inverse_dct_input_t *inverse_dct_input = (inverse_dct_input_t *)input;
    inverse_dct_output_t *inverse_dct_output = (inverse_dct_output_t *)output;

    int stride = inverse_dct_config->stride;

    for (int block = 0; block < inverse_dct_config->block_count; block++) {

        const tran_low_t *input_buff = inverse_dct_input->input_buff[block];
        uint8_t *output_buff = inverse_dct_output->output_buff[block];

        int i, j;
        tran_low_t out[8 * 8];
        tran_low_t *outptr = out;
        tran_low_t temp_in[8], temp_out[8];

        // First transform rows
        for (i = 0; i < 8; ++i) {
            idct8_c(input_buff, outptr);
            input_buff += 8;
            outptr += 8;
        }

        // Then transform columns
        for (i = 0; i < 8; ++i) {
            for (j = 0; j < 8; ++j)
                temp_in[j] = out[j * 8 + i];
            idct8_c(temp_in, temp_out);
            for (j = 0; j < 8; ++j) {
                output_buff[j * stride + i] = clip_pixel_add(output_buff[j * stride + i], ROUND_POWER_OF_TWO(temp_out[j], 5));
            }
        }
    }
}