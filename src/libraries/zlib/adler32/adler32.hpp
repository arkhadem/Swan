#ifndef BCD754D4_9604_4C5E_A1C0_5B4EE0416E80
#define BCD754D4_9604_4C5E_A1C0_5B4EE0416E80

#include <stdint.h>
#include <stdlib.h>

#include "zlib.hpp"

typedef struct adler32_config_s : config_t {
    // 128KB data
    z_size_t len;
    uLong adler;
} adler32_config_t;

typedef struct adler32_input_s : input_t {
    Bytef *buf;
} adler32_input_t;

typedef struct adler32_output_s : output_t {
    uLong *return_value;
} adler32_output_t;

#define DO1(buf, i)        \
    {                      \
        adler += (buf)[i]; \
        sum2 += adler;     \
    }
#define DO2(buf, i) \
    DO1(buf, i);    \
    DO1(buf, i + 1);
#define DO4(buf, i) \
    DO2(buf, i);    \
    DO2(buf, i + 2);
#define DO8(buf, i) \
    DO4(buf, i);    \
    DO4(buf, i + 4);
#define DO16(buf) \
    DO8(buf, 0);  \
    DO8(buf, 8);

#define BASE 65521U /* largest prime smaller than 65536 */
#define NMAX 5552
#define MOD(a) a %= BASE

#ifndef MIN_BLOCK
#define MIN_BLOCK BLOCK_128
#endif

#endif /* BCD754D4_9604_4C5E_A1C0_5B4EE0416E80 */
