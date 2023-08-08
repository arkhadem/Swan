#include "row_opaque.hpp"

#include "skia.hpp"

#include "swan.hpp"

#include "utility.hpp"
#include <stdint.h>

int row_opaque_config_init(size_t cache_size,
                           config_t *&config) {

    row_opaque_config_t *row_opaque_config = (row_opaque_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;

    alloc_1D<row_opaque_config_t>(1, row_opaque_config);
    row_opaque_config->num_rows = rows;
    row_opaque_config->num_cols = columns;
    row_opaque_config->granularity = rows;

    // in/output versions
    size_t input_size = rows * columns * sizeof(uint32_t);
    input_size += 2 * rows * sizeof(uint32_t);
    size_t output_size = rows * columns * sizeof(uint32_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)row_opaque_config;

    return count;
}

#define MAX_U16BIT ((uint16_t)((uint32_t)((uint32_t)1 << 16) - 1))
#define MAX_U32BIT ((uint32_t)((uint64_t)((uint64_t)1 << 32) - 1))

void row_opaque_input_init(int count,
                           config_t *config,
                           input_t **&input) {

    row_opaque_config_t *row_opaque_config = (row_opaque_config_t *)config;
    row_opaque_input_t **row_opaque_input = (row_opaque_input_t **)input;

    // initializing input versions
    alloc_1D<row_opaque_input_t *>(count, row_opaque_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<row_opaque_input_t>(1, row_opaque_input[i]);

        init_alloc_1D<uint16_t>(row_opaque_config->num_rows * row_opaque_config->num_cols, row_opaque_input[i]->src_src, 0, MAX_U16BIT);
        init_alloc_1D<uint32_t>(row_opaque_config->num_rows * row_opaque_config->num_cols, row_opaque_input[i]->src_dst, 0, MAX_U32BIT);
        init_alloc_1D<uint32_t>(row_opaque_config->num_rows, row_opaque_input[i]->src_opaque, 0, MAX_U32BIT);
        init_alloc_1D<uint32_t>(row_opaque_config->num_rows, row_opaque_input[i]->color, 0, MAX_U32BIT);
    }

    input = (input_t **)row_opaque_input;
}

void row_opaque_output_init(int count,
                            config_t *config,
                            output_t **&output) {

    row_opaque_config_t *row_opaque_config = (row_opaque_config_t *)config;
    row_opaque_output_t **row_opaque_output = (row_opaque_output_t **)output;

    // initializing output versions
    alloc_1D<row_opaque_output_t *>(count, row_opaque_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<row_opaque_output_t>(1, row_opaque_output[i]);

        alloc_1D<uint32_t>(row_opaque_config->num_rows * row_opaque_config->num_cols, row_opaque_output[i]->dst_dst);
    }

    output = (output_t **)row_opaque_output;
}

void row_opaque_comparer(config_t *config,
                         output_t *output_scalar,
                         output_t *output_neon) {
    row_opaque_config_t *row_opaque_config = (row_opaque_config_t *)config;
    row_opaque_output_t *row_opaque_output_scalar = (row_opaque_output_t *)output_scalar;
    row_opaque_output_t *row_opaque_output_neon = (row_opaque_output_t *)output_neon;

    compare_1D<uint32_t>(row_opaque_config->num_rows * row_opaque_config->num_cols, row_opaque_output_scalar->dst_dst, row_opaque_output_neon->dst_dst, (char *)"dst_dst");
}

kernel_utility_t row_opaque_utility = {row_opaque_config_init, row_opaque_input_init, row_opaque_output_init, row_opaque_comparer};