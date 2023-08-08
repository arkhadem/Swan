#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "boringssl.hpp"

extern const uint32_t DES_SPtrans[8][64];

typedef struct des_config_s : config_t {
    int block_count;
    int block_size;
    // 16 rounds
    uint64_t subkeys[16];
} des_config_t;

typedef struct des_input_s : input_t {
    // 64K?
    uint64_t *state_in;
} des_input_t;

typedef struct des_output_s : output_t {
    uint64_t *state_out;
} des_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
