#include "sad.hpp"

#include "libvpx.hpp"

#include "swan.hpp"

#include "utility.hpp"

#include <assert.h>

#include <algorithm>
#include <random>
#include <vector>

int sad_config_init(size_t cache_size,
                    config_t *&config) {

    sad_config_t *sad_config = (sad_config_t *)config;

    int rows = SWAN_IMG_INPUT_ROW_SIZE;
    int cols = SWAN_IMG_INPUT_COL_SIZE;
    int block_size = 8 * 8;
#ifdef SWAN_SIMULATION
    int block_count = 1024;
#else
    int block_count = rows * cols / block_size;
#endif
    int stride = cols;

    // configuration
    alloc_1D<sad_config_t>(1, sad_config);
    init_alloc_1D<uint8_t>(rows * cols, sad_config->src_frame_buff);
    init_alloc_1D<uint8_t>(rows * cols, sad_config->ref_frame_buff);
    alloc_1D<int>(block_count, sad_config->coordinations);
    sad_config->rows = rows;
    sad_config->cols = cols;
    sad_config->block_size = block_size;
    sad_config->block_count = block_count;
    sad_config->granularity = block_count;
    sad_config->src_stride = stride;
    sad_config->ref_stride = stride;

    int y_blk = sad_config->cols / 8;
    for (int block = 0; block < block_count; block++) {
        int y = block % y_blk;
        int x = block / y_blk;
        int coordination = x * cols + y;
        SWAN_ASSERT(coordination < (rows * cols));
        sad_config->coordinations[block] = coordination;
    }

    // in/output versions
    size_t input_size = block_count * 2 * block_size * sizeof(uint8_t);
    size_t output_size = block_count * sizeof(uint32_t);
    int count = cache_size / (input_size + output_size) + 1;

    config = (config_t *)sad_config;

    return count;
}

void sad_input_init(int count,
                    config_t *config,
                    input_t **&input) {

    sad_config_t *sad_config = (sad_config_t *)config;
    sad_input_t **sad_input = (sad_input_t **)input;

    // initializing input versions
    alloc_1D<sad_input_t *>(count, sad_input);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sad_input_t>(1, sad_input[i]);
        alloc_1D<uint8_t *>(sad_config->block_count, sad_input[i]->src_ptr);
        alloc_1D<uint8_t *>(sad_config->block_count, sad_input[i]->ref_ptr);

        for (int j = 0; j < sad_config->block_count; j++) {
            sad_input[i]->src_ptr[j] = sad_config->src_frame_buff + sad_config->coordinations[j];
            sad_input[i]->ref_ptr[j] = sad_config->ref_frame_buff + sad_config->coordinations[j];
        }
    }

    input = (input_t **)sad_input;
}

void sad_output_init(int count,
                     config_t *config,
                     output_t **&output) {

    sad_config_t *sad_config = (sad_config_t *)config;
    sad_output_t **sad_output = (sad_output_t **)output;

    // initializing output versions
    alloc_1D<sad_output_t *>(count, sad_output);

    // initializing individual versions
    for (int i = 0; i < count; i++) {
        alloc_1D<sad_output_t>(1, sad_output[i]);

        alloc_1D<uint32_t>(sad_config->block_count, sad_output[i]->return_val);
    }

    output = (output_t **)sad_output;
}

void sad_comparer(config_t *config,
                  output_t *output_scalar,
                  output_t *output_neon) {
    sad_config_t *sad_config = (sad_config_t *)config;
    sad_output_t *sad_output_scalar = (sad_output_t *)output_scalar;
    sad_output_t *sad_output_neon = (sad_output_t *)output_neon;

    compare_1D<uint32_t>(sad_config->block_count, sad_output_scalar->return_val, sad_output_neon->return_val, (char *)"return_val");
}

kernel_utility_t sad_utility = {sad_config_init, sad_input_init, sad_output_init, sad_comparer};