/***********************************************************************
Copyright (c) 2017 Google Inc., Jean-Marc Valin
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

#include "scalar.hpp"
#include "swan.hpp"
#include "warped_autocorrelation.hpp"

/* The following function is the modified version of silk_warped_autocorrelation_FIX_c,
 * provided in the libopus library. Please refer to warped_autocorrelation_FIX.c
 * for the unmodified version in the source library. */
void warped_autocorrelation_scalar(config_t *config,
                                   input_t *input,
                                   output_t *output) {

    warped_autocorrelation_config_t *warped_autocorrelation_config = (warped_autocorrelation_config_t *)config;
    warped_autocorrelation_input_t *warped_autocorrelation_input = (warped_autocorrelation_input_t *)input;
    warped_autocorrelation_output_t *warped_autocorrelation_output = (warped_autocorrelation_output_t *)output;

    // config
    const opus_int warping_Q16 = warped_autocorrelation_config->warping_Q16; /* I    Warping coefficient (1)                                                  */
    const opus_int length = warped_autocorrelation_config->length;           /* I    Length of input_data                                                        */
    const opus_int order = warped_autocorrelation_config->order;             /* I    Correlation order (24)                                                    */
    uint32_t block_count = warped_autocorrelation_config->block_count;

    for (unsigned block = 0; block < block_count; ++block) {

        // input
        const opus_int16 *input_data = warped_autocorrelation_input->input_data[block]; /* I    Input data to correlate [length]                                            */

        // outputs
        opus_int32 *corr = warped_autocorrelation_output->corr[block]; /* O    Result [order + 1]                                                          */
        opus_int *scale = warped_autocorrelation_output->scale[block]; /* O    Scaling of the correlation vector [1]                                       */

        opus_int n, i, lsh;
        opus_int32 tmp1_QS, tmp2_QS;
        opus_int32 state_QS[MAX_SHAPE_LPC_ORDER + 1] = {0};
        opus_int64 corr_QC[MAX_SHAPE_LPC_ORDER + 1] = {0};

        /* Order must be even */
        SWAN_ASSERT((order & 1) == 0)
        SWAN_ASSERT(2 * QS - QC >= 0)

        /* Loop over samples */
        for (n = 0; n < length; n++) {
            tmp1_QS = silk_LSHIFT32((opus_int32)input_data[n], QS);
            /* Loop over allpass sections */
            for (i = 0; i < order; i += 2) {
                /* Output of allpass section */
                tmp2_QS = silk_SMLAWB(state_QS[i], state_QS[i + 1] - tmp1_QS, warping_Q16);
                state_QS[i] = tmp1_QS;
                corr_QC[i] += silk_RSHIFT64(silk_SMULL(tmp1_QS, state_QS[0]), 2 * QS - QC);
                /* Output of allpass section */
                tmp1_QS = silk_SMLAWB(state_QS[i + 1], state_QS[i + 2] - tmp2_QS, warping_Q16);
                state_QS[i + 1] = tmp2_QS;
                corr_QC[i + 1] += silk_RSHIFT64(silk_SMULL(tmp2_QS, state_QS[0]), 2 * QS - QC);
            }
            state_QS[order] = tmp1_QS;
            corr_QC[order] += silk_RSHIFT64(silk_SMULL(tmp1_QS, state_QS[0]), 2 * QS - QC);
        }

        lsh = silk_CLZ64(corr_QC[0]) - 35;
        lsh = silk_LIMIT(lsh, -12 - QC, 30 - QC);
        *scale = -(QC + lsh);
        SWAN_ASSERT(*scale >= -30 && *scale <= 12)
        if (lsh >= 0) {
            for (i = 0; i < order + 1; i++) {
                corr[i] = (opus_int32)silk_CHECK_FIT32(silk_LSHIFT64(corr_QC[i], lsh));
            }
        } else {
            for (i = 0; i < order + 1; i++) {
                corr[i] = (opus_int32)silk_CHECK_FIT32(silk_RSHIFT64(corr_QC[i], -lsh));
            }
        }
        SWAN_ASSERT(corr_QC[0] >= 0)
    }
}