#include "rgb_to_ycbcr.hpp"

#include "libjpeg.hpp"

#include "swan.hpp"

#include "utility.hpp"
#include <stdio.h>

#define F_0_298 19595
#define F_0_587 38470
#define F_0_113 7471
#define F_0_168 11059
#define F_0_331 21709
#define F_0_500 32768
#define F_0_418 27439
#define F_0_081 5329

const uint16_t jsimd_rgb_ycc_neon_consts[8] = {
    F_0_298, F_0_587, F_0_113, F_0_168,
    F_0_331, F_0_500, F_0_418, F_0_081};

#undef F_0_298
#undef F_0_587
#undef F_0_113
#undef F_0_168
#undef F_0_331
#undef F_0_500
#undef F_0_418
#undef F_0_081

void rgb_ycc_start(rgb_to_ycbcr_config_t *rgb_to_ycbcr_config) {
    my_cconvert_ptr cconvert = (my_cconvert_ptr)rgb_to_ycbcr_config->cconvert;
    JLONG i;

    /* Allocate and fill in the conversion tables. */
    alloc_1D<JLONG>(TABLE_SIZE, cconvert->rgb_ycc_tab);

    for (i = 0; i <= MAXJSAMPLE; i++) {
        cconvert->rgb_ycc_tab[i + R_Y_OFF] = FIX(0.29900) * i;
        cconvert->rgb_ycc_tab[i + G_Y_OFF] = FIX(0.58700) * i;
        cconvert->rgb_ycc_tab[i + B_Y_OFF] = FIX(0.11400) * i + ONE_HALF;
        cconvert->rgb_ycc_tab[i + R_CB_OFF] = (-FIX(0.16874)) * i;
        cconvert->rgb_ycc_tab[i + G_CB_OFF] = (-FIX(0.33126)) * i;
        /* We use a rounding fudge-factor of 0.5-epsilon for Cb and Cr.
        * This ensures that the maximum output will round to MAXJSAMPLE
        * not MAXJSAMPLE+1, and thus that we don't have to range-limit.
        */
        cconvert->rgb_ycc_tab[i + B_CB_OFF] = FIX(0.50000) * i + CBCR_OFFSET + ONE_HALF - 1;
        /*  B=>Cb and R=>Cr tables are the same
            cconvert->rgb_ycc_tab[i + R_CR_OFF] = FIX(0.50000) * i  + CBCR_OFFSET + ONE_HALF - 1;
        */
        cconvert->rgb_ycc_tab[i + G_CR_OFF] = (-FIX(0.41869)) * i;
        cconvert->rgb_ycc_tab[i + B_CR_OFF] = (-FIX(0.08131)) * i;
    }
}

int rgb_to_ycbcr_config_init(size_t cache_size,
                             config_t *&config) {

    rgb_to_ycbcr_config_t *rgb_to_ycbcr_config = (rgb_to_ycbcr_config_t *)config;

    // configuration
    alloc_1D<rgb_to_ycbcr_config_t>(1, rgb_to_ycbcr_config);
    alloc_1D<my_color_converter>(1, rgb_to_ycbcr_config->cconvert);
    rgb_to_ycbcr_config->num_rows = SWAN_IMG_INPUT_ROW_SIZE;
    rgb_to_ycbcr_config->num_cols = SWAN_IMG_INPUT_COL_SIZE;
    rgb_to_ycbcr_config->granularity = 1;
    rgb_ycc_start(rgb_to_ycbcr_config);

    // in/output versions
    size_t input_size = (rgb_to_ycbcr_config->num_rows * rgb_to_ycbcr_config->num_cols * RGB_PIXELSIZE) * sizeof(JSAMPLE);
    size_t output_size = ((RGB_PIXELSIZE - 1) * rgb_to_ycbcr_config->num_rows * rgb_to_ycbcr_config->num_cols) * sizeof(JSAMPLE);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)rgb_to_ycbcr_config;

    return count;
}

void rgb_to_ycbcr_input_init(int count,
                             config_t *config,
                             input_t **&input) {

    rgb_to_ycbcr_config_t *rgb_to_ycbcr_config = (rgb_to_ycbcr_config_t *)config;
    rgb_to_ycbcr_input_t **rgb_to_ycbcr_input = (rgb_to_ycbcr_input_t **)input;

    // initializing input versions
    alloc_1D<rgb_to_ycbcr_input_t *>(count, rgb_to_ycbcr_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<rgb_to_ycbcr_input_t>(1, rgb_to_ycbcr_input[i]);
        init_alloc_2D<JSAMPLE>(rgb_to_ycbcr_config->num_rows, rgb_to_ycbcr_config->num_cols * RGB_PIXELSIZE, rgb_to_ycbcr_input[i]->input_buf, 0, 255);
    }

    input = (input_t **)rgb_to_ycbcr_input;
}

void rgb_to_ycbcr_output_init(int count,
                              config_t *config,
                              output_t **&output) {

    rgb_to_ycbcr_config_t *rgb_to_ycbcr_config = (rgb_to_ycbcr_config_t *)config;
    rgb_to_ycbcr_output_t **rgb_to_ycbcr_output = (rgb_to_ycbcr_output_t **)output;

    // initializing output versions
    alloc_1D<rgb_to_ycbcr_output_t *>(count, rgb_to_ycbcr_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<rgb_to_ycbcr_output_t>(1, rgb_to_ycbcr_output[i]);
        alloc_3D<JSAMPLE>((RGB_PIXELSIZE - 1), rgb_to_ycbcr_config->num_rows, rgb_to_ycbcr_config->num_cols, rgb_to_ycbcr_output[i]->output_buf);
    }

    output = (output_t **)rgb_to_ycbcr_output;
}

void rgb_to_ycbcr_comparer(config_t *config,
                           output_t *output_scalar,
                           output_t *output_neon) {
    rgb_to_ycbcr_config_t *rgb_to_ycbcr_config = (rgb_to_ycbcr_config_t *)config;
    rgb_to_ycbcr_output_t *rgb_to_ycbcr_output_scalar = (rgb_to_ycbcr_output_t *)output_scalar;
    rgb_to_ycbcr_output_t *rgb_to_ycbcr_output_neon = (rgb_to_ycbcr_output_t *)output_neon;

    compare_3D<JSAMPLE>((RGB_PIXELSIZE - 1), rgb_to_ycbcr_config->num_rows, rgb_to_ycbcr_config->num_cols, rgb_to_ycbcr_output_scalar->output_buf, rgb_to_ycbcr_output_neon->output_buf, (char *)"output_buf");
}

kernel_utility_t rgb_to_ycbcr_utility = {rgb_to_ycbcr_config_init, rgb_to_ycbcr_input_init, rgb_to_ycbcr_output_init, rgb_to_ycbcr_comparer};