#ifndef EBB2A2C9_1033_42F1_B349_5CBA3485F476
#define EBB2A2C9_1033_42F1_B349_5CBA3485F476

#include <stdint.h>

#include "libvpx.hpp"

typedef struct quant_config_s : config_t {
    // 1 8x8 DCT block of a 720x1280 video
    int rows;
    int cols;
    int block_size;
    int block_count;

    int16_t *zbin_ptr;
    int16_t *round_ptr;
    int16_t *quant_ptr;
    int16_t *quant_shift_ptr;
    int16_t *dequant_ptr;
    const int16_t *scan;
    const int16_t *iscan;
} quant_config_t;

typedef struct quant_input_s : input_t {
    tran_low_t **coeff_ptr;
} quant_input_t;

typedef struct quant_output_s : output_t {
    tran_low_t **qcoeff_ptr;
    tran_low_t **dqcoeff_ptr;
    uint16_t *eob_ptr;
} quant_output_t;

#endif /* EBB2A2C9_1033_42F1_B349_5CBA3485F476 */
