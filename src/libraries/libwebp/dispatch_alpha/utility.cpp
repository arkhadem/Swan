#include "dispatch_alpha.hpp"

#include "libwebp.hpp"

#include "swan.hpp"

#include "utility.hpp"

int dispatch_alpha_config_init(size_t cache_size,
                               config_t *&config) {

    dispatch_alpha_config_t *dispatch_alpha_config = (dispatch_alpha_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;

    alloc_1D<dispatch_alpha_config_t>(1, dispatch_alpha_config);
    dispatch_alpha_config->num_rows = rows;
    dispatch_alpha_config->num_cols = columns;
    dispatch_alpha_config->granularity = 1;

    // in/output versions
    size_t input_size = 4 * (rows * columns) * sizeof(uint8_t);
    size_t output_size = (rows * columns) * sizeof(uint8_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)dispatch_alpha_config;

    return count;
}

void dispatch_alpha_input_init(int count,
                               config_t *config,
                               input_t **&input) {

    dispatch_alpha_config_t *dispatch_alpha_config = (dispatch_alpha_config_t *)config;
    dispatch_alpha_input_t **dispatch_alpha_input = (dispatch_alpha_input_t **)input;

    // initializing input versions
    alloc_1D<dispatch_alpha_input_t *>(count, dispatch_alpha_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<dispatch_alpha_input_t>(1, dispatch_alpha_input[i]);

        init_alloc_1D<uint8_t>(4 * dispatch_alpha_config->num_rows * dispatch_alpha_config->num_cols, dispatch_alpha_input[i]->src);

        init_alloc_1D<uint8_t>(dispatch_alpha_config->num_rows * dispatch_alpha_config->num_cols, dispatch_alpha_input[i]->alpha);
    }

    input = (input_t **)dispatch_alpha_input;
}

void dispatch_alpha_output_init(int count,
                                config_t *config,
                                output_t **&output) {

    dispatch_alpha_config_t *dispatch_alpha_config = (dispatch_alpha_config_t *)config;
    dispatch_alpha_output_t **dispatch_alpha_output = (dispatch_alpha_output_t **)output;

    // initializing output versions
    alloc_1D<dispatch_alpha_output_t *>(count, dispatch_alpha_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<dispatch_alpha_output_t>(1, dispatch_alpha_output[i]);

        alloc_1D<uint8_t>(4 * dispatch_alpha_config->num_rows * dispatch_alpha_config->num_cols, dispatch_alpha_output[i]->dst);

        alloc_1D<int>(1, dispatch_alpha_output[i]->return_val);
    }

    output = (output_t **)dispatch_alpha_output;
}

void dispatch_alpha_comparer(config_t *config,
                             output_t *output_scalar,
                             output_t *output_neon) {
    dispatch_alpha_config_t *dispatch_alpha_config = (dispatch_alpha_config_t *)config;
    dispatch_alpha_output_t *dispatch_alpha_output_scalar = (dispatch_alpha_output_t *)output_scalar;
    dispatch_alpha_output_t *dispatch_alpha_output_neon = (dispatch_alpha_output_t *)output_neon;

    compare_1D<uint8_t>(4 * dispatch_alpha_config->num_rows * dispatch_alpha_config->num_cols, dispatch_alpha_output_scalar->dst, dispatch_alpha_output_neon->dst, (char *)"dst");
    compare_1D<int>(1, dispatch_alpha_output_scalar->return_val, dispatch_alpha_output_neon->return_val, (char *)"return_val");
}

kernel_utility_t dispatch_alpha_utility = {dispatch_alpha_config_init, dispatch_alpha_input_init, dispatch_alpha_output_init, dispatch_alpha_comparer};