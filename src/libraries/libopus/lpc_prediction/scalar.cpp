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

#include "scalar.hpp"
#include "lpc_prediction.hpp"
#include "swan.hpp"
#include <assert.h>

#define QA 24
#define A_LIMIT SILK_FIX_CONST(0.99975, QA)

#define MUL32_FRAC_Q(a32, b32, Q) ((opus_int32)(silk_RSHIFT_ROUND64(silk_SMULL(a32, b32), Q)))

/* Invert int32 value and return result as int32 in a given Q-domain */
static inline opus_int32 silk_INVERSE32_varQ(                      /* O    returns a good approximation of "(1 << Qres) / b32" */
                                             const opus_int32 b32, /* I    denominator (Q0)                */
                                             const opus_int Qres   /* I    Q-domain of result (> 0)        */
) {
    opus_int b_headrm, lshift;
    opus_int32 b32_inv, b32_nrm, err_Q32, result;

    /* Compute number of bits head room and normalize input */
    b_headrm = silk_CLZ32(silk_abs(b32)) - 1;
    b32_nrm = silk_LSHIFT(b32, b_headrm); /* Q: b_headrm                */

    /* Inverse of b32, with 14 bits of precision */
    b32_inv = silk_DIV32_16(silk_int32_MAX >> 2, silk_RSHIFT(b32_nrm, 16)); /* Q: 29 + 16 - b_headrm    */

    /* First approximation */
    result = silk_LSHIFT(b32_inv, 16); /* Q: 61 - b_headrm            */

    /* Compute residual by subtracting product of denominator and first approximation from one */
    err_Q32 = silk_LSHIFT(((opus_int32)1 << 29) - silk_SMULWB(b32_nrm, b32_inv), 3); /* Q32                        */

    /* Refinement */
    result = silk_SMLAWW(result, err_Q32, b32_inv); /* Q: 61 - b_headrm            */

    /* Convert to Qres domain */
    lshift = 61 - b_headrm - Qres;
    if (lshift <= 0) {
        return silk_LSHIFT_SAT32(result, -lshift);
    } else {
        if (lshift < 32) {
            return silk_RSHIFT(result, lshift);
        } else {
            /* Avoid undefined result */
            return 0;
        }
    }
}

