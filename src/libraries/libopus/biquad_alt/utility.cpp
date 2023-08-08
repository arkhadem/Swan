#include "biquad_alt.hpp"

#include "libopus.hpp"

#include "swan.hpp"

#include "utility.hpp"

int biquad_alt_config_init(size_t cache_size,
                           config_t *&config) {

    biquad_alt_config_t *biquad_alt_config = (biquad_alt_config_t *)config;

    // configuration
    int len = SWAN_AUD_INPUT_SIZE;
    int block_count = SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;

    alloc_1D<biquad_alt_config_t>(1, biquad_alt_config);
    init_alloc_1D<opus_int32>(3, biquad_alt_config->B_Q28);
    init_alloc_1D<opus_int32>(2, biquad_alt_config->A_Q28);

    biquad_alt_config->len = len;
    biquad_alt_config->block_count = block_count;
    biquad_alt_config->granularity = block_count;

    // in/output versions
    size_t input_size = block_count * (2 * len + 4) * sizeof(opus_int16);
    size_t output_size = block_count * (2 * len + 4) * sizeof(opus_int16);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)biquad_alt_config;

    return count;
}

void biquad_alt_input_init(int count,
                           config_t *config,
                           input_t **&input) {

    biquad_alt_config_t *biquad_alt_config = (biquad_alt_config_t *)config;
    biquad_alt_input_t **biquad_alt_input = (biquad_alt_input_t **)input;

    // initializing input versions
    alloc_1D<biquad_alt_input_t *>(count, biquad_alt_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<biquad_alt_input_t>(1, biquad_alt_input[i]);

        init_alloc_2D<opus_int16>(biquad_alt_config->block_count, 2 * biquad_alt_config->len + 4, biquad_alt_input[i]->in);
        init_alloc_2D<opus_int32>(biquad_alt_config->block_count, 4, biquad_alt_input[i]->S_in);
    }

    input = (input_t **)biquad_alt_input;
}

void biquad_alt_output_init(int count,
                            config_t *config,
                            output_t **&output) {

    biquad_alt_config_t *biquad_alt_config = (biquad_alt_config_t *)config;
    biquad_alt_output_t **biquad_alt_output = (biquad_alt_output_t **)output;

    // initializing output versions
    alloc_1D<biquad_alt_output_t *>(count, biquad_alt_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<biquad_alt_output_t>(1, biquad_alt_output[i]);

        alloc_2D<opus_int16>(biquad_alt_config->block_count, 2 * biquad_alt_config->len + 4, biquad_alt_output[i]->out);
        alloc_2D<opus_int32>(biquad_alt_config->block_count, 4, biquad_alt_output[i]->S_out);
    }

    output = (output_t **)biquad_alt_output;
}

void biquad_alt_comparer(config_t *config,
                         output_t *output_scalar,
                         output_t *output_neon) {
    biquad_alt_config_t *biquad_alt_config = (biquad_alt_config_t *)config;
    biquad_alt_output_t *biquad_alt_output_scalar = (biquad_alt_output_t *)output_scalar;
    biquad_alt_output_t *biquad_alt_output_neon = (biquad_alt_output_t *)output_neon;

    compare_2D<opus_int16>(biquad_alt_config->block_count, 2 * biquad_alt_config->len + 4, biquad_alt_output_scalar->out, biquad_alt_output_neon->out, (char *)"out");
    compare_2D<opus_int32>(biquad_alt_config->block_count, 4, biquad_alt_output_scalar->S_out, biquad_alt_output_neon->S_out, (char *)"S_out");
}

kernel_utility_t biquad_alt_utility = {biquad_alt_config_init, biquad_alt_input_init, biquad_alt_output_init, biquad_alt_comparer};