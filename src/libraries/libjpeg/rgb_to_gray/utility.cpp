#include "rgb_to_gray.hpp"

#include "libjpeg.hpp"

#include "swan.hpp"

#include "utility.hpp"

void build_rgb_y_table(rgb_to_gray_config_t *rgb_to_gray_config) {
    my_cconvert_ptr cconvert = (my_cconvert_ptr)rgb_to_gray_config->cconvert;
    JLONG i;

    /* Allocate and fill in the conversion tables. */
    alloc_1D<JLONG>(TABLE_SIZE, cconvert->rgb_y_tab);

    for (i = 0; i <= MAXJSAMPLE; i++) {
        cconvert->rgb_y_tab[i + R_Y_OFF] = FIX(0.29900) * i;
        cconvert->rgb_y_tab[i + G_Y_OFF] = FIX(0.58700) * i;
        cconvert->rgb_y_tab[i + B_Y_OFF] = FIX(0.11400) * i + ONE_HALF;
    }
}

int rgb_to_gray_config_init(size_t cache_size,
                            config_t *&config) {

    rgb_to_gray_config_t *rgb_to_gray_config = (rgb_to_gray_config_t *)config;

    // configuration
    alloc_1D<rgb_to_gray_config_t>(1, rgb_to_gray_config);
    alloc_1D<my_color_deconverter>(1, rgb_to_gray_config->cconvert);
    rgb_to_gray_config->num_rows = SWAN_IMG_INPUT_ROW_SIZE;
    rgb_to_gray_config->num_cols = SWAN_IMG_INPUT_COL_SIZE;
    rgb_to_gray_config->granularity = 1;
    build_rgb_y_table(rgb_to_gray_config);

    // in/output versions
    size_t input_size = (rgb_to_gray_config->num_rows * rgb_to_gray_config->num_cols * RGB_PIXELSIZE) * sizeof(JSAMPLE);
    size_t output_size = (rgb_to_gray_config->num_rows * rgb_to_gray_config->num_cols) * sizeof(JSAMPLE);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)rgb_to_gray_config;

    return count;
}

void rgb_to_gray_input_init(int count,
                            config_t *config,
                            input_t **&input) {

    rgb_to_gray_config_t *rgb_to_gray_config = (rgb_to_gray_config_t *)config;
    rgb_to_gray_input_t **rgb_to_gray_input = (rgb_to_gray_input_t **)input;

    // initializing input versions
    alloc_1D<rgb_to_gray_input_t *>(count, rgb_to_gray_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<rgb_to_gray_input_t>(1, rgb_to_gray_input[i]);

        init_alloc_2D<JSAMPLE>(rgb_to_gray_config->num_rows, rgb_to_gray_config->num_cols * RGB_PIXELSIZE, rgb_to_gray_input[i]->input_buf);
    }

    input = (input_t **)rgb_to_gray_input;
}

void rgb_to_gray_output_init(int count,
                             config_t *config,
                             output_t **&output) {

    rgb_to_gray_config_t *rgb_to_gray_config = (rgb_to_gray_config_t *)config;
    rgb_to_gray_output_t **rgb_to_gray_output = (rgb_to_gray_output_t **)output;

    // initializing output versions
    alloc_1D<rgb_to_gray_output_t *>(count, rgb_to_gray_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<rgb_to_gray_output_t>(1, rgb_to_gray_output[i]);

        alloc_2D<JSAMPLE>(rgb_to_gray_config->num_rows, rgb_to_gray_config->num_cols, rgb_to_gray_output[i]->output_buf);
    }

    output = (output_t **)rgb_to_gray_output;
}

void rgb_to_gray_comparer(config_t *config,
                          output_t *output_scalar,
                          output_t *output_neon) {
    rgb_to_gray_config_t *rgb_to_gray_config = (rgb_to_gray_config_t *)config;
    rgb_to_gray_output_t *rgb_to_gray_output_scalar = (rgb_to_gray_output_t *)output_scalar;
    rgb_to_gray_output_t *rgb_to_gray_output_neon = (rgb_to_gray_output_t *)output_neon;

    compare_2D<JSAMPLE>(rgb_to_gray_config->num_rows, rgb_to_gray_config->num_cols, rgb_to_gray_output_scalar->output_buf, rgb_to_gray_output_neon->output_buf, (char *)"output_buf", 1);
}

kernel_utility_t rgb_to_gray_utility = {rgb_to_gray_config_init, rgb_to_gray_input_init, rgb_to_gray_output_init, rgb_to_gray_comparer};