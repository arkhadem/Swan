/*
 * Copyright 2017 The Chromium Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the Chromium source repository LICENSE file.
 *
 * Per http://en.wikipedia.org/wiki/Adler-32 the adler32 A value (aka s1) is
 * the sum of N input data bytes D1 ... DN,
 *
 *   A = A0 + D1 + D2 + ... + DN
 *
 * where A0 is the initial value.
 *
 * SSE2 _mm_sad_epu8() can be used for byte sums (see http://bit.ly/2wpUOeD,
 * for example) and accumulating the byte sums can use SSE shuffle-adds (see
 * the "Integer" section of http://bit.ly/2erPT8t for details). Arm NEON has
 * similar instructions.
 *
 * The adler32 B value (aka s2) sums the A values from each step:
 *
 *   B0 + (A0 + D1) + (A0 + D1 + D2) + ... + (A0 + D1 + D2 + ... + DN) or
 *
 *       B0 + N.A0 + N.D1 + (N-1).D2 + (N-2).D3 + ... + (N-(N-1)).DN
 *
 * B0 being the initial value. For 32 bytes (ideal for garden-variety SIMD):
 *
 *   B = B0 + 32.A0 + [D1 D2 D3 ... D32] x [32 31 30 ... 1].
 *
 * Adjacent blocks of 32 input bytes can be iterated with the expressions to
 * compute the adler32 s1 s2 of M >> 32 input bytes [1].
 *
 * As M grows, the s1 s2 sums grow. If left unchecked, they would eventually
 * overflow the precision of their integer representation (bad). However, s1
 * and s2 also need to be computed modulo the adler BASE value (reduced). If
 * at most NMAX bytes are processed before a reduce, s1 s2 _cannot_ overflow
 * a uint32_t type (the NMAX constraint) [2].
 *
 * [1] the iterative equations for s2 contain constant factors; these can be
 * hoisted from the n-blocks do loop of the SIMD code.
 *
 * [2] zlib adler32_z() uses this fact to implement NMAX-block-based updates
 * of the adler s1 s2 of uint32_t type (see adler32.c).
 */

#include "neon.hpp"
#ifdef SWAN_SIMULATION
#include <fake_neon.h>
#else
#include <arm_neon.h>
#endif
#include <stdint.h>

#include "adler32.hpp"
#include "zlib.hpp"

// Does vertical convolution to produce one output row. The filter values and
// length are given in the first two parameters. These are applied to each
// of the rows pointed to in the |source_data_rows| array, with each row
// being |pixel_width| wide.
//
// The output must have room for |pixel_width * 4| bytes.
#ifndef SWAN_SIMULATION
/* The following function is the modified version of adler32_simd_,
 * provided in the webaudio library. Please refer to adler32_simd.c
 * for the unmodified version in the source library. */
