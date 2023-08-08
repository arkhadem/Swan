#ifndef D197F8E1_C256_4D28_8065_BB3FD4132843
#define D197F8E1_C256_4D28_8065_BB3FD4132843

#include <stdint.h>

#include "libjpeg.hpp"

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

typedef struct rgb_to_gray_config_s : config_t {
    // 1280x720 picture
    JDIMENSION num_rows;
    JDIMENSION num_cols;
    my_cconvert_ptr cconvert;
} rgb_to_gray_config_t;

typedef struct rgb_to_gray_input_s : input_t {
    JSAMPARRAY input_buf;
} rgb_to_gray_input_t;

typedef struct rgb_to_gray_output_s : output_t {
    JSAMPARRAY output_buf;
} rgb_to_gray_output_t;

#endif /* D197F8E1_C256_4D28_8065_BB3FD4132843 */
