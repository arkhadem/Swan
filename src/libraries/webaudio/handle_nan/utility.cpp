#include "handle_nan.hpp"

#include "webaudio.hpp"

#include "swan.hpp"

#include "utility.hpp"

int handle_nan_config_init(size_t cache_size,
                           config_t *&config) {

    handle_nan_config_t *handle_nan_config = (handle_nan_config_t *)config;

    // configuration
    // configuration
    int number_of_values = 32 * SWAN_AUD_INPUT_SIZE;
    int block_count = SWAN_AUD_CHANNEL_SIZE * SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;

    alloc_1D<handle_nan_config_t>(1, handle_nan_config);

    handle_nan_config->number_of_values = number_of_values;
    handle_nan_config->block_count = block_count;
    handle_nan_config->granularity = block_count;

    // in/output versions
    size_t input_size = block_count * number_of_values * sizeof(float);
    size_t output_size = block_count * number_of_values * sizeof(float);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)handle_nan_config;

    return count;
}

void handle_nan_input_init(int count,
                           config_t *config,
                           input_t **&input) {

    handle_nan_config_t *handle_nan_config = (handle_nan_config_t *)config;
    handle_nan_input_t **handle_nan_input = (handle_nan_input_t **)input;

    // initializing input versions
    alloc_1D<handle_nan_input_t *>(count, handle_nan_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<handle_nan_input_t>(1, handle_nan_input[i]);

        float *tmp;
        init_alloc_1D<float>(handle_nan_config->number_of_values, tmp);
        alloc_1D<float *>(handle_nan_config->block_count, handle_nan_input[i]->input_values);
        for (int block = 0; block < handle_nan_config->block_count; block++) {
            handle_nan_input[i]->input_values[block] = tmp;
        }

        // init_alloc_2D<float>(handle_nan_config->block_count, handle_nan_config->number_of_values, handle_nan_input[i]->input_values);
    }

    input = (input_t **)handle_nan_input;
}

void handle_nan_output_init(int count,
                            config_t *config,
                            output_t **&output) {

    handle_nan_config_t *handle_nan_config = (handle_nan_config_t *)config;
    handle_nan_output_t **handle_nan_output = (handle_nan_output_t **)output;

    // initializing output versions
    alloc_1D<handle_nan_output_t *>(count, handle_nan_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<handle_nan_output_t>(1, handle_nan_output[i]);

        alloc_2D<float>(handle_nan_config->block_count, handle_nan_config->number_of_values, handle_nan_output[i]->output_values);
    }

    output = (output_t **)handle_nan_output;
}

void handle_nan_comparer(config_t *config,
                         output_t *output_scalar,
                         output_t *output_neon) {
    handle_nan_config_t *handle_nan_config = (handle_nan_config_t *)config;
    handle_nan_output_t *handle_nan_output_scalar = (handle_nan_output_t *)output_scalar;
    handle_nan_output_t *handle_nan_output_neon = (handle_nan_output_t *)output_neon;

    compare_2D<float>(handle_nan_config->block_count, handle_nan_config->number_of_values, handle_nan_output_scalar->output_values, handle_nan_output_neon->output_values, (char *)"output_values");
}

kernel_utility_t handle_nan_utility = {handle_nan_config_init, handle_nan_input_init, handle_nan_output_init, handle_nan_comparer};