void adler32_neon(config_t *config,
                  input_t *input,
                  output_t *output) {
    adler32_config_t *adler32_config = (adler32_config_t *)config;
    adler32_input_t *adler32_input = (adler32_input_t *)input;
    adler32_output_t *adler32_output = (adler32_output_t *)output;

    uLong adler = adler32_config->adler;
    const Bytef *buf = adler32_input->buf;
    z_size_t len = adler32_config->len;
    const uint16_t mem0[4] = {32, 31, 30, 29};
    uint16x4_t constant0 = vld1_u16(mem0);
    const uint16_t mem1[4] = {28, 27, 26, 25};
    uint16x4_t constant1 = vld1_u16(mem1);
    const uint16_t mem2[4] = {24, 23, 22, 21};
    uint16x4_t constant2 = vld1_u16(mem2);
    const uint16_t mem3[4] = {20, 19, 18, 17};
    uint16x4_t constant3 = vld1_u16(mem3);
    const uint16_t mem4[4] = {16, 15, 14, 13};
    uint16x4_t constant4 = vld1_u16(mem4);
    const uint16_t mem5[4] = {12, 11, 10, 9};
    uint16x4_t constant5 = vld1_u16(mem5);
    const uint16_t mem6[4] = {8, 7, 6, 5};
    uint16x4_t constant6 = vld1_u16(mem6);
    const uint16_t mem7[4] = {4, 3, 2, 1};
    uint16x4_t constant7 = vld1_u16(mem7);

    /*
     * Split Adler-32 into component sums.
     */
    uint32_t s1 = adler & 0xffff;
    uint32_t s2 = adler >> 16;

    /*
     * Process the data in blocks.
     */
    const unsigned BLOCK_SIZE = 1 << 5;

    z_size_t blocks = len / BLOCK_SIZE;
    len -= blocks * BLOCK_SIZE;

    while (blocks) {
        unsigned n = NMAX / BLOCK_SIZE; /* The NMAX constraint. */
        if (n > blocks)
            n = (unsigned)blocks;
        blocks -= n;

        /*
         * Process n blocks of data. At most NMAX data bytes can be
         * processed before s2 must be reduced modulo BASE.
         */
        uint32_t vals[4] = {0, 0, 0, s1 * n};
        uint32x4_t v_s2 = vld1q_u32(vals);
        uint32x4_t v_s1 = vdupq_n_u32(0);

        uint16x8_t v_column_sum_1 = vdupq_n_u16(0);
        uint16x8_t v_column_sum_2 = vdupq_n_u16(0);
        uint16x8_t v_column_sum_3 = vdupq_n_u16(0);
        uint16x8_t v_column_sum_4 = vdupq_n_u16(0);

        do {
            /*
             * Load 32 input bytes.
             */
            const uint8x16_t bytes1 = vld1q_u8((uint8_t *)(buf));
            const uint8x16_t bytes2 = vld1q_u8((uint8_t *)(buf + 16));

            /*
             * Add previous block byte sum to v_s2.
             */
            v_s2 = vaddq_u32(v_s2, v_s1);

            /*
             * Horizontally add the bytes for s1.
             */
            v_s1 = vpadalq_u16(v_s1, vpadalq_u8(vpaddlq_u8(bytes1), bytes2));

            /*
             * Vertically add the bytes for s2.
             */
            v_column_sum_1 = vaddw_u8(v_column_sum_1, vget_low_u8(bytes1));
            v_column_sum_2 = vaddw_u8(v_column_sum_2, vget_high_u8(bytes1));
            v_column_sum_3 = vaddw_u8(v_column_sum_3, vget_low_u8(bytes2));
            v_column_sum_4 = vaddw_u8(v_column_sum_4, vget_high_u8(bytes2));

            buf += BLOCK_SIZE;

        } while (--n);

        v_s2 = vshlq_n_u32(v_s2, 5);

        /*
         * Multiply-add bytes by [ 32, 31, 30, ... ] for s2.
         */
        v_s2 = vmlal_u16(v_s2, vget_low_u16(v_column_sum_1), constant0);
        v_s2 = vmlal_u16(v_s2, vget_high_u16(v_column_sum_1), constant1);
        v_s2 = vmlal_u16(v_s2, vget_low_u16(v_column_sum_2), constant2);
        v_s2 = vmlal_u16(v_s2, vget_high_u16(v_column_sum_2), constant3);
        v_s2 = vmlal_u16(v_s2, vget_low_u16(v_column_sum_3), constant4);
        v_s2 = vmlal_u16(v_s2, vget_high_u16(v_column_sum_3), constant5);
        v_s2 = vmlal_u16(v_s2, vget_low_u16(v_column_sum_4), constant6);
        v_s2 = vmlal_u16(v_s2, vget_high_u16(v_column_sum_4), constant7);

        /*
         * Sum epi32 ints v_s1(s2) and accumulate in s1(s2).
         */
        uint32x2_t sum1 = vpadd_u32(vget_low_u32(v_s1), vget_high_u32(v_s1));
        uint32x2_t sum2 = vpadd_u32(vget_low_u32(v_s2), vget_high_u32(v_s2));
        uint32x2_t s1s2 = vpadd_u32(sum1, sum2);

        s1 += vget_lane_u32(s1s2, 0);
        s2 += vget_lane_u32(s1s2, 1);

        /*
         * Reduce.
         */
        s1 %= BASE;
        s2 %= BASE;
    }

    /*
     * Handle leftover data.
     */
    if (len) {
        if (len >= 16) {
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);

            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);

            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);

            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);

            len -= 16;
        }

        while (len--) {
            s2 += (s1 += *buf++);
        }

        if (s1 >= BASE)
            s1 -= BASE;
        s2 %= BASE;
    }

    /*
     * Return the recombined sums.
     */
    adler32_output->return_value[0] = s1 | (s2 << 16);
}
#else

