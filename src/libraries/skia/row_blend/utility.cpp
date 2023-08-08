#include "row_blend.hpp"

#include "skia.hpp"

#include "swan.hpp"

#include "utility.hpp"

int row_blend_config_init(size_t cache_size,
                          config_t *&config) {

    row_blend_config_t *row_blend_config = (row_blend_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;

    alloc_1D<row_blend_config_t>(1, row_blend_config);
    row_blend_config->num_rows = rows;
    row_blend_config->num_cols = columns;
    row_blend_config->alpha = 32;
    row_blend_config->granularity = rows;

    // in/output versions
    size_t input_size = rows * columns * sizeof(uint32_t);
    size_t output_size = rows * columns * sizeof(uint32_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)row_blend_config;

    return count;
}

void row_blend_input_init(int count,
                          config_t *config,
                          input_t **&input) {

    row_blend_config_t *row_blend_config = (row_blend_config_t *)config;
    row_blend_input_t **row_blend_input = (row_blend_input_t **)input;

    // initializing input versions
    alloc_1D<row_blend_input_t *>(count, row_blend_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<row_blend_input_t>(1, row_blend_input[i]);

        init_alloc_1D<uint32_t>(row_blend_config->num_rows * row_blend_config->num_cols, row_blend_input[i]->src);
    }

    input = (input_t **)row_blend_input;
}

void row_blend_output_init(int count,
                           config_t *config,
                           output_t **&output) {

    row_blend_config_t *row_blend_config = (row_blend_config_t *)config;
    row_blend_output_t **row_blend_output = (row_blend_output_t **)output;

    // initializing output versions
    alloc_1D<row_blend_output_t *>(count, row_blend_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<row_blend_output_t>(1, row_blend_output[i]);

        alloc_1D<uint32_t>(row_blend_config->num_rows * row_blend_config->num_cols, row_blend_output[i]->dst);
    }

    output = (output_t **)row_blend_output;
}

void row_blend_comparer(config_t *config,
                        output_t *output_scalar,
                        output_t *output_neon) {
    row_blend_config_t *row_blend_config = (row_blend_config_t *)config;
    row_blend_output_t *row_blend_output_scalar = (row_blend_output_t *)output_scalar;
    row_blend_output_t *row_blend_output_neon = (row_blend_output_t *)output_neon;

    compare_1D<uint32_t>(row_blend_config->num_rows * row_blend_config->num_cols, row_blend_output_scalar->dst, row_blend_output_neon->dst, (char *)"dst");
}

kernel_utility_t row_blend_utility = {row_blend_config_init, row_blend_input_init, row_blend_output_init, row_blend_comparer};