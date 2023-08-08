#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "boringssl.hpp"

extern const uint8_t sigma[16];

typedef struct chacha20_config_s : config_t {
    size_t in_len;
    uint32_t counter;
} chacha20_config_t;

typedef struct chacha20_input_s : input_t {
    uint8_t *in;
    uint8_t *key;
    uint8_t *nonce;
} chacha20_input_t;

typedef struct chacha20_output_s : output_t {
    uint8_t *out;
} chacha20_output_t;

#if defined(__cplusplus)
extern "C" {
#endif

// ChaCha20_ctr32 is defined in asm/chacha-*.pl.
void ChaCha20_ctr32(uint8_t *out, const uint8_t *in, size_t in_len,
                    const uint32_t key[8], const uint32_t counter[4]);

#if defined(__cplusplus)
} // extern C
#endif

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