void adler32_neon(config_t *config,
                  input_t *input,
                  output_t *output) {

    adler32_config_t *adler32_config = (adler32_config_t *)config;
    adler32_input_t *adler32_input = (adler32_input_t *)input;
    adler32_output_t *adler32_output = (adler32_output_t *)output;

    uLong adler = adler32_config->adler;
    const Bytef *buf = adler32_input->buf;
    z_size_t len = adler32_config->len;
    unsigned BLOCK_SIZE;
    z_size_t total_remaining_blocks;
    unsigned per_iteration_blocks;

    /*
     * Split Adler-32 into component sums.
     */
    uint32_t s1 = adler & 0xffff;
    uint32_t s2 = adler >> 16;

#if SWAN_REG_TYPE == SWAN_REG_1024_TYPE
    BLOCK_SIZE = 1 << 8;
    total_remaining_blocks = len / BLOCK_SIZE;
    if (total_remaining_blocks) {
        per_iteration_blocks = NMAX / BLOCK_SIZE;

        uint16x32_t constants_1024[8];
        uint16_t mem_1024[256];
        for (int i = 0; i < 256; i++) {
            mem_1024[i] = 256 - i;
        }
        for (int i = 0; i < 8; i++) {
            constants_1024[i] = vld1qq_u16(mem_1024 + (i << 5));
        }
        while (total_remaining_blocks) {
            // n: how many blocks in each iteration?
            unsigned n = (per_iteration_blocks > total_remaining_blocks) ? total_remaining_blocks : per_iteration_blocks; /* The NMAX constraint. */
            if (n > total_remaining_blocks)
                n = (unsigned)total_remaining_blocks;
            total_remaining_blocks -= n;
            len -= (n * BLOCK_SIZE);

            /*
            * Process n blocks of data. At most NMAX data bytes can be
            * processed before s2 must be reduced modulo BASE.
            */
            uint32_t vals[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s1 * n};
            uint32x32_t v_s2 = vld1oq_u32(vals);
            uint32x32_t v_s1 = vdupoq_n_u32(0);

            uint16x64_t v_column_sum_1 = vdupoq_n_u16(0);
            uint16x64_t v_column_sum_2 = vdupoq_n_u16(0);
            uint16x64_t v_column_sum_3 = vdupoq_n_u16(0);
            uint16x64_t v_column_sum_4 = vdupoq_n_u16(0);

            do {
                /*
                * Load 128 input bytes.
                */
                const uint8x128_t bytes1 = vld1oq_u8((uint8_t *)(buf));
                const uint8x128_t bytes2 = vld1oq_u8((uint8_t *)(buf + 128));

                /*
                * Add previous block byte sum to v_s2.
                */
                v_s2 = vaddoq_u32(v_s2, v_s1);

                /*
                * Horizontally add the bytes for s1.
                */
                v_s1 = vpadaloq_u16(v_s1, vpadaloq_u8(vpaddloq_u8(bytes1), bytes2));

                /*
                * Vertically add the bytes for s2.
                */
                v_column_sum_1 = vaddwoq_u8(v_column_sum_1, vgetoq_low_u8(bytes1));
                v_column_sum_2 = vaddwoq_u8(v_column_sum_2, vgetoq_high_u8(bytes1));
                v_column_sum_3 = vaddwoq_u8(v_column_sum_3, vgetoq_low_u8(bytes2));
                v_column_sum_4 = vaddwoq_u8(v_column_sum_4, vgetoq_high_u8(bytes2));

                buf += BLOCK_SIZE;

            } while (--n);

            v_s2 = vshloq_n_u32(v_s2, 8);

            /*
            * Multiply-add bytes by [ 32, 31, 30, ... ] for s2.
            */
            v_s2 = vmlaloq_u16(v_s2, vgetoq_low_u16(v_column_sum_1), constants_1024[0]);
            v_s2 = vmlaloq_u16(v_s2, vgetoq_high_u16(v_column_sum_1), constants_1024[1]);
            v_s2 = vmlaloq_u16(v_s2, vgetoq_low_u16(v_column_sum_2), constants_1024[2]);
            v_s2 = vmlaloq_u16(v_s2, vgetoq_high_u16(v_column_sum_2), constants_1024[3]);
            v_s2 = vmlaloq_u16(v_s2, vgetoq_low_u16(v_column_sum_3), constants_1024[4]);
            v_s2 = vmlaloq_u16(v_s2, vgetoq_high_u16(v_column_sum_3), constants_1024[5]);
            v_s2 = vmlaloq_u16(v_s2, vgetoq_low_u16(v_column_sum_4), constants_1024[6]);
            v_s2 = vmlaloq_u16(v_s2, vgetoq_high_u16(v_column_sum_4), constants_1024[7]);

            /*
            * Sum epi32 ints v_s1(s2) and accumulate in s1(s2).
            */
            uint32x16_t sum1x16 = vpaddqq_u32(vgetoq_low_u32(v_s1), vgetoq_high_u32(v_s1));
            uint32x8_t sum1x8 = vpadddq_u32(vgetqq_low_u32(sum1x16), vgetqq_high_u32(sum1x16));
            uint32x4_t sum1x4 = vpaddq_u32(vgetdq_low_u32(sum1x8), vgetdq_high_u32(sum1x8));
            uint32x2_t sum1x2 = vpadd_u32(vget_low_u32(sum1x4), vget_high_u32(sum1x4));
            uint32x16_t sum2x16 = vpaddqq_u32(vgetoq_low_u32(v_s2), vgetoq_high_u32(v_s2));
            uint32x8_t sum2x8 = vpadddq_u32(vgetqq_low_u32(sum2x16), vgetqq_high_u32(sum2x16));
            uint32x4_t sum2x4 = vpaddq_u32(vgetdq_low_u32(sum2x8), vgetdq_high_u32(sum2x8));
            uint32x2_t sum2x2 = vpadd_u32(vget_low_u32(sum2x4), vget_high_u32(sum2x4));
            uint32x2_t s1s2 = vpadd_u32(sum1x2, sum2x2);

            s1 += vget_lane_u32(s1s2, 0);
            s2 += vget_lane_u32(s1s2, 1);

            /*
            * Reduce.
            */
            s1 %= BASE;
            s2 %= BASE;
        }
    }
#endif

#if SWAN_REG_TYPE >= SWAN_REG_512_TYPE
    BLOCK_SIZE = 1 << 7;
    total_remaining_blocks = len / BLOCK_SIZE;
    if (total_remaining_blocks) {
        per_iteration_blocks = NMAX / BLOCK_SIZE;

        uint16x16_t constants_512[8];
        uint16_t mem_512[128];
        for (int i = 0; i < 128; i++) {
            mem_512[i] = 128 - i;
        }
        for (int i = 0; i < 8; i++) {
            constants_512[i] = vld1dq_u16(mem_512 + (i << 4));
        }
        while (total_remaining_blocks) {
            // n: how many blocks in each iteration?
            unsigned n = (per_iteration_blocks > total_remaining_blocks) ? total_remaining_blocks : per_iteration_blocks; /* The NMAX constraint. */
            if (n > total_remaining_blocks)
                n = (unsigned)total_remaining_blocks;
            total_remaining_blocks -= n;
            len -= (n * BLOCK_SIZE);

            /*
            * Process n blocks of data. At most NMAX data bytes can be
            * processed before s2 must be reduced modulo BASE.
            */
            uint32_t vals[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, s1 * n};
            uint32x16_t v_s2 = vld1qq_u32(vals);
            uint32x16_t v_s1 = vdupqq_n_u32(0);

            uint16x32_t v_column_sum_1 = vdupqq_n_u16(0);
            uint16x32_t v_column_sum_2 = vdupqq_n_u16(0);
            uint16x32_t v_column_sum_3 = vdupqq_n_u16(0);
            uint16x32_t v_column_sum_4 = vdupqq_n_u16(0);

            do {
                /*
                * Load 128 input bytes.
                */
                const uint8x64_t bytes1 = vld1qq_u8((uint8_t *)(buf));
                const uint8x64_t bytes2 = vld1qq_u8((uint8_t *)(buf + 64));

                /*
                * Add previous block byte sum to v_s2.
                */
                v_s2 = vaddqq_u32(v_s2, v_s1);

                /*
                * Horizontally add the bytes for s1.
                */
                v_s1 = vpadalqq_u16(v_s1, vpadalqq_u8(vpaddlqq_u8(bytes1), bytes2));

                /*
                * Vertically add the bytes for s2.
                */
                v_column_sum_1 = vaddwqq_u8(v_column_sum_1, vgetqq_low_u8(bytes1));
                v_column_sum_2 = vaddwqq_u8(v_column_sum_2, vgetqq_high_u8(bytes1));
                v_column_sum_3 = vaddwqq_u8(v_column_sum_3, vgetqq_low_u8(bytes2));
                v_column_sum_4 = vaddwqq_u8(v_column_sum_4, vgetqq_high_u8(bytes2));

                buf += BLOCK_SIZE;

            } while (--n);

            v_s2 = vshlqq_n_u32(v_s2, 7);

            /*
            * Multiply-add bytes by [ 32, 31, 30, ... ] for s2.
            */
            v_s2 = vmlalqq_u16(v_s2, vgetqq_low_u16(v_column_sum_1), constants_512[0]);
            v_s2 = vmlalqq_u16(v_s2, vgetqq_high_u16(v_column_sum_1), constants_512[1]);
            v_s2 = vmlalqq_u16(v_s2, vgetqq_low_u16(v_column_sum_2), constants_512[2]);
            v_s2 = vmlalqq_u16(v_s2, vgetqq_high_u16(v_column_sum_2), constants_512[3]);
            v_s2 = vmlalqq_u16(v_s2, vgetqq_low_u16(v_column_sum_3), constants_512[4]);
            v_s2 = vmlalqq_u16(v_s2, vgetqq_high_u16(v_column_sum_3), constants_512[5]);
            v_s2 = vmlalqq_u16(v_s2, vgetqq_low_u16(v_column_sum_4), constants_512[6]);
            v_s2 = vmlalqq_u16(v_s2, vgetqq_high_u16(v_column_sum_4), constants_512[7]);

            /*
            * Sum epi32 ints v_s1(s2) and accumulate in s1(s2).
            */
            uint32x8_t sum1x8 = vpadddq_u32(vgetqq_low_u32(v_s1), vgetqq_high_u32(v_s1));
            uint32x4_t sum1x4 = vpaddq_u32(vgetdq_low_u32(sum1x8), vgetdq_high_u32(sum1x8));
            uint32x2_t sum1x2 = vpadd_u32(vget_low_u32(sum1x4), vget_high_u32(sum1x4));
            uint32x8_t sum2x8 = vpadddq_u32(vgetqq_low_u32(v_s2), vgetqq_high_u32(v_s2));
            uint32x4_t sum2x4 = vpaddq_u32(vgetdq_low_u32(sum2x8), vgetdq_high_u32(sum2x8));
            uint32x2_t sum2x2 = vpadd_u32(vget_low_u32(sum2x4), vget_high_u32(sum2x4));
            uint32x2_t s1s2 = vpadd_u32(sum1x2, sum2x2);

            s1 += vget_lane_u32(s1s2, 0);
            s2 += vget_lane_u32(s1s2, 1);

            /*
            * Reduce.
            */
            s1 %= BASE;
            s2 %= BASE;
        }
    }
#endif

#if SWAN_REG_TYPE >= SWAN_REG_256_TYPE
    BLOCK_SIZE = 1 << 6;
    total_remaining_blocks = len / BLOCK_SIZE;
    if (total_remaining_blocks) {
        per_iteration_blocks = NMAX / BLOCK_SIZE;

        uint16x8_t constants_256[8];
        uint16_t mem_256[64];
        for (int i = 0; i < 64; i++) {
            mem_256[i] = 64 - i;
        }
        for (int i = 0; i < 8; i++) {
            constants_256[i] = vld1q_u16(mem_256 + (i << 3));
        }
        while (total_remaining_blocks) {
            // n: how many blocks in each iteration?
            unsigned n = (per_iteration_blocks > total_remaining_blocks) ? total_remaining_blocks : per_iteration_blocks; /* The NMAX constraint. */
            if (n > total_remaining_blocks)
                n = (unsigned)total_remaining_blocks;
            total_remaining_blocks -= n;
            len -= (n * BLOCK_SIZE);

            /*
            * Process n blocks of data. At most NMAX data bytes can be
            * processed before s2 must be reduced modulo BASE.
            */
            uint32_t vals[8] = {0, 0, 0, 0, 0, 0, 0, s1 * n};
            uint32x8_t v_s2 = vld1dq_u32(vals);
            uint32x8_t v_s1 = vdupdq_n_u32(0);

            uint16x16_t v_column_sum_1 = vdupdq_n_u16(0);
            uint16x16_t v_column_sum_2 = vdupdq_n_u16(0);
            uint16x16_t v_column_sum_3 = vdupdq_n_u16(0);
            uint16x16_t v_column_sum_4 = vdupdq_n_u16(0);

            do {
                /*
             * Load 32 input bytes.
             */
                const uint8x32_t bytes1 = vld1dq_u8((uint8_t *)(buf));
                const uint8x32_t bytes2 = vld1dq_u8((uint8_t *)(buf + 32));

                /*
             * Add previous block byte sum to v_s2.
             */
                v_s2 = vadddq_u32(v_s2, v_s1);

                /*
             * Horizontally add the bytes for s1.
             */
                v_s1 = vpadaldq_u16(v_s1, vpadaldq_u8(vpaddldq_u8(bytes1), bytes2));

                /*
             * Vertically add the bytes for s2.
             */
                v_column_sum_1 = vaddwdq_u8(v_column_sum_1, vgetdq_low_u8(bytes1));
                v_column_sum_2 = vaddwdq_u8(v_column_sum_2, vgetdq_high_u8(bytes1));
                v_column_sum_3 = vaddwdq_u8(v_column_sum_3, vgetdq_low_u8(bytes2));
                v_column_sum_4 = vaddwdq_u8(v_column_sum_4, vgetdq_high_u8(bytes2));

                buf += BLOCK_SIZE;

            } while (--n);

            v_s2 = vshldq_n_u32(v_s2, 6);

            /*
         * Multiply-add bytes by [ 32, 31, 30, ... ] for s2.
         */
            v_s2 = vmlaldq_u16(v_s2, vgetdq_low_u16(v_column_sum_1), constants_256[0]);
            v_s2 = vmlaldq_u16(v_s2, vgetdq_high_u16(v_column_sum_1), constants_256[1]);
            v_s2 = vmlaldq_u16(v_s2, vgetdq_low_u16(v_column_sum_2), constants_256[2]);
            v_s2 = vmlaldq_u16(v_s2, vgetdq_high_u16(v_column_sum_2), constants_256[3]);
            v_s2 = vmlaldq_u16(v_s2, vgetdq_low_u16(v_column_sum_3), constants_256[4]);
            v_s2 = vmlaldq_u16(v_s2, vgetdq_high_u16(v_column_sum_3), constants_256[5]);
            v_s2 = vmlaldq_u16(v_s2, vgetdq_low_u16(v_column_sum_4), constants_256[6]);
            v_s2 = vmlaldq_u16(v_s2, vgetdq_high_u16(v_column_sum_4), constants_256[7]);

            /*
            * Sum epi32 ints v_s1(s2) and accumulate in s1(s2).
            */
            uint32x4_t sum1x4 = vpaddq_u32(vgetdq_low_u32(v_s1), vgetdq_high_u32(v_s1));
            uint32x2_t sum1x2 = vpadd_u32(vget_low_u32(sum1x4), vget_high_u32(sum1x4));
            uint32x4_t sum2x4 = vpaddq_u32(vgetdq_low_u32(v_s2), vgetdq_high_u32(v_s2));
            uint32x2_t sum2x2 = vpadd_u32(vget_low_u32(sum2x4), vget_high_u32(sum2x4));
            uint32x2_t s1s2 = vpadd_u32(sum1x2, sum2x2);

            s1 += vget_lane_u32(s1s2, 0);
            s2 += vget_lane_u32(s1s2, 1);

            /*
         * Reduce.
         */
            s1 %= BASE;
            s2 %= BASE;
        }
    }
#endif
    BLOCK_SIZE = 1 << 5;
    total_remaining_blocks = len / BLOCK_SIZE;
    if (total_remaining_blocks) {
        per_iteration_blocks = NMAX / BLOCK_SIZE;

        // Making constants
        uint16x4_t constants_128[8];
        uint16_t mem_128[32];
        for (int i = 0; i < 32; i++) {
            mem_128[i] = 32 - i;
        }
        for (int i = 0; i < 8; i++) {
            constants_128[i] = vld1_u16(mem_128 + (i << 2));
        }

        while (total_remaining_blocks) {
            // n: how many blocks in each iteration?
            unsigned n = (per_iteration_blocks > total_remaining_blocks) ? total_remaining_blocks : per_iteration_blocks; /* The NMAX constraint. */
            if (n > total_remaining_blocks)
                n = (unsigned)total_remaining_blocks;
            total_remaining_blocks -= n;
            len -= (n * BLOCK_SIZE);

            /*
         * Process n blocks of data. At most NMAX data bytes can be
         * processed before s2 must be reduced modulo BASE.
         */
            uint32_t vals[4] = {0, 0, 0, s1 * n};
            uint32x4_t v_s2 = vld1q_u32(vals);
            uint32x4_t v_s1 = vdupq_n_u32(0);

            uint16x8_t v_column_sum_1 = vdupq_n_u16(0);
            uint16x8_t v_column_sum_2 = vdupq_n_u16(0);
            uint16x8_t v_column_sum_3 = vdupq_n_u16(0);
            uint16x8_t v_column_sum_4 = vdupq_n_u16(0);

            do {
                /*
             * Load 32 input bytes.
             */
                const uint8x16_t bytes1 = vld1q_u8((uint8_t *)(buf));
                const uint8x16_t bytes2 = vld1q_u8((uint8_t *)(buf + 16));

                /*
             * Add previous block byte sum to v_s2.
             */
                v_s2 = vaddq_u32(v_s2, v_s1);

                /*
             * Horizontally add the bytes for s1.
             */
                v_s1 = vpadalq_u16(v_s1, vpadalq_u8(vpaddlq_u8(bytes1), bytes2));

                /*
             * Vertically add the bytes for s2.
             */
                v_column_sum_1 = vaddw_u8(v_column_sum_1, vget_low_u8(bytes1));
                v_column_sum_2 = vaddw_u8(v_column_sum_2, vget_high_u8(bytes1));
                v_column_sum_3 = vaddw_u8(v_column_sum_3, vget_low_u8(bytes2));
                v_column_sum_4 = vaddw_u8(v_column_sum_4, vget_high_u8(bytes2));

                buf += BLOCK_SIZE;

            } while (--n);

            v_s2 = vshlq_n_u32(v_s2, 5);

            /*
         * Multiply-add bytes by [ 32, 31, 30, ... ] for s2.
         */
            v_s2 = vmlal_u16(v_s2, vget_low_u16(v_column_sum_1), constants_128[0]);
            v_s2 = vmlal_u16(v_s2, vget_high_u16(v_column_sum_1), constants_128[1]);
            v_s2 = vmlal_u16(v_s2, vget_low_u16(v_column_sum_2), constants_128[2]);
            v_s2 = vmlal_u16(v_s2, vget_high_u16(v_column_sum_2), constants_128[3]);
            v_s2 = vmlal_u16(v_s2, vget_low_u16(v_column_sum_3), constants_128[4]);
            v_s2 = vmlal_u16(v_s2, vget_high_u16(v_column_sum_3), constants_128[5]);
            v_s2 = vmlal_u16(v_s2, vget_low_u16(v_column_sum_4), constants_128[6]);
            v_s2 = vmlal_u16(v_s2, vget_high_u16(v_column_sum_4), constants_128[7]);

            /*
            * Sum epi32 ints v_s1(s2) and accumulate in s1(s2).
            */
            uint32x2_t sum1 = vpadd_u32(vget_low_u32(v_s1), vget_high_u32(v_s1));
            uint32x2_t sum2 = vpadd_u32(vget_low_u32(v_s2), vget_high_u32(v_s2));
            uint32x2_t s1s2 = vpadd_u32(sum1, sum2);

            s1 += vget_lane_u32(s1s2, 0);
            s2 += vget_lane_u32(s1s2, 1);

            /*
         * Reduce.
         */
            s1 %= BASE;
            s2 %= BASE;
        }
    }

    /*
     * Handle leftover data.
     */
    if (len) {
        if (len >= 16) {
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);

            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);

            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);

            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);
            s2 += (s1 += *buf++);

            len -= 16;
        }

        while (len--) {
            s2 += (s1 += *buf++);
        }

        if (s1 >= BASE)
            s1 -= BASE;
        s2 %= BASE;
    }

    /*
     * Return the recombined sums.
     */
    adler32_output->return_value[0] = s1 | (s2 << 16);
}
#endif