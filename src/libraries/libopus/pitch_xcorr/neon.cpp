/* Copyright (c) 2014-2015 Xiph.Org Foundation
   Written by Viswanath Puttagunta */

/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "neon.hpp"
#include "pitch_xcorr.hpp"
#include <stdio.h>
#ifdef SWAN_SIMULATION
#include <fake_neon.h>
#else
#include <arm_neon.h>
#endif

#ifndef SWAN_SIMULATION
opus_val32 celt_inner_prod_neon(const opus_val16 *x, const opus_val16 *y, int N) {
    int i;
    opus_val32 xy;
    float32x4_t xy_f32x4 = vdupq_n_f32(0);
    float32x2_t xy_f32x2;

    for (i = 0; i < N - 7; i += 8) {
        float32x4_t x_f32x4, y_f32x4;
        x_f32x4 = vld1q_f32(&x[i]);
        y_f32x4 = vld1q_f32(&y[i]);
        xy_f32x4 = vmlaq_f32(xy_f32x4, x_f32x4, y_f32x4);
        x_f32x4 = vld1q_f32(&x[i + 4]);
        y_f32x4 = vld1q_f32(&y[i + 4]);
        xy_f32x4 = vmlaq_f32(xy_f32x4, x_f32x4, y_f32x4);
    }

    if (N - i >= 4) {
        const float32x4_t x_f32x4 = vld1q_f32(&x[i]);
        const float32x4_t y_f32x4 = vld1q_f32(&y[i]);
        xy_f32x4 = vmlaq_f32(xy_f32x4, x_f32x4, y_f32x4);
        i += 4;
    }

    xy_f32x2 = vadd_f32(vget_low_f32(xy_f32x4), vget_high_f32(xy_f32x4));
    xy_f32x2 = vpadd_f32(xy_f32x2, xy_f32x2);
    xy = vget_lane_f32(xy_f32x2, 0);

    for (; i < N; i++) {
        xy = MAC16_16(xy, x[i], y[i]);
    }

    return xy;
}

static void xcorr_kernel_neon_float(const float32_t *x, const float32_t *y, float32_t sum[4], int len) {

    float32x4_t YY[3];
    float32x4_t YEXT[3];
    float32x4_t XX[2];
    float32x2_t XX_2;
    float32x4_t SUMM;
    const float32_t *xi = x;
    const float32_t *yi = y;

    YY[0] = vld1q_f32(yi);
    SUMM = vdupq_n_f32(0);

    /* Consume 8 elements in x vector and 12 elements in y
    * vector. However, the 12'th element never really gets
    * touched in this loop. So, if len == 8, then we only
    * must access y[0] to y[10]. y[11] must not be accessed
    * hence make sure len > 8 and not len >= 8
    */
    while (len > 8) {
        yi += 4;
        YY[1] = vld1q_f32(yi);
        yi += 4;
        YY[2] = vld1q_f32(yi);

        XX[0] = vld1q_f32(xi);
        xi += 4;
        XX[1] = vld1q_f32(xi);
        xi += 4;

        SUMM = vmlaq_lane_f32(SUMM, YY[0], vget_low_f32(XX[0]), 0);
        YEXT[0] = vextq_f32(YY[0], YY[1], 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[0], vget_low_f32(XX[0]), 1);
        YEXT[1] = vextq_f32(YY[0], YY[1], 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[1], vget_high_f32(XX[0]), 0);
        YEXT[2] = vextq_f32(YY[0], YY[1], 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[2], vget_high_f32(XX[0]), 1);

        SUMM = vmlaq_lane_f32(SUMM, YY[1], vget_low_f32(XX[1]), 0);
        YEXT[0] = vextq_f32(YY[1], YY[2], 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[0], vget_low_f32(XX[1]), 1);
        YEXT[1] = vextq_f32(YY[1], YY[2], 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[1], vget_high_f32(XX[1]), 0);
        YEXT[2] = vextq_f32(YY[1], YY[2], 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[2], vget_high_f32(XX[1]), 1);

        YY[0] = YY[2];
        len -= 8;
    }

    /* Consume 4 elements in x vector and 8 elements in y
    * vector. However, the 8'th element in y never really gets
    * touched in this loop. So, if len == 4, then we only
    * must access y[0] to y[6]. y[7] must not be accessed
    * hence make sure len>4 and not len>=4
    */
    if (len > 4) {
        yi += 4;
        YY[1] = vld1q_f32(yi);

        XX[0] = vld1q_f32(xi);
        xi += 4;

        SUMM = vmlaq_lane_f32(SUMM, YY[0], vget_low_f32(XX[0]), 0);
        YEXT[0] = vextq_f32(YY[0], YY[1], 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[0], vget_low_f32(XX[0]), 1);
        YEXT[1] = vextq_f32(YY[0], YY[1], 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[1], vget_high_f32(XX[0]), 0);
        YEXT[2] = vextq_f32(YY[0], YY[1], 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[2], vget_high_f32(XX[0]), 1);

        YY[0] = YY[1];
        len -= 4;
    }

    while (--len > 0) {
        XX_2 = vld1_dup_f32(xi++);
        SUMM = vmlaq_lane_f32(SUMM, YY[0], XX_2, 0);
        YY[0] = vld1q_f32(++yi);
    }

    XX_2 = vld1_dup_f32(xi);
    SUMM = vmlaq_lane_f32(SUMM, YY[0], XX_2, 0);

    vst1q_f32(sum, SUMM);
}

