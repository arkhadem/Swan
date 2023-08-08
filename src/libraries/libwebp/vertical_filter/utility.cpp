#include <stdlib.h>

#include "vertical_filter.hpp"

#include "libwebp.hpp"

#include "swan.hpp"

#include "utility.hpp"

int vertical_filter_config_init(size_t cache_size,
                                config_t *&config) {

    vertical_filter_config_t *vertical_filter_config = (vertical_filter_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;

    alloc_1D<vertical_filter_config_t>(1, vertical_filter_config);
    vertical_filter_config->num_rows = rows;
    vertical_filter_config->num_cols = columns;
    vertical_filter_config->stride = columns;
    vertical_filter_config->granularity = 1;

    // in/output versions
    size_t input_size = (rows + 1) * columns * sizeof(uint8_t);
    size_t output_size = rows * columns * sizeof(uint8_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)vertical_filter_config;

    return count;
}

void vertical_filter_input_init(int count,
                                config_t *config,
                                input_t **&input) {

    vertical_filter_config_t *vertical_filter_config = (vertical_filter_config_t *)config;
    vertical_filter_input_t **vertical_filter_input = (vertical_filter_input_t **)input;

    // initializing input versions
    alloc_1D<vertical_filter_input_t *>(count, vertical_filter_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<vertical_filter_input_t>(1, vertical_filter_input[i]);

        init_alloc_1D<uint8_t>((vertical_filter_config->num_rows + 1) * vertical_filter_config->num_cols, vertical_filter_input[i]->in);
    }

    input = (input_t **)vertical_filter_input;
}

void vertical_filter_output_init(int count,
                                 config_t *config,
                                 output_t **&output) {

    vertical_filter_config_t *vertical_filter_config = (vertical_filter_config_t *)config;
    vertical_filter_output_t **vertical_filter_output = (vertical_filter_output_t **)output;

    // initializing output versions
    alloc_1D<vertical_filter_output_t *>(count, vertical_filter_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<vertical_filter_output_t>(1, vertical_filter_output[i]);

        alloc_1D<uint8_t>(vertical_filter_config->num_rows * vertical_filter_config->num_cols, vertical_filter_output[i]->out);
    }

    output = (output_t **)vertical_filter_output;
}

void vertical_filter_comparer(config_t *config,
                              output_t *output_scalar,
                              output_t *output_neon) {
    vertical_filter_config_t *vertical_filter_config = (vertical_filter_config_t *)config;
    vertical_filter_output_t *vertical_filter_output_scalar = (vertical_filter_output_t *)output_scalar;
    vertical_filter_output_t *vertical_filter_output_neon = (vertical_filter_output_t *)output_neon;

    compare_1D<uint8_t>(vertical_filter_config->num_rows * vertical_filter_config->num_cols, vertical_filter_output_scalar->out, vertical_filter_output_neon->out, (char *)"out");
}

kernel_utility_t vertical_filter_utility = {vertical_filter_config_init, vertical_filter_input_init, vertical_filter_output_init, vertical_filter_comparer};