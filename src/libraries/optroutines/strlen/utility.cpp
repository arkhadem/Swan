#include "strlen.hpp"

#include "optroutines.hpp"

#include "swan.hpp"

#include "utility.hpp"

int strlen_config_init(size_t cache_size,
                       config_t *&config) {

    strlen_config_t *strlen_config = (strlen_config_t *)config;

    // configuration
    int size = SWAN_TXT_INPUT_SIZE;

    alloc_1D<strlen_config_t>(1, strlen_config);

    strlen_config->size = size;
    strlen_config->granularity = 1;

    // in/output versions
    size_t input_size = size * sizeof(char);
    size_t output_size = 0;
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)strlen_config;

    return count;
}

void strlen_input_init(int count,
                       config_t *config,
                       input_t **&input) {

    strlen_config_t *strlen_config = (strlen_config_t *)config;
    strlen_input_t **strlen_input = (strlen_input_t **)input;

    // initializing input versions
    alloc_1D<strlen_input_t *>(count, strlen_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<strlen_input_t>(1, strlen_input[i]);

        alloc_1D<char>(strlen_config->size, strlen_input[i]->src);
        for (int j = 0; j < strlen_config->size; j++) {
            strlen_input[i]->src[j] = 'a';
        }
        strlen_input[i]->src[strlen_config->size - 1] = (char)'\0';
    }

    input = (input_t **)strlen_input;
}

void strlen_output_init(int count,
                        config_t *config,
                        output_t **&output) {

    strlen_config_t *strlen_config = (strlen_config_t *)config;
    strlen_output_t **strlen_output = (strlen_output_t **)output;

    // initializing output versions
    alloc_1D<strlen_output_t *>(count, strlen_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<strlen_output_t>(1, strlen_output[i]);

        alloc_1D<int>(1, strlen_output[i]->return_value);
    }

    output = (output_t **)strlen_output;
}

void strlen_comparer(config_t *config,
                     output_t *output_scalar,
                     output_t *output_neon) {
    strlen_config_t *strlen_config = (strlen_config_t *)config;
    strlen_output_t *strlen_output_scalar = (strlen_output_t *)output_scalar;
    strlen_output_t *strlen_output_neon = (strlen_output_t *)output_neon;

    compare_1D<int>(1, strlen_output_scalar->return_value, strlen_output_neon->return_value, (char *)"return_value");
}

kernel_utility_t strlen_utility = {strlen_config_init, strlen_input_init, strlen_output_init, strlen_comparer};