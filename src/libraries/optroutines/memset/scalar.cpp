

#include "scalar.hpp"
#include "memset.hpp"
#include <stdio.h>
#include <string.h>

/* Scalar memset implementation provided by Swan benchmark suite! */
void memset_scalar(config_t *config,
                   input_t *input,
                   output_t *output) {
    memset_config_t *memset_config = (memset_config_t *)config;
    memset_input_t *memset_input = (memset_input_t *)input;
    memset_output_t *memset_output = (memset_output_t *)output;

    int size = memset_config->size;

    char value = memset_input->value[0];
    char *dst = memset_output->dst;

    uint64_t value_64bit = value * 0x0101010101010101;
    uint64_t *dst_64bit = (uint64_t *)dst;

    int i = 0;

    for (; i + 32 <= size; i += 32) {
        *dst_64bit++ = value_64bit;
        *dst_64bit++ = value_64bit;
        *dst_64bit++ = value_64bit;
        *dst_64bit++ = value_64bit;
    }

    if (i + 16 <= size) {
        *dst_64bit++ = value_64bit;
        *dst_64bit++ = value_64bit;
        i += 16;
    }

    if (i + 8 <= size) {
        *dst_64bit++ = value_64bit;
        i += 16;
    }

    for (; i < size; i++)
        dst[i] = value;
}