#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "zlib.hpp"

extern z_crc_t Coeff_64K_shift;
extern const z_crc_t x2n_table[];
extern const z_crc_t crc_braid_table[][256];
extern const z_crc_t crc_table[];

typedef struct crc32_config_s : config_t {
    // 128 KB data
    z_size_t len;
    z_crc_t crc;
} crc32_config_t;

typedef struct crc32_input_s : input_t {
    unsigned char *buf;
} crc32_input_t;

typedef struct crc32_output_s : output_t {
    z_crc_t *return_value;
} crc32_output_t;

#define N 5
#define W 8

z_crc_t multmodp(z_crc_t a, z_crc_t b);
z_crc_t x2nmodp(int n, unsigned k);
z_crc_t crc32_combine64(z_crc_t crc1, z_crc_t crc2, int len2);
z_crc_t crc_word(z_word_t data);

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
