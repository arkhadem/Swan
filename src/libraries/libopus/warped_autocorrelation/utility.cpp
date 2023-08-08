#include "warped_autocorrelation.hpp"

#include "libopus.hpp"

#include "swan.hpp"

#include "utility.hpp"

int warped_autocorrelation_config_init(size_t cache_size,
                                       config_t *&config) {

    warped_autocorrelation_config_t *warped_autocorrelation_config = (warped_autocorrelation_config_t *)config;

    // configuration
    opus_int warping_Q16 = 1;
    opus_int length = SWAN_AUD_INPUT_SIZE;
    opus_int order = 8;
    int block_count = SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;

    alloc_1D<warped_autocorrelation_config_t>(1, warped_autocorrelation_config);
    alloc_1D<opus_int32>(length + 2 * MAX_SHAPE_LPC_ORDER + 4, warped_autocorrelation_config->input_QST);
    alloc_1D<opus_int32>(length + order + 4, warped_autocorrelation_config->state);
    warped_autocorrelation_config->warping_Q16 = warping_Q16;
    warped_autocorrelation_config->length = length;
    warped_autocorrelation_config->order = order;
    warped_autocorrelation_config->block_count = block_count;
    warped_autocorrelation_config->granularity = block_count;

    // in/output versions
    size_t input_size = block_count * length * sizeof(opus_int16);
    size_t output_size = block_count * (order + 1) * sizeof(opus_int32);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)warped_autocorrelation_config;

    return count;
}

void warped_autocorrelation_input_init(int count,
                                       config_t *config,
                                       input_t **&input) {

    warped_autocorrelation_config_t *warped_autocorrelation_config = (warped_autocorrelation_config_t *)config;
    warped_autocorrelation_input_t **warped_autocorrelation_input = (warped_autocorrelation_input_t **)input;

    // initializing input versions
    alloc_1D<warped_autocorrelation_input_t *>(count, warped_autocorrelation_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<warped_autocorrelation_input_t>(1, warped_autocorrelation_input[i]);

        init_alloc_2D<opus_int16>(warped_autocorrelation_config->block_count, warped_autocorrelation_config->length, warped_autocorrelation_input[i]->input_data);
    }

    input = (input_t **)warped_autocorrelation_input;
}

void warped_autocorrelation_output_init(int count,
                                        config_t *config,
                                        output_t **&output) {

    warped_autocorrelation_config_t *warped_autocorrelation_config = (warped_autocorrelation_config_t *)config;
    warped_autocorrelation_output_t **warped_autocorrelation_output = (warped_autocorrelation_output_t **)output;

    // initializing output versions
    alloc_1D<warped_autocorrelation_output_t *>(count, warped_autocorrelation_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<warped_autocorrelation_output_t>(1, warped_autocorrelation_output[i]);

        alloc_2D<opus_int32>(warped_autocorrelation_config->block_count, warped_autocorrelation_config->order + 1, warped_autocorrelation_output[i]->corr);
        alloc_2D<opus_int>(warped_autocorrelation_config->block_count, 1, warped_autocorrelation_output[i]->scale);
    }

    output = (output_t **)warped_autocorrelation_output;
}

void warped_autocorrelation_comparer(config_t *config,
                                     output_t *output_scalar,
                                     output_t *output_neon) {
    warped_autocorrelation_config_t *warped_autocorrelation_config = (warped_autocorrelation_config_t *)config;
    warped_autocorrelation_output_t *warped_autocorrelation_output_scalar = (warped_autocorrelation_output_t *)output_scalar;
    warped_autocorrelation_output_t *warped_autocorrelation_output_neon = (warped_autocorrelation_output_t *)output_neon;

    compare_2D<opus_int32>(warped_autocorrelation_config->block_count, warped_autocorrelation_config->order + 1, warped_autocorrelation_output_scalar->corr, warped_autocorrelation_output_neon->corr, (char *)"corr");
    compare_2D<opus_int>(warped_autocorrelation_config->block_count, 1, warped_autocorrelation_output_scalar->scale, warped_autocorrelation_output_neon->scale, (char *)"scale");
}

kernel_utility_t warped_autocorrelation_utility = {warped_autocorrelation_config_init, warped_autocorrelation_input_init, warped_autocorrelation_output_init, warped_autocorrelation_comparer};