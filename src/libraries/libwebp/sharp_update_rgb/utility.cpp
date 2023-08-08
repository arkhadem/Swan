#include "sharp_update_rgb.hpp"

#include "libwebp.hpp"

#include "swan.hpp"

#include "utility.hpp"

int sharp_update_rgb_config_init(size_t cache_size,
                                 config_t *&config) {

    sharp_update_rgb_config_t *sharp_update_rgb_config = (sharp_update_rgb_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE * 3;

    alloc_1D<sharp_update_rgb_config_t>(1, sharp_update_rgb_config);
    sharp_update_rgb_config->num_rows = rows;
    sharp_update_rgb_config->num_cols = columns;
    sharp_update_rgb_config->granularity = rows;

    // in/output versions
    size_t input_size = 2 * rows * columns * sizeof(int16_t);
    size_t output_size = rows * columns * sizeof(int16_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)sharp_update_rgb_config;

    return count;
}

void sharp_update_rgb_input_init(int count,
                                 config_t *config,
                                 input_t **&input) {

    sharp_update_rgb_config_t *sharp_update_rgb_config = (sharp_update_rgb_config_t *)config;
    sharp_update_rgb_input_t **sharp_update_rgb_input = (sharp_update_rgb_input_t **)input;

    // initializing input versions
    alloc_1D<sharp_update_rgb_input_t *>(count, sharp_update_rgb_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sharp_update_rgb_input_t>(1, sharp_update_rgb_input[i]);

        init_alloc_2D<int16_t>(sharp_update_rgb_config->num_rows, sharp_update_rgb_config->num_cols, sharp_update_rgb_input[i]->ref);
        init_alloc_2D<int16_t>(sharp_update_rgb_config->num_rows, sharp_update_rgb_config->num_cols, sharp_update_rgb_input[i]->src);
    }

    input = (input_t **)sharp_update_rgb_input;
}

void sharp_update_rgb_output_init(int count,
                                  config_t *config,
                                  output_t **&output) {

    sharp_update_rgb_config_t *sharp_update_rgb_config = (sharp_update_rgb_config_t *)config;
    sharp_update_rgb_output_t **sharp_update_rgb_output = (sharp_update_rgb_output_t **)output;

    // initializing output versions
    alloc_1D<sharp_update_rgb_output_t *>(count, sharp_update_rgb_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sharp_update_rgb_output_t>(1, sharp_update_rgb_output[i]);

        alloc_2D<int16_t>(sharp_update_rgb_config->num_rows, sharp_update_rgb_config->num_cols, sharp_update_rgb_output[i]->dst);
    }

    output = (output_t **)sharp_update_rgb_output;
}

void sharp_update_rgb_comparer(config_t *config,
                               output_t *output_scalar,
                               output_t *output_neon) {
    sharp_update_rgb_config_t *sharp_update_rgb_config = (sharp_update_rgb_config_t *)config;
    sharp_update_rgb_output_t *sharp_update_rgb_output_scalar = (sharp_update_rgb_output_t *)output_scalar;
    sharp_update_rgb_output_t *sharp_update_rgb_output_neon = (sharp_update_rgb_output_t *)output_neon;

    compare_2D<int16_t>(sharp_update_rgb_config->num_rows, sharp_update_rgb_config->num_cols, sharp_update_rgb_output_scalar->dst, sharp_update_rgb_output_neon->dst, (char *)"dst");
}

kernel_utility_t sharp_update_rgb_utility = {sharp_update_rgb_config_init, sharp_update_rgb_input_init, sharp_update_rgb_output_init, sharp_update_rgb_comparer};