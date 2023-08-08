/* Copyright (c) 2014, Google Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#include "scalar.hpp"
#include "chacha20.hpp"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static inline uint32_t CRYPTO_rotl_u32(uint32_t value, int shift) {
    return (value << shift) | (value >> ((-shift) & 31));
}

// QUARTERROUND updates a, b, c, d with a ChaCha "quarter" round.
#define QUARTERROUND(a, b, c, d)             \
    x[a] += x[b];                            \
    x[d] = CRYPTO_rotl_u32(x[d] ^ x[a], 16); \
    x[c] += x[d];                            \
    x[b] = CRYPTO_rotl_u32(x[b] ^ x[c], 12); \
    x[a] += x[b];                            \
    x[d] = CRYPTO_rotl_u32(x[d] ^ x[a], 8);  \
    x[c] += x[d];                            \
    x[b] = CRYPTO_rotl_u32(x[b] ^ x[c], 7);

// chacha_core performs 20 rounds of ChaCha on the input words in
// |input| and writes the 64 output bytes to |output|.
static void chacha_core(uint8_t output[64], const uint32_t input[16]) {

    uint32_t x[16];
    int i;

    uint32_t *my_src = (uint32_t *)input;
    uint32_t *my_dst = x;
    for (int i = 0; i < 16; i++) {
        *my_dst++ = *my_src++;
    }
    for (int i = 20; i > 0; i -= 2) {
        QUARTERROUND(0, 4, 8, 12)
        QUARTERROUND(1, 5, 9, 13)
        QUARTERROUND(2, 6, 10, 14)
        QUARTERROUND(3, 7, 11, 15)
        QUARTERROUND(0, 5, 10, 15)
        QUARTERROUND(1, 6, 11, 12)
        QUARTERROUND(2, 7, 8, 13)
        QUARTERROUND(3, 4, 9, 14)
    }

    for (i = 0; i < 16; ++i) {
        x[i] += input[i];
    }
    my_src = x;
    my_dst = (uint32_t *)output;
    for (int i = 0; i < 16; i++) {
        *my_dst++ = *my_src++;
    }
}

static inline uint32_t CRYPTO_load_u32_le(const void *in) {
    uint32_t v = *((uint32_t *)in);
    return v;
}

/* The following function is the modified version of CRYPTO_chacha_20,
 * provided in the boringssl library. Please refer to chacha.c
 * for the unmodified version in the source library. */
void chacha20_scalar(config_t *config,
                     input_t *input,
                     output_t *output) {
    chacha20_config_t *chacha20_config = (chacha20_config_t *)config;
    chacha20_input_t *chacha20_input = (chacha20_input_t *)input;
    chacha20_output_t *chacha20_output = (chacha20_output_t *)output;

    uint8_t *out = chacha20_output->out;
    const uint8_t *in = chacha20_input->in;
    size_t in_len = chacha20_config->in_len;
    const uint8_t *key = chacha20_input->key;
    const uint8_t *nonce = chacha20_input->nonce;
    uint32_t counter = chacha20_config->counter;

    uint32_t input_buffer[16];
    uint8_t output_buffer[64];
    size_t todo, i;

    input_buffer[0] = CRYPTO_load_u32_le(sigma + 0);
    input_buffer[1] = CRYPTO_load_u32_le(sigma + 4);
    input_buffer[2] = CRYPTO_load_u32_le(sigma + 8);
    input_buffer[3] = CRYPTO_load_u32_le(sigma + 12);

    input_buffer[4] = CRYPTO_load_u32_le(key + 0);
    input_buffer[5] = CRYPTO_load_u32_le(key + 4);
    input_buffer[6] = CRYPTO_load_u32_le(key + 8);
    input_buffer[7] = CRYPTO_load_u32_le(key + 12);

    input_buffer[8] = CRYPTO_load_u32_le(key + 16);
    input_buffer[9] = CRYPTO_load_u32_le(key + 20);
    input_buffer[10] = CRYPTO_load_u32_le(key + 24);
    input_buffer[11] = CRYPTO_load_u32_le(key + 28);

    input_buffer[12] = counter;
    input_buffer[13] = CRYPTO_load_u32_le(nonce + 0);
    input_buffer[14] = CRYPTO_load_u32_le(nonce + 4);
    input_buffer[15] = CRYPTO_load_u32_le(nonce + 8);

    while (in_len > 0) {
        todo = sizeof(output_buffer);
        if (in_len < todo) {
            todo = in_len;
        }

        chacha_core(output_buffer, input_buffer);
        for (i = 0; i < todo; i++) {
            out[i] = in[i] ^ output_buffer[i];
        }

        out += todo;
        in += todo;
        in_len -= todo;

        input_buffer[12]++;
    }
}