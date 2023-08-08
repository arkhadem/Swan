#include <stdlib.h>

#include "dc_prediction.hpp"

#include "libwebp.hpp"

#include "swan.hpp"

#include "utility.hpp"

#include <algorithm>
#include <random>
#include <vector>

int dc_prediction_config_init(size_t cache_size,
                              config_t *&config) {

    dc_prediction_config_t *dc_prediction_config = (dc_prediction_config_t *)config;

    // configuration
    int num_rows = SWAN_IMG_INPUT_ROW_SIZE;
    int num_cols = SWAN_IMG_INPUT_COL_SIZE;
    int block_size = 16 * 16;
    int block_count = num_rows * num_cols / block_size;

    alloc_1D<dc_prediction_config_t>(1, dc_prediction_config);
    alloc_1D<int>(block_count, dc_prediction_config->coordinations);
    dc_prediction_config->num_rows = num_rows;
    dc_prediction_config->num_cols = num_cols;
    dc_prediction_config->block_size = block_size;
    dc_prediction_config->block_count = block_count;
    dc_prediction_config->granularity = block_count;

    int total_idx = 0;
    int block = 0;
    int x_blk = dc_prediction_config->num_rows / 16;
    int y_blk = dc_prediction_config->num_cols / 16;
    while (block != block_count) {
        int y = total_idx;
        int x = 0;
        do {
            if ((x < x_blk) && (y < y_blk)) {
                int coordination = x * num_cols + y;
                SWAN_ASSERT(coordination < (num_rows * num_cols));
                dc_prediction_config->coordinations[block] = coordination;
                block++;
            }
            x++;
            y--;
        } while ((y >= 0) && (x < x_blk) && (block != block_count));
        total_idx++;
    }

    // in/output versions
    size_t input_size = num_rows * num_cols * sizeof(uint8_t);
    size_t output_size = sizeof(uint8_t *);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)dc_prediction_config;

    return count;
}

void dc_prediction_input_init(int count,
                              config_t *config,
                              input_t **&input) {

    dc_prediction_config_t *dc_prediction_config = (dc_prediction_config_t *)config;
    dc_prediction_input_t **dc_prediction_input = (dc_prediction_input_t **)input;

    // initializing input versions
    alloc_1D<dc_prediction_input_t *>(count, dc_prediction_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<dc_prediction_input_t>(1, dc_prediction_input[i]);

        init_alloc_1D<uint8_t>((dc_prediction_config->num_rows + 16) * (dc_prediction_config->num_cols + 16), dc_prediction_input[i]->src);
        dc_prediction_input[i]->src = dc_prediction_input[i]->src + (16 * dc_prediction_config->num_cols + 16);
        alloc_1D<uint8_t *>(dc_prediction_config->block_count, dc_prediction_input[i]->block_src);

        for (int j = 0; j < dc_prediction_config->block_count; j++) {
            dc_prediction_input[i]->block_src[j] = dc_prediction_input[i]->src + dc_prediction_config->coordinations[j];
        }
    }

    input = (input_t **)dc_prediction_input;
}

void dc_prediction_output_init(int count,
                               config_t *config,
                               output_t **&output) {

    dc_prediction_config_t *dc_prediction_config = (dc_prediction_config_t *)config;
    dc_prediction_output_t **dc_prediction_output = (dc_prediction_output_t **)output;

    // initializing output versions
    alloc_1D<dc_prediction_output_t *>(count, dc_prediction_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<dc_prediction_output_t>(1, dc_prediction_output[i]);

        init_alloc_1D<uint8_t>(dc_prediction_config->num_rows * dc_prediction_config->num_cols, dc_prediction_output[i]->dst, 0, 0);
        alloc_1D<uint8_t *>(dc_prediction_config->block_count, dc_prediction_output[i]->block_dst);

        for (int j = 0; j < dc_prediction_config->block_count; j++) {
            dc_prediction_output[i]->block_dst[j] = dc_prediction_output[i]->dst + dc_prediction_config->coordinations[j];
        }
    }

    output = (output_t **)dc_prediction_output;
}

void dc_prediction_comparer(config_t *config,
                            output_t *output_scalar,
                            output_t *output_neon) {
    dc_prediction_config_t *dc_prediction_config = (dc_prediction_config_t *)config;
    dc_prediction_output_t *dc_prediction_output_scalar = (dc_prediction_output_t *)output_scalar;
    dc_prediction_output_t *dc_prediction_output_neon = (dc_prediction_output_t *)output_neon;

    compare_1D<uint8_t>(dc_prediction_config->num_rows * dc_prediction_config->num_cols, dc_prediction_output_scalar->dst, dc_prediction_output_neon->dst, (char *)"dst");
}

kernel_utility_t dc_prediction_utility = {dc_prediction_config_init, dc_prediction_input_init, dc_prediction_output_init, dc_prediction_comparer};