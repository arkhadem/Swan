#ifndef B54AD8F1_0C7E_45FB_B967_B373BEE1E681
#define B54AD8F1_0C7E_45FB_B967_B373BEE1E681

#include <stdint.h>
#include <stdlib.h>

#include "boringssl.hpp"

extern const uint32_t sha256_initial_state[8];
extern const uint32_t sha256_round_constants[64];

typedef struct sha256_config_s : config_t {
    // 2048 512-bit blocks = 128KB
    int len;
} sha256_config_t;

typedef struct sha256_input_s : input_t {
    unsigned char *input;
} sha256_input_t;

typedef struct sha256_output_s : output_t {
    uint32_t *output;
} sha256_output_t;

#endif /* B54AD8F1_0C7E_45FB_B967_B373BEE1E681 */
