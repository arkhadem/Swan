#include "memcmp.hpp"

#include "optroutines.hpp"

#include "swan.hpp"

#include "utility.hpp"
#include <string.h>

#include <stdio.h>
#include <string.h>

int memcmp_config_init(size_t cache_size,
                       config_t *&config) {

    memcmp_config_t *memcmp_config = (memcmp_config_t *)config;

    // configuration
    int size = SWAN_TXT_INPUT_SIZE;

    alloc_1D<memcmp_config_t>(1, memcmp_config);

    memcmp_config->size = size;
    memcmp_config->granularity = 1;

    // in/output versions
    size_t input_size = 2 * size * sizeof(char);
    size_t output_size = 0;
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)memcmp_config;

    return count;
}

void memcmp_input_init(int count,
                       config_t *config,
                       input_t **&input) {

    memcmp_config_t *memcmp_config = (memcmp_config_t *)config;
    memcmp_input_t **memcmp_input = (memcmp_input_t **)input;

    // initializing input versions
    alloc_1D<memcmp_input_t *>(count, memcmp_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<memcmp_input_t>(1, memcmp_input[i]);

        init_alloc_1D<char>(memcmp_config->size, memcmp_input[i]->src1);
        alloc_1D<char>(memcmp_config->size, memcmp_input[i]->src2);
        memcpy(memcmp_input[i]->src2, memcmp_input[i]->src1, memcmp_config->size);
        memcmp_input[i]->src1[memcmp_config->size - 1]++;
    }

    input = (input_t **)memcmp_input;
}

void memcmp_output_init(int count,
                        config_t *config,
                        output_t **&output) {

    memcmp_config_t *memcmp_config = (memcmp_config_t *)config;
    memcmp_output_t **memcmp_output = (memcmp_output_t **)output;

    // initializing output versions
    alloc_1D<memcmp_output_t *>(count, memcmp_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<memcmp_output_t>(1, memcmp_output[i]);

        alloc_1D<int>(1, memcmp_output[i]->return_val);
    }

    output = (output_t **)memcmp_output;
}

void memcmp_comparer(config_t *config,
                     output_t *output_scalar,
                     output_t *output_neon) {
    memcmp_config_t *memcmp_config = (memcmp_config_t *)config;
    memcmp_output_t *memcmp_output_scalar = (memcmp_output_t *)output_scalar;
    memcmp_output_t *memcmp_output_neon = (memcmp_output_t *)output_neon;

    compare_1D<int>(1, memcmp_output_scalar->return_val, memcmp_output_neon->return_val, (char *)"return_val");
}

kernel_utility_t memcmp_utility = {memcmp_config_init, memcmp_input_init, memcmp_output_init, memcmp_comparer};