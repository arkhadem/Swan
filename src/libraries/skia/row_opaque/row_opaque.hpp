#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "skia.hpp"

typedef struct row_opaque_config_s : config_t {
    int num_rows;
    int num_cols;

} row_opaque_config_t;

typedef struct row_opaque_input_s : input_t {
    uint16_t *src_src;
    uint32_t *src_dst;
    uint32_t *src_opaque;
    uint32_t *color;
} row_opaque_input_t;

typedef struct row_opaque_output_s : output_t {
    uint32_t *dst_dst;
} row_opaque_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
