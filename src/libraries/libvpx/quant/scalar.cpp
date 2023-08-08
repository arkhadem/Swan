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
#include "quant.hpp"

static inline int clamp(int value, int low, int high) {
    return value < low ? low : (value > high ? high : value);
}

/* The following function is the modified version of vpx_quantize_b_c,
 * provided in the libvpx library. Please refer to quantize.c
 * for the unmodified version in the source library. */
void quant_scalar(config_t *config,
                  input_t *input,
                  output_t *output) {
    quant_config_t *quant_config = (quant_config_t *)config;
    quant_input_t *quant_input = (quant_input_t *)input;
    quant_output_t *quant_output = (quant_output_t *)output;

    intptr_t block_size = quant_config->block_size;
    const int16_t *zbin_ptr = quant_config->zbin_ptr;
    const int16_t *round_ptr = quant_config->round_ptr;
    const int16_t *quant_ptr = quant_config->quant_ptr;
    const int16_t *quant_shift_ptr = quant_config->quant_shift_ptr;
    const int16_t *dequant_ptr = quant_config->dequant_ptr;
    const int16_t *scan = quant_config->scan;

    for (int block = 0; block < quant_config->block_count; block++) {

        const tran_low_t *coeff_ptr = quant_input->coeff_ptr[block];

        tran_low_t *qcoeff_ptr = quant_output->qcoeff_ptr[block];
        tran_low_t *dqcoeff_ptr = quant_output->dqcoeff_ptr[block];

        int i, non_zero_count = (int)block_size, eob = -1;
        const int zbins[2] = {zbin_ptr[0], zbin_ptr[1]};

        // Quantization pass: All coefficients with index >= zero_flag are
        // skippable. Note: zero_flag can be zero.
        for (i = 0; i < non_zero_count; i++) {
            const int rc = scan[i];
            const int coeff = coeff_ptr[rc];
            const int coeff_sign = (coeff >> 31);
            const int abs_coeff = (coeff ^ coeff_sign) - coeff_sign;

            if (abs_coeff >= zbins[rc != 0]) {
                int tmp = clamp(abs_coeff + round_ptr[rc != 0], INT16_MIN, INT16_MAX);
                tmp = ((((tmp * quant_ptr[rc != 0]) >> 16) + tmp) * quant_shift_ptr[rc != 0]) >> 16; // quantization
                qcoeff_ptr[rc] = (tmp ^ coeff_sign) - coeff_sign;
                dqcoeff_ptr[rc] = (tran_low_t)(qcoeff_ptr[rc] * dequant_ptr[rc != 0]);

                if (tmp)
                    eob = i;
            } else {
                qcoeff_ptr[rc] = 0;
                dqcoeff_ptr[rc] = 0;
            }
        }
        quant_output->eob_ptr[block] = eob + 1;
    }
}