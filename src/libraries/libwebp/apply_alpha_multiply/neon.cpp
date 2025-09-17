// Copyright 2017 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
// Utilities for processing transparent channel, NEON version.
//
// Author: Skal (pascal.massimino@gmail.com)

#include "neon.hpp"

#include <stdint.h>

#include "apply_alpha_multiply.hpp"
#include "libwebp.hpp"

#define MULTIPLIER(a) ((a)*0x8081)
#define PREMULTIPLY(x, m) (((x) * (m)) >> 23)

#if defined(NEON2RVV)
#define GET(v, idx) __riscv_vget_v_u8m1x4_u8m1(v, idx)
#define SET(v, idx, val) v = __riscv_vset_v_u8m1_u8m1x4(v, idx, val)
#define MULTIPLY_BY_ALPHA(V, ALPHA, OTHER)                       \
    do {                                                         \
        const uint8x8_t alpha = GET(V, ALPHA);                   \
        const uint16x8_t r1 = vmull_u8(GET(V, 1), alpha);        \
        const uint16x8_t g1 = vmull_u8(GET(V, 2), alpha);        \
        const uint16x8_t b1 = vmull_u8(GET(V, (OTHER)), alpha);  \
        /* we use: v / 255 = (v + 1 + (v >> 8)) >> 8 */          \
        const uint16x8_t r2 = vsraq_n_u16(r1, r1, 8);            \
        const uint16x8_t g2 = vsraq_n_u16(g1, g1, 8);            \
        const uint16x8_t b2 = vsraq_n_u16(b1, b1, 8);            \
        const uint16x8_t r3 = vaddq_u16(r2, kOne);               \
        const uint16x8_t g3 = vaddq_u16(g2, kOne);               \
        const uint16x8_t b3 = vaddq_u16(b2, kOne);               \
        SET(V, 1, vshrn_n_u16(r3, 8));                           \
        SET(V, 2, vshrn_n_u16(g3, 8));                           \
        SET(V, (OTHER), vshrn_n_u16(b3, 8));                     \
    } while (0)
#else
#define MULTIPLY_BY_ALPHA(V, ALPHA, OTHER)                       \
    do {                                                         \
        const uint8x8_t alpha = (V).val[(ALPHA)];                \
        const uint16x8_t r1 = vmull_u8((V).val[1], alpha);       \
        const uint16x8_t g1 = vmull_u8((V).val[2], alpha);       \
        const uint16x8_t b1 = vmull_u8((V).val[(OTHER)], alpha); \
        /* we use: v / 255 = (v + 1 + (v >> 8)) >> 8 */          \
        const uint16x8_t r2 = vsraq_n_u16(r1, r1, 8);            \
        const uint16x8_t g2 = vsraq_n_u16(g1, g1, 8);            \
        const uint16x8_t b2 = vsraq_n_u16(b1, b1, 8);            \
        const uint16x8_t r3 = vaddq_u16(r2, kOne);               \
        const uint16x8_t g3 = vaddq_u16(g2, kOne);               \
        const uint16x8_t b3 = vaddq_u16(b2, kOne);               \
        (V).val[1] = vshrn_n_u16(r3, 8);                         \
        (V).val[2] = vshrn_n_u16(g3, 8);                         \
        (V).val[(OTHER)] = vshrn_n_u16(b3, 8);                   \
    } while (0)
#endif

/* The following function is the modified version of ApplyAlphaMultiply_NEON,
 * provided in the libwebp library. Please refer to alpha_processing_neon.c
 * for the unmodified version in the source library. */
void apply_alpha_multiply_neon(config_t *config,
                               input_t *input,
                               output_t *output) {
    apply_alpha_multiply_config_t *apply_alpha_multiply_config = (apply_alpha_multiply_config_t *)config;
    apply_alpha_multiply_input_t *apply_alpha_multiply_input = (apply_alpha_multiply_input_t *)input;
    apply_alpha_multiply_output_t *apply_alpha_multiply_output = (apply_alpha_multiply_output_t *)output;

    int h = apply_alpha_multiply_config->num_rows;
    int w = apply_alpha_multiply_config->num_cols;
    bool alpha_first = apply_alpha_multiply_config->alpha_first;
    int stride = w << 2;
    uint8_t *rgba_in = apply_alpha_multiply_input->rgba_in;
    uint8_t *rgba_out = apply_alpha_multiply_output->rgba_out;

    const uint16x8_t kOne = vdupq_n_u16(1u);
    while (h-- > 0) {
        uint32_t *const rgbx_in = (uint32_t *)rgba_in;
        uint32_t *const rgbx_out = (uint32_t *)rgba_out;
        int i = 0;
        if (alpha_first) {
            for (; i + 8 <= w; i += 8) {
                // load aaaa...|rrrr...|gggg...|bbbb...
                uint8x8x4_t RGBX = vld4_u8((const uint8_t *)(rgbx_in + i));
                MULTIPLY_BY_ALPHA(RGBX, 0, 3);
                vst4_u8((uint8_t *)(rgbx_out + i), RGBX);
            }
        } else {
            for (; i + 8 <= w; i += 8) {
                uint8x8x4_t RGBX = vld4_u8((const uint8_t *)(rgbx_in + i));
                MULTIPLY_BY_ALPHA(RGBX, 3, 0);
                vst4_u8((uint8_t *)(rgbx_out + i), RGBX);
            }
        }
        // Finish with left-overs.
        for (; i < w; ++i) {
            uint8_t *const rgb_in = rgba_in + (alpha_first ? 1 : 0);
            uint8_t *const rgb_out = rgba_out + (alpha_first ? 1 : 0);
            const uint8_t *const alpha_in = rgba_in + (alpha_first ? 0 : 3);
            uint8_t *const alpha_out = rgba_in + (alpha_first ? 0 : 3);
            const uint32_t a = alpha_in[4 * i];
            alpha_out[4 * i] = alpha_in[4 * i];
            if (a != 0xff) {
                const uint32_t mult = MULTIPLIER(a);
                rgb_out[4 * i + 0] = PREMULTIPLY(rgb_in[4 * i + 0], mult);
                rgb_out[4 * i + 1] = PREMULTIPLY(rgb_in[4 * i + 1], mult);
                rgb_out[4 * i + 2] = PREMULTIPLY(rgb_in[4 * i + 2], mult);
            }
        }
        rgba_in += stride;
        rgba_out += stride;
    }
}
