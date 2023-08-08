#ifndef EBB2A2C9_1033_42F1_B349_5CBA3485F476
#define EBB2A2C9_1033_42F1_B349_5CBA3485F476

#include <stdint.h>

#include "libvpx.hpp"

typedef struct forward_dct_config_s : config_t {
    // 1 8x8 DCT block of a 720x1280 video
    int stride;
    int rows;
    int cols;
    int block_size;
    int block_count;
    int16_t *input_frame_buff;
    int *coordinations;
} forward_dct_config_t;

typedef struct forward_dct_input_s : input_t {
    int16_t **input_buff;
} forward_dct_input_t;

typedef struct forward_dct_output_s : output_t {
    tran_low_t **output_buff;
} forward_dct_output_t;

#endif /* EBB2A2C9_1033_42F1_B349_5CBA3485F476 */
