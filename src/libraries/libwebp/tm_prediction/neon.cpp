// Copyright 2012 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// ARM NEON version of dsp functions and loop filtering.
//
// Authors: Somnath Banerjee (somnath@google.com)
//          Johann Koenig (johannkoenig@google.com)

#include "neon.hpp"

#include <stdint.h>

#include "libwebp.hpp"
#include "tm_prediction.hpp"

#ifndef SWAN_SIMULATION

/* The following function is the modified version of TM16_NEON,
 * provided in the libwebp library. Please refer to dec_neon.c
 * for the unmodified version in the source library. */
void tm_prediction_neon(config_t *config,
                        input_t *input,
                        output_t *output) {
    tm_prediction_config_t *tm_prediction_config = (tm_prediction_config_t *)config;
    tm_prediction_output_t *tm_prediction_output = (tm_prediction_output_t *)output;
    tm_prediction_input_t *tm_prediction_input = (tm_prediction_input_t *)input;

    int BPS = tm_prediction_config->num_cols;

    for (int block = 0; block < tm_prediction_config->block_count; block++) {
        uint8_t *src = tm_prediction_input->block_src[block];
        uint8_t *dst = tm_prediction_output->block_dst[block];

        uint8_t *src_tmp = src - BPS;
        const uint8x8_t TL = vld1_dup_u8(src_tmp - 1); // top-left pixel 'A[-1]'
        const uint8x16_t T = vld1q_u8(src_tmp);        // top row 'A[0..15]'
        // A[c] - A[-1]
        const int16x8_t d_lo = vreinterpretq_s16_u16(vsubl_u8(vget_low_u8(T), TL));
        const int16x8_t d_hi = vreinterpretq_s16_u16(vsubl_u8(vget_high_u8(T), TL));
        src--;
        for (int y = 0; y < 16; y += 4) {
            // left edge
            const int16x8_t L0 = vreinterpretq_s16_u16(vmovl_u8(vld1_dup_u8(src)));
            src += BPS;
            const int16x8_t L1 = vreinterpretq_s16_u16(vmovl_u8(vld1_dup_u8(src)));
            src += BPS;
            const int16x8_t L2 = vreinterpretq_s16_u16(vmovl_u8(vld1_dup_u8(src)));
            src += BPS;
            const int16x8_t L3 = vreinterpretq_s16_u16(vmovl_u8(vld1_dup_u8(src)));
            src += BPS;
            const int16x8_t r0_lo = vaddq_s16(L0, d_lo); // L[r]  +  A[c]  -  A[-1]
            const int16x8_t r1_lo = vaddq_s16(L1, d_lo);
            const int16x8_t r2_lo = vaddq_s16(L2, d_lo);
            const int16x8_t r3_lo = vaddq_s16(L3, d_lo);
            const int16x8_t r0_hi = vaddq_s16(L0, d_hi);
            const int16x8_t r1_hi = vaddq_s16(L1, d_hi);
            const int16x8_t r2_hi = vaddq_s16(L2, d_hi);
            const int16x8_t r3_hi = vaddq_s16(L3, d_hi);
            // Saturate and store the result.
            const uint8x16_t row0 = vcombine_u8(vqmovun_s16(r0_lo), vqmovun_s16(r0_hi));
            const uint8x16_t row1 = vcombine_u8(vqmovun_s16(r1_lo), vqmovun_s16(r1_hi));
            const uint8x16_t row2 = vcombine_u8(vqmovun_s16(r2_lo), vqmovun_s16(r2_hi));
            const uint8x16_t row3 = vcombine_u8(vqmovun_s16(r3_lo), vqmovun_s16(r3_hi));
            vst1q_u8(dst, row0);
            dst += BPS;
            vst1q_u8(dst, row1);
            dst += BPS;
            vst1q_u8(dst, row2);
            dst += BPS;
            vst1q_u8(dst, row3);
            dst += BPS;
        }
    }
}
#else
void tm_prediction_neon(config_t *config,
                        input_t *input,
                        output_t *output) {
    tm_prediction_config_t *tm_prediction_config = (tm_prediction_config_t *)config;
    tm_prediction_output_t *tm_prediction_output = (tm_prediction_output_t *)output;
    tm_prediction_input_t *tm_prediction_input = (tm_prediction_input_t *)input;

    int BPS = tm_prediction_config->num_cols;

    for (int block = 0; block < tm_prediction_config->block_count; block++) {
        uint8_t *src = tm_prediction_input->block_src[block];
        uint8_t *dst = tm_prediction_output->block_dst[block];

        uint8_t *src_tmp = src - BPS;
#if SWAN_REG_TYPE == SWAN_REG_1024_TYPE
        // A[c] - A[-1]
        const uint8x64_t TL = vld1qq_dup_u8(src_tmp - 1); // top-left pixel 'A[-1]'
        const uint8x16_t T16 = vld1q_u8(src_tmp);         // top row 'A[0..15]'
        const uint8x32_t T32 = vcombinedq_u8(T16, T16);   // top row 'A[0..15]'
        const uint8x64_t T = vcombineqq_u8(T32, T32);     // top row 'A[0..15]'
        const int16x64_t d = vreinterpretoq_s16_u16(vsubloq_u8(T, TL));
        src--;
        // left edge
        const int16x64_t L0123 = vreinterpretoq_s16_u16(vmovloq_u8(vcombineqq_u8(
            vcombinedq_u8(vld1q_dup_u8(src), vld1q_dup_u8((src + BPS))),
            vcombinedq_u8(vld1q_dup_u8(src + 2 * BPS), vld1q_dup_u8(src + 3 * BPS)))));
        src += (BPS * 4);
        const int16x64_t L4567 = vreinterpretoq_s16_u16(vmovloq_u8(vcombineqq_u8(
            vcombinedq_u8(vld1q_dup_u8(src), vld1q_dup_u8((src + BPS))),
            vcombinedq_u8(vld1q_dup_u8(src + 2 * BPS), vld1q_dup_u8(src + 3 * BPS)))));
        src += (BPS * 4);
        const int16x64_t L89ab = vreinterpretoq_s16_u16(vmovloq_u8(vcombineqq_u8(
            vcombinedq_u8(vld1q_dup_u8(src), vld1q_dup_u8((src + BPS))),
            vcombinedq_u8(vld1q_dup_u8(src + 2 * BPS), vld1q_dup_u8(src + 3 * BPS)))));
        src += (BPS * 4);
        const int16x64_t Lcdef = vreinterpretoq_s16_u16(vmovloq_u8(vcombineqq_u8(
            vcombinedq_u8(vld1q_dup_u8(src), vld1q_dup_u8((src + BPS))),
            vcombinedq_u8(vld1q_dup_u8(src + 2 * BPS), vld1q_dup_u8(src + 3 * BPS)))));
        const int16x64_t r0123 = vaddoq_s16(L0123, d); // L[r]  +  A[c]  -  1]
        const int16x64_t r4567 = vaddoq_s16(L4567, d); // L[r]  +  A[c]  -  1]
        const int16x64_t r89ab = vaddoq_s16(L89ab, d); // L[r]  +  A[c]  -  1]
        const int16x64_t rcdef = vaddoq_s16(Lcdef, d); // L[r]  +  A[c]  -  1]
        // Saturate and store the result.
        const uint8x64_t row0123 = vqmovunoq_s16(r0123);
        const uint8x64_t row4567 = vqmovunoq_s16(r4567);
        const uint8x64_t row89ab = vqmovunoq_s16(r89ab);
        const uint8x64_t rowcdef = vqmovunoq_s16(rcdef);
        vst1q_u8(dst, vgetdq_low_u8(vgetqq_low_u8(row0123)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_high_u8(vgetqq_low_u8(row0123)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_low_u8(vgetqq_high_u8(row0123)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_high_u8(vgetqq_high_u8(row0123)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_low_u8(vgetqq_low_u8(row4567)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_high_u8(vgetqq_low_u8(row4567)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_low_u8(vgetqq_high_u8(row4567)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_high_u8(vgetqq_high_u8(row4567)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_low_u8(vgetqq_low_u8(row89ab)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_high_u8(vgetqq_low_u8(row89ab)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_low_u8(vgetqq_high_u8(row89ab)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_high_u8(vgetqq_high_u8(row89ab)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_low_u8(vgetqq_low_u8(rowcdef)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_high_u8(vgetqq_low_u8(rowcdef)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_low_u8(vgetqq_high_u8(rowcdef)));
        dst += BPS;
        vst1q_u8(dst, vgetdq_high_u8(vgetqq_high_u8(rowcdef)));
#elif SWAN_REG_TYPE == SWAN_REG_512_TYPE
        // A[c] - A[-1]
        const uint8x32_t TL = vld1dq_dup_u8(src_tmp - 1); // top-left pixel 'A[-1]'
        const uint8x16_t T16 = vld1q_u8(src_tmp);         // top row 'A[0..15]'
        const uint8x32_t T = vcombinedq_u8(T16, T16);     // top row 'A[0..15]'
        const int16x32_t d = vreinterpretqq_s16_u16(vsublqq_u8(T, TL));
        src--;
        for (int y = 0; y < 16; y += 8) {
            // left edge
            const int16x32_t L01 = vreinterpretqq_s16_u16(vmovlqq_u8(vcombinedq_u8(vld1q_dup_u8(src), vld1q_dup_u8(src + BPS))));
            src += (BPS * 2);
            const int16x32_t L23 = vreinterpretqq_s16_u16(vmovlqq_u8(vcombinedq_u8(vld1q_dup_u8(src), vld1q_dup_u8(src + BPS))));
            src += (BPS * 2);
            const int16x32_t L45 = vreinterpretqq_s16_u16(vmovlqq_u8(vcombinedq_u8(vld1q_dup_u8(src), vld1q_dup_u8(src + BPS))));
            src += (BPS * 2);
            const int16x32_t L67 = vreinterpretqq_s16_u16(vmovlqq_u8(vcombinedq_u8(vld1q_dup_u8(src), vld1q_dup_u8(src + BPS))));
            src += (BPS * 2);
            const int16x32_t r01 = vaddqq_s16(L01, d); // L[r]  +  A[c]  -  1]
            const int16x32_t r23 = vaddqq_s16(L23, d);
            const int16x32_t r45 = vaddqq_s16(L45, d);
            const int16x32_t r67 = vaddqq_s16(L67, d);
            // Saturate and store the result.
            const uint8x32_t row01 = vqmovunqq_s16(r01);
            const uint8x32_t row23 = vqmovunqq_s16(r23);
            const uint8x32_t row45 = vqmovunqq_s16(r45);
            const uint8x32_t row67 = vqmovunqq_s16(r67);
            vst1q_u8(dst, vgetdq_low_u8(row01));
            dst += BPS;
            vst1q_u8(dst, vgetdq_high_u8(row01));
            dst += BPS;
            vst1q_u8(dst, vgetdq_low_u8(row23));
            dst += BPS;
            vst1q_u8(dst, vgetdq_high_u8(row23));
            dst += BPS;
            vst1q_u8(dst, vgetdq_low_u8(row45));
            dst += BPS;
            vst1q_u8(dst, vgetdq_high_u8(row45));
            dst += BPS;
            vst1q_u8(dst, vgetdq_low_u8(row67));
            dst += BPS;
            vst1q_u8(dst, vgetdq_high_u8(row67));
            dst += BPS;
        }
#elif SWAN_REG_TYPE == SWAN_REG_256_TYPE
        // A[c] - A[-1]
        const uint8x16_t TL = vld1q_dup_u8(src_tmp - 1); // top-left pixel 'A[-1]'
        const uint8x16_t T = vld1q_u8(src_tmp);          // top row 'A[0..15]'
        const int16x16_t d = vreinterpretdq_s16_u16(vsubldq_u8(T, TL));
        src--;
        for (int y = 0; y < 16; y += 4) {
            // left edge
            const int16x16_t L0 = vreinterpretdq_s16_u16(vmovldq_u8(vld1q_dup_u8(src)));
            src += BPS;
            const int16x16_t L1 = vreinterpretdq_s16_u16(vmovldq_u8(vld1q_dup_u8(src)));
            src += BPS;
            const int16x16_t L2 = vreinterpretdq_s16_u16(vmovldq_u8(vld1q_dup_u8(src)));
            src += BPS;
            const int16x16_t L3 = vreinterpretdq_s16_u16(vmovldq_u8(vld1q_dup_u8(src)));
            src += BPS;
            const int16x16_t r0 = vadddq_s16(L0, d); // L[r]  +  A[c]  -  1]
            const int16x16_t r1 = vadddq_s16(L1, d);
            const int16x16_t r2 = vadddq_s16(L2, d);
            const int16x16_t r3 = vadddq_s16(L3, d);
            // Saturate and store the result.
            const uint8x16_t row0 = vqmovundq_s16(r0);
            const uint8x16_t row1 = vqmovundq_s16(r1);
            const uint8x16_t row2 = vqmovundq_s16(r2);
            const uint8x16_t row3 = vqmovundq_s16(r3);
            vst1q_u8(dst, row0);
            dst += BPS;
            vst1q_u8(dst, row1);
            dst += BPS;
            vst1q_u8(dst, row2);
            dst += BPS;
            vst1q_u8(dst, row3);
            dst += BPS;
        }
#else
        // A[c] - A[-1]
        const uint8x8_t TL = vld1_dup_u8(src_tmp - 1); // top-left pixel 'A[-1]'
        const uint8x16_t T = vld1q_u8(src_tmp);        // top row 'A[0..15]'
        const int16x8_t d_lo = vreinterpretq_s16_u16(vsubl_u8(vget_low_u8(T), TL));
        const int16x8_t d_hi = vreinterpretq_s16_u16(vsubl_u8(vget_high_u8(T), TL));
        src--;
        for (int y = 0; y < 16; y += 4) {
            // left edge
            const int16x8_t L0 = vreinterpretq_s16_u16(vmovl_u8(vld1_dup_u8(src)));
            src += BPS;
            const int16x8_t L1 = vreinterpretq_s16_u16(vmovl_u8(vld1_dup_u8(src)));
            src += BPS;
            const int16x8_t L2 = vreinterpretq_s16_u16(vmovl_u8(vld1_dup_u8(src)));
            src += BPS;
            const int16x8_t L3 = vreinterpretq_s16_u16(vmovl_u8(vld1_dup_u8(src)));
            src += BPS;
            const int16x8_t r0_lo = vaddq_s16(L0, d_lo); // L[r]  +  A[c]  -  A[-1]
            const int16x8_t r1_lo = vaddq_s16(L1, d_lo);
            const int16x8_t r2_lo = vaddq_s16(L2, d_lo);
            const int16x8_t r3_lo = vaddq_s16(L3, d_lo);
            const int16x8_t r0_hi = vaddq_s16(L0, d_hi);
            const int16x8_t r1_hi = vaddq_s16(L1, d_hi);
            const int16x8_t r2_hi = vaddq_s16(L2, d_hi);
            const int16x8_t r3_hi = vaddq_s16(L3, d_hi);
            // Saturate and store the result.
            const uint8x16_t row0 = vcombine_u8(vqmovun_s16(r0_lo), vqmovun_s16(r0_hi));
            const uint8x16_t row1 = vcombine_u8(vqmovun_s16(r1_lo), vqmovun_s16(r1_hi));
            const uint8x16_t row2 = vcombine_u8(vqmovun_s16(r2_lo), vqmovun_s16(r2_hi));
            const uint8x16_t row3 = vcombine_u8(vqmovun_s16(r3_lo), vqmovun_s16(r3_hi));
            vst1q_u8(dst, row0);
            dst += BPS;
            vst1q_u8(dst, row1);
            dst += BPS;
            vst1q_u8(dst, row2);
            dst += BPS;
            vst1q_u8(dst, row3);
            dst += BPS;
        }
#endif
#ifdef SWAN_SIMULATION
        fake_neon_flusher();
#endif
    }
}
#endif