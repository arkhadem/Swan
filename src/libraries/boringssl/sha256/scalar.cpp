// This file is taken from putty
// There is no C/++ implementation of SHA256 in boringssl

/* PuTTY is copyright 1997-2022 Simon Tatham.
 * 
 * Portions copyright Robert de Bath, Joris van Rantwijk, Delian
 * Delchev, Andreas Schultz, Jeroen Massar, Wez Furlong, Nicolas Barry,
 * Justin Bradford, Ben Harris, Malcolm Smith, Ahmad Khalifa, Markus
 * Kuhn, Colin Watson, Christopher Staite, Lorenz Diener, Christian
 * Brabandt, Jeff Smith, Pavel Kryukov, Maxim Kuznetsov, Svyatoslav
 * Kuzmich, Nico Williams, Viktor Dukhovni, Josh Dersch, Lars Brinkhoff,
 * and CORE SDI S.A.
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "scalar.hpp"
#include "sha256.hpp"

static inline uint32_t ror(uint32_t x, unsigned y) {
    return (x << (31 & -y)) | (x >> (31 & y));
}

static inline uint32_t Ch(uint32_t ctrl, uint32_t if1, uint32_t if0) {
    return if0 ^ (ctrl & (if1 ^ if0));
}

static inline uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) | (z & (x | y));
}

static inline uint32_t Sigma_0(uint32_t x) {
    return ror(x, 2) ^ ror(x, 13) ^ ror(x, 22);
}

static inline uint32_t Sigma_1(uint32_t x) {
    return ror(x, 6) ^ ror(x, 11) ^ ror(x, 25);
}

static inline uint32_t sigma_0(uint32_t x) {
    return ror(x, 7) ^ ror(x, 18) ^ (x >> 3);
}

static inline uint32_t sigma_1(uint32_t x) {
    return ror(x, 17) ^ ror(x, 19) ^ (x >> 10);
}

static inline void sha256_sw_round(
    unsigned round_index, const uint32_t *schedule,
    uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,
    uint32_t *e, uint32_t *f, uint32_t *g, uint32_t *h) {
    uint32_t t1 = *h + Sigma_1(*e) + Ch(*e, *f, *g) +
                  sha256_round_constants[round_index] + schedule[round_index];

    uint32_t t2 = Sigma_0(*a) + Maj(*a, *b, *c);

    *d += t1;
    *h = t1 + t2;
}

static inline uint32_t GET_32BIT_MSB_FIRST(const void *vp) {
    const uint8_t *p = (const uint8_t *)vp;
    return (((uint32_t)p[3]) | ((uint32_t)p[2] << 8) |
            ((uint32_t)p[1] << 16) | ((uint32_t)p[0] << 24));
}

static void sha256_sw_block(uint32_t *core, const uint8_t *block) {
    uint32_t w[SHA256_ROUNDS];
    uint32_t a, b, c, d, e, f, g, h;

    for (size_t t = 0; t < 16; t++)
        w[t] = GET_32BIT_MSB_FIRST(block + 4 * t);

    for (size_t t = 16; t < SHA256_ROUNDS; t++)
        w[t] = sigma_1(w[t - 2]) + w[t - 7] + sigma_0(w[t - 15]) + w[t - 16];

    a = core[0];
    b = core[1];
    c = core[2];
    d = core[3];
    e = core[4];
    f = core[5];
    g = core[6];
    h = core[7];

    for (size_t t = 0; t < SHA256_ROUNDS; t += 8) {
        sha256_sw_round(t + 0, w, &a, &b, &c, &d, &e, &f, &g, &h);
        sha256_sw_round(t + 1, w, &h, &a, &b, &c, &d, &e, &f, &g);
        sha256_sw_round(t + 2, w, &g, &h, &a, &b, &c, &d, &e, &f);
        sha256_sw_round(t + 3, w, &f, &g, &h, &a, &b, &c, &d, &e);
        sha256_sw_round(t + 4, w, &e, &f, &g, &h, &a, &b, &c, &d);
        sha256_sw_round(t + 5, w, &d, &e, &f, &g, &h, &a, &b, &c);
        sha256_sw_round(t + 6, w, &c, &d, &e, &f, &g, &h, &a, &b);
        sha256_sw_round(t + 7, w, &b, &c, &d, &e, &f, &g, &h, &a);
    }

    core[0] += a;
    core[1] += b;
    core[2] += c;
    core[3] += d;
    core[4] += e;
    core[5] += f;
    core[6] += g;
    core[7] += h;
}

void sha256_scalar(config_t *config,
                   input_t *input,
                   output_t *output) {
    sha256_config_t *sha256_config = (sha256_config_t *)config;
    sha256_input_t *sha256_input = (sha256_input_t *)input;
    sha256_output_t *sha256_output = (sha256_output_t *)output;

    // len in bytes
    int len = sha256_config->len;

    unsigned char *input_buff = sha256_input->input;
    uint32_t *output_buff = sha256_output->output;

    for (int i = 0; i < 8; i++)
        output_buff[i] = sha256_initial_state[i];

    while (len > 0) {
        // This is the entry point to putty's SHA256 function
        sha256_sw_block(output_buff, input_buff);
        input_buff += 64;
        len -= 64;
    }
}