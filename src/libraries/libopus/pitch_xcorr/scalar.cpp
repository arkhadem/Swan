/* Copyright (c) 2007-2008 CSIRO
   Copyright (c) 2007-2009 Xiph.Org Foundation
   Written by Jean-Marc Valin */

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

#include "scalar.hpp"
#include "pitch_xcorr.hpp"

static inline void xcorr_kernel_c(const opus_val16 *x, const opus_val16 *y, opus_val32 sum[4], int len) {
    int j;
    opus_val16 y_0, y_1, y_2, y_3;
    y_3 = 0; /* gcc doesn't realize that y_3 can't be used uninitialized */
    y_0 = *y++;
    y_1 = *y++;
    y_2 = *y++;
    for (j = 0; j < len - 3; j += 4) {
        opus_val16 tmp;
        tmp = *x++;
        y_3 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_0);
        sum[1] = MAC16_16(sum[1], tmp, y_1);
        sum[2] = MAC16_16(sum[2], tmp, y_2);
        sum[3] = MAC16_16(sum[3], tmp, y_3);
        tmp = *x++;
        y_0 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_1);
        sum[1] = MAC16_16(sum[1], tmp, y_2);
        sum[2] = MAC16_16(sum[2], tmp, y_3);
        sum[3] = MAC16_16(sum[3], tmp, y_0);
        tmp = *x++;
        y_1 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_2);
        sum[1] = MAC16_16(sum[1], tmp, y_3);
        sum[2] = MAC16_16(sum[2], tmp, y_0);
        sum[3] = MAC16_16(sum[3], tmp, y_1);
        tmp = *x++;
        y_2 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_3);
        sum[1] = MAC16_16(sum[1], tmp, y_0);
        sum[2] = MAC16_16(sum[2], tmp, y_1);
        sum[3] = MAC16_16(sum[3], tmp, y_2);
    }
    if (j++ < len) {
        opus_val16 tmp = *x++;
        y_3 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_0);
        sum[1] = MAC16_16(sum[1], tmp, y_1);
        sum[2] = MAC16_16(sum[2], tmp, y_2);
        sum[3] = MAC16_16(sum[3], tmp, y_3);
    }
    if (j++ < len) {
        opus_val16 tmp = *x++;
        y_0 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_1);
        sum[1] = MAC16_16(sum[1], tmp, y_2);
        sum[2] = MAC16_16(sum[2], tmp, y_3);
        sum[3] = MAC16_16(sum[3], tmp, y_0);
    }
    if (j < len) {
        opus_val16 tmp = *x++;
        y_1 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_2);
        sum[1] = MAC16_16(sum[1], tmp, y_3);
        sum[2] = MAC16_16(sum[2], tmp, y_0);
        sum[3] = MAC16_16(sum[3], tmp, y_1);
    }
}

static inline opus_val32 celt_inner_prod_c(const opus_val16 *x, const opus_val16 *y, int N) {
    int i;
    opus_val32 xy = 0;
    for (i = 0; i < N; i++)
        xy = MAC16_16(xy, x[i], y[i]);
    return xy;
}

/* The following function is the modified version of celt_pitch_xcorr_c,
 * provided in the libopus library. Please refer to pitch.c
 * for the unmodified version in the source library. */
void pitch_xcorr_scalar(config_t *config,
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

        for (i = 0; i < max_pitch - 3; i += 4) {
            opus_val32 sum[4] = {0, 0, 0, 0};
            xcorr_kernel_c(x, y + i, sum, len);
            xcorr[i] = sum[0];
            xcorr[i + 1] = sum[1];
            xcorr[i + 2] = sum[2];
            xcorr[i + 3] = sum[3];
        }

        for (; i < max_pitch; i++) {
            opus_val32 sum;
            sum = celt_inner_prod_c(x, y + i, len);
            xcorr[i] = sum;
        }
    }
}