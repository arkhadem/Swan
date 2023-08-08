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

#include "neon.hpp"
#include "sha256.hpp"
#include <arm_neon.h>

typedef struct sha256_neon_core {
    uint32x4_t abcd, efgh;
} sha256_neon_core;

static inline uint32x4_t sha256_neon_load_input(const uint8_t *p) {
    return vreinterpretq_u32_u8(vrev32q_u8(vld1q_u8(p)));
}

static inline sha256_neon_core sha256_neon_round4(sha256_neon_core old, uint32x4_t sched, unsigned round) {
    sha256_neon_core neww;

    uint32x4_t round_input = vaddq_u32(sched, vld1q_u32(sha256_round_constants + round));
    neww.abcd = vsha256hq_u32(old.abcd, old.efgh, round_input);
    neww.efgh = vsha256h2q_u32(old.efgh, old.abcd, round_input);
    return neww;
}

static inline uint32x4_t sha256_neon_schedule_update(uint32x4_t m4, uint32x4_t m3, uint32x4_t m2, uint32x4_t m1) {
    return vsha256su1q_u32(vsha256su0q_u32(m4, m3), m2, m1);
}

static inline void sha256_neon_block(sha256_neon_core *core, const uint8_t *p) {
    uint32x4_t s0, s1, s2, s3;
    sha256_neon_core cr = *core;

    s0 = sha256_neon_load_input(p);
    cr = sha256_neon_round4(cr, s0, 0);
    s1 = sha256_neon_load_input(p + 16);
    cr = sha256_neon_round4(cr, s1, 4);
    s2 = sha256_neon_load_input(p + 32);
    cr = sha256_neon_round4(cr, s2, 8);
    s3 = sha256_neon_load_input(p + 48);
    cr = sha256_neon_round4(cr, s3, 12);
    s0 = sha256_neon_schedule_update(s0, s1, s2, s3);
    cr = sha256_neon_round4(cr, s0, 16);
    s1 = sha256_neon_schedule_update(s1, s2, s3, s0);
    cr = sha256_neon_round4(cr, s1, 20);
    s2 = sha256_neon_schedule_update(s2, s3, s0, s1);
    cr = sha256_neon_round4(cr, s2, 24);
    s3 = sha256_neon_schedule_update(s3, s0, s1, s2);
    cr = sha256_neon_round4(cr, s3, 28);
    s0 = sha256_neon_schedule_update(s0, s1, s2, s3);
    cr = sha256_neon_round4(cr, s0, 32);
    s1 = sha256_neon_schedule_update(s1, s2, s3, s0);
    cr = sha256_neon_round4(cr, s1, 36);
    s2 = sha256_neon_schedule_update(s2, s3, s0, s1);
    cr = sha256_neon_round4(cr, s2, 40);
    s3 = sha256_neon_schedule_update(s3, s0, s1, s2);
    cr = sha256_neon_round4(cr, s3, 44);
    s0 = sha256_neon_schedule_update(s0, s1, s2, s3);
    cr = sha256_neon_round4(cr, s0, 48);
    s1 = sha256_neon_schedule_update(s1, s2, s3, s0);
    cr = sha256_neon_round4(cr, s1, 52);
    s2 = sha256_neon_schedule_update(s2, s3, s0, s1);
    cr = sha256_neon_round4(cr, s2, 56);
    s3 = sha256_neon_schedule_update(s3, s0, s1, s2);
    cr = sha256_neon_round4(cr, s3, 60);

    core->abcd = vaddq_u32(core->abcd, cr.abcd);
    core->efgh = vaddq_u32(core->efgh, cr.efgh);
}

void sha256_neon(config_t *config,
                 input_t *input,
                 output_t *output) {
    sha256_config_t *sha256_config = (sha256_config_t *)config;
    sha256_input_t *sha256_input = (sha256_input_t *)input;
    sha256_output_t *sha256_output = (sha256_output_t *)output;

    // len in bytes
    int len = sha256_config->len;

    unsigned char *input_buff = sha256_input->input;
    uint32_t *output_buff = sha256_output->output;

    sha256_neon_core core;
    core.abcd = vld1q_u32(sha256_initial_state);
    core.efgh = vld1q_u32(sha256_initial_state + 4);

    while (len > 0) {
        // This is the entry point to putty's SHA256 function
        sha256_neon_block(&core, input_buff);
        input_buff += 64;
        len -= 64;
    }

    vst1q_u32(output_buff, core.abcd);
    vst1q_u32(output_buff + 4, core.efgh);
}