/* The following function is the modified version of celt_pitch_xcorr_float_neon,
 * provided in the libopus library. Please refer to celt_neon_intr.c
 * for the unmodified version in the source library. */
void pitch_xcorr_neon(config_t *config,
                      input_t *input,
                      output_t *output) {

    pitch_xcorr_config_t *pitch_xcorr_config = (pitch_xcorr_config_t *)config;
    pitch_xcorr_input_t *pitch_xcorr_input = (pitch_xcorr_input_t *)input;
    pitch_xcorr_output_t *pitch_xcorr_output = (pitch_xcorr_output_t *)output;

    int len = pitch_xcorr_config->len;             // fastN = n (1024) - lag (24)
    int max_pitch = pitch_xcorr_config->max_pitch; // lag (24) + 1
    uint32_t block_count = pitch_xcorr_config->block_count;

    for (unsigned block = 0; block < block_count; ++block) {
        opus_val16 *x = pitch_xcorr_input->x[block];
        opus_val16 *y = pitch_xcorr_input->y[block];
        opus_val32 *xcorr = pitch_xcorr_output->xcorr[block]; // ac[24 + 1]

        int i;

        for (i = 0; i < (max_pitch - 3); i += 4) {
            xcorr_kernel_neon_float((const float32_t *)x, (const float32_t *)y + i, (float32_t *)xcorr + i, len);
        }

        /* In case max_pitch isn't a multiple of 4, do non-unrolled version. */
        for (; i < max_pitch; i++) {
            xcorr[i] = celt_inner_prod_neon(x, y + i, len);
        }
    }
}
#else
opus_val32 sum1(const opus_val16 *x, const opus_val16 *y, int N) {
    int i = 0;
    opus_val32 xy;

#if SWAN_REG_TYPE >= SWAN_REG_1024_TYPE
    float32x32_t xy_f32x32 = vdupoq_n_f32(0);
    for (; i + 64 <= N; i += 64) {
        float32x32_t x_f32x32 = vld1oq_f32(&x[i]);
        float32x32_t y_f32x32 = vld1oq_f32(&y[i]);
        xy_f32x32 = vmlaoq_f32(xy_f32x32, x_f32x32, y_f32x32);
        x_f32x32 = vld1oq_f32(&x[i + 32]);
        y_f32x32 = vld1oq_f32(&y[i + 32]);
        xy_f32x32 = vmlaoq_f32(xy_f32x32, x_f32x32, y_f32x32);
    }
    for (; i + 32 <= N; i += 32) {
        float32x32_t x_f32x32 = vld1oq_f32(&x[i]);
        float32x32_t y_f32x32 = vld1oq_f32(&y[i]);
        xy_f32x32 = vmlaoq_f32(xy_f32x32, x_f32x32, y_f32x32);
    }
#endif

#if SWAN_REG_TYPE == SWAN_REG_512_TYPE
    float32x16_t xy_f32x16 = vdupqq_n_f32(0);
    for (; i + 32 <= N; i += 32) {
        float32x16_t x_f32x16 = vld1qq_f32(&x[i]);
        float32x16_t y_f32x16 = vld1qq_f32(&y[i]);
        xy_f32x16 = vmlaqq_f32(xy_f32x16, x_f32x16, y_f32x16);
        x_f32x16 = vld1qq_f32(&x[i + 16]);
        y_f32x16 = vld1qq_f32(&y[i + 16]);
        xy_f32x16 = vmlaqq_f32(xy_f32x16, x_f32x16, y_f32x16);
    }
#elif SWAN_REG_TYPE > SWAN_REG_512_TYPE
    float32x16_t xy_f32x16 = vaddqq_f32(vgetoq_low_f32(xy_f32x32), vgetoq_high_f32(xy_f32x32));
#endif
#if SWAN_REG_TYPE >= SWAN_REG_512_TYPE
    for (; i + 16 <= N; i += 16) {
        float32x16_t x_f32x16 = vld1qq_f32(&x[i]);
        float32x16_t y_f32x16 = vld1qq_f32(&y[i]);
        xy_f32x16 = vmlaqq_f32(xy_f32x16, x_f32x16, y_f32x16);
    }
#endif

#if SWAN_REG_TYPE == SWAN_REG_256_TYPE
    float32x8_t xy_f32x8 = vdupdq_n_f32(0);
    for (; i + 16 <= N; i += 16) {
        float32x8_t x_f32x8 = vld1dq_f32(&x[i]);
        float32x8_t y_f32x8 = vld1dq_f32(&y[i]);
        xy_f32x8 = vmladq_f32(xy_f32x8, x_f32x8, y_f32x8);
        x_f32x8 = vld1dq_f32(&x[i + 8]);
        y_f32x8 = vld1dq_f32(&y[i + 8]);
        xy_f32x8 = vmladq_f32(xy_f32x8, x_f32x8, y_f32x8);
    }
#elif SWAN_REG_TYPE > SWAN_REG_256_TYPE
    float32x8_t xy_f32x8 = vadddq_f32(vgetqq_low_f32(xy_f32x16), vgetqq_high_f32(xy_f32x16));
#endif
#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
    for (; i + 8 <= N; i += 8) {
        float32x8_t x_f32x8 = vld1dq_f32(&x[i]);
        float32x8_t y_f32x8 = vld1dq_f32(&y[i]);
        xy_f32x8 = vmladq_f32(xy_f32x8, x_f32x8, y_f32x8);
    }
#endif

#if SWAN_REG_TYPE == SWAN_REG_128_TYPE
    float32x4_t xy_f32x4 = vdupq_n_f32(0);
    for (; i + 8 <= N; i += 8) {
        float32x4_t x_f32x4, y_f32x4;
        x_f32x4 = vld1q_f32(&x[i]);
        y_f32x4 = vld1q_f32(&y[i]);
        xy_f32x4 = vmlaq_f32(xy_f32x4, x_f32x4, y_f32x4);
        x_f32x4 = vld1q_f32(&x[i + 4]);
        y_f32x4 = vld1q_f32(&y[i + 4]);
        xy_f32x4 = vmlaq_f32(xy_f32x4, x_f32x4, y_f32x4);
    }
#else
    float32x4_t xy_f32x4 = vaddq_f32(vgetdq_low_f32(xy_f32x8), vgetdq_high_f32(xy_f32x8));
#endif
    for (; i + 4 <= N; i += 4) {
        const float32x4_t x_f32x4 = vld1q_f32(&x[i]);
        const float32x4_t y_f32x4 = vld1q_f32(&y[i]);
        xy_f32x4 = vmlaq_f32(xy_f32x4, x_f32x4, y_f32x4);
        i += 4;
    }

    float32x2_t xy_f32x2 = vadd_f32(vget_low_f32(xy_f32x4), vget_high_f32(xy_f32x4));
    xy_f32x2 = vpadd_f32(xy_f32x2, xy_f32x2);
    xy = vget_lane_f32(xy_f32x2, 0);

    for (; i + 1 <= N; i++) {
        xy = MAC16_16(xy, x[i], y[i]);
    }

    return xy;
}

