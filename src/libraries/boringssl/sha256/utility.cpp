#include "sha256.hpp"

#include "boringssl.hpp"

#include "swan.hpp"

#include "utility.hpp"

const uint32_t sha256_initial_state[8] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19,
};

const uint32_t sha256_round_constants[64] = {
    0x428a2f98,
    0x71374491,
    0xb5c0fbcf,
    0xe9b5dba5,
    0x3956c25b,
    0x59f111f1,
    0x923f82a4,
    0xab1c5ed5,
    0xd807aa98,
    0x12835b01,
    0x243185be,
    0x550c7dc3,
    0x72be5d74,
    0x80deb1fe,
    0x9bdc06a7,
    0xc19bf174,
    0xe49b69c1,
    0xefbe4786,
    0x0fc19dc6,
    0x240ca1cc,
    0x2de92c6f,
    0x4a7484aa,
    0x5cb0a9dc,
    0x76f988da,
    0x983e5152,
    0xa831c66d,
    0xb00327c8,
    0xbf597fc7,
    0xc6e00bf3,
    0xd5a79147,
    0x06ca6351,
    0x14292967,
    0x27b70a85,
    0x2e1b2138,
    0x4d2c6dfc,
    0x53380d13,
    0x650a7354,
    0x766a0abb,
    0x81c2c92e,
    0x92722c85,
    0xa2bfe8a1,
    0xa81a664b,
    0xc24b8b70,
    0xc76c51a3,
    0xd192e819,
    0xd6990624,
    0xf40e3585,
    0x106aa070,
    0x19a4c116,
    0x1e376c08,
    0x2748774c,
    0x34b0bcb5,
    0x391c0cb3,
    0x4ed8aa4a,
    0x5b9cca4f,
    0x682e6ff3,
    0x748f82ee,
    0x78a5636f,
    0x84c87814,
    0x8cc70208,
    0x90befffa,
    0xa4506ceb,
    0xbef9a3f7,
    0xc67178f2,
};

int sha256_config_init(size_t cache_size,
                       config_t *&config) {

    sha256_config_t *sha256_config = (sha256_config_t *)config;

    // configuration
    int len = SWAN_TXT_INPUT_SIZE;
    int block_size = 64;
    int block_count = len / block_size;

    alloc_1D<sha256_config_t>(1, sha256_config);

    sha256_config->len = len;
    sha256_config->granularity = 1;

    // in/output versions
    size_t input_size = len * sizeof(unsigned char);
    size_t output_size = 0;
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)sha256_config;

    return count;
}

void sha256_input_init(int count,
                       config_t *config,
                       input_t **&input) {

    sha256_config_t *sha256_config = (sha256_config_t *)config;
    sha256_input_t **sha256_input = (sha256_input_t **)input;

    // initializing input versions
    alloc_1D<sha256_input_t *>(count, sha256_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sha256_input_t>(1, sha256_input[i]);

        init_alloc_1D<unsigned char>(sha256_config->len, sha256_input[i]->input);
    }

    input = (input_t **)sha256_input;
}

void sha256_output_init(int count,
                        config_t *config,
                        output_t **&output) {

    sha256_config_t *sha256_config = (sha256_config_t *)config;
    sha256_output_t **sha256_output = (sha256_output_t **)output;

    // initializing output versions
    alloc_1D<sha256_output_t *>(count, sha256_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sha256_output_t>(1, sha256_output[i]);

        alloc_1D<uint32_t>(8, sha256_output[i]->output);
    }

    output = (output_t **)sha256_output;
}

void sha256_comparer(config_t *config,
                     output_t *output_scalar,
                     output_t *output_neon) {
    sha256_config_t *sha256_config = (sha256_config_t *)config;
    sha256_output_t *sha256_output_scalar = (sha256_output_t *)output_scalar;
    sha256_output_t *sha256_output_neon = (sha256_output_t *)output_neon;

    compare_1D<uint32_t>(8, sha256_output_scalar->output, sha256_output_neon->output, (char *)"output");
}

kernel_utility_t sha256_utility = {sha256_config_init, sha256_input_init, sha256_output_init, sha256_comparer};