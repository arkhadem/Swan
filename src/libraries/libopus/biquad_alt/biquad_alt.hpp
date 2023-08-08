#ifndef C2371007_63A6_4C04_8307_0A99502CD7C1
#define C2371007_63A6_4C04_8307_0A99502CD7C1

#include <stdint.h>
#include <stdlib.h>

#include "libopus.hpp"

typedef struct biquad_alt_config_s : config_t {
    // 64K samples
    opus_int32 len;
    uint32_t block_count;
    opus_int32 *B_Q28;
    opus_int32 *A_Q28;
} biquad_alt_config_t;

typedef struct biquad_alt_input_c : input_t {
    opus_int16 **in;
    opus_int32 **S_in;
} biquad_alt_input_t;

typedef struct biquad_alt_output_c : output_t {
    opus_int16 **out;
    opus_int32 **S_out;
} biquad_alt_output_t;

#endif /* C2371007_63A6_4C04_8307_0A99502CD7C1 */