static void sum4(const float *x, const float *y, float sum[4], int len) {
    float32x4_t YY0, YY1, YY2;
    float32x4_t YEXT;
    float32x4_t XX0, XX1;
    float32x2_t XX_2;
    float32x4_t SUMM;
    const float *xi = x;
    const float *yi = y;

    YY0 = vld1q_f32(yi);
    SUMM = vdupq_n_f32(0);

    /* Consume 8 elements in x vector and 12 elements in y
    * vector. However, the 12'th element never really gets
    * touched in this loop. So, if len == 8, then we only
    * must access y[0] to y[10]. y[11] must not be accessed
    * hence make sure len > 8 and not len >= 8
    */

    while (len > 8) {
        yi += 4;
        YY1 = vld1q_f32(yi);
        yi += 4;
        YY2 = vld1q_f32(yi);

        XX0 = vld1q_f32(xi);
        xi += 4;
        XX1 = vld1q_f32(xi);
        xi += 4;

        SUMM = vmlaq_lane_f32(SUMM, YY0, vget_low_f32(XX0), 0);
        YEXT = vextq_f32(YY0, YY1, 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_low_f32(XX0), 1);
        YEXT = vextq_f32(YY0, YY1, 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 0);
        YEXT = vextq_f32(YY0, YY1, 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 1);

        SUMM = vmlaq_lane_f32(SUMM, YY1, vget_low_f32(XX1), 0);
        YEXT = vextq_f32(YY1, YY2, 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 1);
        YEXT = vextq_f32(YY1, YY2, 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 0);
        YEXT = vextq_f32(YY1, YY2, 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 1);

        YY0 = YY2;
        len -= 8;
    }

    /* Consume 4 elements in x vector and 8 elements in y
    * vector. However, the 8'th element in y never really gets
    * touched in this loop. So, if len == 4, then we only
    * must access y[0] to y[6]. y[7] must not be accessed
    * hence make sure len>4 and not len>=4
    */
    if (len > 4) {
        yi += 4;
        YY1 = vld1q_f32(yi);

        XX0 = vld1q_f32(xi);
        xi += 4;

        SUMM = vmlaq_lane_f32(SUMM, YY0, vget_low_f32(XX0), 0);
        YEXT = vextq_f32(YY0, YY1, 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_low_f32(XX0), 1);
        YEXT = vextq_f32(YY0, YY1, 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 0);
        YEXT = vextq_f32(YY0, YY1, 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 1);

        YY0 = YY1;
        len -= 4;
    }

    while (--len > 0) {
        XX_2 = vld1_dup_f32(xi++);
        SUMM = vmlaq_lane_f32(SUMM, YY0, XX_2, 0);
        YY0 = vld1q_f32(++yi);
    }

    XX_2 = vld1_dup_f32(xi);
    SUMM = vmlaq_lane_f32(SUMM, YY0, XX_2, 0);
    vst1q_f32(sum, SUMM);
}

