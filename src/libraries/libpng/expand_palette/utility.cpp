#include "expand_palette.hpp"

#include "libpng.hpp"

#include "swan.hpp"

#include "utility.hpp"
#include <stdio.h>

int expand_palette_config_init(size_t cache_size,
                               config_t *&config) {

    expand_palette_config_t *expand_palette_config = (expand_palette_config_t *)config;

    // configuration
    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int columns = SWAN_IMG_INPUT_COL_SIZE;

    alloc_1D<expand_palette_config_t>(1, expand_palette_config);
    expand_palette_config->num_rows = rows;
    expand_palette_config->num_cols = columns;
    expand_palette_config->granularity = rows;
    alloc_1D<png_byte>(256 * 4, expand_palette_config->riffled_palette);
    alloc_1D<png_color>(256, expand_palette_config->rgb_palette);
    init_alloc_1D<png_byte>(256, expand_palette_config->a_palette);
    for (int j = 0; j < 256; j++) {
        png_byte a = expand_palette_config->a_palette[j];
        png_byte r = expand_palette_config->rgb_palette[j].red = rand() % 256;
        png_byte g = expand_palette_config->rgb_palette[j].green = rand() % 256;
        png_byte b = expand_palette_config->rgb_palette[j].blue = rand() % 256;
        expand_palette_config->riffled_palette[(j << 2) + 0] = r;
        expand_palette_config->riffled_palette[(j << 2) + 1] = g;
        expand_palette_config->riffled_palette[(j << 2) + 2] = b;
        expand_palette_config->riffled_palette[(j << 2) + 3] = a;
    }

    // in/output versions
    size_t input_size = (rows * columns) * sizeof(png_byte);
    size_t output_size = (rows * columns * 4) * sizeof(png_byte);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)expand_palette_config;

    return count;
}

void expand_palette_input_init(int count,
                               config_t *config,
                               input_t **&input) {

    expand_palette_config_t *expand_palette_config = (expand_palette_config_t *)config;
    expand_palette_input_t **expand_palette_input = (expand_palette_input_t **)input;

    // initializing input versions
    alloc_1D<expand_palette_input_t *>(count, expand_palette_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<expand_palette_input_t>(1, expand_palette_input[i]);

        init_alloc_2D<png_byte>(expand_palette_config->num_rows, expand_palette_config->num_cols, expand_palette_input[i]->input_buf);
    }

    input = (input_t **)expand_palette_input;
}

void expand_palette_output_init(int count,
                                config_t *config,
                                output_t **&output) {

    expand_palette_config_t *expand_palette_config = (expand_palette_config_t *)config;
    expand_palette_output_t **expand_palette_output = (expand_palette_output_t **)output;

    // initializing output versions
    alloc_1D<expand_palette_output_t *>(count, expand_palette_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<expand_palette_output_t>(1, expand_palette_output[i]);

        alloc_2D<png_byte>(expand_palette_config->num_rows, expand_palette_config->num_cols * 4, expand_palette_output[i]->output_buf);
    }

    output = (output_t **)expand_palette_output;
}

void expand_palette_comparer(config_t *config,
                             output_t *output_scalar,
                             output_t *output_neon) {
    expand_palette_config_t *expand_palette_config = (expand_palette_config_t *)config;
    expand_palette_output_t *expand_palette_output_scalar = (expand_palette_output_t *)output_scalar;
    expand_palette_output_t *expand_palette_output_neon = (expand_palette_output_t *)output_neon;

    compare_2D<png_byte>(expand_palette_config->num_rows, expand_palette_config->num_cols * 4, expand_palette_output_scalar->output_buf, expand_palette_output_neon->output_buf, (char *)"output_buf");
}

kernel_utility_t expand_palette_utility = {expand_palette_config_init, expand_palette_input_init, expand_palette_output_init, expand_palette_comparer};