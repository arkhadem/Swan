#ifndef B54AD8F1_0C7E_45FB_B967_B373BEE1E681
#define B54AD8F1_0C7E_45FB_B967_B373BEE1E681

#include <stdint.h>
#include <stdlib.h>

#include "optroutines.hpp"

typedef struct strlen_config_s : config_t {
    // 128 KB string
    int size;
} strlen_config_t;

typedef struct strlen_input_s : input_t {
    char *src;
} strlen_input_t;

typedef struct strlen_output_s : output_t {
    int *return_value;
} strlen_output_t;

#endif /* B54AD8F1_0C7E_45FB_B967_B373BEE1E681 */