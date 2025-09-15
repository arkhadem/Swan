/*
 *  Copyright (c) 2014 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "neon.hpp"
#include "inverse_dct.hpp"

static inline int16x8_t dct_const_round_shift_low_8(const int32x4_t *const in) {
    return vcombine_s16(vrshrn_n_s32(in[0], DCT_CONST_BITS), vrshrn_n_s32(in[1], DCT_CONST_BITS));
}

static inline void dct_const_round_shift_low_8_dual(const int32x4_t *const t32,
                                                    int16x8_t *const d0,
                                                    int16x8_t *const d1) {
    *d0 = dct_const_round_shift_low_8(t32 + 0);
    *d1 = dct_const_round_shift_low_8(t32 + 2);
}

static inline void idct8x8_64_1d_bd8_kernel(const int16x4_t cospis0,
                                            const int16x4_t cospis1,
                                            int16x8_t *const io) {
    int16x4_t input1l, input1h, input3l, input3h, input5l, input5h, input7l, input7h;
    int16x4_t step1l[4], step1h[4];
    int16x8_t step1[8], step2[8];
    int32x4_t t32[8];

    // stage 1
    input1l = vget_low_s16(io[1]);
    input1h = vget_high_s16(io[1]);
    input3l = vget_low_s16(io[3]);
    input3h = vget_high_s16(io[3]);
    input5l = vget_low_s16(io[5]);
    input5h = vget_high_s16(io[5]);
    input7l = vget_low_s16(io[7]);
    input7h = vget_high_s16(io[7]);
    step1l[0] = vget_low_s16(io[0]);
    step1h[0] = vget_high_s16(io[0]);
    step1l[1] = vget_low_s16(io[2]);
    step1h[1] = vget_high_s16(io[2]);
    step1l[2] = vget_low_s16(io[4]);
    step1h[2] = vget_high_s16(io[4]);
    step1l[3] = vget_low_s16(io[6]);
    step1h[3] = vget_high_s16(io[6]);

    t32[0] = vmull_lane_s16(input1l, cospis1, 3);
    t32[1] = vmull_lane_s16(input1h, cospis1, 3);
    t32[2] = vmull_lane_s16(input3l, cospis1, 2);
    t32[3] = vmull_lane_s16(input3h, cospis1, 2);
    t32[4] = vmull_lane_s16(input3l, cospis1, 1);
    t32[5] = vmull_lane_s16(input3h, cospis1, 1);
    t32[6] = vmull_lane_s16(input1l, cospis1, 0);
    t32[7] = vmull_lane_s16(input1h, cospis1, 0);
    t32[0] = vmlsl_lane_s16(t32[0], input7l, cospis1, 0);
    t32[1] = vmlsl_lane_s16(t32[1], input7h, cospis1, 0);
    t32[2] = vmlal_lane_s16(t32[2], input5l, cospis1, 1);
    t32[3] = vmlal_lane_s16(t32[3], input5h, cospis1, 1);
    t32[4] = vmlsl_lane_s16(t32[4], input5l, cospis1, 2);
    t32[5] = vmlsl_lane_s16(t32[5], input5h, cospis1, 2);
    t32[6] = vmlal_lane_s16(t32[6], input7l, cospis1, 3);
    t32[7] = vmlal_lane_s16(t32[7], input7h, cospis1, 3);
    dct_const_round_shift_low_8_dual(&t32[0], &step1[4], &step1[5]);
    dct_const_round_shift_low_8_dual(&t32[4], &step1[6], &step1[7]);

    // stage 2
    t32[2] = vmull_lane_s16(step1l[0], cospis0, 2);
    t32[3] = vmull_lane_s16(step1h[0], cospis0, 2);
    t32[4] = vmull_lane_s16(step1l[1], cospis0, 3);
    t32[5] = vmull_lane_s16(step1h[1], cospis0, 3);
    t32[6] = vmull_lane_s16(step1l[1], cospis0, 1);
    t32[7] = vmull_lane_s16(step1h[1], cospis0, 1);
    t32[0] = vmlal_lane_s16(t32[2], step1l[2], cospis0, 2);
    t32[1] = vmlal_lane_s16(t32[3], step1h[2], cospis0, 2);
    t32[2] = vmlsl_lane_s16(t32[2], step1l[2], cospis0, 2);
    t32[3] = vmlsl_lane_s16(t32[3], step1h[2], cospis0, 2);
    t32[4] = vmlsl_lane_s16(t32[4], step1l[3], cospis0, 1);
    t32[5] = vmlsl_lane_s16(t32[5], step1h[3], cospis0, 1);
    t32[6] = vmlal_lane_s16(t32[6], step1l[3], cospis0, 3);
    t32[7] = vmlal_lane_s16(t32[7], step1h[3], cospis0, 3);
    dct_const_round_shift_low_8_dual(&t32[0], &step2[0], &step2[1]);
    dct_const_round_shift_low_8_dual(&t32[4], &step2[2], &step2[3]);

    step2[4] = vaddq_s16(step1[4], step1[5]);
    step2[5] = vsubq_s16(step1[4], step1[5]);
    step2[6] = vsubq_s16(step1[7], step1[6]);
    step2[7] = vaddq_s16(step1[7], step1[6]);

    // stage 3
    step1[0] = vaddq_s16(step2[0], step2[3]);
    step1[1] = vaddq_s16(step2[1], step2[2]);
    step1[2] = vsubq_s16(step2[1], step2[2]);
    step1[3] = vsubq_s16(step2[0], step2[3]);

    t32[2] = vmull_lane_s16(vget_low_s16(step2[6]), cospis0, 2);
    t32[3] = vmull_lane_s16(vget_high_s16(step2[6]), cospis0, 2);
    t32[0] = vmlsl_lane_s16(t32[2], vget_low_s16(step2[5]), cospis0, 2);
    t32[1] = vmlsl_lane_s16(t32[3], vget_high_s16(step2[5]), cospis0, 2);
    t32[2] = vmlal_lane_s16(t32[2], vget_low_s16(step2[5]), cospis0, 2);
    t32[3] = vmlal_lane_s16(t32[3], vget_high_s16(step2[5]), cospis0, 2);
    dct_const_round_shift_low_8_dual(t32, &step1[5], &step1[6]);

    // stage 4
    io[0] = vaddq_s16(step1[0], step2[7]);
    io[1] = vaddq_s16(step1[1], step1[6]);
    io[2] = vaddq_s16(step1[2], step1[5]);
    io[3] = vaddq_s16(step1[3], step2[4]);
    io[4] = vsubq_s16(step1[3], step2[4]);
    io[5] = vsubq_s16(step1[2], step1[5]);
    io[6] = vsubq_s16(step1[1], step1[6]);
    io[7] = vsubq_s16(step1[0], step2[7]);
}

// Transpose 64 bit elements as follows:
// a0: 00 01 02 03 04 05 06 07
// a1: 16 17 18 19 20 21 22 23
//
// b0.val[0]: 00 01 02 03 16 17 18 19
// b0.val[1]: 04 05 06 07 20 21 22 23
static inline int16x8x2_t vpx_vtrnq_s64_to_s16(int32x4_t a0, int32x4_t a1) {
    int16x8x2_t b0;
#if defined(NEON2RVV)
    #define GET(v, idx) __riscv_vget_v_i16m1x2_i16m1(v, idx)
    #define SET(v, idx, val) v = __riscv_vset_v_i16m1_i16m1x2(v, idx, val)
    SET(b0, 0, vcombine_s16(vreinterpret_s16_s32(vget_low_s32(a0)),
                             vreinterpret_s16_s32(vget_low_s32(a1))));
    SET(b0, 1, vcombine_s16(vreinterpret_s16_s32(vget_high_s32(a0)),
                             vreinterpret_s16_s32(vget_high_s32(a1))));
#else
    b0.val[0] = vcombine_s16(vreinterpret_s16_s32(vget_low_s32(a0)),
                             vreinterpret_s16_s32(vget_low_s32(a1)));
    b0.val[1] = vcombine_s16(vreinterpret_s16_s32(vget_high_s32(a0)),
                             vreinterpret_s16_s32(vget_high_s32(a1)));
#endif
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

#if defined(NEON2RVV)
    const int32x4x2_t c0 = vtrnq_s32(vreinterpretq_s32_s16(GET(b0, 0)),
                                     vreinterpretq_s32_s16(GET(b1, 0)));
    const int32x4x2_t c1 = vtrnq_s32(vreinterpretq_s32_s16(GET(b0, 1)),
                                     vreinterpretq_s32_s16(GET(b1, 1)));
    const int32x4x2_t c2 = vtrnq_s32(vreinterpretq_s32_s16(GET(b2, 0)),
                                     vreinterpretq_s32_s16(GET(b3, 0)));
    const int32x4x2_t c3 = vtrnq_s32(vreinterpretq_s32_s16(GET(b2, 1)),
                                     vreinterpretq_s32_s16(GET(b3, 1)));
    #undef GET
    #undef SET
#else
    const int32x4x2_t c0 = vtrnq_s32(vreinterpretq_s32_s16(b0.val[0]),
                                     vreinterpretq_s32_s16(b1.val[0]));
    const int32x4x2_t c1 = vtrnq_s32(vreinterpretq_s32_s16(b0.val[1]),
                                     vreinterpretq_s32_s16(b1.val[1]));
    const int32x4x2_t c2 = vtrnq_s32(vreinterpretq_s32_s16(b2.val[0]),
                                     vreinterpretq_s32_s16(b3.val[0]));
    const int32x4x2_t c3 = vtrnq_s32(vreinterpretq_s32_s16(b2.val[1]),
                                     vreinterpretq_s32_s16(b3.val[1]));
#endif

    // Swap 64 bit elements resulting in:
    // d0.val[0]: 00 10 20 30 40 50 60 70
    // d0.val[1]: 04 14 24 34 44 54 64 74
    // d1.val[0]: 01 11 21 31 41 51 61 71
    // d1.val[1]: 05 15 25 35 45 55 65 75
    // d2.val[0]: 02 12 22 32 42 52 62 72
    // d2.val[1]: 06 16 26 36 46 56 66 76
    // d3.val[0]: 03 13 23 33 43 53 63 73
    // d3.val[1]: 07 17 27 37 47 57 67 77
#if defined(NEON2RVV)
    #define GET(v, idx) __riscv_vget_v_i32m1x2_i32m1(v, idx)
    const int16x8x2_t d0 = vpx_vtrnq_s64_to_s16(GET(c0, 0), GET(c2, 0));
    const int16x8x2_t d1 = vpx_vtrnq_s64_to_s16(GET(c1, 0), GET(c3, 0));
    const int16x8x2_t d2 = vpx_vtrnq_s64_to_s16(GET(c0, 1), GET(c2, 1));
    const int16x8x2_t d3 = vpx_vtrnq_s64_to_s16(GET(c1, 1), GET(c3, 1));
    #undef GET

    #define GET(v, idx) __riscv_vget_v_i16m1x2_i16m1(v, idx)
    *a0 = GET(d0, 0);
    *a1 = GET(d1, 0);
    *a2 = GET(d2, 0);
    *a3 = GET(d3, 0);
    *a4 = GET(d0, 1);
    *a5 = GET(d1, 1);
    *a6 = GET(d2, 1);
    *a7 = GET(d3, 1);
    #undef GET
#else
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
#endif
}

static inline void idct8x8_64_1d_bd8(const int16x4_t cospis0,
                                     const int16x4_t cospis1,
                                     int16x8_t *const io) {
    transpose_s16_8x8(&io[0], &io[1], &io[2], &io[3], &io[4], &io[5], &io[6], &io[7]);
    idct8x8_64_1d_bd8_kernel(cospis0, cospis1, io);
}

static inline int16x8_t load_tran_low_to_s16q(const tran_low_t *buf) {
    return vld1q_s16(buf);
}

static const int16_t kCospi[16] = {
    16384 /*  cospi_0_64  */, 15137 /*  cospi_8_64  */,
    11585 /*  cospi_16_64 */, 6270 /*  cospi_24_64 */,
    16069 /*  cospi_4_64  */, 13623 /*  cospi_12_64 */,
    -9102 /* -cospi_20_64 */, 3196 /*  cospi_28_64 */,
    16305 /*  cospi_2_64  */, 1606 /*  cospi_30_64 */,
    14449 /*  cospi_10_64 */, 7723 /*  cospi_22_64 */,
    15679 /*  cospi_6_64  */, -4756 /* -cospi_26_64 */,
    12665 /*  cospi_14_64 */, -10394 /* -cospi_18_64 */
};

