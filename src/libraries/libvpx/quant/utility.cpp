#include "quant.hpp"

#include "libvpx.hpp"

#include "swan.hpp"

#include "utility.hpp"

#include <assert.h>

#include <algorithm>
#include <random>
#include <vector>

#define MAX_NEIGHBORS 2

typedef struct {
    const int16_t *scan;
    const int16_t *iscan;
    const int16_t *neighbors;
} scan_order;

static const int16_t default_scan_8x8_neighbors[65 * MAX_NEIGHBORS] __attribute__((aligned(16))) = {
    0,
    0,
    0,
    0,
    0,
    0,
    8,
    8,
    1,
    8,
    1,
    1,
    9,
    16,
    16,
    16,
    2,
    9,
    2,
    2,
    10,
    17,
    17,
    24,
    24,
    24,
    3,
    10,
    3,
    3,
    18,
    25,
    25,
    32,
    11,
    18,
    32,
    32,
    4,
    11,
    26,
    33,
    19,
    26,
    4,
    4,
    33,
    40,
    12,
    19,
    40,
    40,
    5,
    12,
    27,
    34,
    34,
    41,
    20,
    27,
    13,
    20,
    5,
    5,
    41,
    48,
    48,
    48,
    28,
    35,
    35,
    42,
    21,
    28,
    6,
    6,
    6,
    13,
    42,
    49,
    49,
    56,
    36,
    43,
    14,
    21,
    29,
    36,
    7,
    14,
    43,
    50,
    50,
    57,
    22,
    29,
    37,
    44,
    15,
    22,
    44,
    51,
    51,
    58,
    30,
    37,
    23,
    30,
    52,
    59,
    45,
    52,
    38,
    45,
    31,
    38,
    53,
    60,
    46,
    53,
    39,
    46,
    54,
    61,
    47,
    54,
    55,
    62,
    0,
    0,
};

static const int16_t vp9_default_iscan_8x8[64] __attribute__((aligned(16))) = {
    1,
    3,
    6,
    10,
    15,
    23,
    32,
    38,
    2,
    5,
    9,
    14,
    20,
    27,
    39,
    45,
    4,
    7,
    11,
    18,
    25,
    31,
    43,
    50,
    8,
    12,
    16,
    22,
    30,
    37,
    48,
    54,
    13,
    17,
    21,
    28,
    35,
    44,
    53,
    58,
    19,
    24,
    29,
    36,
    42,
    49,
    57,
    61,
    26,
    33,
    40,
    46,
    51,
    56,
    60,
    63,
    34,
    41,
    47,
    52,
    55,
    59,
    62,
    64,
};

static const int16_t default_scan_8x8[64] __attribute__((aligned(16))) = {
    0,
    8,
    1,
    16,
    9,
    2,
    17,
    24,
    10,
    3,
    18,
    25,
    32,
    11,
    4,
    26,
    33,
    19,
    40,
    12,
    34,
    27,
    5,
    41,
    20,
    48,
    13,
    35,
    42,
    28,
    21,
    6,
    49,
    56,
    36,
    43,
    29,
    7,
    14,
    50,
    57,
    44,
    22,
    37,
    15,
    51,
    58,
    30,
    45,
    23,
    52,
    59,
    38,
    31,
    60,
    53,
    46,
    39,
    61,
    54,
    47,
    62,
    55,
    63,
};

