/*
 *  Copyright (c) 2015 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "neon.hpp"
#include "forward_dct.hpp"
#include <arm_neon.h>

static inline void store_s16q_to_tran_low(tran_low_t *buf, const int16x8_t a) {
    vst1q_s16(buf, a);
}

// Transpose 64 bit elements as follows:
// a0: 00 01 02 03 04 05 06 07
// a1: 16 17 18 19 20 21 22 23
//
// b0.val[0]: 00 01 02 03 16 17 18 19
// b0.val[1]: 04 05 06 07 20 21 22 23
static inline int16x8x2_t vpx_vtrnq_s64_to_s16(int32x4_t a0, int32x4_t a1) {
    int16x8x2_t b0;
    b0.val[0] = vcombine_s16(vreinterpret_s16_s32(vget_low_s32(a0)),
                             vreinterpret_s16_s32(vget_low_s32(a1)));
    b0.val[1] = vcombine_s16(vreinterpret_s16_s32(vget_high_s32(a0)),
                             vreinterpret_s16_s32(vget_high_s32(a1)));
    return b0;
}

static inline void transpose_s16_8x8(int16x8_t *a0, int16x8_t *a1,
                                     int16x8_t *a2, int16x8_t *a3,
                                     int16x8_t *a4, int16x8_t *a5,
                                     int16x8_t *a6, int16x8_t *a7) {
    // Swap 16 bit elements. Goes from:
    // a0: 00 01 02 03 04 05 06 07
    // a1: 10 11 12 13 14 15 16 17
    // a2: 20 21 22 23 24 25 26 27
    // a3: 30 31 32 33 34 35 36 37
    // a4: 40 41 42 43 44 45 46 47
    // a5: 50 51 52 53 54 55 56 57
    // a6: 60 61 62 63 64 65 66 67
    // a7: 70 71 72 73 74 75 76 77
    // to:
    // b0.val[0]: 00 10 02 12 04 14 06 16
    // b0.val[1]: 01 11 03 13 05 15 07 17
    // b1.val[0]: 20 30 22 32 24 34 26 36
    // b1.val[1]: 21 31 23 33 25 35 27 37
    // b2.val[0]: 40 50 42 52 44 54 46 56
    // b2.val[1]: 41 51 43 53 45 55 47 57
    // b3.val[0]: 60 70 62 72 64 74 66 76
    // b3.val[1]: 61 71 63 73 65 75 67 77

    const int16x8x2_t b0 = vtrnq_s16(*a0, *a1);
    const int16x8x2_t b1 = vtrnq_s16(*a2, *a3);
    const int16x8x2_t b2 = vtrnq_s16(*a4, *a5);
    const int16x8x2_t b3 = vtrnq_s16(*a6, *a7);

    // Swap 32 bit elements resulting in:
    // c0.val[0]: 00 10 20 30 04 14 24 34
    // c0.val[1]: 02 12 22 32 06 16 26 36
    // c1.val[0]: 01 11 21 31 05 15 25 35
    // c1.val[1]: 03 13 23 33 07 17 27 37
    // c2.val[0]: 40 50 60 70 44 54 64 74
    // c2.val[1]: 42 52 62 72 46 56 66 76
    // c3.val[0]: 41 51 61 71 45 55 65 75
    // c3.val[1]: 43 53 63 73 47 57 67 77

    const int32x4x2_t c0 = vtrnq_s32(vreinterpretq_s32_s16(b0.val[0]),
                                     vreinterpretq_s32_s16(b1.val[0]));
    const int32x4x2_t c1 = vtrnq_s32(vreinterpretq_s32_s16(b0.val[1]),
                                     vreinterpretq_s32_s16(b1.val[1]));
    const int32x4x2_t c2 = vtrnq_s32(vreinterpretq_s32_s16(b2.val[0]),
                                     vreinterpretq_s32_s16(b3.val[0]));
    const int32x4x2_t c3 = vtrnq_s32(vreinterpretq_s32_s16(b2.val[1]),
                                     vreinterpretq_s32_s16(b3.val[1]));

    // Swap 64 bit elements resulting in:
    // d0.val[0]: 00 10 20 30 40 50 60 70
    // d0.val[1]: 04 14 24 34 44 54 64 74
    // d1.val[0]: 01 11 21 31 41 51 61 71
    // d1.val[1]: 05 15 25 35 45 55 65 75
    // d2.val[0]: 02 12 22 32 42 52 62 72
    // d2.val[1]: 06 16 26 36 46 56 66 76
    // d3.val[0]: 03 13 23 33 43 53 63 73
    // d3.val[1]: 07 17 27 37 47 57 67 77
    const int16x8x2_t d0 = vpx_vtrnq_s64_to_s16(c0.val[0], c2.val[0]);
    const int16x8x2_t d1 = vpx_vtrnq_s64_to_s16(c1.val[0], c3.val[0]);
    const int16x8x2_t d2 = vpx_vtrnq_s64_to_s16(c0.val[1], c2.val[1]);
    const int16x8x2_t d3 = vpx_vtrnq_s64_to_s16(c1.val[1], c3.val[1]);

    *a0 = d0.val[0];
    *a1 = d1.val[0];
    *a2 = d2.val[0];
    *a3 = d3.val[0];
    *a4 = d0.val[1];
    *a5 = d1.val[1];
    *a6 = d2.val[1];
    *a7 = d3.val[1];
}

// fdct_round_shift((a +/- b) * c)
// Variant that performs fast vqrdmulh_s16 operation on full vector
// can be slightly less accurate, adequate for pass1
static inline void butterfly_one_coeff_s16_fast(const int16x8_t a,
                                                const int16x8_t b,
                                                const tran_coef_t constant,
                                                int16x8_t *add,
                                                int16x8_t *sub) {
    int16x8_t c = vdupq_n_s16(2 * constant);
    *add = vqrdmulhq_s16(vaddq_s16(a, b), c);
    *sub = vqrdmulhq_s16(vsubq_s16(a, b), c);
}

// fdct_round_shift(a * c1 +/- b * c2)
// Original Variant that performs normal implementation on full vector
// more accurate does 32-bit processing, takes and returns 16-bit values
// returns narrowed results
static inline void butterfly_two_coeff(const int16x8_t a, const int16x8_t b,
                                       const tran_coef_t constant1,
                                       const tran_coef_t constant2,
                                       int16x8_t *add, int16x8_t *sub) {
    const int32x4_t a1 = vmull_n_s16(vget_low_s16(a), constant1);
    const int32x4_t a2 = vmull_n_s16(vget_high_s16(a), constant1);
    const int32x4_t a3 = vmull_n_s16(vget_low_s16(a), constant2);
    const int32x4_t a4 = vmull_n_s16(vget_high_s16(a), constant2);
    const int32x4_t sum0 = vmlal_n_s16(a1, vget_low_s16(b), constant2);
    const int32x4_t sum1 = vmlal_n_s16(a2, vget_high_s16(b), constant2);
    const int32x4_t diff0 = vmlsl_n_s16(a3, vget_low_s16(b), constant1);
    const int32x4_t diff1 = vmlsl_n_s16(a4, vget_high_s16(b), constant1);
    const int16x4_t rounded0 = vqrshrn_n_s32(sum0, DCT_CONST_BITS);
    const int16x4_t rounded1 = vqrshrn_n_s32(sum1, DCT_CONST_BITS);
    const int16x4_t rounded2 = vqrshrn_n_s32(diff0, DCT_CONST_BITS);
    const int16x4_t rounded3 = vqrshrn_n_s32(diff1, DCT_CONST_BITS);
    *add = vcombine_s16(rounded0, rounded1);
    *sub = vcombine_s16(rounded2, rounded3);
}

// fdct_round_shift((a +/- b) * c)
// Variant that performs fast vqrdmulhq_s32 operation on full vector
// more accurate does 32-bit processing, takes 16-bit input values,
// returns full 32-bit values, high/low
static inline void butterfly_one_coeff_s16_s32_fast(
    const int16x8_t a, const int16x8_t b, const tran_coef_t constant,
    int32x4_t *add_lo, int32x4_t *add_hi, int32x4_t *sub_lo,
    int32x4_t *sub_hi) {
    int32x4_t c = vdupq_n_s32(constant << 17);
    const int16x4_t a_lo = vget_low_s16(a);
    const int16x4_t a_hi = vget_high_s16(a);
    const int16x4_t b_lo = vget_low_s16(b);
    const int16x4_t b_hi = vget_high_s16(b);
    *add_lo = vqrdmulhq_s32(vaddl_s16(a_lo, b_lo), c);
    *add_hi = vqrdmulhq_s32(vaddl_s16(a_hi, b_hi), c);
    *sub_lo = vqrdmulhq_s32(vsubl_s16(a_lo, b_lo), c);
    *sub_hi = vqrdmulhq_s32(vsubl_s16(a_hi, b_hi), c);
}

// fdct_round_shift((a +/- b) * c)
// Variant that performs fast vqrdmulhq_s32 operation on full vector
// more accurate does 32-bit processing, takes 16-bit input values,
// returns full 32-bit values, high/low
static inline void butterfly_one_coeff_s16_s32_fast_narrow(
    const int16x8_t a, const int16x8_t b, const tran_coef_t constant,
    int16x8_t *add, int16x8_t *sub) {
    int32x4_t add_lo, add_hi, sub_lo, sub_hi;
    butterfly_one_coeff_s16_s32_fast(a, b, constant, &add_lo, &add_hi, &sub_lo, &sub_hi);
    *add = vcombine_s16(vmovn_s32(add_lo), vmovn_s32(add_hi));
    *sub = vcombine_s16(vmovn_s32(sub_lo), vmovn_s32(sub_hi));
}

static inline void vpx_fdct8x8_pass1_notranspose_neon(int16x8_t *in, int16x8_t *out) {
    int16x8_t s[8], x[4], t[2];

    s[0] = vaddq_s16(in[0], in[7]);
    s[1] = vaddq_s16(in[1], in[6]);
    s[2] = vaddq_s16(in[2], in[5]);
    s[3] = vaddq_s16(in[3], in[4]);
    s[4] = vsubq_s16(in[3], in[4]);
    s[5] = vsubq_s16(in[2], in[5]);
    s[6] = vsubq_s16(in[1], in[6]);
    s[7] = vsubq_s16(in[0], in[7]);
    // fdct4(step, step);
    x[0] = vaddq_s16(s[0], s[3]);
    x[1] = vaddq_s16(s[1], s[2]);
    x[2] = vsubq_s16(s[1], s[2]);
    x[3] = vsubq_s16(s[0], s[3]);

    // fdct4(step, step);
    // out[0] = (tran_low_t)fdct_round_shift((x0 + x1) * cospi_16_64)
    // out[4] = (tran_low_t)fdct_round_shift((x0 - x1) * cospi_16_64)
    butterfly_one_coeff_s16_fast(x[0], x[1], cospi_16_64, &out[0], &out[4]);
    // out[2] = (tran_low_t)fdct_round_shift(x2 * cospi_24_64 + x3 * cospi_8_64)
    // out[6] = (tran_low_t)fdct_round_shift(-x2 * cospi_8_64 + x3 * cospi_24_64)
    butterfly_two_coeff(x[3], x[2], cospi_8_64, cospi_24_64, &out[2], &out[6]);

    // Stage 2
    // t0 = (s6 - s5) * cospi_16_64;
    // t1 = (s6 + s5) * cospi_16_64;
    butterfly_one_coeff_s16_fast(s[6], s[5], cospi_16_64, &t[1], &t[0]);

    // Stage 3
    x[0] = vaddq_s16(s[4], t[0]);
    x[1] = vsubq_s16(s[4], t[0]);
    x[2] = vsubq_s16(s[7], t[1]);
    x[3] = vaddq_s16(s[7], t[1]);

    // Stage 4
    // out[1] = (tran_low_t)fdct_round_shift(x0 * cospi_28_64 + x3 * cospi_4_64)
    // out[7] = (tran_low_t)fdct_round_shift(x3 * cospi_28_64 + x0 * -cospi_4_64)
    butterfly_two_coeff(x[3], x[0], cospi_4_64, cospi_28_64, &out[1], &out[7]);

    // out[5] = (tran_low_t)fdct_round_shift(x1 * cospi_12_64 + x2 * cospi_20_64)
    // out[3] = (tran_low_t)fdct_round_shift(x2 * cospi_12_64 + x1 * -cospi_20_64)
    butterfly_two_coeff(x[2], x[1], cospi_20_64, cospi_12_64, &out[5], &out[3]);
}

static inline void vpx_fdct8x8_pass2_notranspose_neon(int16x8_t *in, int16x8_t *out) {
    int16x8_t s[8], x[4], t[2];

    s[0] = vaddq_s16(in[0], in[7]);
    s[1] = vaddq_s16(in[1], in[6]);
    s[2] = vaddq_s16(in[2], in[5]);
    s[3] = vaddq_s16(in[3], in[4]);
    s[4] = vsubq_s16(in[3], in[4]);
    s[5] = vsubq_s16(in[2], in[5]);
    s[6] = vsubq_s16(in[1], in[6]);
    s[7] = vsubq_s16(in[0], in[7]);
    // fdct4(step, step);
    x[0] = vaddq_s16(s[0], s[3]);
    x[1] = vaddq_s16(s[1], s[2]);
    x[2] = vsubq_s16(s[1], s[2]);
    x[3] = vsubq_s16(s[0], s[3]);

    // fdct4(step, step);
    // out[0] = (tran_low_t)fdct_round_shift((x0 + x1) * cospi_16_64)
    // out[4] = (tran_low_t)fdct_round_shift((x0 - x1) * cospi_16_64)
    butterfly_one_coeff_s16_s32_fast_narrow(x[0], x[1], cospi_16_64, &out[0], &out[4]);
    // out[2] = (tran_low_t)fdct_round_shift(x2 * cospi_24_64 + x3 * cospi_8_64)
    // out[6] = (tran_low_t)fdct_round_shift(-x2 * cospi_8_64 + x3 * cospi_24_64)
    butterfly_two_coeff(x[3], x[2], cospi_8_64, cospi_24_64, &out[2], &out[6]);

    // Stage 2
    // t0 = (s6 - s5) * cospi_16_64;
    // t1 = (s6 + s5) * cospi_16_64;
    butterfly_one_coeff_s16_s32_fast_narrow(s[6], s[5], cospi_16_64, &t[1], &t[0]);

    // Stage 3
    x[0] = vaddq_s16(s[4], t[0]);
    x[1] = vsubq_s16(s[4], t[0]);
    x[2] = vsubq_s16(s[7], t[1]);
    x[3] = vaddq_s16(s[7], t[1]);

    // Stage 4
    // out[1] = (tran_low_t)fdct_round_shift(x0 * cospi_28_64 + x3 * cospi_4_64)
    // out[7] = (tran_low_t)fdct_round_shift(x3 * cospi_28_64 + x0 * -cospi_4_64)
    butterfly_two_coeff(x[3], x[0], cospi_4_64, cospi_28_64, &out[1], &out[7]);

    // out[5] = (tran_low_t)fdct_round_shift(x1 * cospi_12_64 + x2 * cospi_20_64)
    // out[3] = (tran_low_t)fdct_round_shift(x2 * cospi_12_64 + x1 * -cospi_20_64)
    butterfly_two_coeff(x[2], x[1], cospi_20_64, cospi_12_64, &out[5], &out[3]);
}

static inline void vpx_fdct8x8_pass1_neon(int16x8_t *in) {
    int16x8_t out[8];
    vpx_fdct8x8_pass1_notranspose_neon(in, out);
    // transpose 8x8
    transpose_s16_8x8(&out[0], &out[1], &out[2], &out[3], &out[4], &out[5], &out[6], &out[7]);
    in[0] = out[0];
    in[1] = out[1];
    in[2] = out[2];
    in[3] = out[3];
    in[4] = out[4];
    in[5] = out[5];
    in[6] = out[6];
    in[7] = out[7];
}

static inline void vpx_fdct8x8_pass2_neon(int16x8_t *in) {
    int16x8_t out[8];
    vpx_fdct8x8_pass2_notranspose_neon(in, out);
    // transpose 8x8
    transpose_s16_8x8(&out[0], &out[1], &out[2], &out[3], &out[4], &out[5], &out[6], &out[7]);
    in[0] = out[0];
    in[1] = out[1];
    in[2] = out[2];
    in[3] = out[3];
    in[4] = out[4];
    in[5] = out[5];
    in[6] = out[6];
    in[7] = out[7];
}

/* The following function is the modified version of vpx_fdct8x8_neon,
 * provided in the libvpx library. Please refer to fdct8x8_neon.c
 * for the unmodified version in the source library. */
