#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "libwebp.hpp"

typedef struct sharp_filter_row_config_s : config_t {
    // A row of 1280 pixels
    int num_rows;
    int num_cols;
    int bit_depth;
} sharp_filter_row_config_t;

typedef struct sharp_filter_row_input_s : input_t {
    int16_t **A;
    int16_t **B;
    uint16_t **best_y;
} sharp_filter_row_input_t;

typedef struct sharp_filter_row_output_s : output_t {
    uint16_t **out;
} sharp_filter_row_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
