

#include "neon.hpp"
#include "memchr.hpp"
#include <stdio.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

void *__memchr_aarch64(const void *, int, size_t);

#if defined(__cplusplus)
} // extern C
#endif

void memchr_neon(config_t *config,
                 input_t *input,
                 output_t *output) {
    memchr_config_t *memchr_config = (memchr_config_t *)config;
    memchr_input_t *memchr_input = (memchr_input_t *)input;
    memchr_output_t *memchr_output = (memchr_output_t *)output;

    int size = memchr_config->size;

    char value = memchr_input->value[0];
    char *src = memchr_input->src;

    memchr_output->return_value = (char *)__memchr_aarch64(src, value, size);
}