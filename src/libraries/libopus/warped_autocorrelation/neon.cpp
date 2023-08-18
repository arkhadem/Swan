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

#include "neon.hpp"
#include "swan.hpp"
#include "warped_autocorrelation.hpp"

static inline void calc_corr(const opus_int32 *const input_QS, opus_int64 *const corr_QC, const opus_int offset, const int32x4_t state_QS_s32x4) {
    int64x2_t corr_QC_s64x2[2], t_s64x2[2];
    const int32x4_t input_QS_s32x4 = vld1q_s32(input_QS + offset);
#ifdef SWAN_SIMULATION
    corr_QC_s64x2[0] = vld1q_s64(corr_QC + offset + 0);
    corr_QC_s64x2[1] = vld1q_s64(corr_QC + offset + 2);
#else
    corr_QC_s64x2[0] = vld1q_s64((const long *)(corr_QC + offset + 0));
    corr_QC_s64x2[1] = vld1q_s64((const long *)(corr_QC + offset + 2));
#endif

    t_s64x2[0] = vmull_s32(vget_low_s32(state_QS_s32x4), vget_low_s32(input_QS_s32x4));
    t_s64x2[1] = vmull_s32(vget_high_s32(state_QS_s32x4), vget_high_s32(input_QS_s32x4));
    corr_QC_s64x2[0] = vsraq_n_s64(corr_QC_s64x2[0], t_s64x2[0], 2 * QS - QC);
    corr_QC_s64x2[1] = vsraq_n_s64(corr_QC_s64x2[1], t_s64x2[1], 2 * QS - QC);

#ifdef SWAN_SIMULATION
    vst1q_s64(corr_QC + offset + 0, corr_QC_s64x2[0]);
    vst1q_s64(corr_QC + offset + 2, corr_QC_s64x2[1]);
#else
    vst1q_s64((long *)(corr_QC + offset + 0), corr_QC_s64x2[0]);
    vst1q_s64((long *)(corr_QC + offset + 2), corr_QC_s64x2[1]);
#endif
}

static inline int32x4_t calc_state(const int32x4_t state_QS0_s32x4, const int32x4_t state_QS0_1_s32x4, const int32x4_t state_QS1_1_s32x4, const int32x4_t warping_Q16_s32x4) {
    int32x4_t t_s32x4 = vsubq_s32(state_QS0_s32x4, state_QS0_1_s32x4);
    t_s32x4 = vqdmulhq_s32(t_s32x4, warping_Q16_s32x4);
    return vaddq_s32(state_QS1_1_s32x4, t_s32x4);
}

/* The following function is the modified version of silk_warped_autocorrelation_FIX_neon,
 * provided in the libopus library. Please refer to warped_autocorrelation_FIX_neon_intr.c
 * for the unmodified version in the source library. */
