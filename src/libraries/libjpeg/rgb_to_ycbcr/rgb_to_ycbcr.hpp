#ifndef CF2D2AD5_CAD9_4839_888A_ED759CD32BB2
#define CF2D2AD5_CAD9_4839_888A_ED759CD32BB2

#include "libjpeg.hpp"

extern const uint16_t jsimd_rgb_ycc_neon_consts[8];

typedef struct {
    /* Private state for RGB->YCC conversion */
    JLONG *rgb_ycc_tab; /* => table for RGB to YCbCr conversion */
} my_color_converter;

typedef my_color_converter *my_cconvert_ptr;

typedef struct rgb_to_ycbcr_config_s : config_t {
    // 720x1280 image
    JDIMENSION num_rows;
    JDIMENSION num_cols;
    my_cconvert_ptr cconvert;
} rgb_to_ycbcr_config_t;

typedef struct rgb_to_ycbcr_input_s : input_t {
    JSAMPARRAY input_buf;
} rgb_to_ycbcr_input_t;

typedef struct rgb_to_ycbcr_output_s : output_t {
    JSAMPIMAGE output_buf;
} rgb_to_ycbcr_output_t;

#endif /* CF2D2AD5_CAD9_4839_888A_ED759CD32BB2 */
