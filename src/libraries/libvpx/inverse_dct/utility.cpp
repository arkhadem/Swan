#include "inverse_dct.hpp"

#include "libvpx.hpp"

#include "swan.hpp"

#include "utility.hpp"

#include <assert.h>

#include <algorithm>
#include <random>
#include <vector>

int inverse_dct_config_init(size_t cache_size,
                            config_t *&config) {

    inverse_dct_config_t *inverse_dct_config = (inverse_dct_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int cols = SWAN_IMG_INPUT_COL_SIZE;
    int block_size = 8 * 8;
    int block_count = rows * cols / block_size;
    int stride = cols;

    // configuration
    alloc_1D<inverse_dct_config_t>(1, inverse_dct_config);
    alloc_1D<uint8_t>(rows * cols, inverse_dct_config->output_frame_buff);
    alloc_1D<int>(block_count, inverse_dct_config->coordinations);
    inverse_dct_config->rows = rows;
    inverse_dct_config->cols = cols;
    inverse_dct_config->block_size = block_size;
    inverse_dct_config->block_count = block_count;
    inverse_dct_config->granularity = block_count;
    inverse_dct_config->stride = stride;

    int y_blk = inverse_dct_config->cols / 8;
    for (int block = 0; block < block_count; block++) {
        int y = block % y_blk;
        int x = block / y_blk;
        int coordination = x * cols + y;
        SWAN_ASSERT(coordination < (rows * cols));
        inverse_dct_config->coordinations[block] = coordination;
    }

    // in/output versions
    size_t input_size = block_count * block_size * sizeof(tran_low_t);
    size_t output_size = block_count * block_size * sizeof(uint8_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)inverse_dct_config;

    return count;
}

void inverse_dct_input_init(int count,
                            config_t *config,
                            input_t **&input) {

    inverse_dct_config_t *inverse_dct_config = (inverse_dct_config_t *)config;
    inverse_dct_input_t **inverse_dct_input = (inverse_dct_input_t **)input;

    // initializing input versions
    alloc_1D<inverse_dct_input_t *>(count, inverse_dct_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<inverse_dct_input_t>(1, inverse_dct_input[i]);

        init_alloc_2D<tran_low_t>(inverse_dct_config->block_count, inverse_dct_config->block_size, inverse_dct_input[i]->input_buff);
    }

    input = (input_t **)inverse_dct_input;
}

void inverse_dct_output_init(int count,
                             config_t *config,
                             output_t **&output) {

    inverse_dct_config_t *inverse_dct_config = (inverse_dct_config_t *)config;
    inverse_dct_output_t **inverse_dct_output = (inverse_dct_output_t **)output;

    // initializing output versions
    alloc_1D<inverse_dct_output_t *>(count, inverse_dct_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<inverse_dct_output_t>(1, inverse_dct_output[i]);
        alloc_1D<uint8_t *>(inverse_dct_config->block_count, inverse_dct_output[i]->output_buff);

        for (int j = 0; j < inverse_dct_config->block_count; j++) {
            inverse_dct_output[i]->output_buff[j] = inverse_dct_config->output_frame_buff + inverse_dct_config->coordinations[j];
        }
    }

    output = (output_t **)inverse_dct_output;
}

void inverse_dct_comparer(config_t *config,
                          output_t *output_scalar,
                          output_t *output_neon) {
    inverse_dct_config_t *inverse_dct_config = (inverse_dct_config_t *)config;
    inverse_dct_output_t *inverse_dct_output_scalar = (inverse_dct_output_t *)output_scalar;
    inverse_dct_output_t *inverse_dct_output_neon = (inverse_dct_output_t *)output_neon;

    compare_2D<uint8_t>(inverse_dct_config->block_count, inverse_dct_config->block_size, inverse_dct_output_scalar->output_buff, inverse_dct_output_neon->output_buff, (char *)"output_buff");
}

kernel_utility_t inverse_dct_utility = {inverse_dct_config_init, inverse_dct_input_init, inverse_dct_output_init, inverse_dct_comparer};