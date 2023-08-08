

#include "scalar.hpp"
#include "strlen.hpp"
#include <stdio.h>
#include <string.h>

/* Scalar strlen implementation provided by Swan benchmark suite! */
void strlen_scalar(config_t *config,
                   input_t *input,
                   output_t *output) {
    strlen_input_t *strlen_input = (strlen_input_t *)input;
    strlen_output_t *strlen_output = (strlen_output_t *)output;

    char *src = strlen_input->src;
    char *my_src = src;

    int size = 0;
    while (true) {
        char cmp1 = *my_src++ && *my_src++;
        char cmp2 = *my_src++ && *my_src++;
        char cmp12 = cmp1 && cmp2;
        char cmp3 = *my_src++ && *my_src++;
        char cmp4 = *my_src++ && *my_src++;
        char cmp34 = cmp3 && cmp4;
        char cmp = cmp12 && cmp34;
        if (cmp) {
            size += 8;
        } else {
            break;
        }
    }

    src += size;
    while (*src++)
        size++;

    strlen_output->return_value[0] = size;
}