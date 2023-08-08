#include "read_avg.hpp"

#include "libpng.hpp"

#include "swan.hpp"

#include "utility.hpp"

int read_avg_config_init(size_t cache_size,
                         config_t *&config) {

    read_avg_config_t *read_avg_config = (read_avg_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE * SWAN_IMG_INPUT_DEFAULT_CHANNEL_SIZE;

    alloc_1D<read_avg_config_t>(1, read_avg_config);
    read_avg_config->num_rows = rows;
    read_avg_config->num_cols = columns;
    read_avg_config->granularity = rows;

    // in/output versions
    size_t input_size = 2 * rows * columns * sizeof(png_byte);
    size_t output_size = rows * columns * sizeof(png_byte);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)read_avg_config;

    return count;
}

void read_avg_input_init(int count,
                         config_t *config,
                         input_t **&input) {

    read_avg_config_t *read_avg_config = (read_avg_config_t *)config;
    read_avg_input_t **read_avg_input = (read_avg_input_t **)input;

    // initializing input versions
    alloc_1D<read_avg_input_t *>(count, read_avg_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<read_avg_input_t>(1, read_avg_input[i]);

        png_byte **tmp;

        init_alloc_2D<png_byte>(read_avg_config->num_rows + 1, read_avg_config->num_cols, tmp);

        alloc_1D<png_byte *>(read_avg_config->num_rows, read_avg_input[i]->input_buf);
        alloc_1D<png_byte *>(read_avg_config->num_rows, read_avg_input[i]->prev_input_buf);
        for (int row = 0; row < read_avg_config->num_rows; row++) {
            read_avg_input[i]->input_buf[row] = tmp[row + 1];
            read_avg_input[i]->prev_input_buf[row] = tmp[row];
        }

        // init_alloc_2D<png_byte>(read_avg_config->num_rows, read_avg_config->num_cols, read_avg_input[i]->input_buf);
        // init_alloc_2D<png_byte>(read_avg_config->num_rows, read_avg_config->num_cols, read_avg_input[i]->prev_input_buf);
    }

    input = (input_t **)read_avg_input;
}

void read_avg_output_init(int count,
                          config_t *config,
                          output_t **&output) {

    read_avg_config_t *read_avg_config = (read_avg_config_t *)config;
    read_avg_output_t **read_avg_output = (read_avg_output_t **)output;

    // initializing output versions
    alloc_1D<read_avg_output_t *>(count, read_avg_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<read_avg_output_t>(1, read_avg_output[i]);

        alloc_2D<png_byte>(read_avg_config->num_rows, read_avg_config->num_cols, read_avg_output[i]->output_buf);
    }

    output = (output_t **)read_avg_output;
}

void read_avg_comparer(config_t *config,
                       output_t *output_scalar,
                       output_t *output_neon) {
    read_avg_config_t *read_avg_config = (read_avg_config_t *)config;
    read_avg_output_t *read_avg_output_scalar = (read_avg_output_t *)output_scalar;
    read_avg_output_t *read_avg_output_neon = (read_avg_output_t *)output_neon;

    compare_2D<png_byte>(read_avg_config->num_rows, read_avg_config->num_cols, read_avg_output_scalar->output_buf, read_avg_output_neon->output_buf, (char *)"output_buf");
}

kernel_utility_t read_avg_utility = {read_avg_config_init, read_avg_input_init, read_avg_output_init, read_avg_comparer};