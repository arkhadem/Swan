#ifndef C2371007_63A6_4C04_8307_0A99502CD7C1
#define C2371007_63A6_4C04_8307_0A99502CD7C1

#include <stdint.h>
#include <stdlib.h>

#include "libopus.hpp"

typedef struct lpc_prediction_config_s : config_t {
    // order 4
    opus_int order;
    uint32_t block_count;
} lpc_prediction_config_t;

typedef struct lpc_prediction_input_c : input_t {
    opus_int16 **A_Q12;
} lpc_prediction_input_t;

typedef struct lpc_prediction_output_c : output_t {
    opus_int32 *return_val;
} lpc_prediction_output_t;

#endif /* C2371007_63A6_4C04_8307_0A99502CD7C1 */
