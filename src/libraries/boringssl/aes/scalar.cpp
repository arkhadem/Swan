#include "scalar.hpp"
#include "aes.hpp"

static unsigned char xtime(unsigned char x) {
    return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

void aes_scalar(config_t *config,
                input_t *input,
                output_t *output) {
    aes_config_t *aes_config = (aes_config_t *)config;
    aes_input_t *aes_input = (aes_input_t *)input;
    aes_output_t *aes_output = (aes_output_t *)output;

    int block_count = aes_config->block_count;

    unsigned char *state_in = aes_input->state_in;
    unsigned char *state_out = aes_output->state_out;
    unsigned char *RoundKey = aes_config->RoundKey;

    int round = 0;

    // Add the First round key to the state_in before starting the rounds.
    unsigned char *my_state_in = state_in;
    unsigned char *my_state_out = state_out;
    for (int __i = 0; __i < block_count; __i++) {
        for (int i = 0; i < 16; ++i) {
            my_state_out[i] = my_state_in[i] ^ RoundKey[i];
        }

        my_state_in += 16;
        my_state_out += 16;
    }

    // There will be 10 rounds.
    // The first 10-1 rounds are identical.
    // These 10 rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1;; ++round) {
        // SubBytes
        unsigned char *my_state = state_out;
        for (int __i = 0; __i < block_count; __i++) {
            for (int i = 0; i < 16; ++i) {
                my_state[i] = sbox[my_state[i]];
            }

            my_state += 16;
        }

        // ShiftRows
        unsigned char temp;
        my_state = state_out;
        for (int __i = 0; __i < block_count; __i++) {
            // Rotate first row 1 columns to left
            temp = my_state[1];
            my_state[1] = my_state[5];
            my_state[5] = my_state[9];
            my_state[9] = my_state[13];
            my_state[13] = temp;

            // Rotate second row 2 columns to left
            temp = my_state[2];
            my_state[2] = my_state[10];
            my_state[10] = temp;

            temp = my_state[6];
            my_state[6] = my_state[14];
            my_state[14] = temp;

            // Rotate third row 3 columns to left
            temp = my_state[3];
            my_state[3] = my_state[15];
            my_state[15] = my_state[11];
            my_state[11] = my_state[7];
            my_state[7] = temp;

            my_state += 16;
        }

        if (round == 10) {
            break;
        }

        // MixColumns
        unsigned char Tmp, Tm, t;
        my_state = state_out;
        for (int __i = 0; __i < block_count; __i++) {
            for (int i = 0; i < 16; i += 4) {
                t = my_state[i + 0];
                Tmp = my_state[i + 0] ^ my_state[i + 1] ^ my_state[i + 2] ^ my_state[i + 3];
                Tm = my_state[i + 0] ^ my_state[i + 1];
                Tm = xtime(Tm);
                my_state[i + 0] ^= Tm ^ Tmp;
                Tm = my_state[i + 1] ^ my_state[i + 2];
                Tm = xtime(Tm);
                my_state[i + 1] ^= Tm ^ Tmp;
                Tm = my_state[i + 2] ^ my_state[i + 3];
                Tm = xtime(Tm);
                my_state[i + 2] ^= Tm ^ Tmp;
                Tm = my_state[i + 3] ^ t;
                Tm = xtime(Tm);
                my_state[i + 3] ^= Tm ^ Tmp;
            }
            my_state += 16;
        }

        // AddRoundKey
        const unsigned char *RoundKey_addr = RoundKey + (round * 16);
        my_state = state_out;
        for (int __i = 0; __i < block_count; __i++) {
            for (int i = 0; i < 16; ++i) {
                my_state[i] ^= RoundKey_addr[i];
            }

            my_state += 16;
        }
    }

    // Add round key to last round
    // AddRoundKey
    const unsigned char *RoundKey_addr = RoundKey + 160;
    unsigned char *my_state = state_out;
    for (int __i = 0; __i < block_count; __i++) {
        for (int i = 0; i < 16; ++i) {
            my_state[i] ^= RoundKey_addr[i];
        }

        my_state += 16;
    }
}