#include "memset.hpp"

#include "optroutines.hpp"

#include "swan.hpp"

#include "utility.hpp"

int memset_config_init(size_t cache_size,
                       config_t *&config) {

    memset_config_t *memset_config = (memset_config_t *)config;

    // configuration
    int size = SWAN_TXT_INPUT_SIZE;

    alloc_1D<memset_config_t>(1, memset_config);

    memset_config->size = size;
    memset_config->granularity = 1;

    // in/output versions
    size_t input_size = 0;
    size_t output_size = size * sizeof(char);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)memset_config;

    return count;
}

void memset_input_init(int count,
                       config_t *config,
                       input_t **&input) {

    memset_config_t *memset_config = (memset_config_t *)config;
    memset_input_t **memset_input = (memset_input_t **)input;

    // initializing input versions
    alloc_1D<memset_input_t *>(count, memset_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<memset_input_t>(1, memset_input[i]);

        init_alloc_1D<char>(1, memset_input[i]->value);
    }

    input = (input_t **)memset_input;
}

void memset_output_init(int count,
                        config_t *config,
                        output_t **&output) {

    memset_config_t *memset_config = (memset_config_t *)config;
    memset_output_t **memset_output = (memset_output_t **)output;

    // initializing output versions
    alloc_1D<memset_output_t *>(count, memset_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<memset_output_t>(1, memset_output[i]);

        alloc_1D<char>(memset_config->size, memset_output[i]->dst);
    }

    output = (output_t **)memset_output;
}

void memset_comparer(config_t *config,
                     output_t *output_scalar,
                     output_t *output_neon) {
    memset_config_t *memset_config = (memset_config_t *)config;
    memset_output_t *memset_output_scalar = (memset_output_t *)output_scalar;
    memset_output_t *memset_output_neon = (memset_output_t *)output_neon;

    compare_1D<char>(memset_config->size, memset_output_scalar->dst, memset_output_neon->dst, (char *)"dst");
}

kernel_utility_t memset_utility = {memset_config_init, memset_input_init, memset_output_init, memset_comparer};