#include "forward_dct.hpp"

#include "libvpx.hpp"

#include "swan.hpp"

#include "utility.hpp"

#include <assert.h>

#include <algorithm>
#include <random>
#include <vector>

int forward_dct_config_init(size_t cache_size,
                            config_t *&config) {

    forward_dct_config_t *forward_dct_config = (forward_dct_config_t *)config;

    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int cols = SWAN_IMG_INPUT_COL_SIZE;
    int block_size = 8 * 8;
    int block_count = rows * cols / block_size;
    int stride = cols;

    // configuration
    alloc_1D<forward_dct_config_t>(1, forward_dct_config);
    init_alloc_1D<int16_t>(rows * cols, forward_dct_config->input_frame_buff);
    alloc_1D<int>(block_count, forward_dct_config->coordinations);
    forward_dct_config->rows = rows;
    forward_dct_config->cols = cols;
    forward_dct_config->block_size = block_size;
    forward_dct_config->block_count = block_count;
    forward_dct_config->granularity = block_count;
    forward_dct_config->stride = stride;

    int y_blk = forward_dct_config->cols / 8;
    for (int block = 0; block < block_count; block++) {
        int y = block % y_blk;
        int x = block / y_blk;
        int coordination = x * cols + y;
        SWAN_ASSERT(coordination < (rows * cols));
        forward_dct_config->coordinations[block] = coordination;
    }

    // in/output versions
    size_t input_size = block_count * block_size * sizeof(int16_t);
    size_t output_size = block_count * block_size * sizeof(tran_low_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)forward_dct_config;

    return count;
}

void forward_dct_input_init(int count,
                            config_t *config,
                            input_t **&input) {

    forward_dct_config_t *forward_dct_config = (forward_dct_config_t *)config;
    forward_dct_input_t **forward_dct_input = (forward_dct_input_t **)input;

    // initializing input versions
    alloc_1D<forward_dct_input_t *>(count, forward_dct_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<forward_dct_input_t>(1, forward_dct_input[i]);
        alloc_1D<int16_t *>(forward_dct_config->block_count, forward_dct_input[i]->input_buff);

        for (int j = 0; j < forward_dct_config->block_count; j++) {
            forward_dct_input[i]->input_buff[j] = forward_dct_config->input_frame_buff + forward_dct_config->coordinations[j];
        }
    }

    input = (input_t **)forward_dct_input;
}

void forward_dct_output_init(int count,
                             config_t *config,
                             output_t **&output) {

    forward_dct_config_t *forward_dct_config = (forward_dct_config_t *)config;
    forward_dct_output_t **forward_dct_output = (forward_dct_output_t **)output;

    // initializing input versions
    alloc_1D<forward_dct_output_t *>(count, forward_dct_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<forward_dct_output_t>(1, forward_dct_output[i]);

        alloc_2D<tran_low_t>(forward_dct_config->block_count, forward_dct_config->block_size, forward_dct_output[i]->output_buff);
    }

    output = (output_t **)forward_dct_output;
}

void forward_dct_comparer(config_t *config,
                          output_t *output_scalar,
                          output_t *output_neon) {
    forward_dct_config_t *forward_dct_config = (forward_dct_config_t *)config;
    forward_dct_output_t *forward_dct_output_scalar = (forward_dct_output_t *)output_scalar;
    forward_dct_output_t *forward_dct_output_neon = (forward_dct_output_t *)output_neon;

    compare_2D<tran_low_t>(forward_dct_config->block_count, forward_dct_config->block_size, forward_dct_output_scalar->output_buff, forward_dct_output_neon->output_buff, (char *)"output_buff", 1);
}

kernel_utility_t forward_dct_utility = {forward_dct_config_init, forward_dct_input_init, forward_dct_output_init, forward_dct_comparer};