#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "skia.hpp"

typedef struct convolve_horizontally_config_s : config_t {
    int num_rows;
    int num_cols;
    int filter_length;
    int shift_value;

} convolve_horizontally_config_t;

typedef struct convolve_horizontally_input_s : input_t {
    // 4 colors (RGBA)
    // 1024 rows
    // 4 columns + 32 coefficients
    uint8_t *src_data;
    int16_t *filter_values;
} convolve_horizontally_input_t;

typedef struct convolve_horizontally_output_s : output_t {
    // 4 colors (RGBA)
    // 1024 rows
    // 4 columns
    uint8_t *out_row;
} convolve_horizontally_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
