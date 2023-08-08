#include <stdio.h>
#include <stdlib.h>

#include "he_prediction.hpp"

#include "libwebp.hpp"

#include "swan.hpp"

#include "utility.hpp"

#include <algorithm>
#include <random>
#include <vector>

int he_prediction_config_init(size_t cache_size,
                              config_t *&config) {

    he_prediction_config_t *he_prediction_config = (he_prediction_config_t *)config;

    // configuration
    int num_rows = SWAN_IMG_INPUT_ROW_SIZE;
    int num_cols = SWAN_IMG_INPUT_COL_SIZE;
    int block_size = 16 * 16;
    int block_count = num_rows * num_cols / block_size;

    alloc_1D<he_prediction_config_t>(1, he_prediction_config);
    alloc_1D<int>(block_count, he_prediction_config->coordinations);
    he_prediction_config->num_rows = num_rows;
    he_prediction_config->num_cols = num_cols;
    he_prediction_config->block_size = block_size;
    he_prediction_config->block_count = block_count;
    he_prediction_config->granularity = block_count;

    int total_idx = 0;
    int block = 0;
    int x_blk = he_prediction_config->num_rows / 16;
    int y_blk = he_prediction_config->num_cols / 16;
    while (block != block_count) {
        int y = total_idx;
        int x = 0;
        do {
            if ((x < x_blk) && (y < y_blk)) {
                int coordination = x * num_cols + y;
                SWAN_ASSERT(coordination < (num_rows * num_cols));
                he_prediction_config->coordinations[block] = coordination;
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

    config = (config_t *)he_prediction_config;

    return count;
}

void he_prediction_input_init(int count,
                              config_t *config,
                              input_t **&input) {

    he_prediction_config_t *he_prediction_config = (he_prediction_config_t *)config;
    he_prediction_input_t **he_prediction_input = (he_prediction_input_t **)input;

    // initializing input versions
    alloc_1D<he_prediction_input_t *>(count, he_prediction_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<he_prediction_input_t>(1, he_prediction_input[i]);

        init_alloc_1D<uint8_t>((he_prediction_config->num_rows + 16) * (he_prediction_config->num_cols + 16), he_prediction_input[i]->src);
        he_prediction_input[i]->src = he_prediction_input[i]->src + (16 * he_prediction_config->num_cols + 16);
        alloc_1D<uint8_t *>(he_prediction_config->block_count, he_prediction_input[i]->block_src);

        for (int j = 0; j < he_prediction_config->block_count; j++) {
            he_prediction_input[i]->block_src[j] = he_prediction_input[i]->src + he_prediction_config->coordinations[j];
        }
    }

    input = (input_t **)he_prediction_input;
}

void he_prediction_output_init(int count,
                               config_t *config,
                               output_t **&output) {

    he_prediction_config_t *he_prediction_config = (he_prediction_config_t *)config;
    he_prediction_output_t **he_prediction_output = (he_prediction_output_t **)output;

    // initializing output versions
    alloc_1D<he_prediction_output_t *>(count, he_prediction_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<he_prediction_output_t>(1, he_prediction_output[i]);

        init_alloc_1D<uint8_t>(he_prediction_config->num_rows * he_prediction_config->num_cols, he_prediction_output[i]->dst, 0, 0);
        alloc_1D<uint8_t *>(he_prediction_config->block_count, he_prediction_output[i]->block_dst);

        for (int j = 0; j < he_prediction_config->block_count; j++) {
            he_prediction_output[i]->block_dst[j] = he_prediction_output[i]->dst + he_prediction_config->coordinations[j];
        }
    }

    output = (output_t **)he_prediction_output;
}

void he_prediction_comparer(config_t *config,
                            output_t *output_scalar,
                            output_t *output_neon) {
    he_prediction_config_t *he_prediction_config = (he_prediction_config_t *)config;
    he_prediction_output_t *he_prediction_output_scalar = (he_prediction_output_t *)output_scalar;
    he_prediction_output_t *he_prediction_output_neon = (he_prediction_output_t *)output_neon;

    compare_1D<uint8_t>(he_prediction_config->num_rows * he_prediction_config->num_cols, he_prediction_output_scalar->dst, he_prediction_output_neon->dst, (char *)"dst");
}

kernel_utility_t he_prediction_utility = {he_prediction_config_init, he_prediction_input_init, he_prediction_output_init, he_prediction_comparer};