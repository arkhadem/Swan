#include "pitch_xcorr.hpp"

#include "libopus.hpp"

#include "swan.hpp"

#include "utility.hpp"

int pitch_xcorr_config_init(size_t cache_size,
                            config_t *&config) {

    pitch_xcorr_config_t *pitch_xcorr_config = (pitch_xcorr_config_t *)config;

    // configuration
    int len = SWAN_AUD_INPUT_SIZE;
    int block_count = SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;
    int max_pitch = 25;

    alloc_1D<pitch_xcorr_config_t>(1, pitch_xcorr_config);

    pitch_xcorr_config->len = len;
    pitch_xcorr_config->block_count = block_count;
    pitch_xcorr_config->granularity = block_count;
    pitch_xcorr_config->max_pitch = max_pitch;

    // in/output versions
    size_t input_size = block_count * (2 * len + max_pitch) * sizeof(opus_val16);
    size_t output_size = block_count * max_pitch * sizeof(opus_val32);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)pitch_xcorr_config;

    return count;
}

void pitch_xcorr_input_init(int count,
                            config_t *config,
                            input_t **&input) {

    pitch_xcorr_config_t *pitch_xcorr_config = (pitch_xcorr_config_t *)config;
    pitch_xcorr_input_t **pitch_xcorr_input = (pitch_xcorr_input_t **)input;

    // initializing input versions
    alloc_1D<pitch_xcorr_input_t *>(count, pitch_xcorr_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<pitch_xcorr_input_t>(1, pitch_xcorr_input[i]);

        init_alloc_2D<opus_val16>(pitch_xcorr_config->block_count, pitch_xcorr_config->len, pitch_xcorr_input[i]->x, -1.00, 1.00);
        init_alloc_2D<opus_val16>(pitch_xcorr_config->block_count, pitch_xcorr_config->len + pitch_xcorr_config->max_pitch, pitch_xcorr_input[i]->y, -1.00, 1.00);
    }

    input = (input_t **)pitch_xcorr_input;
}

void pitch_xcorr_output_init(int count,
                             config_t *config,
                             output_t **&output) {

    pitch_xcorr_config_t *pitch_xcorr_config = (pitch_xcorr_config_t *)config;
    pitch_xcorr_output_t **pitch_xcorr_output = (pitch_xcorr_output_t **)output;

    // initializing output versions
    alloc_1D<pitch_xcorr_output_t *>(count, pitch_xcorr_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<pitch_xcorr_output_t>(1, pitch_xcorr_output[i]);

        alloc_2D<opus_val32>(pitch_xcorr_config->block_count, pitch_xcorr_config->max_pitch, pitch_xcorr_output[i]->xcorr);
    }

    output = (output_t **)pitch_xcorr_output;
}

void pitch_xcorr_comparer(config_t *config,
                          output_t *output_scalar,
                          output_t *output_neon) {
    pitch_xcorr_config_t *pitch_xcorr_config = (pitch_xcorr_config_t *)config;
    pitch_xcorr_output_t *pitch_xcorr_output_scalar = (pitch_xcorr_output_t *)output_scalar;
    pitch_xcorr_output_t *pitch_xcorr_output_neon = (pitch_xcorr_output_t *)output_neon;

    compare_2D<opus_val32>(pitch_xcorr_config->block_count, pitch_xcorr_config->max_pitch, pitch_xcorr_output_scalar->xcorr, pitch_xcorr_output_neon->xcorr, (char *)"xcorr", 0.1);
}

kernel_utility_t pitch_xcorr_utility = {pitch_xcorr_config_init, pitch_xcorr_input_init, pitch_xcorr_output_init, pitch_xcorr_comparer};