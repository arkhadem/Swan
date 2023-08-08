

#include "scalar.hpp"
#include "memcmp.hpp"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Scalar memcmp implementation provided by Swan benchmark suite! */
void memcmp_scalar(config_t *config,
                   input_t *input,
                   output_t *output) {
    memcmp_config_t *memcmp_config = (memcmp_config_t *)config;
    memcmp_input_t *memcmp_input = (memcmp_input_t *)input;
    memcmp_output_t *memcmp_output = (memcmp_output_t *)output;

    int size = memcmp_config->size;

    char *src1 = memcmp_input->src1;
    char *src2 = memcmp_input->src2;

    uint64_t *src1_64bit = (uint64_t *)src1;
    uint64_t *src2_64bit = (uint64_t *)src2;

    int i;

    for (i = 0; i + 32 <= size; i += 32) {
        uint64_t src1_val1 = *src1_64bit++;
        uint64_t src2_val1 = *src2_64bit++;
        bool tmp = (src1_val1 == src2_val1);

        uint64_t src1_val2 = *src1_64bit++;
        uint64_t src2_val2 = *src2_64bit++;
        tmp &= (src1_val2 == src2_val2);

        uint64_t src1_val3 = *src1_64bit++;
        uint64_t src2_val3 = *src2_64bit++;
        tmp &= (src1_val3 == src2_val3);

        uint64_t src1_val4 = *src1_64bit++;
        uint64_t src2_val4 = *src2_64bit++;
        tmp &= (src1_val4 == src2_val4);

        if (tmp == false)
            break;
    }

    src1 += i;
    src2 += i;

    while ((i < size) && (*src1 == *src2)) {
        src1++;
        src2++;
        i++;
    }

    memcmp_output->return_val[0] = (i == size) ? 0 : (*src1 > *src2) ? 1
                                                                     : -1;
}