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
#include "sad.hpp"

#ifndef SWAN_SIMULATION
static inline uint16x8_t sad8x(const uint8_t *src_ptr, int src_stride,
                               const uint8_t *ref_ptr, int ref_stride,
                               const int height) {
    int i;
    uint16x8_t abs = vdupq_n_u16(0);

    for (i = 0; i < height; ++i) {
        const uint8x8_t a_u8 = vld1_u8(src_ptr);
        const uint8x8_t b_u8 = vld1_u8(ref_ptr);
        src_ptr += src_stride;
        ref_ptr += ref_stride;
        abs = vabal_u8(abs, a_u8, b_u8);
    }
    return abs;
}

/* The following function is the modified version of vpx_sad8x##n##_neon macro with n = 8,
 * provided in the libvpx library. Please refer to sad_neon.c for the unmodified version in the source library. */
void sad_neon(config_t *config,
              input_t *input,
              output_t *output) {
    sad_config_t *sad_config = (sad_config_t *)config;
    sad_input_t *sad_input = (sad_input_t *)input;
    sad_output_t *sad_output = (sad_output_t *)output;

    int src_stride = sad_config->src_stride;
    int ref_stride = sad_config->ref_stride;

    for (int block = 0; block < sad_config->block_count; block++) {
        const uint8_t *src_ptr = sad_input->src_ptr[block];
        const uint8_t *ref_ptr = sad_input->ref_ptr[block];

        const uint16x8_t abs = sad8x(src_ptr, src_stride, ref_ptr, ref_stride, 8);
        sad_output->return_val[block] = vaddlvq_u16(abs);
    }
}
#else
static inline uint32_t sad8x(const uint8_t *src_ptr, int src_stride,
                             const uint8_t *ref_ptr, int ref_stride,
                             const int height) {
    int i = 0;

#if SWAN_REG_TYPE == SWAN_REG_1024_TYPE
    uint16x64_t absx64 = vdupoq_n_u16(0);
    for (; i + 8 <= height; i += 8) {
        const uint8x8_t a1_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a2_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a3_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a4_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a5_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a6_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a7_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a8_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x64_t a_u8 = vcombineqq_u8(vcombinedq_u8(vcombine_u8(a1_u8, a2_u8), vcombine_u8(a3_u8, a4_u8)),
                                              vcombinedq_u8(vcombine_u8(a5_u8, a6_u8), vcombine_u8(a7_u8, a8_u8)));
        const uint8x8_t b1_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b2_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b3_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b4_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b5_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b6_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b7_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b8_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x64_t b_u8 = vcombineqq_u8(vcombinedq_u8(vcombine_u8(b1_u8, b2_u8), vcombine_u8(b3_u8, b4_u8)),
                                              vcombinedq_u8(vcombine_u8(b5_u8, b6_u8), vcombine_u8(b7_u8, b8_u8)));
        absx64 = vabaloq_u8(absx64, a_u8, b_u8);
    }
#endif
#if SWAN_REG_TYPE > SWAN_REG_512_TYPE
    uint16x32_t absx32 = vaddqq_u16(vgetoq_high_u16(absx64), vgetoq_low_u16(absx64));
#elif SWAN_REG_TYPE == SWAN_REG_512_TYPE
    uint16x32_t absx32 = vdupqq_n_u16(0);
#endif
#if SWAN_REG_TYPE >= SWAN_REG_512_TYPE
    for (; i + 4 <= height; i += 4) {
        const uint8x8_t a1_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a2_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a3_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a4_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x32_t a_u8 = vcombinedq_u8(vcombine_u8(a1_u8, a2_u8), vcombine_u8(a3_u8, a4_u8));
        const uint8x8_t b1_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b2_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b3_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b4_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x32_t b_u8 = vcombinedq_u8(vcombine_u8(b1_u8, b2_u8), vcombine_u8(b3_u8, b4_u8));
        absx32 = vabalqq_u8(absx32, a_u8, b_u8);
    }
#endif
#if SWAN_REG_TYPE > SWAN_REG_256_TYPE
    uint16x16_t absx16 = vadddq_u16(vgetqq_high_u16(absx32), vgetqq_low_u16(absx32));
#elif SWAN_REG_TYPE == SWAN_REG_256_TYPE
    uint16x16_t absx16 = vdupdq_n_u16(0);
#endif
#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
    for (; i + 2 <= height; i += 2) {
        const uint8x8_t a1_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x8_t a2_u8 = vld1_u8(src_ptr);
        src_ptr += src_stride;
        const uint8x16_t a_u8 = vcombine_u8(a1_u8, a2_u8);
        const uint8x8_t b1_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x8_t b2_u8 = vld1_u8(ref_ptr);
        ref_ptr += ref_stride;
        const uint8x16_t b_u8 = vcombine_u8(b1_u8, b2_u8);
        absx16 = vabaldq_u8(absx16, a_u8, b_u8);
    }
#endif
#if SWAN_REG_TYPE > SWAN_REG_128_TYPE
    uint16x8_t absx8 = vaddq_u16(vgetdq_high_u16(absx16), vgetdq_low_u16(absx16));
#elif SWAN_REG_TYPE == SWAN_REG_128_TYPE
    uint16x8_t absx8 = vdupq_n_u16(0);
#endif
    for (; i + 1 <= height; i += 1) {
        const uint8x8_t a_u8 = vld1_u8(src_ptr);
        const uint8x8_t b_u8 = vld1_u8(ref_ptr);
        src_ptr += src_stride;
        ref_ptr += ref_stride;
        absx8 = vabal_u8(absx8, a_u8, b_u8);
    }
    return vaddlvq_u16(absx8);
}

void sad_neon(config_t *config,
              input_t *input,
              output_t *output) {
    sad_config_t *sad_config = (sad_config_t *)config;
    sad_input_t *sad_input = (sad_input_t *)input;
    sad_output_t *sad_output = (sad_output_t *)output;

    int src_stride = sad_config->src_stride;
    int ref_stride = sad_config->ref_stride;

    for (int block = 0; block < sad_config->block_count; block++) {
        const uint8_t *src_ptr = sad_input->src_ptr[block];
        const uint8_t *ref_ptr = sad_input->ref_ptr[block];

        sad_output->return_val[block] = sad8x(src_ptr, src_stride, ref_ptr, ref_stride, 8);
#ifdef SWAN_SIMULATION
#if !defined(NEON2RVV)
        fake_neon_flusher();
#endif
#endif
    }
}
#endif
