#include "pffft.hpp"

#include "swan.hpp"

#include "utility.hpp"

#include <assert.h>

#include <algorithm>
#include <random>
#include <vector>

int fft_forward_real_config_init(size_t cache_size,
                                 config_t *&config) {

    pffft_config_t *pffft_config = (pffft_config_t *)config;

    // configuration
    int N = SWAN_AUD_INPUT_SIZE;
    int block_count = SWAN_AUD_CHANNEL_SIZE * SWAN_AUD_SAMPLE_RATE / SWAN_AUD_INPUT_SIZE;

    // configuration
    alloc_1D<pffft_config_t>(1, pffft_config);
    pffft_config->N = N;
    pffft_config->block_count = block_count;
    pffft_config->granularity = block_count;
    pffft_config->scalar_setup = pffft_new_setup(platform_t::SCALAR_PLAT, N, pffft_transform_t::PFFFT_REAL, sizeof(float));
    pffft_config->neon_setup = pffft_new_setup(platform_t::NEON_PLAT, N, pffft_transform_t::PFFFT_REAL, 4 * sizeof(float));
    pffft_config->direction = pffft_direction_t::PFFFT_FORWARD;

    // in/output versions
    // REAL: in/output/work size = N
    // COMP: in/output/work size = 2N
    size_t input_size = block_count * (N) * sizeof(float);
    size_t output_size = block_count * (N) * sizeof(float);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)pffft_config;

    return count;
}

void fft_forward_real_input_init(int count,
                                 config_t *config,
                                 input_t **&input) {

    pffft_config_t *pffft_config = (pffft_config_t *)config;
    pffft_input_t **pffft_input = (pffft_input_t **)input;

    // initializing input versions
    alloc_1D<pffft_input_t *>(count, pffft_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<pffft_input_t>(1, pffft_input[i]);

        init_alloc_2D<float>(pffft_config->block_count, pffft_config->N, pffft_input[i]->input_buff);
        init_alloc_1D<float>(pffft_config->N, pffft_input[i]->work);
    }

    input = (input_t **)pffft_input;
}

void fft_forward_real_output_init(int count,
                                  config_t *config,
                                  output_t **&output) {

    pffft_config_t *pffft_config = (pffft_config_t *)config;
    pffft_output_t **pffft_output = (pffft_output_t **)output;

    // initializing output versions
    alloc_1D<pffft_output_t *>(count, pffft_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<pffft_output_t>(1, pffft_output[i]);

        alloc_2D<float>(pffft_config->block_count, pffft_config->N, pffft_output[i]->output_buff);
    }

    output = (output_t **)pffft_output;
}

void fft_forward_real_comparer(config_t *config,
                               output_t *output_scalar,
                               output_t *output_neon) {
    pffft_config_t *pffft_config = (pffft_config_t *)config;
    pffft_output_t *pffft_output_scalar = (pffft_output_t *)output_scalar;
    pffft_output_t *pffft_output_neon = (pffft_output_t *)output_neon;

    compare_2D<float>(pffft_config->block_count, pffft_config->N, pffft_output_scalar->output_buff, pffft_output_neon->output_buff, (char *)"output_buff", 0.1);
}

kernel_utility_t fft_forward_real_utility = {fft_forward_real_config_init, fft_forward_real_input_init, fft_forward_real_output_init, fft_forward_real_comparer};