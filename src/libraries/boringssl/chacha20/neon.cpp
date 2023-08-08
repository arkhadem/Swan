#include "neon.hpp"
#include <arm_neon.h>
#include <stdint.h>

#include "boringssl.hpp"
#include "chacha20.hpp"

void chacha20_neon(config_t *config,
                   input_t *input,
                   output_t *output) {
    chacha20_config_t *chacha20_config = (chacha20_config_t *)config;
    chacha20_input_t *chacha20_input = (chacha20_input_t *)input;
    chacha20_output_t *chacha20_output = (chacha20_output_t *)output;

    uint8_t *out = chacha20_output->out;
    const uint8_t *in = chacha20_input->in;
    size_t in_len = chacha20_config->in_len;
    const uint32_t *key = (const uint32_t *)chacha20_input->key;
    const uint32_t *nonce = (const uint32_t *)chacha20_input->nonce;
    uint32_t counter[4] = {chacha20_config->counter, nonce[0], nonce[1], nonce[2]};

    ChaCha20_ctr32(out, in, in_len, key, counter);
}