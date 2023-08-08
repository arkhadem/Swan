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
#include "forward_dct.hpp"

static inline tran_high_t fdct_round_shift(tran_high_t input) {
    tran_high_t rv = ROUND_POWER_OF_TWO(input, DCT_CONST_BITS);
    return rv;
}

/* The following function is the modified version of vpx_fdct8x8_c,
 * provided in the libvpx library. Please refer to fwd_txfm.c
 * for the unmodified version in the source library. */
void forward_dct_scalar(config_t *config,
                        input_t *input,
                        output_t *output) {
    forward_dct_config_t *forward_dct_config = (forward_dct_config_t *)config;
    forward_dct_input_t *forward_dct_input = (forward_dct_input_t *)input;
    forward_dct_output_t *forward_dct_output = (forward_dct_output_t *)output;

    int stride = forward_dct_config->stride;

    for (int block = 0; block < forward_dct_config->block_count; block++) {

        const int16_t *input_buff = forward_dct_input->input_buff[block];
        tran_low_t *output_buff = forward_dct_output->output_buff[block];

        int i, j;
        tran_low_t intermediate[64];
        int pass;
        tran_low_t *out = intermediate;
        const tran_low_t *in = NULL;

        // Transform columns
        for (pass = 0; pass < 2; ++pass) {
            tran_high_t s0, s1, s2, s3, s4, s5, s6, s7; // canbe16
            tran_high_t t0, t1, t2, t3;                 // needs32
            tran_high_t x0, x1, x2, x3;                 // canbe16

            for (i = 0; i < 8; i++) {
                // stage 1
                if (pass == 0) {
                    s0 = (input_buff[0 * stride] + input_buff[7 * stride]) * 4;
                    s1 = (input_buff[1 * stride] + input_buff[6 * stride]) * 4;
                    s2 = (input_buff[2 * stride] + input_buff[5 * stride]) * 4;
                    s3 = (input_buff[3 * stride] + input_buff[4 * stride]) * 4;
                    s4 = (input_buff[3 * stride] - input_buff[4 * stride]) * 4;
                    s5 = (input_buff[2 * stride] - input_buff[5 * stride]) * 4;
                    s6 = (input_buff[1 * stride] - input_buff[6 * stride]) * 4;
                    s7 = (input_buff[0 * stride] - input_buff[7 * stride]) * 4;
                    ++input_buff;
                } else {
                    s0 = in[0 * 8] + in[7 * 8];
                    s1 = in[1 * 8] + in[6 * 8];
                    s2 = in[2 * 8] + in[5 * 8];
                    s3 = in[3 * 8] + in[4 * 8];
                    s4 = in[3 * 8] - in[4 * 8];
                    s5 = in[2 * 8] - in[5 * 8];
                    s6 = in[1 * 8] - in[6 * 8];
                    s7 = in[0 * 8] - in[7 * 8];
                    ++in;
                }

                // fdct4(step, step);
                x0 = s0 + s3;
                x1 = s1 + s2;
                x2 = s1 - s2;
                x3 = s0 - s3;
                t0 = (x0 + x1) * cospi_16_64;
                t1 = (x0 - x1) * cospi_16_64;
                t2 = x2 * cospi_24_64 + x3 * cospi_8_64;
                t3 = -x2 * cospi_8_64 + x3 * cospi_24_64;
                out[0] = (tran_low_t)fdct_round_shift(t0);
                out[2] = (tran_low_t)fdct_round_shift(t2);
                out[4] = (tran_low_t)fdct_round_shift(t1);
                out[6] = (tran_low_t)fdct_round_shift(t3);

                // Stage 2
                t0 = (s6 - s5) * cospi_16_64;
                t1 = (s6 + s5) * cospi_16_64;
                t2 = fdct_round_shift(t0);
                t3 = fdct_round_shift(t1);

                // Stage 3
                x0 = s4 + t2;
                x1 = s4 - t2;
                x2 = s7 - t3;
                x3 = s7 + t3;

                // Stage 4
                t0 = x0 * cospi_28_64 + x3 * cospi_4_64;
                t1 = x1 * cospi_12_64 + x2 * cospi_20_64;
                t2 = x2 * cospi_12_64 + x1 * -cospi_20_64;
                t3 = x3 * cospi_28_64 + x0 * -cospi_4_64;
                out[1] = (tran_low_t)fdct_round_shift(t0);
                out[3] = (tran_low_t)fdct_round_shift(t2);
                out[5] = (tran_low_t)fdct_round_shift(t1);
                out[7] = (tran_low_t)fdct_round_shift(t3);
                out += 8;
            }
            in = intermediate;
            out = output_buff;
        }

        // Rows
        for (i = 0; i < 8; ++i) {
            for (j = 0; j < 8; ++j)
                output_buff[j + i * 8] /= 2;
        }
    }
}