void warped_autocorrelation_neon(config_t *config,
                                 input_t *input,
                                 output_t *output) {

    warped_autocorrelation_config_t *warped_autocorrelation_config = (warped_autocorrelation_config_t *)config;
    warped_autocorrelation_input_t *warped_autocorrelation_input = (warped_autocorrelation_input_t *)input;
    warped_autocorrelation_output_t *warped_autocorrelation_output = (warped_autocorrelation_output_t *)output;

    // config
    const opus_int warping_Q16 = warped_autocorrelation_config->warping_Q16; /* I    Warping coefficient (1)                                                  */
    const opus_int length = warped_autocorrelation_config->length;           /* I    Length of input_data                                                        */
    const opus_int order = warped_autocorrelation_config->order;             /* I    Correlation order (24)                                                    */
    opus_int32 *input_QST = warped_autocorrelation_config->input_QST;
    opus_int32 *state = warped_autocorrelation_config->state;
    uint32_t block_count = warped_autocorrelation_config->block_count;

    for (unsigned block = 0; block < block_count; ++block) {

        // input
        const opus_int16 *input_data = warped_autocorrelation_input->input_data[block]; /* I    Input data to correlate [length]                                            */

        // outputs
        opus_int32 *corr = warped_autocorrelation_output->corr[block]; /* O    Result [order + 1]                                                          */
        opus_int *scale = warped_autocorrelation_output->scale[block]; /* O    Scaling of the correlation vector [1]                                       */

        SWAN_ASSERT((MAX_SHAPE_LPC_ORDER <= 24) && (order >= 6))

        opus_int n, i, lsh;
        opus_int64 corr_QC[MAX_SHAPE_LPC_ORDER + 1] = {0}; /* In reverse order */
        opus_int64 corr_QC_orderT;
        int64x2_t lsh_s64x2;
        const opus_int orderT = (order + 3) & ~3;
        opus_int64 *corr_QCT;
        opus_int32 *input_QS;

        /* Order must be even */
        SWAN_ASSERT((order & 1) == 0)
        SWAN_ASSERT(2 * QS - QC >= 0)

        /* The additional +4 is to ensure a later vld1q_s32 call does not overflow.               */
        /* Strictly, only +3 is needed but +4 simplifies initialization using the 4x32 neon load. */

        input_QS = input_QST;
        /* input_QS has zero paddings in the beginning and end. */
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;

        /* Loop over samples */
        for (n = 0; n < length - 7; n += 8, input_QS += 8) {
            const int16x8_t t0_s16x4 = vld1q_s16(input_data + n);
            vst1q_s32(input_QS + 0, vshll_n_s16(vget_low_s16(t0_s16x4), QS));
            vst1q_s32(input_QS + 4, vshll_n_s16(vget_high_s16(t0_s16x4), QS));
        }
        for (; n < length; n++, input_QS++) {
            input_QS[0] = silk_LSHIFT32((opus_int32)input_data[n], QS);
        }
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS += 4;
        vst1q_s32(input_QS, vdupq_n_s32(0));
        input_QS = input_QST + MAX_SHAPE_LPC_ORDER - orderT;

        {
            const int32x4_t warping_Q16_s32x4 = vdupq_n_s32(warping_Q16 << 15);
            const opus_int32 *in = input_QS + orderT;
            opus_int o = orderT;
            int32x4_t state_QS_s32x4[3][2];

            /* The additional +4 is to ensure a later vld1q_s32 call does not overflow. */
            state_QS_s32x4[2][1] = vdupq_n_s32(0);

            /* Calculate 8 taps of all inputs in each loop. */
            do {
                state_QS_s32x4[0][0] = state_QS_s32x4[0][1] =
                    state_QS_s32x4[1][0] = state_QS_s32x4[1][1] = vdupq_n_s32(0);
                n = 0;
                do {
                    calc_corr(input_QS + n, corr_QC, o - 8, state_QS_s32x4[0][0]);
                    calc_corr(input_QS + n, corr_QC, o - 4, state_QS_s32x4[0][1]);
                    state_QS_s32x4[2][1] = vld1q_s32(in + n);
                    vst1q_lane_s32(state + n, state_QS_s32x4[0][0], 0);
                    state_QS_s32x4[2][0] = vextq_s32(state_QS_s32x4[0][0], state_QS_s32x4[0][1], 1);
                    state_QS_s32x4[2][1] = vextq_s32(state_QS_s32x4[0][1], state_QS_s32x4[2][1], 1);
                    state_QS_s32x4[0][0] = calc_state(state_QS_s32x4[0][0], state_QS_s32x4[2][0], state_QS_s32x4[1][0], warping_Q16_s32x4);
                    state_QS_s32x4[0][1] = calc_state(state_QS_s32x4[0][1], state_QS_s32x4[2][1], state_QS_s32x4[1][1], warping_Q16_s32x4);
                    state_QS_s32x4[1][0] = state_QS_s32x4[2][0];
                    state_QS_s32x4[1][1] = state_QS_s32x4[2][1];
                } while (++n < (length + order));
                in = state;
                o -= 8;
            } while (o > 4);

            if (o) {
                /* Calculate the last 4 taps of all inputs. */
                opus_int32 *stateT = state;
                SWAN_ASSERT(o == 4)
                state_QS_s32x4[0][0] = state_QS_s32x4[1][0] = vdupq_n_s32(0);
                n = length + order;
                do {
                    calc_corr(input_QS, corr_QC, 0, state_QS_s32x4[0][0]);
                    state_QS_s32x4[2][0] = vld1q_s32(stateT);
                    vst1q_lane_s32(stateT, state_QS_s32x4[0][0], 0);
                    state_QS_s32x4[2][0] = vextq_s32(state_QS_s32x4[0][0], state_QS_s32x4[2][0], 1);
                    state_QS_s32x4[0][0] = calc_state(state_QS_s32x4[0][0], state_QS_s32x4[2][0], state_QS_s32x4[1][0], warping_Q16_s32x4);
                    state_QS_s32x4[1][0] = state_QS_s32x4[2][0];
                    input_QS++;
                    stateT++;
                } while (--n);
            }
        }

        {
            const opus_int16 *inputT = input_data;
            int32x4_t t_s32x4;
            int64x1_t t_s64x1;
            int64x2_t t_s64x2 = vdupq_n_s64(0);
            for (n = 0; n <= length - 8; n += 8) {
                int16x8_t input_s16x8 = vld1q_s16(inputT);
                t_s32x4 = vmull_s16(vget_low_s16(input_s16x8), vget_low_s16(input_s16x8));
                t_s32x4 = vmlal_s16(t_s32x4, vget_high_s16(input_s16x8), vget_high_s16(input_s16x8));
                t_s64x2 = vaddw_s32(t_s64x2, vget_low_s32(t_s32x4));
                t_s64x2 = vaddw_s32(t_s64x2, vget_high_s32(t_s32x4));
                inputT += 8;
            }
            t_s64x1 = vadd_s64(vget_low_s64(t_s64x2), vget_high_s64(t_s64x2));
            corr_QC_orderT = vget_lane_s64(t_s64x1, 0);
            for (; n < length; n++) {
                corr_QC_orderT += silk_SMULL(input_data[n], input_data[n]);
            }
            corr_QC_orderT = silk_LSHIFT64(corr_QC_orderT, QC);
            corr_QC[orderT] = corr_QC_orderT;
        }

        corr_QCT = corr_QC + orderT - order;
        lsh = silk_CLZ64(corr_QC_orderT) - 35;
        lsh = silk_LIMIT(lsh, -12 - QC, 30 - QC);

        *scale = -(QC + lsh);
        SWAN_ASSERT(*scale >= -30 && *scale <= 12);
        lsh_s64x2 = vdupq_n_s64(lsh);
        for (i = 0; i <= order - 3; i += 4) {
            int32x4_t corr_s32x4;
            int64x2_t corr_QC0_s64x2, corr_QC1_s64x2;
#ifdef SWAN_SIMULATION
            corr_QC0_s64x2 = vld1q_s64(corr_QCT + i);
            corr_QC1_s64x2 = vld1q_s64(corr_QCT + i + 2);
#else
            corr_QC0_s64x2 = vld1q_s64((const long *)(corr_QCT + i));
            corr_QC1_s64x2 = vld1q_s64((const long *)(corr_QCT + i + 2));
#endif
            corr_QC0_s64x2 = vshlq_s64(corr_QC0_s64x2, lsh_s64x2);
            corr_QC1_s64x2 = vshlq_s64(corr_QC1_s64x2, lsh_s64x2);
            corr_s32x4 = vcombine_s32(vmovn_s64(corr_QC1_s64x2), vmovn_s64(corr_QC0_s64x2));
            corr_s32x4 = vrev64q_s32(corr_s32x4);
            vst1q_s32(corr + order - i - 3, corr_s32x4);
        }
        if (lsh >= 0) {
            for (; i < order + 1; i++) {
                corr[order - i] = (opus_int32)silk_CHECK_FIT32(silk_LSHIFT64(corr_QCT[i], lsh));
            }
        } else {
            for (; i < order + 1; i++) {
                corr[order - i] = (opus_int32)silk_CHECK_FIT32(silk_RSHIFT64(corr_QCT[i], -lsh));
            }
        }
        SWAN_ASSERT(corr_QCT[order] >= 0)
    }
}