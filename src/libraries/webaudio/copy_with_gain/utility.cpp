#include "copy_with_gain.hpp"

#include "webaudio.hpp"

#include "swan.hpp"

#include "utility.hpp"

int copy_with_gain_config_init(size_t cache_size,
                               config_t *&config) {

    copy_with_gain_config_t *copy_with_gain_config = (copy_with_gain_config_t *)config;

    // configuration
    int data_size = 32 * SWAN_AUD_INPUT_SIZE;
    int block_count = SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;
    int number_of_channels = SWAN_AUD_CHANNEL_SIZE;

    alloc_1D<copy_with_gain_config_t>(1, copy_with_gain_config);

    copy_with_gain_config->data_size = data_size;
    copy_with_gain_config->block_count = block_count;
    copy_with_gain_config->gain = 236.2886;
    copy_with_gain_config->number_of_channels = number_of_channels;
    copy_with_gain_config->granularity = block_count;

    // in/output versions
    size_t input_size = block_count * number_of_channels * data_size * sizeof(float);
    size_t output_size = block_count * number_of_channels * data_size * sizeof(float);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)copy_with_gain_config;

    return count;
}

void copy_with_gain_input_init(int count,
                               config_t *config,
                               input_t **&input) {

    copy_with_gain_config_t *copy_with_gain_config = (copy_with_gain_config_t *)config;
    copy_with_gain_input_t **copy_with_gain_input = (copy_with_gain_input_t **)input;

    // initializing input versions
    alloc_1D<copy_with_gain_input_t *>(count, copy_with_gain_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<copy_with_gain_input_t>(1, copy_with_gain_input[i]);

        float **tmp;
        init_alloc_2D<float>(copy_with_gain_config->number_of_channels, copy_with_gain_config->data_size, tmp);
        alloc_1D<float **>(copy_with_gain_config->block_count, copy_with_gain_input[i]->sources);
        for (int block = 0; block < copy_with_gain_config->block_count; block++) {
            copy_with_gain_input[i]->sources[block] = tmp;
        }

        // init_alloc_3D<float>(copy_with_gain_config->block_count, copy_with_gain_config->number_of_channels, copy_with_gain_config->data_size, copy_with_gain_input[i]->sources);
    }

    input = (input_t **)copy_with_gain_input;
}

void copy_with_gain_output_init(int count,
                                config_t *config,
                                output_t **&output) {

    copy_with_gain_config_t *copy_with_gain_config = (copy_with_gain_config_t *)config;
    copy_with_gain_output_t **copy_with_gain_output = (copy_with_gain_output_t **)output;

    // initializing output versions
    alloc_1D<copy_with_gain_output_t *>(count, copy_with_gain_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<copy_with_gain_output_t>(1, copy_with_gain_output[i]);

        alloc_3D<float>(copy_with_gain_config->block_count, copy_with_gain_config->number_of_channels, copy_with_gain_config->data_size, copy_with_gain_output[i]->destinations);
    }

    output = (output_t **)copy_with_gain_output;
}

void copy_with_gain_comparer(config_t *config,
                             output_t *output_scalar,
                             output_t *output_neon) {
    copy_with_gain_config_t *copy_with_gain_config = (copy_with_gain_config_t *)config;
    copy_with_gain_output_t *copy_with_gain_output_scalar = (copy_with_gain_output_t *)output_scalar;
    copy_with_gain_output_t *copy_with_gain_output_neon = (copy_with_gain_output_t *)output_neon;

    compare_3D<float>(copy_with_gain_config->block_count, copy_with_gain_config->number_of_channels, copy_with_gain_config->data_size, copy_with_gain_output_scalar->destinations, copy_with_gain_output_neon->destinations, (char *)"destinations");
}

kernel_utility_t copy_with_gain_utility = {copy_with_gain_config_init, copy_with_gain_input_init, copy_with_gain_output_init, copy_with_gain_comparer};