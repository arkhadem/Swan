#include "convolve_horizontally.hpp"

#include "skia.hpp"

#include "swan.hpp"

#include "utility.hpp"

int convolve_horizontally_config_init(size_t cache_size,
                                      config_t *&config) {

    convolve_horizontally_config_t *convolve_horizontally_config = (convolve_horizontally_config_t *)config;

    // configuration
#ifdef SWAN_SIMULATION
    int rows = 1;
#else
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
#endif
    int columns = SWAN_IMG_INPUT_COL_SIZE;
    int filter_length = 32;

    alloc_1D<convolve_horizontally_config_t>(1, convolve_horizontally_config);
    convolve_horizontally_config->num_rows = rows;
    convolve_horizontally_config->num_cols = columns;
    convolve_horizontally_config->filter_length = filter_length;
    convolve_horizontally_config->shift_value = 2;
    convolve_horizontally_config->granularity = rows;

    // in/output versions
    size_t input_size = 4 * rows * (columns + filter_length) * sizeof(uint8_t);
    input_size += filter_length * sizeof(int16_t);
    size_t output_size = 4 * rows * columns * sizeof(uint8_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)convolve_horizontally_config;

    return count;
}

void convolve_horizontally_input_init(int count,
                                      config_t *config,
                                      input_t **&input) {

    convolve_horizontally_config_t *convolve_horizontally_config = (convolve_horizontally_config_t *)config;
    convolve_horizontally_input_t **convolve_horizontally_input = (convolve_horizontally_input_t **)input;

    // initializing input versions
    alloc_1D<convolve_horizontally_input_t *>(count, convolve_horizontally_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<convolve_horizontally_input_t>(1, convolve_horizontally_input[i]);

        init_alloc_1D<uint8_t>(4 * convolve_horizontally_config->num_rows * (convolve_horizontally_config->num_cols + convolve_horizontally_config->filter_length), convolve_horizontally_input[i]->src_data);
        init_alloc_1D<int16_t>(convolve_horizontally_config->filter_length, convolve_horizontally_input[i]->filter_values);
    }

    input = (input_t **)convolve_horizontally_input;
}

void convolve_horizontally_output_init(int count,
                                       config_t *config,
                                       output_t **&output) {

    convolve_horizontally_config_t *convolve_horizontally_config = (convolve_horizontally_config_t *)config;
    convolve_horizontally_output_t **convolve_horizontally_output = (convolve_horizontally_output_t **)output;

    // initializing output versions
    alloc_1D<convolve_horizontally_output_t *>(count, convolve_horizontally_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<convolve_horizontally_output_t>(1, convolve_horizontally_output[i]);

        alloc_1D<uint8_t>(4 * convolve_horizontally_config->num_rows * convolve_horizontally_config->num_cols, convolve_horizontally_output[i]->out_row);
    }

    output = (output_t **)convolve_horizontally_output;
}

void convolve_horizontally_comparer(config_t *config,
                                    output_t *output_scalar,
                                    output_t *output_neon) {
    convolve_horizontally_config_t *convolve_horizontally_config = (convolve_horizontally_config_t *)config;
    convolve_horizontally_output_t *convolve_horizontally_output_scalar = (convolve_horizontally_output_t *)output_scalar;
    convolve_horizontally_output_t *convolve_horizontally_output_neon = (convolve_horizontally_output_t *)output_neon;

    compare_1D<uint8_t>(4 * convolve_horizontally_config->num_rows * convolve_horizontally_config->num_cols, convolve_horizontally_output_scalar->out_row, convolve_horizontally_output_neon->out_row, (char *)"out_row");
}

kernel_utility_t convolve_horizontally_utility = {convolve_horizontally_config_init, convolve_horizontally_input_init, convolve_horizontally_output_init, convolve_horizontally_comparer};