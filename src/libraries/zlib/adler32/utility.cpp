#include "adler32.hpp"

#include "zlib.hpp"

#include "swan.hpp"

#include "utility.hpp"

int adler32_config_init(size_t cache_size,
                        config_t *&config) {

    adler32_config_t *adler32_config = (adler32_config_t *)config;

    // configuration
    int length = SWAN_TXT_INPUT_SIZE;

    alloc_1D<adler32_config_t>(1, adler32_config);

    adler32_config->len = length;
    adler32_config->adler = 734276;
    adler32_config->granularity = 1;

    // in/output versions
    size_t input_size = length * sizeof(Bytef);
    size_t output_size = 0;
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)adler32_config;

    return count;
}

void adler32_input_init(int count,
                        config_t *config,
                        input_t **&input) {

    adler32_config_t *adler32_config = (adler32_config_t *)config;
    adler32_input_t **adler32_input = (adler32_input_t **)input;

    // initializing input versions
    alloc_1D<adler32_input_t *>(count, adler32_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<adler32_input_t>(1, adler32_input[i]);

        init_alloc_1D<Bytef>(adler32_config->len, adler32_input[i]->buf);
    }

    input = (input_t **)adler32_input;
}

void adler32_output_init(int count,
                         config_t *config,
                         output_t **&output) {

    adler32_config_t *adler32_config = (adler32_config_t *)config;
    adler32_output_t **adler32_output = (adler32_output_t **)output;

    // initializing output versions
    alloc_1D<adler32_output_t *>(count, adler32_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<adler32_output_t>(1, adler32_output[i]);

        alloc_1D<uLong>(1, adler32_output[i]->return_value);
    }

    output = (output_t **)adler32_output;
}

void adler32_comparer(config_t *config,
                      output_t *output_scalar,
                      output_t *output_neon) {
    adler32_config_t *adler32_config = (adler32_config_t *)config;
    adler32_output_t *adler32_output_scalar = (adler32_output_t *)output_scalar;
    adler32_output_t *adler32_output_neon = (adler32_output_t *)output_neon;

    compare_1D<uLong>(1, adler32_output_scalar->return_value, adler32_output_neon->return_value, (char *)"return_value");
}

kernel_utility_t adler32_utility = {adler32_config_init, adler32_input_init, adler32_output_init, adler32_comparer};
