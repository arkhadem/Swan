#include "convolve_vertically.hpp"

#include "skia.hpp"

#include "swan.hpp"

#include "utility.hpp"

int convolve_vertically_config_init(size_t cache_size,
                                    config_t *&config) {

    convolve_vertically_config_t *convolve_vertically_config = (convolve_vertically_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;
    int filter_length = 32;

    alloc_1D<convolve_vertically_config_t>(1, convolve_vertically_config);
    convolve_vertically_config->num_rows = rows;
    convolve_vertically_config->num_cols = columns;
    convolve_vertically_config->filter_length = filter_length;
    convolve_vertically_config->shift_value = 2;
    convolve_vertically_config->granularity = rows;

    // in/output versions
    size_t input_size = (rows + filter_length) * (columns * 4) * sizeof(uint8_t);
    input_size += filter_length * sizeof(int16_t);
    size_t output_size = (rows) * (4 * columns) * sizeof(uint8_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)convolve_vertically_config;

    return count;
}

void convolve_vertically_input_init(int count,
                                    config_t *config,
                                    input_t **&input) {

    convolve_vertically_config_t *convolve_vertically_config = (convolve_vertically_config_t *)config;
    convolve_vertically_input_t **convolve_vertically_input = (convolve_vertically_input_t **)input;

    // initializing input versions
    alloc_1D<convolve_vertically_input_t *>(count, convolve_vertically_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<convolve_vertically_input_t>(1, convolve_vertically_input[i]);

        init_alloc_2D<uint8_t>(convolve_vertically_config->num_rows + convolve_vertically_config->filter_length, convolve_vertically_config->num_cols * 4, convolve_vertically_input[i]->src_data);
        init_alloc_1D<int16_t>(convolve_vertically_config->filter_length, convolve_vertically_input[i]->filter_values);
    }

    input = (input_t **)convolve_vertically_input;
}

void convolve_vertically_output_init(int count,
                                     config_t *config,
                                     output_t **&output) {

    convolve_vertically_config_t *convolve_vertically_config = (convolve_vertically_config_t *)config;
    convolve_vertically_output_t **convolve_vertically_output = (convolve_vertically_output_t **)output;

    // initializing output versions
    alloc_1D<convolve_vertically_output_t *>(count, convolve_vertically_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<convolve_vertically_output_t>(1, convolve_vertically_output[i]);

        alloc_2D<uint8_t>(convolve_vertically_config->num_rows, 4 * convolve_vertically_config->num_cols, convolve_vertically_output[i]->out_row);
    }

    output = (output_t **)convolve_vertically_output;
}

void convolve_vertically_comparer(config_t *config,
                                  output_t *output_scalar,
                                  output_t *output_neon) {
    convolve_vertically_config_t *convolve_vertically_config = (convolve_vertically_config_t *)config;
    convolve_vertically_output_t *convolve_vertically_output_scalar = (convolve_vertically_output_t *)output_scalar;
    convolve_vertically_output_t *convolve_vertically_output_neon = (convolve_vertically_output_t *)output_neon;

    compare_2D<uint8_t>(convolve_vertically_config->num_rows, 4 * convolve_vertically_config->num_cols, convolve_vertically_output_scalar->out_row, convolve_vertically_output_neon->out_row, (char *)"out_row");
}

kernel_utility_t convolve_vertically_utility = {convolve_vertically_config_init, convolve_vertically_input_init, convolve_vertically_output_init, convolve_vertically_comparer};