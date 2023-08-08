#include "sum_from.hpp"

#include "webaudio.hpp"

#include "swan.hpp"

#include "utility.hpp"

int sum_from_config_init(size_t cache_size,
                         config_t *&config) {

    sum_from_config_t *sum_from_config = (sum_from_config_t *)config;

    // configuration
    int data_size = 32 * SWAN_AUD_INPUT_SIZE;
    int block_count = SWAN_AUD_CHANNEL_SIZE * SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;

    alloc_1D<sum_from_config_t>(1, sum_from_config);

    sum_from_config->data_size = data_size;
    sum_from_config->block_count = block_count;
    sum_from_config->granularity = block_count;

    // in/output versions
    size_t input_size = 2 * block_count * data_size * sizeof(float);
    size_t output_size = block_count * data_size * sizeof(float);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)sum_from_config;

    return count;
}

void sum_from_input_init(int count,
                         config_t *config,
                         input_t **&input) {

    sum_from_config_t *sum_from_config = (sum_from_config_t *)config;
    sum_from_input_t **sum_from_input = (sum_from_input_t **)input;

    // initializing input versions
    alloc_1D<sum_from_input_t *>(count, sum_from_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sum_from_input_t>(1, sum_from_input[i]);

        float *tmp1;
        float *tmp2;
        init_alloc_1D<float>(sum_from_config->data_size, tmp1);
        init_alloc_1D<float>(sum_from_config->data_size, tmp2);
        alloc_1D<float *>(sum_from_config->block_count, sum_from_input[i]->source1);
        alloc_1D<float *>(sum_from_config->block_count, sum_from_input[i]->source2);
        for (int block = 0; block < sum_from_config->block_count; block++) {
            sum_from_input[i]->source1[block] = tmp1;
            sum_from_input[i]->source2[block] = tmp2;
        }

        // init_alloc_2D<float>(sum_from_config->block_count, sum_from_config->data_size, sum_from_input[i]->source1);
        // init_alloc_2D<float>(sum_from_config->block_count, sum_from_config->data_size, sum_from_input[i]->source2);
    }

    input = (input_t **)sum_from_input;
}

void sum_from_output_init(int count,
                          config_t *config,
                          output_t **&output) {

    sum_from_config_t *sum_from_config = (sum_from_config_t *)config;
    sum_from_output_t **sum_from_output = (sum_from_output_t **)output;

    // initializing output versions
    alloc_1D<sum_from_output_t *>(count, sum_from_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sum_from_output_t>(1, sum_from_output[i]);

        alloc_2D<float>(sum_from_config->block_count, sum_from_config->data_size, sum_from_output[i]->destination);
    }

    output = (output_t **)sum_from_output;
}

void sum_from_comparer(config_t *config,
                       output_t *output_scalar,
                       output_t *output_neon) {
    sum_from_config_t *sum_from_config = (sum_from_config_t *)config;
    sum_from_output_t *sum_from_output_scalar = (sum_from_output_t *)output_scalar;
    sum_from_output_t *sum_from_output_neon = (sum_from_output_t *)output_neon;

    compare_2D<float>(sum_from_config->block_count, sum_from_config->data_size, sum_from_output_scalar->destination, sum_from_output_neon->destination, (char *)"destination");
}

kernel_utility_t sum_from_utility = {sum_from_config_init, sum_from_input_init, sum_from_output_init, sum_from_comparer};