/* Compute inverse of LPC prediction gain, and                          */
/* test if LPC coefficients are stable (all poles within unit circle)   */
/* O   Returns inverse prediction gain in energy domain, Q30    */
static opus_int32 LPC_inverse_pred_gain_QA_c(
    opus_int32 A_QA[SILK_MAX_ORDER_LPC], /* I   Prediction coefficients                                  */
    const opus_int order                 /* I   Prediction order                                         */
) {
    opus_int k, n, mult2Q;
    opus_int32 invGain_Q30, rc_Q31, rc_mult1_Q30, rc_mult2, tmp1, tmp2;

    invGain_Q30 = SILK_FIX_CONST(1, 30);
    for (k = order - 1; k > 0; k--) {
        /* Check for stability */
        if ((A_QA[k] > A_LIMIT) || (A_QA[k] < -A_LIMIT)) {
            // making sure input values excersize all of the function
            SWAN_ASSERT(false)
            return 0;
        }

        /* Set RC equal to negated AR coef */
        rc_Q31 = -silk_LSHIFT(A_QA[k], 31 - QA);

        /* rc_mult1_Q30 range: [ 1 : 2^30 ] */
        rc_mult1_Q30 = silk_SUB32(SILK_FIX_CONST(1, 30), silk_SMMUL(rc_Q31, rc_Q31));

        /* Update inverse gain */
        /* invGain_Q30 range: [ 0 : 2^30 ] */
        invGain_Q30 = silk_LSHIFT(silk_SMMUL(invGain_Q30, rc_mult1_Q30), 2);
        if (invGain_Q30 < SILK_FIX_CONST(1.0f / MAX_PREDICTION_POWER_GAIN, 30)) {
            // making sure input values excersize all of the function
            SWAN_ASSERT(false)
            return 0;
        }

        /* rc_mult2 range: [ 2^30 : silk_int32_MAX ] */
        mult2Q = 32 - silk_CLZ32(silk_abs(rc_mult1_Q30));
        rc_mult2 = silk_INVERSE32_varQ(rc_mult1_Q30, mult2Q + 30);

        /* Update AR coefficient */
        for (n = 0; n < (k + 1) >> 1; n++) {
            opus_int64 tmp64;
            tmp1 = A_QA[n];
            tmp2 = A_QA[k - n - 1];
            tmp64 = silk_RSHIFT_ROUND64(silk_SMULL(silk_SUB_SAT32(tmp1, MUL32_FRAC_Q(tmp2, rc_Q31, 31)), rc_mult2), mult2Q);
            if (tmp64 > silk_int32_MAX || tmp64 < silk_int32_MIN) {
                // making sure input values excersize all of the function
                SWAN_ASSERT(false)
                return 0;
            }
            A_QA[n] = (opus_int32)tmp64;
            tmp64 = silk_RSHIFT_ROUND64(silk_SMULL(silk_SUB_SAT32(tmp2, MUL32_FRAC_Q(tmp1, rc_Q31, 31)), rc_mult2), mult2Q);
            if (tmp64 > silk_int32_MAX || tmp64 < silk_int32_MIN) {
                // making sure input values excersize all of the function
                SWAN_ASSERT(false)
                return 0;
            }
            A_QA[k - n - 1] = (opus_int32)tmp64;
        }
    }

    /* Check for stability */
    if ((A_QA[k] > A_LIMIT) || (A_QA[k] < -A_LIMIT)) {
        // making sure input values excersize all of the function
        SWAN_ASSERT(false)
        return 0;
    }

    /* Set RC equal to negated AR coef */
    rc_Q31 = -silk_LSHIFT(A_QA[0], 31 - QA);

    /* Range: [ 1 : 2^30 ] */
    rc_mult1_Q30 = silk_SUB32(SILK_FIX_CONST(1, 30), silk_SMMUL(rc_Q31, rc_Q31));

    /* Update inverse gain */
    /* Range: [ 0 : 2^30 ] */
    invGain_Q30 = silk_LSHIFT(silk_SMMUL(invGain_Q30, rc_mult1_Q30), 2);
    if (invGain_Q30 < SILK_FIX_CONST(1.0f / MAX_PREDICTION_POWER_GAIN, 30)) {
        // making sure input values excersize all of the function
        SWAN_ASSERT(false)
        return 0;
    }

    return invGain_Q30;
}

/* The following function is the modified version of silk_LPC_inverse_pred_gain_c,
 * provided in the libopus library. Please refer to LPC_inv_pred_gain.c
 * for the unmodified version in the source library. */
void lpc_prediction_scalar(config_t *config,
                           input_t *input,
                           output_t *output) {

    lpc_prediction_config_t *lpc_prediction_config = (lpc_prediction_config_t *)config;
    lpc_prediction_input_t *lpc_prediction_input = (lpc_prediction_input_t *)input;
    lpc_prediction_output_t *lpc_prediction_output = (lpc_prediction_output_t *)output;

    const opus_int order = lpc_prediction_config->order; /* I   Prediction order                                       */
    const uint32_t block_count = lpc_prediction_config->block_count;
    opus_int32 *return_val = lpc_prediction_output->return_val;

    for (unsigned block = 0; block < block_count; ++block) {
        const opus_int16 *A_Q12 = lpc_prediction_input->A_Q12[block]; /* I   Prediction coefficients, Q12 [order]                         */

        opus_int k;
        opus_int32 Atmp_QA[SILK_MAX_ORDER_LPC];
        opus_int32 DC_resp = 0;

        /* Increase Q domain of the AR coefficients */
        for (k = 0; k < order; k++) {
            DC_resp += (opus_int32)A_Q12[k];
            Atmp_QA[k] = silk_LSHIFT32((opus_int32)A_Q12[k], QA - 12);
        }
        /* If the DC is unstable, we don't even need to do the full calculations */
        if (DC_resp >= 4096) {
            // making sure input values excersize all of the function
            SWAN_ASSERT(false)
            return_val[0] = 0;
        }

        return_val[block] = LPC_inverse_pred_gain_QA_c(Atmp_QA, order);
    }
}