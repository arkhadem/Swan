#include "aes.hpp"

#include "boringssl.hpp"

#include "swan.hpp"

#include "utility.hpp"

static const unsigned char Rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
unsigned char Key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
const unsigned char sbox[256] = {
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// This function produces 4(10+1) round keys. The round keys are used in each round to decrypt the states.
void KeyExpansion(unsigned char *RoundKey) {
    unsigned i, j, k;
    unsigned char tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < 4; ++i) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = 4; i < 4 * (10 + 1); ++i) {
        {
            k = (i - 1) * 4;
            tempa[0] = RoundKey[k + 0];
            tempa[1] = RoundKey[k + 1];
            tempa[2] = RoundKey[k + 2];
            tempa[3] = RoundKey[k + 3];
        }

        if (i % 4 == 0) {
            // This function shifts the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                const unsigned char u8tmp = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = u8tmp;
            }

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = sbox[tempa[0]];
                tempa[1] = sbox[tempa[1]];
                tempa[2] = sbox[tempa[2]];
                tempa[3] = sbox[tempa[3]];
            }

            tempa[0] = tempa[0] ^ Rcon[i / 4];
        }

        j = i * 4;
        k = (i - 4) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

int aes_config_init(size_t cache_size,
                    config_t *&config) {

    aes_config_t *aes_config = (aes_config_t *)config;

    // configuration
    int block_size = 16;
    int block_count = SWAN_TXT_INPUT_SIZE / block_size;

    alloc_1D<aes_config_t>(1, aes_config);

    aes_config->block_size = block_size;
    aes_config->block_count = block_count;
    aes_config->granularity = 1;
    KeyExpansion(aes_config->RoundKey);

    // in/output versions
    size_t input_size = block_count * 16 * sizeof(unsigned char);
    size_t output_size = block_count * 16 * sizeof(unsigned char);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)aes_config;

    return count;
}

void aes_input_init(int count,
                    config_t *config,
                    input_t **&input) {

    aes_config_t *aes_config = (aes_config_t *)config;
    aes_input_t **aes_input = (aes_input_t **)input;

    // initializing input versions
    alloc_1D<aes_input_t *>(count, aes_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<aes_input_t>(1, aes_input[i]);

        init_alloc_1D<unsigned char>(aes_config->block_count * 16, aes_input[i]->state_in);
    }

    input = (input_t **)aes_input;
}

void aes_output_init(int count,
                     config_t *config,
                     output_t **&output) {

    aes_config_t *aes_config = (aes_config_t *)config;
    aes_output_t **aes_output = (aes_output_t **)output;

    // initializing output versions
    alloc_1D<aes_output_t *>(count, aes_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<aes_output_t>(1, aes_output[i]);

        alloc_1D<unsigned char>(aes_config->block_count * 16, aes_output[i]->state_out);
    }

    output = (output_t **)aes_output;
}

void aes_comparer(config_t *config,
                  output_t *output_scalar,
                  output_t *output_neon) {

    aes_config_t *aes_config = (aes_config_t *)config;
    aes_output_t *aes_output_scalar = (aes_output_t *)output_scalar;
    aes_output_t *aes_output_neon = (aes_output_t *)output_neon;

    compare_1D<unsigned char>(aes_config->block_count * 16, aes_output_scalar->state_out, aes_output_neon->state_out, (char *)"state_out");
}

kernel_utility_t aes_utility = {aes_config_init, aes_input_init, aes_output_init, aes_comparer};