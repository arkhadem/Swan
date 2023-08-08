#ifndef C2371007_63A6_4C04_8307_0A99502CD7C1
#define C2371007_63A6_4C04_8307_0A99502CD7C1

#include <stdint.h>
#include <stdlib.h>

#include "libopus.hpp"

typedef struct warped_autocorrelation_config_s : config_t {
    // 64K samples
    opus_int warping_Q16;
    opus_int length;
    uint32_t block_count;
    opus_int order;
    opus_int32 *input_QST;
    opus_int32 *state;
} warped_autocorrelation_config_t;

typedef struct warped_autocorrelation_input_c : input_t {
    opus_int16 **input_data;
} warped_autocorrelation_input_t;

typedef struct warped_autocorrelation_output_c : output_t {
    opus_int32 **corr;
    opus_int **scale;
} warped_autocorrelation_output_t;

#endif /* C2371007_63A6_4C04_8307_0A99502CD7C1 */
