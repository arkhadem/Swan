#include "downsample.hpp"

#include "libjpeg.hpp"

#include "swan.hpp"

#include "utility.hpp"
#include <stddef.h>
#include <stdio.h>

int downsample_config_init(size_t cache_size,
                           config_t *&config) {

    downsample_config_t *downsample_config = (downsample_config_t *)config;

    int input_num_rows = SWAN_IMG_INPUT_ROW_SIZE;
    int input_num_cols = (RGB_PIXELSIZE - 1) * SWAN_IMG_INPUT_COL_SIZE;
    int output_num_rows = input_num_rows / 2;
    int output_num_cols = input_num_cols / 2;

    // configuration
    alloc_1D<downsample_config_t>(1, downsample_config);
    downsample_config->input_num_rows = input_num_rows;
    downsample_config->input_num_cols = input_num_cols;
    downsample_config->output_num_rows = output_num_rows;
    downsample_config->output_num_cols = output_num_cols;
    downsample_config->granularity = input_num_rows;

    // in/output versions
    size_t input_size = (downsample_config->input_num_rows * downsample_config->input_num_cols) * sizeof(JSAMPLE);
    size_t output_size = (downsample_config->output_num_rows * downsample_config->output_num_cols) * sizeof(JSAMPLE);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)downsample_config;

    return count;
}

void downsample_input_init(int count,
                           config_t *config,
                           input_t **&input) {

    downsample_config_t *downsample_config = (downsample_config_t *)config;
    downsample_input_t **downsample_input = (downsample_input_t **)input;

    // initializing input versions
    alloc_1D<downsample_input_t *>(count, downsample_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<downsample_input_t>(1, downsample_input[i]);

        init_alloc_2D<JSAMPLE>(downsample_config->input_num_rows, downsample_config->input_num_cols, downsample_input[i]->input_buf);
    }

    input = (input_t **)downsample_input;
}

void downsample_output_init(int count,
                            config_t *config,
                            output_t **&output) {

    downsample_config_t *downsample_config = (downsample_config_t *)config;
    downsample_output_t **downsample_output = (downsample_output_t **)output;

    // initializing output versions
    alloc_1D<downsample_output_t *>(count, downsample_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<downsample_output_t>(1, downsample_output[i]);

        alloc_2D<JSAMPLE>(downsample_config->output_num_rows, downsample_config->output_num_cols, downsample_output[i]->output_buf);
    }

    output = (output_t **)downsample_output;
}

void downsample_comparer(config_t *config,
                         output_t *output_scalar,
                         output_t *output_neon) {
    downsample_config_t *downsample_config = (downsample_config_t *)config;
    downsample_output_t *downsample_output_scalar = (downsample_output_t *)output_scalar;
    downsample_output_t *downsample_output_neon = (downsample_output_t *)output_neon;

    compare_2D<JSAMPLE>(downsample_config->output_num_rows, downsample_config->output_num_cols, downsample_output_scalar->output_buf, downsample_output_neon->output_buf, (char *)"output_buf");
}

kernel_utility_t downsample_utility = {downsample_config_init, downsample_input_init, downsample_output_init, downsample_comparer};