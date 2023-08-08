#include <stdlib.h>

#include "gradient_filter.hpp"

#include "libwebp.hpp"

#include "swan.hpp"

#include "utility.hpp"

int gradient_filter_config_init(size_t cache_size,
                                config_t *&config) {

    gradient_filter_config_t *gradient_filter_config = (gradient_filter_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;

    alloc_1D<gradient_filter_config_t>(1, gradient_filter_config);
    gradient_filter_config->num_rows = rows;
    gradient_filter_config->num_cols = columns;
    gradient_filter_config->stride = columns;
    gradient_filter_config->granularity = 1;

    // in/output versions
    size_t input_size = (rows * columns) * sizeof(uint8_t);
    size_t output_size = (rows * columns) * sizeof(uint8_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)gradient_filter_config;

    return count;
}

void gradient_filter_input_init(int count,
                                config_t *config,
                                input_t **&input) {

    gradient_filter_config_t *gradient_filter_config = (gradient_filter_config_t *)config;
    gradient_filter_input_t **gradient_filter_input = (gradient_filter_input_t **)input;

    // initializing input versions
    alloc_1D<gradient_filter_input_t *>(count, gradient_filter_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<gradient_filter_input_t>(1, gradient_filter_input[i]);

        init_alloc_1D<uint8_t>(gradient_filter_config->num_rows * gradient_filter_config->num_cols, gradient_filter_input[i]->in);
    }

    input = (input_t **)gradient_filter_input;
}

void gradient_filter_output_init(int count,
                                 config_t *config,
                                 output_t **&output) {

    gradient_filter_config_t *gradient_filter_config = (gradient_filter_config_t *)config;
    gradient_filter_output_t **gradient_filter_output = (gradient_filter_output_t **)output;

    // initializing output versions
    alloc_1D<gradient_filter_output_t *>(count, gradient_filter_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<gradient_filter_output_t>(1, gradient_filter_output[i]);

        alloc_1D<uint8_t>(gradient_filter_config->num_rows * gradient_filter_config->num_cols, gradient_filter_output[i]->out);
    }

    output = (output_t **)gradient_filter_output;
}

void gradient_filter_comparer(config_t *config,
                              output_t *output_scalar,
                              output_t *output_neon) {
    gradient_filter_config_t *gradient_filter_config = (gradient_filter_config_t *)config;
    gradient_filter_output_t *gradient_filter_output_scalar = (gradient_filter_output_t *)output_scalar;
    gradient_filter_output_t *gradient_filter_output_neon = (gradient_filter_output_t *)output_neon;

    compare_1D<uint8_t>(gradient_filter_config->num_rows * gradient_filter_config->num_cols, gradient_filter_output_scalar->out, gradient_filter_output_neon->out, (char *)"out");
}

kernel_utility_t gradient_filter_utility = {gradient_filter_config_init, gradient_filter_input_init, gradient_filter_output_init, gradient_filter_comparer};