#include "neon.hpp"
#include <arm_neon.h>
#include <stdint.h>

#include "aes.hpp"
#include "boringssl.hpp"

void aes_neon(config_t *config,
              input_t *input,
              output_t *output) {
    aes_config_t *aes_config = (aes_config_t *)config;
    aes_input_t *aes_input = (aes_input_t *)input;
    aes_output_t *aes_output = (aes_output_t *)output;

    int block_count = aes_config->block_count;

    unsigned char *state_in = aes_input->state_in;
    unsigned char *state_out = aes_output->state_out;
    unsigned char *RoundKey = aes_config->RoundKey;

    uint8x16_t RoundKey_v[11];

    uint8x16_t zero_v = vdupq_n_u8(0);

    for (int round = 0; round < 11; round++) {
        RoundKey_v[round] = vld1q_u8(RoundKey);
        RoundKey += 16;
    }

    for (int sample = 0; sample < block_count; sample++) {
        uint8x16_t state_v = vld1q_u8(state_in);

        state_v = veorq_u8(state_v, RoundKey_v[0]);

        int round = 0;

        for (round = 1;; ++round) {
            // SubBytes
            // ShiftRows
            state_v = vaeseq_u8(state_v, zero_v);
            if (round == 10) {
                break;
            }
            // MixColumns
            state_v = vaesmcq_u8(state_v);
            // AddRoundKey
            state_v = veorq_u8(state_v, RoundKey_v[round]);
        }
        state_v = veorq_u8(state_v, RoundKey_v[10]);

        vst1q_u8(state_out, state_v);

        state_in += 16;
        state_out += 16;
    }
}
