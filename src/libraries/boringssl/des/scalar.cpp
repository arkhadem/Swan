/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 *
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 *
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 *
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.] */

#include "scalar.hpp"
#include "des.hpp"
#include <stdio.h>

static inline uint32_t CRYPTO_rotr_u32(uint32_t value, int shift) {
    return (value >> shift) | (value << ((-shift) & 31));
}

#define PERM_OP(a, b, t, n, m)              \
    do {                                    \
        (t) = ((((a) >> (n)) ^ (b)) & (m)); \
        (b) ^= (t);                         \
        (a) ^= ((t) << (n));                \
    } while (0)

#define IP(l, r)                            \
    do {                                    \
        uint32_t tt;                        \
        PERM_OP(r, l, tt, 4, 0x0f0f0f0fL);  \
        PERM_OP(l, r, tt, 16, 0x0000ffffL); \
        PERM_OP(r, l, tt, 2, 0x33333333L);  \
        PERM_OP(l, r, tt, 8, 0x00ff00ffL);  \
        PERM_OP(r, l, tt, 1, 0x55555555L);  \
    } while (0)

#define FP(l, r)                            \
    do {                                    \
        uint32_t tt;                        \
        PERM_OP(l, r, tt, 1, 0x55555555L);  \
        PERM_OP(r, l, tt, 8, 0x00ff00ffL);  \
        PERM_OP(l, r, tt, 2, 0x33333333L);  \
        PERM_OP(r, l, tt, 16, 0x0000ffffL); \
        PERM_OP(l, r, tt, 4, 0x0f0f0f0fL);  \
    } while (0)

#define LOAD_DATA(SK0, SK1, R, S, u, t) \
    do {                                \
        (u) = (R) ^ SK0;                \
        (t) = (R) ^ SK1;                \
    } while (0)

#define D_ENCRYPT(SK, LL, R, S)                        \
    do {                                               \
        (SK0) = SK[(S)*2];                             \
        (SK1) = SK[(S)*2 + 1];                         \
        (u) = (R) ^ (SK0);                             \
        (t) = (R) ^ (SK1);                             \
        t = CRYPTO_rotr_u32(t, 4);                     \
        (LL) ^=                                        \
            ((u >> 2L) & 0x3f) ^ ((u >> 10L) & 0x3f) ^ \
            ((u >> 18L) & 0x3f) ^                      \
            ((u >> 26L) & 0x3f) ^ ((t >> 2L) & 0x3f) ^ \
            ((t >> 10L) & 0x3f) ^                      \
            ((t >> 18L) & 0x3f) ^ ((t >> 26L) & 0x3f); \
    } while (0)

/* The following function is the modified version of DES_encrypt1,
 * provided in the boringssl library. Please refer to des.c
 * for the unmodified version in the source library. */
void des_scalar(config_t *config,
                input_t *input,
                output_t *output) {
    des_config_t *des_config = (des_config_t *)config;
    des_input_t *des_input = (des_input_t *)input;
    des_output_t *des_output = (des_output_t *)output;

    int block_count = des_config->block_count;

    uint32_t *state_in = (uint32_t *)des_input->state_in;
    uint32_t *state_out = (uint32_t *)des_output->state_out;
    uint32_t *subkeys = (uint32_t *)des_config->subkeys;

    uint32_t l, r, t, u, SK0, SK1;

    for (int blk = 0; blk < block_count; blk++) {

        r = state_in[0];
        l = state_in[1];

        IP(r, l);
        // Things have been modified so that the initial rotate is done outside
        // the loop.  This required the DES_SPtrans values in sp.h to be
        // rotated 1 bit to the right. One perl script later and things have a
        // 5% speed up on a sparc2. Thanks to Richard Outerbridge
        // <71755.204@CompuServe.COM> for pointing this out.
        // clear the top bits on machines with 8byte longs
        // shift left by 2

        r = CRYPTO_rotr_u32(r, 29);
        l = CRYPTO_rotr_u32(l, 29);

        // I don't know if it is worth the effort of loop unrolling the
        // inner loop
        D_ENCRYPT(subkeys, l, r, 0);
        D_ENCRYPT(subkeys, r, l, 1);
        D_ENCRYPT(subkeys, l, r, 2);
        D_ENCRYPT(subkeys, r, l, 3);
        D_ENCRYPT(subkeys, l, r, 4);
        D_ENCRYPT(subkeys, r, l, 5);
        D_ENCRYPT(subkeys, l, r, 6);
        D_ENCRYPT(subkeys, r, l, 7);
        D_ENCRYPT(subkeys, l, r, 8);
        D_ENCRYPT(subkeys, r, l, 9);
        D_ENCRYPT(subkeys, l, r, 10);
        D_ENCRYPT(subkeys, r, l, 11);
        D_ENCRYPT(subkeys, l, r, 12);
        D_ENCRYPT(subkeys, r, l, 13);
        D_ENCRYPT(subkeys, l, r, 14);
        D_ENCRYPT(subkeys, r, l, 15);

        // rotate and clear the top bits on machines with 8byte longs
        l = CRYPTO_rotr_u32(l, 3);
        r = CRYPTO_rotr_u32(r, 3);

        FP(r, l);
        state_out[0] = l;
        state_out[1] = r;
        state_in += 2;
        state_out += 2;
    }
}