int quant_config_init(size_t cache_size,
                      config_t *&config) {

    quant_config_t *quant_config = (quant_config_t *)config;

    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int cols = SWAN_IMG_INPUT_COL_SIZE;
    int block_size = 8 * 8;
    int block_count = rows * cols / block_size;

    // configuration
    alloc_1D<quant_config_t>(1, quant_config);
    quant_config->rows = rows;
    quant_config->cols = cols;
    quant_config->block_size = block_size;
    quant_config->block_count = block_count;
    quant_config->granularity = block_count;

    // coefficients
    init_alloc_1D<int16_t>(8, quant_config->zbin_ptr, 1140, 1143);
    // init_alloc_1D<int16_t>(8, quant_config->zbin_ptr, 0, 1143);
    init_alloc_1D<int16_t>(8, quant_config->round_ptr, 0, 685);
    init_alloc_1D<int16_t>(8, quant_config->quant_ptr, -32703, 1);
    init_alloc_1D<int16_t>(8, quant_config->quant_shift_ptr, 0, 16384);
    init_alloc_1D<int16_t>(8, quant_config->dequant_ptr, 1, 1828);
    for (int j = 2; j < 8; j++) {
        quant_config->zbin_ptr[j] = quant_config->zbin_ptr[1];
        quant_config->round_ptr[j] = quant_config->round_ptr[1];
        quant_config->quant_ptr[j] = quant_config->quant_ptr[1];
        quant_config->quant_shift_ptr[j] = quant_config->quant_shift_ptr[1];
        quant_config->dequant_ptr[j] = quant_config->dequant_ptr[1];
    }
    scan_order my_scan = {default_scan_8x8, vp9_default_iscan_8x8, default_scan_8x8_neighbors};
    quant_config->scan = my_scan.scan;
    quant_config->iscan = my_scan.iscan;

    // in/output versions
    size_t input_size = block_count * block_size * sizeof(tran_low_t);
    size_t output_size = block_count * 2 * block_size * sizeof(tran_low_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)quant_config;

    return count;
}

void quant_input_init(int count,
                      config_t *config,
                      input_t **&input) {

    quant_config_t *quant_config = (quant_config_t *)config;
    quant_input_t **quant_input = (quant_input_t **)input;

    // initializing input versions
    alloc_1D<quant_input_t *>(count, quant_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<quant_input_t>(1, quant_input[i]);

        tran_low_t *tmp;

        init_alloc_1D<tran_low_t>(quant_config->block_size, tmp, -2 * 1143, 2 * 1143);

        alloc_1D<tran_low_t *>(quant_config->block_count, quant_input[i]->coeff_ptr);

        for (int block = 0; block < quant_config->block_count; block++)
            quant_input[i]->coeff_ptr[block] = tmp;

        // init_alloc_2D<tran_low_t>(quant_config->block_count, quant_config->block_size, quant_input[i]->coeff_ptr, -2 * 1143, 2 * 1143);
    }

    input = (input_t **)quant_input;
}

void quant_output_init(int count,
                       config_t *config,
                       output_t **&output) {

    quant_config_t *quant_config = (quant_config_t *)config;
    quant_output_t **quant_output = (quant_output_t **)output;

    // initializing output versions
    alloc_1D<quant_output_t *>(count, quant_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<quant_output_t>(1, quant_output[i]);

        alloc_2D<tran_low_t>(quant_config->block_count, quant_config->block_size, quant_output[i]->qcoeff_ptr);
        alloc_2D<tran_low_t>(quant_config->block_count, quant_config->block_size, quant_output[i]->dqcoeff_ptr);
        alloc_1D<uint16_t>(quant_config->block_count, quant_output[i]->eob_ptr);
    }

    output = (output_t **)quant_output;
}

void quant_comparer(config_t *config,
                    output_t *output_scalar,
                    output_t *output_neon) {
    quant_config_t *quant_config = (quant_config_t *)config;
    quant_output_t *quant_output_scalar = (quant_output_t *)output_scalar;
    quant_output_t *quant_output_neon = (quant_output_t *)output_neon;

    compare_2D<tran_low_t>(quant_config->block_count, quant_config->block_size, quant_output_scalar->qcoeff_ptr, quant_output_neon->qcoeff_ptr, (char *)"qcoeff_ptr");
    compare_2D<tran_low_t>(quant_config->block_count, quant_config->block_size, quant_output_scalar->dqcoeff_ptr, quant_output_neon->dqcoeff_ptr, (char *)"dqcoeff_ptr");
    compare_1D<uint16_t>(quant_config->block_count, quant_output_scalar->eob_ptr, quant_output_neon->eob_ptr, (char *)"eob_ptr");
}

kernel_utility_t quant_utility = {quant_config_init, quant_input_init, quant_output_init, quant_comparer};