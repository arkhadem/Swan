

#include "neon.hpp"
#include "strlen.hpp"
#include <stdio.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

size_t __strlen_aarch64(const char *);

#if defined(__cplusplus)
} // extern C
#endif

void strlen_neon(config_t *config,
                 input_t *input,
                 output_t *output) {
    strlen_input_t *strlen_input = (strlen_input_t *)input;
    strlen_output_t *strlen_output = (strlen_output_t *)output;

    char *src = strlen_input->src;

    strlen_output->return_value[0] = __strlen_aarch64(src);
}