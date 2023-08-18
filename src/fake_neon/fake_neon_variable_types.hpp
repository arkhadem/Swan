#ifndef D69AB9A9_4678_49E1_9066_9DD7283850C0
#define D69AB9A9_4678_49E1_9066_9DD7283850C0

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

// #define __fp16 int16_t
// #define _Float16 uint16_t

enum FAKE_NEON_REG_TYPE_t : uint32_t {
    TYPE_64 = 0,
    TYPE_128 = 1,
    TYPE_256 = 2,
    TYPE_512 = 3,
    TYPE_1024 = 4,
    TYPE_MAX = 5,
};

typedef union {
    uint8_t ub[16 / 2];
    int8_t sb[16 / 2];
    uint16_t uh[8 / 2];
    int16_t sh[8 / 2];
    // __fp16 fh[8 / 2];
    uint32_t us[4 / 2];
    int32_t ss[4 / 2];
    float fs[4 / 2];
    uint64_t ud[2 / 2];
    int64_t sd[2 / 2];
    double fd[2 / 2];
} __64bit_var;

typedef union {
    uint8_t ub[16];
    int8_t sb[16];
    uint16_t uh[8];
    int16_t sh[8];
    // __fp16 fh[8];
    uint32_t us[4];
    int32_t ss[4];
    float fs[4];
    uint64_t ud[2];
    int64_t sd[2];
    double fd[2];
} __128bit_var;

typedef union {
    uint8_t ub[16 * 2];
    int8_t sb[16 * 2];
    uint16_t uh[8 * 2];
    int16_t sh[8 * 2];
    // __fp16 fh[8 * 2];
    uint32_t us[4 * 2];
    int32_t ss[4 * 2];
    float fs[4 * 2];
    uint64_t ud[2 * 2];
    int64_t sd[2 * 2];
    double fd[2 * 2];
} __256bit_var;

typedef union {
    uint8_t ub[16 * 4];
    int8_t sb[16 * 4];
    uint16_t uh[8 * 4];
    int16_t sh[8 * 4];
    // __fp16 fh[8 * 4];
    uint32_t us[4 * 4];
    int32_t ss[4 * 4];
    float fs[4 * 4];
    uint64_t ud[2 * 4];
    int64_t sd[2 * 4];
    double fd[2 * 4];
} __512bit_var;

typedef union {
    uint8_t ub[16 * 8];
    int8_t sb[16 * 8];
    uint16_t uh[8 * 8];
    int16_t sh[8 * 8];
    // __fp16 fh[8 * 8];
    uint32_t us[4 * 8];
    int32_t ss[4 * 8];
    float fs[4 * 8];
    uint64_t ud[2 * 8];
    int64_t sd[2 * 8];
    double fd[2 * 8];
} __1024bit_var;

#endif /* D69AB9A9_4678_49E1_9066_9DD7283850C0 */
