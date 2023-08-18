/*
 *  Copyright (c) 2017 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "neon.hpp"
#include "quant.hpp"

static inline void calculate_dqcoeff_and_store(const int16x8_t qcoeff,
                                               const int16x8_t dequant,
                                               tran_low_t *dqcoeff_ptr) {
    vst1q_s16(dqcoeff_ptr, vmulq_s16(qcoeff, dequant));
}

static inline int16x8_t load_tran_low_to_s16q(const tran_low_t *buf) {
    return vld1q_s16(buf);
}

static inline void store_s16q_to_tran_low(tran_low_t *buf, const int16x8_t a) {
    vst1q_s16(buf, a);
}

static inline int16x8_t quantize_b_neon(const tran_low_t *coeff_ptr, tran_low_t *qcoeff_ptr,
                                        tran_low_t *dqcoeff_ptr, const int16x8_t zbin,
                                        const int16x8_t round, const int16x8_t quant,
                                        const int16x8_t quant_shift, const int16x8_t dequant) {
    // Load coeffs as 8 x 16-bit ints, take sign and abs values
    const int16x8_t coeff = load_tran_low_to_s16q(coeff_ptr);
    const int16x8_t coeff_sign = vshrq_n_s16(coeff, 15);
    const int16x8_t coeff_abs = vabsq_s16(coeff);

    // Calculate mask of elements outside the bin
    const int16x8_t zbin_mask = vreinterpretq_s16_u16(vcgeq_s16(coeff_abs, zbin));

    // Get the rounded values
    const int16x8_t rounded = vqaddq_s16(coeff_abs, round);

    // (round * quant * 2) >> 16 >> 1 == (round * quant) >> 16
    int16x8_t qcoeff = vshrq_n_s16(vqdmulhq_s16(rounded, quant), 1);

    qcoeff = vaddq_s16(qcoeff, rounded);

    // (qcoeff * quant_shift * 2) >> 16 >> 1 == (qcoeff * quant_shift) >> 16
    qcoeff = vshrq_n_s16(vqdmulhq_s16(qcoeff, quant_shift), 1);

    // Restore the sign bit.
    qcoeff = veorq_s16(qcoeff, coeff_sign);
    qcoeff = vsubq_s16(qcoeff, coeff_sign);

    // Only keep the relevant coeffs
    qcoeff = vandq_s16(qcoeff, zbin_mask);
    store_s16q_to_tran_low(qcoeff_ptr, qcoeff);

    calculate_dqcoeff_and_store(qcoeff, dequant, dqcoeff_ptr);

    return qcoeff;
}

/* The following function is the modified version of vpx_quantize_b_neon,
 * provided in the libvpx library. Please refer to quantize_neon.c
 * for the unmodified version in the source library. */
void quant_neon(config_t *config,
                input_t *input,
                output_t *output) {
    quant_config_t *quant_config = (quant_config_t *)config;
    quant_input_t *quant_input = (quant_input_t *)input;
    quant_output_t *quant_output = (quant_output_t *)output;

    const int16_t *zbin_ptr = quant_config->zbin_ptr;
    const int16_t *round_ptr = quant_config->round_ptr;
    const int16_t *quant_ptr = quant_config->quant_ptr;
    const int16_t *quant_shift_ptr = quant_config->quant_shift_ptr;
    const int16_t *dequant_ptr = quant_config->dequant_ptr;

    for (int block = 0; block < quant_config->block_count; block++) {
        const int16_t *iscan = quant_config->iscan;
        intptr_t block_size = quant_config->block_size;

        const tran_low_t *coeff_ptr = quant_input->coeff_ptr[block];

        tran_low_t *qcoeff_ptr = quant_output->qcoeff_ptr[block];
        tran_low_t *dqcoeff_ptr = quant_output->dqcoeff_ptr[block];

        const int16x8_t neg_one = vdupq_n_s16(-1);
        uint16x8_t eob_max;

        // Only the first element of each vector is DC.
        int16x8_t zbin = vld1q_s16(zbin_ptr);
        int16x8_t round = vld1q_s16(round_ptr);
        int16x8_t quant = vld1q_s16(quant_ptr);
        int16x8_t quant_shift = vld1q_s16(quant_shift_ptr);
        int16x8_t dequant = vld1q_s16(dequant_ptr);

        // Process first 8 values which include a dc component.
        {
            const uint16x8_t v_iscan = vreinterpretq_u16_s16(vld1q_s16(iscan));

            const int16x8_t qcoeff = quantize_b_neon(coeff_ptr, qcoeff_ptr, dqcoeff_ptr, zbin, round, quant, quant_shift, dequant);

            // Set non-zero elements to -1 and use that to extract values for eob.
            eob_max = vandq_u16(vtstq_s16(qcoeff, neg_one), v_iscan);

            coeff_ptr += 8;
            iscan += 8;
            qcoeff_ptr += 8;
            dqcoeff_ptr += 8;
        }

        block_size -= 8;

        {
            zbin = vdupq_lane_s16(vget_low_s16(zbin), 1);
            round = vdupq_lane_s16(vget_low_s16(round), 1);
            quant = vdupq_lane_s16(vget_low_s16(quant), 1);
            quant_shift = vdupq_lane_s16(vget_low_s16(quant_shift), 1);
            dequant = vdupq_lane_s16(vget_low_s16(dequant), 1);

            do {
                const uint16x8_t v_iscan = vreinterpretq_u16_s16(vld1q_s16(iscan));

                const int16x8_t qcoeff = quantize_b_neon(coeff_ptr, qcoeff_ptr, dqcoeff_ptr, zbin, round, quant, quant_shift, dequant);

                // Set non-zero elements to -1 and use that to extract values for eob.
                eob_max = vmaxq_u16(eob_max, vandq_u16(vtstq_s16(qcoeff, neg_one), v_iscan));

                coeff_ptr += 8;
                iscan += 8;
                qcoeff_ptr += 8;
                dqcoeff_ptr += 8;
                block_size -= 8;
            } while (block_size > 0);
        }

        quant_output->eob_ptr[block] = vmaxvq_u16(eob_max);
    }
}
