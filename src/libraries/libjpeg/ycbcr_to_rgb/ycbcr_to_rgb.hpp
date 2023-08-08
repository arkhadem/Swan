#ifndef CF2D2AD5_CAD9_4839_888A_ED759CD32BB2
#define CF2D2AD5_CAD9_4839_888A_ED759CD32BB2

#include "libjpeg.hpp"

extern const int16_t ycbcr_to_rgb_const[4];

typedef struct {
    /* Private state for YCC->RGB conversion */
    int *Cr_r_tab;   /* => table for Cr to R conversion */
    int *Cb_b_tab;   /* => table for Cb to B conversion */
    JLONG *Cr_g_tab; /* => table for Cr to G conversion */
    JLONG *Cb_g_tab; /* => table for Cb to G conversion */

    /* Private state for RGB->Y conversion */
    JLONG *rgb_y_tab; /* => table for RGB to Y conversion */
} my_color_deconverter;

typedef my_color_deconverter *my_cconvert_ptr;

typedef struct ycbcr_to_rgb_config_s : config_t {
    // 720x1280 image
    JDIMENSION num_rows;
    JDIMENSION num_cols;
    JSAMPLE *sample_range_limit;
    my_cconvert_ptr cconvert;
} ycbcr_to_rgb_config_t;

typedef struct ycbcr_to_rgb_input_s : input_t {
    JSAMPIMAGE input_buf;
} ycbcr_to_rgb_input_t;

typedef struct ycbcr_to_rgb_output_s : output_t {
    JSAMPARRAY output_buf;
} ycbcr_to_rgb_output_t;

#endif /* CF2D2AD5_CAD9_4839_888A_ED759CD32BB2 */
