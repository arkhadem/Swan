#include "apply_alpha_multiply.hpp"

#include "libwebp.hpp"

#include "swan.hpp"

#include "utility.hpp"

int apply_alpha_multiply_config_init(size_t cache_size,
                                     config_t *&config) {

    apply_alpha_multiply_config_t *apply_alpha_multiply_config = (apply_alpha_multiply_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;

    alloc_1D<apply_alpha_multiply_config_t>(1, apply_alpha_multiply_config);
    apply_alpha_multiply_config->num_rows = rows;
    apply_alpha_multiply_config->num_cols = columns;
    apply_alpha_multiply_config->alpha_first = false;
    apply_alpha_multiply_config->granularity = 1;

    // in/output versions
    size_t input_size = 4 * (rows * columns) * sizeof(uint8_t);
    size_t output_size = 0;
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)apply_alpha_multiply_config;

    return count;
}

void apply_alpha_multiply_input_init(int count,
                                     config_t *config,
                                     input_t **&input) {

    apply_alpha_multiply_config_t *apply_alpha_multiply_config = (apply_alpha_multiply_config_t *)config;
    apply_alpha_multiply_input_t **apply_alpha_multiply_input = (apply_alpha_multiply_input_t **)input;

    // initializing input versions
    alloc_1D<apply_alpha_multiply_input_t *>(count, apply_alpha_multiply_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<apply_alpha_multiply_input_t>(1, apply_alpha_multiply_input[i]);

        init_alloc_1D<uint8_t>(4 * apply_alpha_multiply_config->num_rows * apply_alpha_multiply_config->num_cols, apply_alpha_multiply_input[i]->rgba_in);
    }

    input = (input_t **)apply_alpha_multiply_input;
}

void apply_alpha_multiply_output_init(int count,
                                      config_t *config,
                                      output_t **&output) {

    apply_alpha_multiply_config_t *apply_alpha_multiply_config = (apply_alpha_multiply_config_t *)config;
    apply_alpha_multiply_output_t **apply_alpha_multiply_output = (apply_alpha_multiply_output_t **)output;

    // initializing output versions
    alloc_1D<apply_alpha_multiply_output_t *>(count, apply_alpha_multiply_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<apply_alpha_multiply_output_t>(1, apply_alpha_multiply_output[i]);

        alloc_1D<uint8_t>(4 * apply_alpha_multiply_config->num_rows * apply_alpha_multiply_config->num_cols, apply_alpha_multiply_output[i]->rgba_out);
    }

    output = (output_t **)apply_alpha_multiply_output;
}

void apply_alpha_multiply_comparer(config_t *config,
                                   output_t *output_scalar,
                                   output_t *output_neon) {
    apply_alpha_multiply_config_t *apply_alpha_multiply_config = (apply_alpha_multiply_config_t *)config;
    apply_alpha_multiply_output_t *apply_alpha_multiply_output_scalar = (apply_alpha_multiply_output_t *)output_scalar;
    apply_alpha_multiply_output_t *apply_alpha_multiply_output_neon = (apply_alpha_multiply_output_t *)output_neon;

    compare_1D<uint8_t>(4 * apply_alpha_multiply_config->num_rows * apply_alpha_multiply_config->num_cols, apply_alpha_multiply_output_scalar->rgba_out, apply_alpha_multiply_output_neon->rgba_out, (char *)"rgba_out");
}

kernel_utility_t apply_alpha_multiply_utility = {apply_alpha_multiply_config_init, apply_alpha_multiply_input_init, apply_alpha_multiply_output_init, apply_alpha_multiply_comparer};
