#include "lpc_prediction.hpp"

#include "libopus.hpp"

#include "swan.hpp"

#include "utility.hpp"

int lpc_prediction_config_init(size_t cache_size,
                               config_t *&config) {

    lpc_prediction_config_t *lpc_prediction_config = (lpc_prediction_config_t *)config;

    // configuration
    opus_int order = 4;
    int block_count = SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;

    alloc_1D<lpc_prediction_config_t>(1, lpc_prediction_config);
    lpc_prediction_config->order = order;
    lpc_prediction_config->block_count = block_count;
    lpc_prediction_config->granularity = block_count;

    // in/output versions
    size_t input_size = block_count * order * sizeof(opus_int16);
    size_t output_size = block_count * sizeof(opus_int32);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)lpc_prediction_config;

    return count;
}

void lpc_prediction_input_init(int count,
                               config_t *config,
                               input_t **&input) {

    lpc_prediction_config_t *lpc_prediction_config = (lpc_prediction_config_t *)config;
    lpc_prediction_input_t **lpc_prediction_input = (lpc_prediction_input_t **)input;

    // initializing input versions
    alloc_1D<lpc_prediction_input_t *>(count, lpc_prediction_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<lpc_prediction_input_t>(1, lpc_prediction_input[i]);

        init_alloc_2D<opus_int16>(lpc_prediction_config->block_count, lpc_prediction_config->order, lpc_prediction_input[i]->A_Q12);
    }

    input = (input_t **)lpc_prediction_input;
}

void lpc_prediction_output_init(int count,
                                config_t *config,
                                output_t **&output) {

    lpc_prediction_config_t *lpc_prediction_config = (lpc_prediction_config_t *)config;
    lpc_prediction_output_t **lpc_prediction_output = (lpc_prediction_output_t **)output;

    // initializing output versions
    alloc_1D<lpc_prediction_output_t *>(count, lpc_prediction_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<lpc_prediction_output_t>(1, lpc_prediction_output[i]);

        alloc_1D<opus_int32>(lpc_prediction_config->block_count, lpc_prediction_output[i]->return_val);
    }

    output = (output_t **)lpc_prediction_output;
}

void lpc_prediction_comparer(config_t *config,
                             output_t *output_scalar,
                             output_t *output_neon) {
    lpc_prediction_config_t *lpc_prediction_config = (lpc_prediction_config_t *)config;
    lpc_prediction_output_t *lpc_prediction_output_scalar = (lpc_prediction_output_t *)output_scalar;
    lpc_prediction_output_t *lpc_prediction_output_neon = (lpc_prediction_output_t *)output_neon;

    compare_1D<opus_int32>(lpc_prediction_config->block_count, lpc_prediction_output_scalar->return_val, lpc_prediction_output_neon->return_val, (char *)"return_val");
}

kernel_utility_t lpc_prediction_utility = {lpc_prediction_config_init, lpc_prediction_input_init, lpc_prediction_output_init, lpc_prediction_comparer};