#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "libpng.hpp"

typedef struct png_color_struct {
    png_byte red;
    png_byte green;
    png_byte blue;
} png_color;
typedef png_color *png_colorp;

typedef struct expand_palette_config_s : config_t {
    int num_rows;
    int num_cols;
    // For NEON
    png_bytep riffled_palette;
    // For Scalar
    png_colorp rgb_palette;
    png_bytep a_palette;
} expand_palette_config_t;

typedef struct expand_palette_input_s : input_t {
    png_byte **input_buf;
} expand_palette_input_t;

typedef struct expand_palette_output_s : output_t {
    png_byte **output_buf;
} expand_palette_output_t;

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
