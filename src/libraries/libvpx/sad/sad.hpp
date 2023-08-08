#ifndef EBB2A2C9_1033_42F1_B349_5CBA3485F476
#define EBB2A2C9_1033_42F1_B349_5CBA3485F476

#include <stdint.h>

#include "libvpx.hpp"

typedef struct sad_config_s : config_t {
    // 1 8x8 DCT block of a 720x1280 video
    int block_size;
    int src_stride;
    int ref_stride;
    int rows;
    int cols;
    int block_count;
    uint8_t *src_frame_buff;
    uint8_t *ref_frame_buff;
    int *coordinations;
} sad_config_t;

typedef struct sad_input_s : input_t {
    uint8_t **src_ptr;
    uint8_t **ref_ptr;
} sad_input_t;

typedef struct sad_output_s : output_t {
    uint32_t *return_val;
} sad_output_t;

#endif /* EBB2A2C9_1033_42F1_B349_5CBA3485F476 */
