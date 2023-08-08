#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "libpng.hpp"

typedef struct read_sub_config_s : config_t {
    // one row of 1280 pixels
    int num_rows;
    int num_cols;
} read_sub_config_t;

typedef struct read_sub_input_s : input_t {
    png_bytepp input_buf;
} read_sub_input_t;

typedef struct read_sub_output_s : output_t {
    png_bytepp output_buf;
} read_sub_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
