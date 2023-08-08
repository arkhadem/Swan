

#include "neon.hpp"
#include "memset.hpp"
#include <stdio.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

void *__memset_aarch64(void *, int, size_t);

#if defined(__cplusplus)
} // extern C
#endif

void memset_neon(config_t *config,
                 input_t *input,
                 output_t *output) {
    memset_config_t *memset_config = (memset_config_t *)config;
    memset_input_t *memset_input = (memset_input_t *)input;
    memset_output_t *memset_output = (memset_output_t *)output;

    int size = memset_config->size;

    char value = memset_input->value[0];
    char *dst = memset_output->dst;

    __memset_aarch64(dst, value, size);
}