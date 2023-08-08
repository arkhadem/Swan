#ifndef B54AD8F1_0C7E_45FB_B967_B373BEE1E681
#define B54AD8F1_0C7E_45FB_B967_B373BEE1E681

#include <stdint.h>
#include <stdlib.h>

#include "optroutines.hpp"

typedef struct memchr_config_s : config_t {
    int size;

} memchr_config_t;

typedef struct memchr_input_s : input_t {
    char *value;
    char *src;
} memchr_input_t;

typedef struct memchr_output_s : output_t {
    char *return_value;
} memchr_output_t;

#endif /* B54AD8F1_0C7E_45FB_B967_B373BEE1E681 */