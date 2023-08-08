#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "webaudio.hpp"

typedef struct sum_from_config_s : config_t {
    // 64K samples, 6 channels
    uint32_t data_size;
    uint32_t block_count;
} sum_from_config_t;

typedef struct sum_from_input_s : input_t {
    float **source2;
    float **source1;
} sum_from_input_t;

typedef struct sum_from_output_s : output_t {
    float **destination;
} sum_from_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
