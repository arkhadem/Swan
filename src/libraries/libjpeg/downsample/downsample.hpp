#ifndef D0C85835_AEFF_4B8D_A696_B9C901A64527
#define D0C85835_AEFF_4B8D_A696_B9C901A64527

#include <stdint.h>

#include "libjpeg.hpp"

extern const int16_t downsample_consts[4];

typedef struct downsample_config_s : config_t {
    // 720 rows (height) x 1280 columns (width)
    JDIMENSION input_num_rows;
    JDIMENSION input_num_cols;
    // 720/2 rows (height) x 1280/2 columns (width)
    JDIMENSION output_num_rows;
    JDIMENSION output_num_cols;
} downsample_config_t;

typedef struct downsample_input_s : input_t {
    JSAMPARRAY input_buf;
} downsample_input_t;

typedef struct downsample_output_s : output_t {
    JSAMPARRAY output_buf;
} downsample_output_t;

#endif /* D0C85835_AEFF_4B8D_A696_B9C901A64527 */
