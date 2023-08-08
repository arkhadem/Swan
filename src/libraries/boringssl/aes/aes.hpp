#ifndef B54AD8F1_0C7E_45FB_B967_B373BEE1E681
#define B54AD8F1_0C7E_45FB_B967_B373BEE1E681

#include <stdint.h>
#include <stdlib.h>

#include "boringssl.hpp"

extern const unsigned char sbox[256];

typedef struct aes_config_s : config_t {
    // 8192 16-byte blocks = 128KB
    int block_count;
    int block_size;
    unsigned char RoundKey[176];
} aes_config_t;

typedef struct aes_input_s : input_t {
    unsigned char *state_in;
} aes_input_t;

typedef struct aes_output_s : output_t {
    unsigned char *state_out;
} aes_output_t;

#endif /* B54AD8F1_0C7E_45FB_B967_B373BEE1E681 */