static inline void idct8x8_add8x1(const int16x8_t a, uint8_t **const dest, const int stride) {
    const uint8x8_t s = vld1_u8(*dest);
    const int16x8_t res = vrshrq_n_s16(a, 5);
    const uint16x8_t q = vaddw_u8(vreinterpretq_u16_s16(res), s);
    const uint8x8_t d = vqmovun_s16(vreinterpretq_s16_u16(q));
    vst1_u8(*dest, d);
    *dest += stride;
}

static inline void idct8x8_add8x8_neon(int16x8_t *const out, uint8_t *dest,
                                       const int stride) {
    idct8x8_add8x1(out[0], &dest, stride);
    idct8x8_add8x1(out[1], &dest, stride);
    idct8x8_add8x1(out[2], &dest, stride);
    idct8x8_add8x1(out[3], &dest, stride);
    idct8x8_add8x1(out[4], &dest, stride);
    idct8x8_add8x1(out[5], &dest, stride);
    idct8x8_add8x1(out[6], &dest, stride);
    idct8x8_add8x1(out[7], &dest, stride);
}

/* The following function is the modified version of vpx_idct8x8_64_add_neon,
 * provided in the libvpx library. Please refer to idct8x8_add_neon.c
 * for the unmodified version in the source library. */
void inverse_dct_neon(config_t *config,
                      input_t *input,
                      output_t *output) {
    inverse_dct_config_t *inverse_dct_config = (inverse_dct_config_t *)config;
    inverse_dct_input_t *inverse_dct_input = (inverse_dct_input_t *)input;
    inverse_dct_output_t *inverse_dct_output = (inverse_dct_output_t *)output;

    int stride = inverse_dct_config->stride;

    for (int block = 0; block < inverse_dct_config->block_count; block++) {

        const tran_low_t *input_buff = inverse_dct_input->input_buff[block];
        uint8_t *output_buff = inverse_dct_output->output_buff[block];

        const int16x8_t cospis = vld1q_s16(kCospi);
        const int16x4_t cospis0 = vget_low_s16(cospis);  // cospi 0, 8, 16, 24
        const int16x4_t cospis1 = vget_high_s16(cospis); // cospi 4, 12, 20, 28
        int16x8_t a[8];

        a[0] = load_tran_low_to_s16q(input_buff);
        a[1] = load_tran_low_to_s16q(input_buff + 8);
        a[2] = load_tran_low_to_s16q(input_buff + 16);
        a[3] = load_tran_low_to_s16q(input_buff + 24);
        a[4] = load_tran_low_to_s16q(input_buff + 32);
        a[5] = load_tran_low_to_s16q(input_buff + 40);
        a[6] = load_tran_low_to_s16q(input_buff + 48);
        a[7] = load_tran_low_to_s16q(input_buff + 56);

        idct8x8_64_1d_bd8(cospis0, cospis1, a);
        idct8x8_64_1d_bd8(cospis0, cospis1, a);
        idct8x8_add8x8_neon(a, output_buff, stride);
    }
}