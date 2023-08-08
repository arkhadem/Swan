#ifndef CDEF0667_0942_43D0_95E6_090E75EDEE29
#define CDEF0667_0942_43D0_95E6_090E75EDEE29

#include <stdint.h>

#include "libjpeg.hpp"

extern const int16_t upsample_consts[4];

typedef struct {
    /* Private state for YCC->RGB conversion */
    int *Cr_r_tab;   /* => table for Cr to R conversion */
    int *Cb_b_tab;   /* => table for Cb to B conversion */
    JLONG *Cr_g_tab; /* => table for Cr to G conversion */
    JLONG *Cb_g_tab; /* => table for Cb to G conversion */
} my_merged_upsampler;

typedef my_merged_upsampler *my_merged_upsample_ptr;

typedef struct upsample_config_s : config_t {
    // 720 rows (height) x 1280 columns (width)
    JDIMENSION input_num_rows;
    JDIMENSION input_num_cols;
    // 720*2 rows (height) x 1280*2 columns (width)
    JDIMENSION output_num_rows;
    JDIMENSION output_num_cols;
    JSAMPLE *sample_range_limit;
    my_merged_upsample_ptr cconvert;
} upsample_config_t;

typedef struct upsample_input_s : input_t {
    // 3 channels: Y, CB, CR

    // 16 input rows
    // (for Y, but it is 8 for CB and CR which is not used)

    // 1024 input columns
    // (for Y, but it is 512 for CB and CR which is not used)

    JSAMPIMAGE input_buf;
} upsample_input_t;

typedef struct upsample_output_s : output_t {
    JSAMPARRAY output_buf;
} upsample_output_t;

#endif /* CDEF0667_0942_43D0_95E6_090E75EDEE29 */
