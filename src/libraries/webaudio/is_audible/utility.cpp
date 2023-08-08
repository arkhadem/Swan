#include "is_audible.hpp"

#include "webaudio.hpp"

#include "swan.hpp"

#include "utility.hpp"

int is_audible_config_init(size_t cache_size,
                           config_t *&config) {

    is_audible_config_t *is_audible_config = (is_audible_config_t *)config;

    // configuration
#ifdef SWAN_SIMULATION
    int data_size = SWAN_AUD_INPUT_SIZE;
    int block_count = 64;
#else
    int data_size = 32 * SWAN_AUD_INPUT_SIZE;
    int block_count = SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;
#endif

    int number_of_channels = SWAN_AUD_CHANNEL_SIZE;

    alloc_1D<is_audible_config_t>(1, is_audible_config);

    is_audible_config->data_size = data_size;
    is_audible_config->block_count = block_count;
    is_audible_config->number_of_channels = number_of_channels;
    is_audible_config->granularity = block_count;

    // in/output versions
    size_t input_size = block_count * number_of_channels * data_size * sizeof(float);
    size_t output_size = block_count * sizeof(int);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)is_audible_config;

    return count;
}

void is_audible_input_init(int count,
                           config_t *config,
                           input_t **&input) {

    is_audible_config_t *is_audible_config = (is_audible_config_t *)config;
    is_audible_input_t **is_audible_input = (is_audible_input_t **)input;

    // initializing input versions
    alloc_1D<is_audible_input_t *>(count, is_audible_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<is_audible_input_t>(1, is_audible_input[i]);

        float **tmp;
        init_alloc_2D<float>(is_audible_config->number_of_channels, is_audible_config->data_size, tmp, -10, 10);
        alloc_1D<float **>(is_audible_config->block_count, is_audible_input[i]->data);
        for (int block = 0; block < is_audible_config->block_count; block++) {
            is_audible_input[i]->data[block] = tmp;
        }

        // init_alloc_3D<float>(is_audible_config->block_count, is_audible_config->number_of_channels, is_audible_config->data_size, is_audible_input[i]->data);
    }

    input = (input_t **)is_audible_input;
}

void is_audible_output_init(int count,
                            config_t *config,
                            output_t **&output) {

    is_audible_config_t *is_audible_config = (is_audible_config_t *)config;
    is_audible_output_t **is_audible_output = (is_audible_output_t **)output;

    // initializing output versions
    alloc_1D<is_audible_output_t *>(count, is_audible_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<is_audible_output_t>(1, is_audible_output[i]);

        alloc_1D<int>(is_audible_config->block_count, is_audible_output[i]->return_value);
    }

    output = (output_t **)is_audible_output;
}

void is_audible_comparer(config_t *config,
                         output_t *output_scalar,
                         output_t *output_neon) {
    is_audible_config_t *is_audible_config = (is_audible_config_t *)config;
    is_audible_output_t *is_audible_output_scalar = (is_audible_output_t *)output_scalar;
    is_audible_output_t *is_audible_output_neon = (is_audible_output_t *)output_neon;

    compare_1D<int>(is_audible_config->block_count, is_audible_output_scalar->return_value, is_audible_output_neon->return_value, (char *)"return_value");
}

kernel_utility_t is_audible_utility = {is_audible_config_init, is_audible_input_init, is_audible_output_init, is_audible_comparer};