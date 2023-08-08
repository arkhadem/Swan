#include "sharp_filter_row.hpp"

#include "libwebp.hpp"

#include "swan.hpp"

#include "utility.hpp"

int sharp_filter_row_config_init(size_t cache_size,
                                 config_t *&config) {

    sharp_filter_row_config_t *sharp_filter_row_config = (sharp_filter_row_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;

    alloc_1D<sharp_filter_row_config_t>(1, sharp_filter_row_config);
    sharp_filter_row_config->num_rows = rows;
    sharp_filter_row_config->num_cols = columns;
    sharp_filter_row_config->bit_depth = 4;
    sharp_filter_row_config->granularity = rows;

    // in/output versions
    size_t input_size = rows * 2 * (columns + 1) * sizeof(int16_t);
    input_size += rows * 2 * columns * sizeof(uint16_t);
    size_t output_size = rows * 2 * columns * sizeof(uint16_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)sharp_filter_row_config;

    return count;
}

void sharp_filter_row_input_init(int count,
                                 config_t *config,
                                 input_t **&input) {

    sharp_filter_row_config_t *sharp_filter_row_config = (sharp_filter_row_config_t *)config;
    sharp_filter_row_input_t **sharp_filter_row_input = (sharp_filter_row_input_t **)input;

    // initializing input versions
    alloc_1D<sharp_filter_row_input_t *>(count, sharp_filter_row_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sharp_filter_row_input_t>(1, sharp_filter_row_input[i]);

        init_alloc_2D<int16_t>(sharp_filter_row_config->num_rows, (sharp_filter_row_config->num_cols + 1), sharp_filter_row_input[i]->A);
        init_alloc_2D<int16_t>(sharp_filter_row_config->num_rows, (sharp_filter_row_config->num_cols + 1), sharp_filter_row_input[i]->B);
        init_alloc_2D<uint16_t>(sharp_filter_row_config->num_rows, 2 * sharp_filter_row_config->num_cols, sharp_filter_row_input[i]->best_y);
    }

    input = (input_t **)sharp_filter_row_input;
}

void sharp_filter_row_output_init(int count,
                                  config_t *config,
                                  output_t **&output) {

    sharp_filter_row_config_t *sharp_filter_row_config = (sharp_filter_row_config_t *)config;
    sharp_filter_row_output_t **sharp_filter_row_output = (sharp_filter_row_output_t **)output;

    // initializing output versions
    alloc_1D<sharp_filter_row_output_t *>(count, sharp_filter_row_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sharp_filter_row_output_t>(1, sharp_filter_row_output[i]);

        alloc_2D<uint16_t>(sharp_filter_row_config->num_rows, 2 * sharp_filter_row_config->num_cols, sharp_filter_row_output[i]->out);
    }

    output = (output_t **)sharp_filter_row_output;
}

void sharp_filter_row_comparer(config_t *config,
                               output_t *output_scalar,
                               output_t *output_neon) {
    sharp_filter_row_config_t *sharp_filter_row_config = (sharp_filter_row_config_t *)config;
    sharp_filter_row_output_t *sharp_filter_row_output_scalar = (sharp_filter_row_output_t *)output_scalar;
    sharp_filter_row_output_t *sharp_filter_row_output_neon = (sharp_filter_row_output_t *)output_neon;

    compare_2D<uint16_t>(sharp_filter_row_config->num_rows, 2 * sharp_filter_row_config->num_cols, sharp_filter_row_output_scalar->out, sharp_filter_row_output_neon->out, (char *)"out");
}

kernel_utility_t sharp_filter_row_utility = {sharp_filter_row_config_init, sharp_filter_row_input_init, sharp_filter_row_output_init, sharp_filter_row_comparer};