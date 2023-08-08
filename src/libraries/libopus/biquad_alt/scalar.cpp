/***********************************************************************
Copyright (c) 2006-2011, Skype Limited. All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
- Neither the name of Internet Society, IETF or IETF Trust, nor the
names of specific contributors, may be used to endorse or promote
products derived from this software without specific prior written
permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/

/*                                                                      *
 * silk_biquad_alt.c                                                    *
 *                                                                      *
 * Second order ARMA filter                                             *
 * Can handle slowly varying filter coefficients                        *
 *                                                                      */

#include "scalar.hpp"
#include "biquad_alt.hpp"

/* The following function is the modified version of silk_biquad_alt_stride2_c,
 * provided in the libopus library. Please refer to biquad_alt.c
 * for the unmodified version in the source library. */
void biquad_alt_scalar(config_t *config,
                       input_t *input,
                       output_t *output) {

    biquad_alt_config_t *biquad_alt_config = (biquad_alt_config_t *)config;
    biquad_alt_input_t *biquad_alt_input = (biquad_alt_input_t *)input;
    biquad_alt_output_t *biquad_alt_output = (biquad_alt_output_t *)output;

    uint32_t block_count = biquad_alt_config->block_count;
    opus_int32 len = biquad_alt_config->len;      /* I     signal length (must be even)                               */
    opus_int32 *B_Q28 = biquad_alt_config->B_Q28; /* I     MA coefficients [3]                                        */
    opus_int32 *A_Q28 = biquad_alt_config->A_Q28; /* I     AR coefficients [2]                                        */

    /* DIRECT FORM II TRANSPOSED (uses 2 element state vector) */
    opus_int k;
    opus_int32 A0_U_Q28, A0_L_Q28, A1_U_Q28, A1_L_Q28, out32_Q14[2];

    /* Negate A_Q28 values and split in two parts */
    A0_L_Q28 = (-A_Q28[0]) & 0x00003FFF;   /* lower part */
    A0_U_Q28 = silk_RSHIFT(-A_Q28[0], 14); /* upper part */
    A1_L_Q28 = (-A_Q28[1]) & 0x00003FFF;   /* lower part */
    A1_U_Q28 = silk_RSHIFT(-A_Q28[1], 14); /* upper part */

    for (unsigned block = 0; block < block_count; ++block) {
        opus_int16 *in = biquad_alt_input->in[block];        /* I     input signal                                               */
        opus_int32 *S_in = biquad_alt_input->S_in[block];    /* I/O   State vector [4]                                           */
        opus_int16 *out = biquad_alt_output->out[block];     /* O     output signal                                              */
        opus_int32 *S_out = biquad_alt_output->S_out[block]; /* I/O   State vector [4]                                           */

        S_out[0] = S_in[0];
        S_out[1] = S_in[1];
        S_out[2] = S_in[2];
        S_out[3] = S_in[3];

        for (k = 0; k < len; k++) {
            /* S_out[ 0 ], S_out[ 1 ], S_out[ 2 ], S_out[ 3 ]: Q12 */
            out32_Q14[0] = silk_LSHIFT(silk_SMLAWB(S_out[0], B_Q28[0], in[2 * k + 0]), 2);
            out32_Q14[1] = silk_LSHIFT(silk_SMLAWB(S_out[2], B_Q28[0], in[2 * k + 1]), 2);

            S_out[0] = S_out[1] + silk_RSHIFT_ROUND(silk_SMULWB(out32_Q14[0], A0_L_Q28), 14);
            S_out[2] = S_out[3] + silk_RSHIFT_ROUND(silk_SMULWB(out32_Q14[1], A0_L_Q28), 14);
            S_out[0] = silk_SMLAWB(S_out[0], out32_Q14[0], A0_U_Q28);
            S_out[2] = silk_SMLAWB(S_out[2], out32_Q14[1], A0_U_Q28);
            S_out[0] = silk_SMLAWB(S_out[0], B_Q28[1], in[2 * k + 0]);
            S_out[2] = silk_SMLAWB(S_out[2], B_Q28[1], in[2 * k + 1]);

            S_out[1] = silk_RSHIFT_ROUND(silk_SMULWB(out32_Q14[0], A1_L_Q28), 14);
            S_out[3] = silk_RSHIFT_ROUND(silk_SMULWB(out32_Q14[1], A1_L_Q28), 14);
            S_out[1] = silk_SMLAWB(S_out[1], out32_Q14[0], A1_U_Q28);
            S_out[3] = silk_SMLAWB(S_out[3], out32_Q14[1], A1_U_Q28);
            S_out[1] = silk_SMLAWB(S_out[1], B_Q28[2], in[2 * k + 0]);
            S_out[3] = silk_SMLAWB(S_out[3], B_Q28[2], in[2 * k + 1]);

            /* Scale back to Q0 and saturate */
            out[2 * k + 0] = (opus_int16)silk_SAT16(silk_RSHIFT(out32_Q14[0] + (1 << 14) - 1, 14));
            out[2 * k + 1] = (opus_int16)silk_SAT16(silk_RSHIFT(out32_Q14[1] + (1 << 14) - 1, 14));
        }
    }
}