#if SWAN_REG_TYPE >= 256
static void sum8(const float *x, const float *y, float sum[8], int len) {
    float32x8_t YY0, YY1, YY2;
    float32x8_t YEXT;
    float32x8_t SUMM;
    const float *xi = x;
    const float *yi = y;

    YY0 = vld1dq_f32(yi);
    SUMM = vdupdq_n_f32(0);

    if (len > 16) {
        yi += 8;
        YY1 = vld1dq_f32(yi);
        yi += 8;
        YY2 = vld1dq_f32(yi);

        float32x4_t XX0 = vld1q_f32(xi + 0);
        float32x4_t XX1 = vld1q_f32(xi + 4);
        float32x4_t XX2 = vld1q_f32(xi + 8);
        float32x4_t XX3 = vld1q_f32(xi + 12);
        xi += 16;

        SUMM = vmladq_lane_f32(SUMM, YY0, vget_low_f32(XX0), 0);
        YEXT = vextdq_f32(YY0, YY1, 1);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX0), 1);
        YEXT = vextdq_f32(YY0, YY1, 2);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 0);
        YEXT = vextdq_f32(YY0, YY1, 3);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 1);
        YEXT = vextdq_f32(YY0, YY1, 4);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 0);
        YEXT = vextdq_f32(YY0, YY1, 5);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 1);
        YEXT = vextdq_f32(YY0, YY1, 6);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 0);
        YEXT = vextdq_f32(YY0, YY1, 7);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 1);

        SUMM = vmladq_lane_f32(SUMM, YY1, vget_low_f32(XX2), 0);
        YEXT = vextdq_f32(YY1, YY2, 1);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX2), 1);
        YEXT = vextdq_f32(YY1, YY2, 2);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX2), 0);
        YEXT = vextdq_f32(YY1, YY2, 3);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX2), 1);
        YEXT = vextdq_f32(YY1, YY2, 4);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX3), 0);
        YEXT = vextdq_f32(YY1, YY2, 5);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX3), 1);
        YEXT = vextdq_f32(YY1, YY2, 6);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX3), 0);
        YEXT = vextdq_f32(YY1, YY2, 7);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX3), 1);

        YY0 = YY2;
        len -= 16;
    }

    if (len > 8) {
        yi += 8;
        YY1 = vld1dq_f32(yi);

        float32x4_t XX0 = vld1q_f32(xi);
        float32x4_t XX1 = vld1q_f32(xi + 4);
        xi += 8;

        SUMM = vmladq_lane_f32(SUMM, YY0, vget_low_f32(XX0), 0);
        YEXT = vextdq_f32(YY0, YY1, 1);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX0), 1);
        YEXT = vextdq_f32(YY0, YY1, 2);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 0);
        YEXT = vextdq_f32(YY0, YY1, 3);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 1);
        YEXT = vextdq_f32(YY0, YY1, 4);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 0);
        YEXT = vextdq_f32(YY0, YY1, 5);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 1);
        YEXT = vextdq_f32(YY0, YY1, 6);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 0);
        YEXT = vextdq_f32(YY0, YY1, 7);
        SUMM = vmladq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 1);

        YY0 = YY1;
        len -= 8;
    }

    while (--len > 0) {
        float32x8_t XX = vld1dq_dup_f32(xi++);
        SUMM = vmladq_f32(SUMM, YY0, XX);
        YY0 = vld1dq_f32(++yi);
    }

    float32x8_t XX = vld1dq_dup_f32(xi++);
    SUMM = vmladq_f32(SUMM, YY0, XX);
    vst1dq_f32(sum, SUMM);
}
#endif

