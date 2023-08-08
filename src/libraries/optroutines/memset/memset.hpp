#ifndef B54AD8F1_0C7E_45FB_B967_B373BEE1E681
#define B54AD8F1_0C7E_45FB_B967_B373BEE1E681

#include <stdint.h>
#include <stdlib.h>

#include "optroutines.hpp"

typedef struct memset_config_s : config_t {
    // 128 KB data
    int size;
} memset_config_t;

typedef struct memset_input_s : input_t {
    char *value;
} memset_input_t;

typedef struct memset_output_s : output_t {
    char *dst;
} memset_output_t;

#endif /* B54AD8F1_0C7E_45FB_B967_B373BEE1E681 */
