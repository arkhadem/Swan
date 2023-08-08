#include "upsample.hpp"

#include "libjpeg.hpp"

#include "swan.hpp"

#include "utility.hpp"

/* Allocate and fill in the sample_range_limit table */
void prepare_range_limit_table(upsample_config_t *upsampling_config) {
    JSAMPLE *table;
    int i;
    alloc_1D<JSAMPLE>((5 * (MAXJSAMPLE + 1) + CENTERJSAMPLE), table);
    table += (MAXJSAMPLE + 1); /* allow negative subscripts of simple table */
    upsampling_config->sample_range_limit = table;
    /* First segment of "simple" table: limit[x] = 0 for x < 0 */
    memset(table - (MAXJSAMPLE + 1), 0, (MAXJSAMPLE + 1) * sizeof(JSAMPLE));
    /* Main part of "simple" table: limit[x] = x */
    for (i = 0; i <= MAXJSAMPLE; i++)
        table[i] = (JSAMPLE)i;
    table += CENTERJSAMPLE; /* Point to where post-IDCT table starts */
    /* End of simple table, rest of first half of post-IDCT table */
    for (i = CENTERJSAMPLE; i < 2 * (MAXJSAMPLE + 1); i++)
        table[i] = MAXJSAMPLE;
    /* Second half of post-IDCT table */
    memset(table + (2 * (MAXJSAMPLE + 1)), 0, (2 * (MAXJSAMPLE + 1) - CENTERJSAMPLE) * sizeof(JSAMPLE));
    memcpy(table + (4 * (MAXJSAMPLE + 1) - CENTERJSAMPLE), upsampling_config->sample_range_limit, CENTERJSAMPLE * sizeof(JSAMPLE));
}

void build_ycc_rgb_table(upsample_config_t *upsample_config) {
    my_merged_upsample_ptr cconvert = upsample_config->cconvert;
    int i;
    JLONG x;

    alloc_1D<int>(MAXJSAMPLE + 1, cconvert->Cr_r_tab);
    alloc_1D<int>(MAXJSAMPLE + 1, cconvert->Cb_b_tab);
    alloc_1D<JLONG>(MAXJSAMPLE + 1, cconvert->Cr_g_tab);
    alloc_1D<JLONG>(MAXJSAMPLE + 1, cconvert->Cb_g_tab);

    for (i = 0, x = -CENTERJSAMPLE; i <= MAXJSAMPLE; i++, x++) {
        /* i is the actual input pixel value, in the range 0..MAXJSAMPLE */
        /* The Cb or Cr value we are thinking of is x = i - CENTERJSAMPLE */
        /* Cr=>R value is nearest int to 1.40200 * x */
        cconvert->Cr_r_tab[i] = (int)RIGHT_SHIFT(FIX(1.40200) * x + ONE_HALF, SCALEBITS);
        /* Cb=>B value is nearest int to 1.77200 * x */
        cconvert->Cb_b_tab[i] = (int)RIGHT_SHIFT(FIX(1.77200) * x + ONE_HALF, SCALEBITS);
        /* Cr=>G value is scaled-up -0.71414 * x */
        cconvert->Cr_g_tab[i] = (-FIX(0.71414)) * x;
        /* Cb=>G value is scaled-up -0.34414 * x */
        /* We also add in ONE_HALF so that need not do it in inner loop */
        cconvert->Cb_g_tab[i] = (-FIX(0.34414)) * x + ONE_HALF;
    }
}

int upsample_config_init(size_t cache_size,
                         config_t *&config) {

    upsample_config_t *upsample_config = (upsample_config_t *)config;

    int input_num_rows = SWAN_IMG_INPUT_ROW_SIZE;
    int input_num_cols = SWAN_IMG_INPUT_COL_SIZE;
    int output_num_rows = input_num_rows * 2;
    int output_num_cols = input_num_cols * 2;

    // configuration
    alloc_1D<upsample_config_t>(1, upsample_config);
    alloc_1D<my_merged_upsampler>(1, upsample_config->cconvert);
    upsample_config->input_num_rows = input_num_rows;
    upsample_config->input_num_cols = input_num_cols;
    upsample_config->output_num_rows = output_num_rows;
    upsample_config->output_num_cols = output_num_cols;
    upsample_config->granularity = input_num_rows;
    build_ycc_rgb_table(upsample_config);
    prepare_range_limit_table(upsample_config);

    // in/output versions
    // Y
    size_t input_size = (upsample_config->output_num_rows * upsample_config->output_num_cols) * sizeof(JSAMPLE);
    // Cb
    input_size += (2 * upsample_config->input_num_rows * upsample_config->input_num_cols) * sizeof(JSAMPLE);
    // Cr
    size_t output_size = (upsample_config->output_num_rows * upsample_config->output_num_cols * RGB_PIXELSIZE) * sizeof(JSAMPLE);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)upsample_config;

    return count;
}

void upsample_input_init(int count,
                         config_t *config,
                         input_t **&input) {

    upsample_config_t *upsample_config = (upsample_config_t *)config;
    upsample_input_t **upsample_input = (upsample_input_t **)input;

    // initializing input versions
    alloc_1D<upsample_input_t *>(count, upsample_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<upsample_input_t>(1, upsample_input[i]);

        init_alloc_3D<JSAMPLE>((RGB_PIXELSIZE - 1), upsample_config->output_num_rows, upsample_config->output_num_cols, upsample_input[i]->input_buf);
    }

    input = (input_t **)upsample_input;
}

void upsample_output_init(int count,
                          config_t *config,
                          output_t **&output) {

    upsample_config_t *upsample_config = (upsample_config_t *)config;
    upsample_output_t **upsample_output = (upsample_output_t **)output;

    // initializing output versions
    alloc_1D<upsample_output_t *>(count, upsample_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<upsample_output_t>(1, upsample_output[i]);

        init_alloc_2D<JSAMPLE>(upsample_config->output_num_rows, upsample_config->output_num_cols * RGB_PIXELSIZE, upsample_output[i]->output_buf, 0, 0);
    }

    output = (output_t **)upsample_output;
}

void upsample_comparer(config_t *config,
                       output_t *output_scalar,
                       output_t *output_neon) {
    upsample_config_t *upsample_config = (upsample_config_t *)config;
    upsample_output_t *upsample_output_scalar = (upsample_output_t *)output_scalar;
    upsample_output_t *upsample_output_neon = (upsample_output_t *)output_neon;

    compare_2D<JSAMPLE>(upsample_config->output_num_rows, upsample_config->output_num_cols * RGB_PIXELSIZE, upsample_output_scalar->output_buf, upsample_output_neon->output_buf, (char *)"output_buf");
}

kernel_utility_t upsample_utility = {upsample_config_init, upsample_input_init, upsample_output_init, upsample_comparer};

#define F_0_344 11277 /* 0.3441467 = 11277 * 2^-15 */
#define F_0_714 23401 /* 0.7141418 = 23401 * 2^-15 */
#define F_1_402 22971 /* 1.4020386 = 22971 * 2^-14 */
#define F_1_772 29033 /* 1.7720337 = 29033 * 2^-14 */

const int16_t upsample_consts[4] = {-F_0_344, F_0_714, F_1_402, F_1_772};

#undef F_0_344
#undef F_0_714
#undef F_1_402
#undef F_1_772