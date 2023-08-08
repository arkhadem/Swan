#include "chacha20.hpp"

#include "boringssl.hpp"

#include "swan.hpp"

#include "utility.hpp"

#include "arm_arch.h"

const uint8_t sigma[16] = {'e', 'x', 'p', 'a', 'n', 'd', ' ', '3',
                           '2', '-', 'b', 'y', 't', 'e', ' ', 'k'};

uint32_t OPENSSL_armcap_P;

int chacha20_config_init(size_t cache_size,
                         config_t *&config) {

    chacha20_config_t *chacha20_config = (chacha20_config_t *)config;

    // configuration
    int length = SWAN_TXT_INPUT_SIZE;
    int block_size = 64;
    int block_count = length / block_size;

    alloc_1D<chacha20_config_t>(1, chacha20_config);
    chacha20_config->in_len = length;
    chacha20_config->counter = 4291;
    chacha20_config->granularity = 1;

    // in/output versions
    size_t input_size = length * sizeof(uint8_t);
    input_size += 32 * sizeof(uint8_t);
    input_size += 12 * sizeof(uint8_t);
    size_t output_size = length * sizeof(uint8_t);
    int count = cache_size / (input_size + output_size) + 1;

#if defined(__ARMEL__) || defined(_M_ARM) || defined(__AARCH64EL__) || defined(_M_ARM64)
    OPENSSL_armcap_P = 0;
    OPENSSL_armcap_P |= ARMV7_NEON;
    OPENSSL_armcap_P |= ARMV8_AES;
    OPENSSL_armcap_P |= ARMV8_PMULL;
    OPENSSL_armcap_P |= ARMV8_SHA1;
    OPENSSL_armcap_P |= ARMV8_SHA256;
    OPENSSL_armcap_P |= ARMV8_SHA512;
#endif

    config = (config_t *)chacha20_config;

    return count;
}

void chacha20_input_init(int count,
                         config_t *config,
                         input_t **&input) {

    chacha20_config_t *chacha20_config = (chacha20_config_t *)config;
    chacha20_input_t **chacha20_input = (chacha20_input_t **)input;

    // initializing input versions
    alloc_1D<chacha20_input_t *>(count, chacha20_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<chacha20_input_t>(1, chacha20_input[i]);

        init_alloc_1D<uint8_t>(chacha20_config->in_len, chacha20_input[i]->in);

        init_alloc_1D<uint8_t>(32, chacha20_input[i]->key);

        init_alloc_1D<uint8_t>(12, chacha20_input[i]->nonce);
    }

    input = (input_t **)chacha20_input;
}

void chacha20_output_init(int count,
                          config_t *config,
                          output_t **&output) {

    chacha20_config_t *chacha20_config = (chacha20_config_t *)config;
    chacha20_output_t **chacha20_output = (chacha20_output_t **)output;

    // initializing output versions
    alloc_1D<chacha20_output_t *>(count, chacha20_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<chacha20_output_t>(1, chacha20_output[i]);

        alloc_1D<uint8_t>(chacha20_config->in_len, chacha20_output[i]->out);
    }

    output = (output_t **)chacha20_output;
}

void chacha20_comparer(config_t *config,
                       output_t *output_scalar,
                       output_t *output_neon) {
    chacha20_config_t *chacha20_config = (chacha20_config_t *)config;
    chacha20_output_t *chacha20_output_scalar = (chacha20_output_t *)output_scalar;
    chacha20_output_t *chacha20_output_neon = (chacha20_output_t *)output_neon;

    compare_1D<unsigned char>(chacha20_config->in_len, chacha20_output_scalar->out, chacha20_output_neon->out, (char *)"out");
}

kernel_utility_t chacha20_utility = {chacha20_config_init, chacha20_input_init, chacha20_output_init, chacha20_comparer};