#if SWAN_REG_TYPE >= 512
static void sum16(const float *x, const float *y, float sum[16], int len) {
    float32x16_t YY0, YY1, YY2;
    float32x16_t YEXT;
    float32x16_t SUMM;
    const float *xi = x;
    const float *yi = y;

    YY0 = vld1qq_f32(yi);
    SUMM = vdupqq_n_f32(0);

    if (len > 32) {
        yi += 16;
        YY1 = vld1qq_f32(yi);
        yi += 16;
        YY2 = vld1qq_f32(yi);

        float32x4_t XX0 = vld1q_f32(xi + 0);
        float32x4_t XX1 = vld1q_f32(xi + 4);
        float32x4_t XX2 = vld1q_f32(xi + 8);
        float32x4_t XX3 = vld1q_f32(xi + 12);
        float32x4_t XX4 = vld1q_f32(xi + 16);
        float32x4_t XX5 = vld1q_f32(xi + 20);
        float32x4_t XX6 = vld1q_f32(xi + 24);
        float32x4_t XX7 = vld1q_f32(xi + 28);
        xi += 32;

        SUMM = vmlaqq_lane_f32(SUMM, YY0, vget_low_f32(XX0), 0);
        YEXT = vextqq_f32(YY0, YY1, 1);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX0), 1);
        YEXT = vextqq_f32(YY0, YY1, 2);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 0);
        YEXT = vextqq_f32(YY0, YY1, 3);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 1);
        YEXT = vextqq_f32(YY0, YY1, 4);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 0);
        YEXT = vextqq_f32(YY0, YY1, 5);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 1);
        YEXT = vextqq_f32(YY0, YY1, 6);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 0);
        YEXT = vextqq_f32(YY0, YY1, 7);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 1);
        YEXT = vextqq_f32(YY0, YY1, 8);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX2), 0);
        YEXT = vextqq_f32(YY0, YY1, 9);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX2), 1);
        YEXT = vextqq_f32(YY0, YY1, 10);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX2), 0);
        YEXT = vextqq_f32(YY0, YY1, 11);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX2), 1);
        YEXT = vextqq_f32(YY0, YY1, 12);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX3), 0);
        YEXT = vextqq_f32(YY0, YY1, 13);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX3), 1);
        YEXT = vextqq_f32(YY0, YY1, 14);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX3), 0);
        YEXT = vextqq_f32(YY0, YY1, 15);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX3), 1);

        SUMM = vmlaqq_lane_f32(SUMM, YY1, vget_low_f32(XX4), 0);
        YEXT = vextqq_f32(YY1, YY2, 1);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX4), 1);
        YEXT = vextqq_f32(YY1, YY2, 2);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX4), 0);
        YEXT = vextqq_f32(YY1, YY2, 3);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX4), 1);
        YEXT = vextqq_f32(YY1, YY2, 4);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX5), 0);
        YEXT = vextqq_f32(YY1, YY2, 5);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX5), 1);
        YEXT = vextqq_f32(YY1, YY2, 6);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX5), 0);
        YEXT = vextqq_f32(YY1, YY2, 7);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX5), 1);
        YEXT = vextqq_f32(YY1, YY2, 8);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX6), 0);
        YEXT = vextqq_f32(YY1, YY2, 9);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX6), 1);
        YEXT = vextqq_f32(YY1, YY2, 10);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX6), 0);
        YEXT = vextqq_f32(YY1, YY2, 11);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX6), 1);
        YEXT = vextqq_f32(YY1, YY2, 12);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX7), 0);
        YEXT = vextqq_f32(YY1, YY2, 13);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX7), 1);
        YEXT = vextqq_f32(YY1, YY2, 14);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX7), 0);
        YEXT = vextqq_f32(YY1, YY2, 15);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX7), 1);

        YY0 = YY2;
        len -= 32;
    }

    if (len > 16) {
        yi += 16;
        YY1 = vld1qq_f32(yi);

        float32x4_t XX0 = vld1q_f32(xi + 0);
        float32x4_t XX1 = vld1q_f32(xi + 4);
        float32x4_t XX2 = vld1q_f32(xi + 8);
        float32x4_t XX3 = vld1q_f32(xi + 12);
        xi += 16;

        SUMM = vmlaqq_lane_f32(SUMM, YY0, vget_low_f32(XX0), 0);
        YEXT = vextqq_f32(YY0, YY1, 1);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX0), 1);
        YEXT = vextqq_f32(YY0, YY1, 2);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 0);
        YEXT = vextqq_f32(YY0, YY1, 3);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX0), 1);
        YEXT = vextqq_f32(YY0, YY1, 4);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 0);
        YEXT = vextqq_f32(YY0, YY1, 5);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX1), 1);
        YEXT = vextqq_f32(YY0, YY1, 6);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 0);
        YEXT = vextqq_f32(YY0, YY1, 7);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX1), 1);
        YEXT = vextqq_f32(YY0, YY1, 8);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX2), 0);
        YEXT = vextqq_f32(YY0, YY1, 9);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX2), 1);
        YEXT = vextqq_f32(YY0, YY1, 10);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX2), 0);
        YEXT = vextqq_f32(YY0, YY1, 11);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX2), 1);
        YEXT = vextqq_f32(YY0, YY1, 12);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX3), 0);
        YEXT = vextqq_f32(YY0, YY1, 13);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_low_f32(XX3), 1);
        YEXT = vextqq_f32(YY0, YY1, 14);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX3), 0);
        YEXT = vextqq_f32(YY0, YY1, 15);
        SUMM = vmlaqq_lane_f32(SUMM, YEXT, vget_high_f32(XX3), 1);

        YY0 = YY1;
        len -= 16;
    }

    while (--len > 0) {
        float32x16_t XX = vld1qq_dup_f32(xi++);
        SUMM = vmlaqq_f32(SUMM, YY0, XX);
        YY0 = vld1qq_f32(++yi);
    }

    float32x16_t XX = vld1qq_dup_f32(xi++);
    SUMM = vmlaqq_f32(SUMM, YY0, XX);
    vst1qq_f32(sum, SUMM);
}
#endif

