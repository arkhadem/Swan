#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "libwebp.hpp"

typedef struct dispatch_alpha_config_s : config_t {
    // an image of 720x1280 pixels
    int num_rows;
    int num_cols;
} dispatch_alpha_config_t;

typedef struct dispatch_alpha_input_s : input_t {
    uint8_t *src;
    uint8_t *alpha;
} dispatch_alpha_input_t;

typedef struct dispatch_alpha_output_s : output_t {
    uint8_t *dst;
    int *return_val;
} dispatch_alpha_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
