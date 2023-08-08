

#include "neon.hpp"
#include "memcmp.hpp"
#include <stdio.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

int __memcmp_aarch64(const void *, const void *, size_t);

#if defined(__cplusplus)
} // extern C
#endif

void memcmp_neon(config_t *config,
                 input_t *input,
                 output_t *output) {
    memcmp_config_t *memcmp_config = (memcmp_config_t *)config;
    memcmp_input_t *memcmp_input = (memcmp_input_t *)input;
    memcmp_output_t *memcmp_output = (memcmp_output_t *)output;

    int size = memcmp_config->size;

    char *src1 = memcmp_input->src1;
    char *src2 = memcmp_input->src2;

    memcmp_output->return_val[0] = __memcmp_aarch64(src1, src2, size);
}