void pitch_xcorr_neon(config_t *config,
                      input_t *input,
                      output_t *output) {

    pitch_xcorr_config_t *pitch_xcorr_config = (pitch_xcorr_config_t *)config;
    pitch_xcorr_input_t *pitch_xcorr_input = (pitch_xcorr_input_t *)input;
    pitch_xcorr_output_t *pitch_xcorr_output = (pitch_xcorr_output_t *)output;

    int len = pitch_xcorr_config->len;             // fastN = n (1024) - lag (24)
    int max_pitch = pitch_xcorr_config->max_pitch; // lag (24) + 1
    uint32_t block_count = pitch_xcorr_config->block_count;

    for (unsigned block = 0; block < block_count; ++block) {
        opus_val16 *x = pitch_xcorr_input->x[block];
        opus_val16 *y = pitch_xcorr_input->y[block];
        opus_val32 *xcorr = pitch_xcorr_output->xcorr[block]; // ac[24 + 1]

        int i = 0;

        // #if SWAN_REG_TYPE >= 512
        //         for (; i + 16 <= max_pitch; i += 16) {
        //             sum16((const float *)x,
        //                   (const float *)y + i,
        //                   (float *)xcorr + i, len);
        //         }
        // #endif

        // #if SWAN_REG_TYPE >= 256
        //         for (; i + 8 <= max_pitch; i += 8) {
        //             sum8((const float *)x,
        //                  (const float *)y + i,
        //                  (float *)xcorr + i, len);
        //         }
        // #endif

        // #if SWAN_REG_TYPE == 128
        //         for (; i + 4 <= max_pitch; i += 4) {
        //             sum4((const float *)x,
        //                  (const float *)y + i,
        //                  (float *)xcorr + i, len);
        //         }
        // #endif

        /* In case max_pitch isn't a multiple of 4, do non-unrolled version. */
        for (; i + 1 <= max_pitch; i++) {
            xcorr[i] = sum1(x, y + i, len);
        }
    }
}

#endif