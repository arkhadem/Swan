#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "skia.hpp"

typedef struct row_blend_config_s : config_t {
    int num_rows;
    int num_cols;
    uint8_t alpha;
} row_blend_config_t;

typedef struct row_blend_input_s : input_t {
    uint32_t *src;
} row_blend_input_t;

typedef struct row_blend_output_s : output_t {
    uint32_t *dst;
} row_blend_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
