#ifndef EB6E52BD_79AF_40E4_9EF0_FE0FBEAE172F
#define EB6E52BD_79AF_40E4_9EF0_FE0FBEAE172F

#include <stdint.h>
#include <stdio.h>

#define CACHE_SIZE 2097152

#define SWAN_TXT_INPUT_SIZE 131072

#define SWAN_IMG_INPUT_DEFAULT_CHANNEL_SIZE 4
#define SWAN_IMG_INPUT_ROW_SIZE 720
#define SWAN_IMG_INPUT_COL_SIZE 1280

#define SWAN_AUD_CHANNEL_SIZE 2
#define SWAN_AUD_INPUT_SIZE 128
#define SWAN_AUD_SAMPLE_RATE 44100

#define CACHE_STATUS_COLD 0
#define CACHE_STATUS_WARM 1

#ifndef CACHE_STATUS
#define CACHE_STATUS CACHE_STATUS_WARM
#endif

typedef struct config_s {
    int granularity;
} config_t;

typedef struct input_s {
    int dummy[1];
} input_t;

typedef struct output_s {
    uint32_t dummy[1];
} output_t;

typedef void (*kernel_func)(config_t *, input_t *, output_t *);

// Utility functions
typedef int (*config_init_func)(size_t, config_t *&);
typedef void (*input_init_func)(int, config_t *, input_t **&);
typedef void (*output_init_func)(int, config_t *, output_t **&);
typedef void (*comparer_func)(config_t *, output_t *, output_t *);

typedef struct kernel_utility_c {
    config_init_func config_init;
    input_init_func input_init;
    output_init_func output_init;
    comparer_func comparer;
} kernel_utility_t;

#define STR(x) #x
#define SWAN_ASSERT(x)                                                                                                       \
    if (!(x)) {                                                                                                              \
        printf("ASSERTION FAILED: (%s), function %s, file %s, line %d.\n", STR(x), __PRETTY_FUNCTION__, __FILE__, __LINE__); \
        abort();                                                                                                             \
    }

typedef enum platform_e {
    SCALAR_PLAT,
    NEON_PLAT,
    MAX_PLAT
} platform_t;

void benchmark_runner(platform_t platform, char *library, char *kernel, int rounds, bool execute);
long *pollute_cache(size_t size);

// #ifndef SWAN_SIMULATION
// #define SWAN_SIMULATION
// #endif

#ifdef SWAN_SIMULATION
#define SWAN_REG_128_TYPE 128
#define SWAN_REG_256_TYPE 256
#define SWAN_REG_512_TYPE 512
#define SWAN_REG_1024_TYPE 1024

#ifndef SWAN_REG_TYPE
#define SWAN_REG_TYPE SWAN_REG_512_TYPE
#endif

#endif

#endif /* EB6E52BD_79AF_40E4_9EF0_FE0FBEAE172F */
