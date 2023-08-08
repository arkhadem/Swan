

#include "scalar.hpp"
#include "memchr.hpp"
#include <stdio.h>
#include <string.h>

/* Scalar memchr implementation provided by Swan benchmark suite! */
void memchr_scalar(config_t *config,
                   input_t *input,
                   output_t *output) {
    memchr_config_t *memchr_config = (memchr_config_t *)config;
    memchr_input_t *memchr_input = (memchr_input_t *)input;
    memchr_output_t *memchr_output = (memchr_output_t *)output;

    int size = memchr_config->size;

    char value = memchr_input->value[0];
    char *src = memchr_input->src;
    char *src_end = memchr_input->src + size;

    uint64_t value_64bit = value * 0x0101010101010101;
    uint64_t *src_64bit = (uint64_t *)src;

    char tmp_mem_8b[8];
    uint64_t *tmp_mem_64b = (uint64_t *)tmp_mem_8b;

    for (int i = 0; i + 8 <= size; i += 8) {
        *tmp_mem_64b = value_64bit ^ *src_64bit++;
        char cmp1 = tmp_mem_8b[0] && tmp_mem_8b[1];
        char cmp2 = tmp_mem_8b[2] && tmp_mem_8b[3];
        char cmp12 = cmp1 && cmp2;
        char cmp3 = tmp_mem_8b[4] && tmp_mem_8b[5];
        char cmp4 = tmp_mem_8b[6] && tmp_mem_8b[6];
        char cmp34 = cmp3 && cmp4;
        char cmp = cmp12 && cmp34;
        if (cmp)
            src += 8;
        else
            break;
    }

    while ((src != src_end) && (*src != value)) {
        src++;
    }

    memchr_output->return_value = src;
}