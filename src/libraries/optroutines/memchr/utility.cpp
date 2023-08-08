#include "memchr.hpp"

#include "optroutines.hpp"

#include "swan.hpp"

#include "utility.hpp"

int memchr_config_init(size_t cache_size,
                       config_t *&config) {

    memchr_config_t *memchr_config = (memchr_config_t *)config;

    // configuration
    int size = SWAN_TXT_INPUT_SIZE;

    alloc_1D<memchr_config_t>(1, memchr_config);

    memchr_config->size = size;
    memchr_config->granularity = 1;

    // in/output versions
    size_t input_size = size * sizeof(char);
    size_t output_size = 0;
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)memchr_config;

    return count;
}

void memchr_input_init(int count,
                       config_t *config,
                       input_t **&input) {

    memchr_config_t *memchr_config = (memchr_config_t *)config;
    memchr_input_t **memchr_input = (memchr_input_t **)input;

    // initializing input versions
    alloc_1D<memchr_input_t *>(count, memchr_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<memchr_input_t>(1, memchr_input[i]);

        init_alloc_1D<char>(1, memchr_input[i]->value);
        memchr_input[i]->value[0] = (char)129;
        alloc_1D<char>(memchr_config->size, memchr_input[i]->src);
        for (int j = 0; j < memchr_config->size; j++) {
            memchr_input[i]->src[j] = (char)128;
        }
        memchr_input[i]->src[memchr_config->size - 1] = (char)129;
    }

    input = (input_t **)memchr_input;
}

void memchr_output_init(int count,
                        config_t *config,
                        output_t **&output) {

    memchr_config_t *memchr_config = (memchr_config_t *)config;
    memchr_output_t **memchr_output = (memchr_output_t **)output;

    // initializing output versions
    alloc_1D<memchr_output_t *>(count, memchr_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<memchr_output_t>(1, memchr_output[i]);

        alloc_1D<char>(1, memchr_output[i]->return_value);
    }

    output = (output_t **)memchr_output;
}

void memchr_comparer(config_t *config,
                     output_t *output_scalar,
                     output_t *output_neon) {
    memchr_config_t *memchr_config = (memchr_config_t *)config;
    memchr_output_t *memchr_output_scalar = (memchr_output_t *)output_scalar;
    memchr_output_t *memchr_output_neon = (memchr_output_t *)output_neon;

    compare_1D<char>(1, memchr_output_scalar->return_value, memchr_output_neon->return_value, (char *)"return_value");
}

kernel_utility_t memchr_utility = {memchr_config_init, memchr_input_init, memchr_output_init, memchr_comparer};