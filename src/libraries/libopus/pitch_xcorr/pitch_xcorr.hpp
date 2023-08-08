#ifndef C2371007_63A6_4C04_8307_0A99502CD7C1
#define C2371007_63A6_4C04_8307_0A99502CD7C1

#include <stdint.h>
#include <stdlib.h>

#include "libopus.hpp"

typedef struct pitch_xcorr_config_s : config_t {
    // 64K samples
    int len;
    uint32_t block_count;
    int max_pitch;
} pitch_xcorr_config_t;

typedef struct pitch_xcorr_input_c : input_t {
    opus_val16 **x;
    opus_val16 **y;
} pitch_xcorr_input_t;

typedef struct pitch_xcorr_output_c : output_t {
    opus_val32 **xcorr;
} pitch_xcorr_output_t;

#endif /* C2371007_63A6_4C04_8307_0A99502CD7C1 */
