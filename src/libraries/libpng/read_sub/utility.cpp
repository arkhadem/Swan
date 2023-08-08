#include "read_sub.hpp"

#include "libpng.hpp"

#include "swan.hpp"

#include "utility.hpp"

int read_sub_config_init(size_t cache_size,
                         config_t *&config) {

    read_sub_config_t *read_sub_config = (read_sub_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE * SWAN_IMG_INPUT_DEFAULT_CHANNEL_SIZE;

    alloc_1D<read_sub_config_t>(1, read_sub_config);
    read_sub_config->num_rows = rows;
    read_sub_config->num_cols = columns;
    read_sub_config->granularity = rows;

    // in/output versions
    size_t input_size = rows * columns * sizeof(png_byte);
    size_t output_size = rows * columns * sizeof(png_byte);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)read_sub_config;

    return count;
}

void read_sub_input_init(int count,
                         config_t *config,
                         input_t **&input) {

    read_sub_config_t *read_sub_config = (read_sub_config_t *)config;
    read_sub_input_t **read_sub_input = (read_sub_input_t **)input;

    // initializing input versions
    alloc_1D<read_sub_input_t *>(count, read_sub_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<read_sub_input_t>(1, read_sub_input[i]);

        init_alloc_2D<png_byte>(read_sub_config->num_rows, read_sub_config->num_cols, read_sub_input[i]->input_buf);
    }

    input = (input_t **)read_sub_input;
}

void read_sub_output_init(int count,
                          config_t *config,
                          output_t **&output) {

    read_sub_config_t *read_sub_config = (read_sub_config_t *)config;
    read_sub_output_t **read_sub_output = (read_sub_output_t **)output;

    // initializing output versions
    alloc_1D<read_sub_output_t *>(count, read_sub_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<read_sub_output_t>(1, read_sub_output[i]);

        alloc_2D<png_byte>(read_sub_config->num_rows, read_sub_config->num_cols, read_sub_output[i]->output_buf);
    }

    output = (output_t **)read_sub_output;
}

void read_sub_comparer(config_t *config,
                       output_t *output_scalar,
                       output_t *output_neon) {
    read_sub_config_t *read_sub_config = (read_sub_config_t *)config;
    read_sub_output_t *read_sub_output_scalar = (read_sub_output_t *)output_scalar;
    read_sub_output_t *read_sub_output_neon = (read_sub_output_t *)output_neon;

    compare_2D<png_byte>(read_sub_config->num_rows, read_sub_config->num_cols, read_sub_output_scalar->output_buf, read_sub_output_neon->output_buf, (char *)"output_buf");
}

kernel_utility_t read_sub_utility = {read_sub_config_init, read_sub_input_init, read_sub_output_init, read_sub_comparer};