void forward_dct_neon(config_t *config,
                      input_t *input,
                      output_t *output) {
    forward_dct_config_t *forward_dct_config = (forward_dct_config_t *)config;
    forward_dct_input_t *forward_dct_input = (forward_dct_input_t *)input;
    forward_dct_output_t *forward_dct_output = (forward_dct_output_t *)output;

    int stride = forward_dct_config->stride;

    for (int block = 0; block < forward_dct_config->block_count; block++) {

        const int16_t *input_buff = forward_dct_input->input_buff[block];
        tran_low_t *output_buff = forward_dct_output->output_buff[block];

        // stage 1
        int16x8_t in[8];
        in[0] = vshlq_n_s16(vld1q_s16(&input_buff[0 * stride]), 2);
        in[1] = vshlq_n_s16(vld1q_s16(&input_buff[1 * stride]), 2);
        in[2] = vshlq_n_s16(vld1q_s16(&input_buff[2 * stride]), 2);
        in[3] = vshlq_n_s16(vld1q_s16(&input_buff[3 * stride]), 2);
        in[4] = vshlq_n_s16(vld1q_s16(&input_buff[4 * stride]), 2);
        in[5] = vshlq_n_s16(vld1q_s16(&input_buff[5 * stride]), 2);
        in[6] = vshlq_n_s16(vld1q_s16(&input_buff[6 * stride]), 2);
        in[7] = vshlq_n_s16(vld1q_s16(&input_buff[7 * stride]), 2);

        vpx_fdct8x8_pass1_neon(in);
        vpx_fdct8x8_pass2_neon(in);
        {
            // from vpx_dct_sse2.c
            // Post-condition (division by two)
            //    division of two 16 bits signed numbers using shifts
            //    n / 2 = (n - (n >> 15)) >> 1
            const int16x8_t sign_in0 = vshrq_n_s16(in[0], 15);
            const int16x8_t sign_in1 = vshrq_n_s16(in[1], 15);
            const int16x8_t sign_in2 = vshrq_n_s16(in[2], 15);
            const int16x8_t sign_in3 = vshrq_n_s16(in[3], 15);
            const int16x8_t sign_in4 = vshrq_n_s16(in[4], 15);
            const int16x8_t sign_in5 = vshrq_n_s16(in[5], 15);
            const int16x8_t sign_in6 = vshrq_n_s16(in[6], 15);
            const int16x8_t sign_in7 = vshrq_n_s16(in[7], 15);
            in[0] = vhsubq_s16(in[0], sign_in0);
            in[1] = vhsubq_s16(in[1], sign_in1);
            in[2] = vhsubq_s16(in[2], sign_in2);
            in[3] = vhsubq_s16(in[3], sign_in3);
            in[4] = vhsubq_s16(in[4], sign_in4);
            in[5] = vhsubq_s16(in[5], sign_in5);
            in[6] = vhsubq_s16(in[6], sign_in6);
            in[7] = vhsubq_s16(in[7], sign_in7);
            // store results
            store_s16q_to_tran_low(output_buff + 0 * 8, in[0]);
            store_s16q_to_tran_low(output_buff + 1 * 8, in[1]);
            store_s16q_to_tran_low(output_buff + 2 * 8, in[2]);
            store_s16q_to_tran_low(output_buff + 3 * 8, in[3]);
            store_s16q_to_tran_low(output_buff + 4 * 8, in[4]);
            store_s16q_to_tran_low(output_buff + 5 * 8, in[5]);
            store_s16q_to_tran_low(output_buff + 6 * 8, in[6]);
            store_s16q_to_tran_low(output_buff + 7 * 8, in[7]);
        }
    }
}