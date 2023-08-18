#include "fake_neon_intrinsics.hpp"
#include "fake_neon_data_types.hpp"
#include "fake_neon_include.hpp"
#include "fake_neon_variable_types.hpp"
#include <stdio.h>
#include <string.h>
#include <string>

#define SATQ_S16(VAL)                                                     \
    ((int32_t)(VAL) > 32767) ? 32767 : ((int32_t)(VAL) < -32768) ? -32768 \
                                                                 : (int32_t)(VAL)

#define SATQ_S32(VAL)                                                                                                                     \
    ((int64_t)(VAL) > (int64_t)(2147483647)) ? (int64_t)(2147483647) : ((int64_t)(VAL) < (int64_t)(-2147483648)) ? (int64_t)(-2147483648) \
                                                                                                                 : (int64_t)(VAL)

void fake_neon_initializer(char *exp_name) { fake_neon::initializer(exp_name); }

void fake_neon_finisher() { fake_neon::finisher(); }

void fake_neon_flusher() { fake_neon::flusher(); }

// Load
uint8x8_t vld1_u8(uint8_t const *ptr) {
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint8x16_t vld1q_u8(uint8_t const *ptr) {
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint8x32_t vld1dq_u8(uint8_t const *ptr) {
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint8x64_t vld1qq_u8(uint8_t const *ptr) {
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint8x128_t vld1oq_u8(uint8_t const *ptr) {
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}

uint16x4_t vld1_u16(uint16_t const *ptr) {
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint16x8_t vld1q_u16(uint16_t const *ptr) {
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint16x16_t vld1dq_u16(uint16_t const *ptr) {
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint16x32_t vld1qq_u16(uint16_t const *ptr) {
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint16x64_t vld1oq_u16(uint16_t const *ptr) {
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}

int16x4_t vld1_s16(int16_t const *ptr) {
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int16x8_t vld1q_s16(int16_t const *ptr) {
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int16x16_t vld1dq_s16(int16_t const *ptr) {
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int16x32_t vld1qq_s16(int16_t const *ptr) {
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int16x64_t vld1oq_s16(int16_t const *ptr) {
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}

uint32x2_t vld1_u32(uint32_t const *ptr) {
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint32x4_t vld1q_u32(uint32_t const *ptr) {
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint32x8_t vld1dq_u32(uint32_t const *ptr) {
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint32x16_t vld1qq_u32(uint32_t const *ptr) {
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
uint32x32_t vld1oq_u32(uint32_t const *ptr) {
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}

int32x2_t vld1_s32(int32_t const *ptr) {
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int32x4_t vld1q_s32(int32_t const *ptr) {
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int32x8_t vld1dq_s32(int32_t const *ptr) {
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int32x16_t vld1qq_s32(int32_t const *ptr) {
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int32x32_t vld1oq_s32(int32_t const *ptr) {
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}

float32x2_t vld1_f32(float const *ptr) {
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
float32x4_t vld1q_f32(float const *ptr) {
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
float32x8_t vld1dq_f32(float const *ptr) {
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
float32x16_t vld1qq_f32(float const *ptr) {
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
float32x32_t vld1oq_f32(float const *ptr) {
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}

int64x1_t vld1_s64(int64_t const *ptr) {
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int64x2_t vld1q_s64(int64_t const *ptr) {
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int64x4_t vld1dq_s64(int64_t const *ptr) {
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int64x8_t vld1qq_s64(int64_t const *ptr) {
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}
int64x16_t vld1oq_s64(int64_t const *ptr) {
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = ptr[i];
    }
    o.id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, o.type);
    return o;
}

uint32x2x4_t vld4_u32(uint32_t const *ptr) {
    uint32x2x4_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    __64bit_var o_var2;
    __64bit_var o_var3;
    for (int i = 0; i < 2; i++) {
        o_var0.us[i] = ptr[i * 4 + 0];
        o_var1.us[i] = ptr[i * 4 + 1];
        o_var2.us[i] = ptr[i * 4 + 2];
        o_var3.us[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}
uint32x4x4_t vld4q_u32(uint32_t const *ptr) {
    uint32x4x4_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    __128bit_var o_var2;
    __128bit_var o_var3;
    for (int i = 0; i < 4; i++) {
        o_var0.us[i] = ptr[i * 4 + 0];
        o_var1.us[i] = ptr[i * 4 + 1];
        o_var2.us[i] = ptr[i * 4 + 2];
        o_var3.us[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}
uint32x8x4_t vld4dq_u32(uint32_t const *ptr) {
    uint32x8x4_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    __256bit_var o_var2;
    __256bit_var o_var3;
    for (int i = 0; i < 8; i++) {
        o_var0.us[i] = ptr[i * 4 + 0];
        o_var1.us[i] = ptr[i * 4 + 1];
        o_var2.us[i] = ptr[i * 4 + 2];
        o_var3.us[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}
uint32x16x4_t vld4qq_u32(uint32_t const *ptr) {
    uint32x16x4_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    __512bit_var o_var2;
    __512bit_var o_var3;
    for (int i = 0; i < 16; i++) {
        o_var0.us[i] = ptr[i * 4 + 0];
        o_var1.us[i] = ptr[i * 4 + 1];
        o_var2.us[i] = ptr[i * 4 + 2];
        o_var3.us[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}
uint32x32x4_t vld4oq_u32(uint32_t const *ptr) {
    uint32x32x4_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    __1024bit_var o_var2;
    __1024bit_var o_var3;
    for (int i = 0; i < 32; i++) {
        o_var0.us[i] = ptr[i * 4 + 0];
        o_var1.us[i] = ptr[i * 4 + 1];
        o_var2.us[i] = ptr[i * 4 + 2];
        o_var3.us[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}

uint8x8x2_t vld2_u8(uint8_t const *ptr) {
    uint8x8x2_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    for (int i = 0; i < 8; i++) {
        o_var0.ub[i] = ptr[i * 2 + 0];
        o_var1.ub[i] = ptr[i * 2 + 1];
    }
    o.val[0].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var1, o.val[1].type);
    return o;
}
uint8x16x2_t vld2q_u8(uint8_t const *ptr) {
    uint8x16x2_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    for (int i = 0; i < 16; i++) {
        o_var0.ub[i] = ptr[i * 2 + 0];
        o_var1.ub[i] = ptr[i * 2 + 1];
    }
    o.val[0].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var1, o.val[1].type);
    return o;
}
uint8x32x2_t vld2dq_u8(uint8_t const *ptr) {
    uint8x32x2_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    for (int i = 0; i < 32; i++) {
        o_var0.ub[i] = ptr[i * 2 + 0];
        o_var1.ub[i] = ptr[i * 2 + 1];
    }
    o.val[0].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var1, o.val[1].type);
    return o;
}
uint8x64x2_t vld2qq_u8(uint8_t const *ptr) {
    uint8x64x2_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    for (int i = 0; i < 64; i++) {
        o_var0.ub[i] = ptr[i * 2 + 0];
        o_var1.ub[i] = ptr[i * 2 + 1];
    }
    o.val[0].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var1, o.val[1].type);
    return o;
}
uint8x128x2_t vld2oq_u8(uint8_t const *ptr) {
    uint8x128x2_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    for (int i = 0; i < 128; i++) {
        o_var0.ub[i] = ptr[i * 2 + 0];
        o_var1.ub[i] = ptr[i * 2 + 1];
    }
    o.val[0].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD2", (void const *)ptr, o_var1, o.val[1].type);
    return o;
}

uint8x8x4_t vld4_u8(uint8_t const *ptr) {
    uint8x8x4_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    __64bit_var o_var2;
    __64bit_var o_var3;
    for (int i = 0; i < 8; i++) {
        o_var0.ub[i] = ptr[i * 4 + 0];
        o_var1.ub[i] = ptr[i * 4 + 1];
        o_var2.ub[i] = ptr[i * 4 + 2];
        o_var3.ub[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}
uint8x16x4_t vld4q_u8(uint8_t const *ptr) {
    uint8x16x4_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    __128bit_var o_var2;
    __128bit_var o_var3;
    for (int i = 0; i < 16; i++) {
        o_var0.ub[i] = ptr[i * 4 + 0];
        o_var1.ub[i] = ptr[i * 4 + 1];
        o_var2.ub[i] = ptr[i * 4 + 2];
        o_var3.ub[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}
uint8x32x4_t vld4dq_u8(uint8_t const *ptr) {
    uint8x32x4_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    __256bit_var o_var2;
    __256bit_var o_var3;
    for (int i = 0; i < 32; i++) {
        o_var0.ub[i] = ptr[i * 4 + 0];
        o_var1.ub[i] = ptr[i * 4 + 1];
        o_var2.ub[i] = ptr[i * 4 + 2];
        o_var3.ub[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}
uint8x64x4_t vld4qq_u8(uint8_t const *ptr) {
    uint8x64x4_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    __512bit_var o_var2;
    __512bit_var o_var3;
    for (int i = 0; i < 64; i++) {
        o_var0.ub[i] = ptr[i * 4 + 0];
        o_var1.ub[i] = ptr[i * 4 + 1];
        o_var2.ub[i] = ptr[i * 4 + 2];
        o_var3.ub[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}
uint8x128x4_t vld4oq_u8(uint8_t const *ptr) {
    uint8x128x4_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    __1024bit_var o_var2;
    __1024bit_var o_var3;
    for (int i = 0; i < 128; i++) {
        o_var0.ub[i] = ptr[i * 4 + 0];
        o_var1.ub[i] = ptr[i * 4 + 1];
        o_var2.ub[i] = ptr[i * 4 + 2];
        o_var3.ub[i] = ptr[i * 4 + 3];
    }
    o.val[0].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var1, o.val[1].type);
    o.val[2].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var2, o.val[2].type);
    o.val[3].id = fake_neon::new_operation("LD4", (void const *)ptr, o_var3, o.val[3].type);
    return o;
}

int16x4_t vld1_lane_s16(int16_t const *ptr, int16x4_t src, const int lane) {
    int16x4_t o;
    __64bit_var o_var;
    __64bit_var src_var = fake_neon::get_value_64(src.id, src.type);
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = src_var.sh[i];
    }
    o_var.sh[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}
int16x8_t vld1q_lane_s16(int16_t const *ptr, int16x8_t src, const int lane) {
    int16x8_t o;
    __128bit_var o_var;
    __128bit_var src_var = fake_neon::get_value_128(src.id, src.type);
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = src_var.sh[i];
    }
    o_var.sh[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}
int16x16_t vld1dq_lane_s16(int16_t const *ptr, int16x16_t src, const int lane) {
    int16x16_t o;
    __256bit_var o_var;
    __256bit_var src_var = fake_neon::get_value_256(src.id, src.type);
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = src_var.sh[i];
    }
    o_var.sh[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}
int16x32_t vld1qq_lane_s16(int16_t const *ptr, int16x32_t src, const int lane) {
    int16x32_t o;
    __512bit_var o_var;
    __512bit_var src_var = fake_neon::get_value_512(src.id, src.type);
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = src_var.sh[i];
    }
    o_var.sh[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}
int16x64_t vld1oq_lane_s16(int16_t const *ptr, int16x64_t src, const int lane) {
    int16x64_t o;
    __1024bit_var o_var;
    __1024bit_var src_var = fake_neon::get_value_1024(src.id, src.type);
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = src_var.sh[i];
    }
    o_var.sh[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}

uint32x2_t vld1_lane_u32(uint32_t const *ptr, uint32x2_t src, const int lane) {
    uint32x2_t o;
    __64bit_var o_var;
    __64bit_var src_var = fake_neon::get_value_64(src.id, src.type);
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = src_var.us[i];
    }
    o_var.us[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}
uint32x4_t vld1q_lane_u32(uint32_t const *ptr, uint32x4_t src, const int lane) {
    uint32x4_t o;
    __128bit_var o_var;
    __128bit_var src_var = fake_neon::get_value_128(src.id, src.type);
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = src_var.us[i];
    }
    o_var.us[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}
uint32x8_t vld1dq_lane_u32(uint32_t const *ptr, uint32x8_t src, const int lane) {
    uint32x8_t o;
    __256bit_var o_var;
    __256bit_var src_var = fake_neon::get_value_256(src.id, src.type);
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = src_var.us[i];
    }
    o_var.us[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}
uint32x16_t vld1qq_lane_u32(uint32_t const *ptr, uint32x16_t src, const int lane) {
    uint32x16_t o;
    __512bit_var o_var;
    __512bit_var src_var = fake_neon::get_value_512(src.id, src.type);
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = src_var.us[i];
    }
    o_var.us[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}
uint32x32_t vld1oq_lane_u32(uint32_t const *ptr, uint32x32_t src, const int lane) {
    uint32x32_t o;
    __1024bit_var o_var;
    __1024bit_var src_var = fake_neon::get_value_1024(src.id, src.type);
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = src_var.us[i];
    }
    o_var.us[lane] = *ptr;
    o.id = fake_neon::new_operation("LD1", src.id, src.type, (void const *)ptr, o_var, o.type);
    return o;
}

// Store

void vst1_u8(uint8_t *ptr, uint8x8_t val) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    for (int i = 0; i < 8; i++) {
        *(ptr + i) = val_var.ub[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_u8(uint8_t *ptr, uint8x16_t val) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    for (int i = 0; i < 16; i++) {
        *(ptr + i) = val_var.ub[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_u8(uint8_t *ptr, uint8x32_t val) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    for (int i = 0; i < 32; i++) {
        *(ptr + i) = val_var.ub[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_u8(uint8_t *ptr, uint8x64_t val) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    for (int i = 0; i < 64; i++) {
        *(ptr + i) = val_var.ub[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_u8(uint8_t *ptr, uint8x128_t val) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    for (int i = 0; i < 128; i++) {
        *(ptr + i) = val_var.ub[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst4_u8(uint8_t *ptr, uint8x8x4_t val) {
    __64bit_var val0_var = fake_neon::get_value_64(val.val[0].id, val.val[0].type);
    __64bit_var val1_var = fake_neon::get_value_64(val.val[1].id, val.val[1].type);
    __64bit_var val2_var = fake_neon::get_value_64(val.val[2].id, val.val[2].type);
    __64bit_var val3_var = fake_neon::get_value_64(val.val[3].id, val.val[3].type);
    for (int i = 0; i < 8; i++) {
        *(ptr + i * 4 + 0) = val0_var.ub[i];
        *(ptr + i * 4 + 1) = val1_var.ub[i];
        *(ptr + i * 4 + 2) = val2_var.ub[i];
        *(ptr + i * 4 + 3) = val3_var.ub[i];
    }
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4q_u8(uint8_t *ptr, uint8x16x4_t val) {
    __128bit_var val0_var = fake_neon::get_value_128(val.val[0].id, val.val[0].type);
    __128bit_var val1_var = fake_neon::get_value_128(val.val[1].id, val.val[1].type);
    __128bit_var val2_var = fake_neon::get_value_128(val.val[2].id, val.val[2].type);
    __128bit_var val3_var = fake_neon::get_value_128(val.val[3].id, val.val[3].type);
    for (int i = 0; i < 16; i++) {
        *(ptr + i * 4 + 0) = val0_var.ub[i];
        *(ptr + i * 4 + 1) = val1_var.ub[i];
        *(ptr + i * 4 + 2) = val2_var.ub[i];
        *(ptr + i * 4 + 3) = val3_var.ub[i];
    }
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4dq_u8(uint8_t *ptr, uint8x32x4_t val) {
    __256bit_var val0_var = fake_neon::get_value_256(val.val[0].id, val.val[0].type);
    __256bit_var val1_var = fake_neon::get_value_256(val.val[1].id, val.val[1].type);
    __256bit_var val2_var = fake_neon::get_value_256(val.val[2].id, val.val[2].type);
    __256bit_var val3_var = fake_neon::get_value_256(val.val[3].id, val.val[3].type);
    for (int i = 0; i < 32; i++) {
        *(ptr + i * 4 + 0) = val0_var.ub[i];
        *(ptr + i * 4 + 1) = val1_var.ub[i];
        *(ptr + i * 4 + 2) = val2_var.ub[i];
        *(ptr + i * 4 + 3) = val3_var.ub[i];
    }
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4qq_u8(uint8_t *ptr, uint8x64x4_t val) {
    __512bit_var val0_var = fake_neon::get_value_512(val.val[0].id, val.val[0].type);
    __512bit_var val1_var = fake_neon::get_value_512(val.val[1].id, val.val[1].type);
    __512bit_var val2_var = fake_neon::get_value_512(val.val[2].id, val.val[2].type);
    __512bit_var val3_var = fake_neon::get_value_512(val.val[3].id, val.val[3].type);
    for (int i = 0; i < 64; i++) {
        *(ptr + i * 4 + 0) = val0_var.ub[i];
        *(ptr + i * 4 + 1) = val1_var.ub[i];
        *(ptr + i * 4 + 2) = val2_var.ub[i];
        *(ptr + i * 4 + 3) = val3_var.ub[i];
    }
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4oq_u8(uint8_t *ptr, uint8x128x4_t val) {
    __1024bit_var val0_var = fake_neon::get_value_1024(val.val[0].id, val.val[0].type);
    __1024bit_var val1_var = fake_neon::get_value_1024(val.val[1].id, val.val[1].type);
    __1024bit_var val2_var = fake_neon::get_value_1024(val.val[2].id, val.val[2].type);
    __1024bit_var val3_var = fake_neon::get_value_1024(val.val[3].id, val.val[3].type);
    for (int i = 0; i < 128; i++) {
        *(ptr + i * 4 + 0) = val0_var.ub[i];
        *(ptr + i * 4 + 1) = val1_var.ub[i];
        *(ptr + i * 4 + 2) = val2_var.ub[i];
        *(ptr + i * 4 + 3) = val3_var.ub[i];
    }
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}

void vst1_u16(uint16_t *ptr, uint16x4_t val) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    for (int i = 0; i < 4; i++) {
        *(ptr + i) = val_var.uh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_u16(uint16_t *ptr, uint16x8_t val) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    for (int i = 0; i < 8; i++) {
        *(ptr + i) = val_var.uh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_u16(uint16_t *ptr, uint16x16_t val) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    for (int i = 0; i < 16; i++) {
        *(ptr + i) = val_var.uh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_u16(uint16_t *ptr, uint16x32_t val) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    for (int i = 0; i < 32; i++) {
        *(ptr + i) = val_var.uh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_u16(uint16_t *ptr, uint16x64_t val) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    for (int i = 0; i < 64; i++) {
        *(ptr + i) = val_var.uh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst1_s16(int16_t *ptr, int16x4_t val) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    for (int i = 0; i < 4; i++) {
        *(ptr + i) = val_var.sh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_s16(int16_t *ptr, int16x8_t val) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    for (int i = 0; i < 8; i++) {
        *(ptr + i) = val_var.sh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_s16(int16_t *ptr, int16x16_t val) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    for (int i = 0; i < 16; i++) {
        *(ptr + i) = val_var.sh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_s16(int16_t *ptr, int16x32_t val) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    for (int i = 0; i < 32; i++) {
        *(ptr + i) = val_var.sh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_s16(int16_t *ptr, int16x64_t val) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    for (int i = 0; i < 64; i++) {
        *(ptr + i) = val_var.sh[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst1_u32(uint32_t *ptr, uint32x2_t val) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    for (int i = 0; i < 2; i++) {
        *(ptr + i) = val_var.us[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_u32(uint32_t *ptr, uint32x4_t val) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    for (int i = 0; i < 4; i++) {
        *(ptr + i) = val_var.us[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_u32(uint32_t *ptr, uint32x8_t val) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    for (int i = 0; i < 8; i++) {
        *(ptr + i) = val_var.us[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_u32(uint32_t *ptr, uint32x16_t val) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    for (int i = 0; i < 16; i++) {
        *(ptr + i) = val_var.us[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_u32(uint32_t *ptr, uint32x32_t val) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    for (int i = 0; i < 32; i++) {
        *(ptr + i) = val_var.us[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst1_s32(int32_t *ptr, int32x2_t val) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    for (int i = 0; i < 2; i++) {
        *(ptr + i) = val_var.ss[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_s32(int32_t *ptr, int32x4_t val) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    for (int i = 0; i < 4; i++) {
        *(ptr + i) = val_var.ss[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_s32(int32_t *ptr, int32x8_t val) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    for (int i = 0; i < 8; i++) {
        *(ptr + i) = val_var.ss[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_s32(int32_t *ptr, int32x16_t val) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    for (int i = 0; i < 16; i++) {
        *(ptr + i) = val_var.ss[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_s32(int32_t *ptr, int32x32_t val) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    for (int i = 0; i < 32; i++) {
        *(ptr + i) = val_var.ss[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst1_f32(float *ptr, float32x2_t val) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    for (int i = 0; i < 2; i++) {
        *(ptr + i) = val_var.fs[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_f32(float *ptr, float32x4_t val) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    for (int i = 0; i < 4; i++) {
        *(ptr + i) = val_var.fs[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_f32(float *ptr, float32x8_t val) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    for (int i = 0; i < 8; i++) {
        *(ptr + i) = val_var.fs[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_f32(float *ptr, float32x16_t val) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    for (int i = 0; i < 16; i++) {
        *(ptr + i) = val_var.fs[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_f32(float *ptr, float32x32_t val) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    for (int i = 0; i < 32; i++) {
        *(ptr + i) = val_var.fs[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst1_s64(int64_t *ptr, int64x1_t val) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    for (int i = 0; i < 1; i++) {
        *(ptr + i) = val_var.sd[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_s64(int64_t *ptr, int64x2_t val) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    for (int i = 0; i < 2; i++) {
        *(ptr + i) = val_var.sd[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_s64(int64_t *ptr, int64x4_t val) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    for (int i = 0; i < 4; i++) {
        *(ptr + i) = val_var.sd[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_s64(int64_t *ptr, int64x8_t val) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    for (int i = 0; i < 8; i++) {
        *(ptr + i) = val_var.sd[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_s64(int64_t *ptr, int64x16_t val) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    for (int i = 0; i < 16; i++) {
        *(ptr + i) = val_var.sd[i];
    }
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst1_lane_s16(int16_t *ptr, int16x4_t val, const int lane) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    *(ptr) = val_var.sh[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_lane_s16(int16_t *ptr, int16x8_t val, const int lane) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    *(ptr) = val_var.sh[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_lane_s16(int16_t *ptr, int16x16_t val, const int lane) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    *(ptr) = val_var.sh[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_lane_s16(int16_t *ptr, int16x32_t val, const int lane) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    *(ptr) = val_var.sh[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_lane_s16(int16_t *ptr, int16x64_t val, const int lane) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    *(ptr) = val_var.sh[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst1_lane_u32(uint32_t *ptr, uint32x2_t val, const int lane) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    *(ptr) = val_var.us[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_lane_u32(uint32_t *ptr, uint32x4_t val, const int lane) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    *(ptr) = val_var.us[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_lane_u32(uint32_t *ptr, uint32x8_t val, const int lane) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    *(ptr) = val_var.us[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_lane_u32(uint32_t *ptr, uint32x16_t val, const int lane) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    *(ptr) = val_var.us[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_lane_u32(uint32_t *ptr, uint32x32_t val, const int lane) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    *(ptr) = val_var.us[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst1_lane_s32(int32_t *ptr, int32x2_t val, const int lane) {
    __64bit_var val_var = fake_neon::get_value_64(val.id, val.type);
    *(ptr) = val_var.ss[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1q_lane_s32(int32_t *ptr, int32x4_t val, const int lane) {
    __128bit_var val_var = fake_neon::get_value_128(val.id, val.type);
    *(ptr) = val_var.ss[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1dq_lane_s32(int32_t *ptr, int32x8_t val, const int lane) {
    __256bit_var val_var = fake_neon::get_value_256(val.id, val.type);
    *(ptr) = val_var.ss[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1qq_lane_s32(int32_t *ptr, int32x16_t val, const int lane) {
    __512bit_var val_var = fake_neon::get_value_512(val.id, val.type);
    *(ptr) = val_var.ss[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}
void vst1oq_lane_s32(int32_t *ptr, int32x32_t val, const int lane) {
    __1024bit_var val_var = fake_neon::get_value_1024(val.id, val.type);
    *(ptr) = val_var.ss[lane];
    fake_neon::new_operation("ST1", val.id, val.type, (void const *)ptr);
}

void vst4_lane_u8(uint8_t *ptr, uint8x8x4_t val, const int lane) {
    __64bit_var val0_var = fake_neon::get_value_64(val.val[0].id, val.val[0].type);
    __64bit_var val1_var = fake_neon::get_value_64(val.val[1].id, val.val[1].type);
    __64bit_var val2_var = fake_neon::get_value_64(val.val[2].id, val.val[2].type);
    __64bit_var val3_var = fake_neon::get_value_64(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4q_lane_u8(uint8_t *ptr, uint8x16x4_t val, const int lane) {
    __128bit_var val0_var = fake_neon::get_value_128(val.val[0].id, val.val[0].type);
    __128bit_var val1_var = fake_neon::get_value_128(val.val[1].id, val.val[1].type);
    __128bit_var val2_var = fake_neon::get_value_128(val.val[2].id, val.val[2].type);
    __128bit_var val3_var = fake_neon::get_value_128(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4dq_lane_u8(uint8_t *ptr, uint8x32x4_t val, const int lane) {
    __256bit_var val0_var = fake_neon::get_value_256(val.val[0].id, val.val[0].type);
    __256bit_var val1_var = fake_neon::get_value_256(val.val[1].id, val.val[1].type);
    __256bit_var val2_var = fake_neon::get_value_256(val.val[2].id, val.val[2].type);
    __256bit_var val3_var = fake_neon::get_value_256(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4qq_lane_u8(uint8_t *ptr, uint8x64x4_t val, const int lane) {
    __512bit_var val0_var = fake_neon::get_value_512(val.val[0].id, val.val[0].type);
    __512bit_var val1_var = fake_neon::get_value_512(val.val[1].id, val.val[1].type);
    __512bit_var val2_var = fake_neon::get_value_512(val.val[2].id, val.val[2].type);
    __512bit_var val3_var = fake_neon::get_value_512(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4oq_lane_u8(uint8_t *ptr, uint8x128x4_t val, const int lane) {
    __1024bit_var val0_var = fake_neon::get_value_1024(val.val[0].id, val.val[0].type);
    __1024bit_var val1_var = fake_neon::get_value_1024(val.val[1].id, val.val[1].type);
    __1024bit_var val2_var = fake_neon::get_value_1024(val.val[2].id, val.val[2].type);
    __1024bit_var val3_var = fake_neon::get_value_1024(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.ub[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}

void vst4_lane_u32(uint32_t *ptr, uint32x2x4_t val, const int lane) {
    __64bit_var val0_var = fake_neon::get_value_64(val.val[0].id, val.val[0].type);
    __64bit_var val1_var = fake_neon::get_value_64(val.val[1].id, val.val[1].type);
    __64bit_var val2_var = fake_neon::get_value_64(val.val[2].id, val.val[2].type);
    __64bit_var val3_var = fake_neon::get_value_64(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.us[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.us[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.us[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.us[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4q_lane_u32(uint32_t *ptr, uint32x4x4_t val, const int lane) {
    __128bit_var val0_var = fake_neon::get_value_128(val.val[0].id, val.val[0].type);
    __128bit_var val1_var = fake_neon::get_value_128(val.val[1].id, val.val[1].type);
    __128bit_var val2_var = fake_neon::get_value_128(val.val[2].id, val.val[2].type);
    __128bit_var val3_var = fake_neon::get_value_128(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.us[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.us[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.us[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.us[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4dq_lane_u32(uint32_t *ptr, uint32x8x4_t val, const int lane) {
    __256bit_var val0_var = fake_neon::get_value_256(val.val[0].id, val.val[0].type);
    __256bit_var val1_var = fake_neon::get_value_256(val.val[1].id, val.val[1].type);
    __256bit_var val2_var = fake_neon::get_value_256(val.val[2].id, val.val[2].type);
    __256bit_var val3_var = fake_neon::get_value_256(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.us[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.us[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.us[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.us[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4qq_lane_u32(uint32_t *ptr, uint32x16x4_t val, const int lane) {
    __512bit_var val0_var = fake_neon::get_value_512(val.val[0].id, val.val[0].type);
    __512bit_var val1_var = fake_neon::get_value_512(val.val[1].id, val.val[1].type);
    __512bit_var val2_var = fake_neon::get_value_512(val.val[2].id, val.val[2].type);
    __512bit_var val3_var = fake_neon::get_value_512(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.us[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.us[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.us[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.us[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}
void vst4oq_lane_u32(uint32_t *ptr, uint32x32x4_t val, const int lane) {
    __1024bit_var val0_var = fake_neon::get_value_1024(val.val[0].id, val.val[0].type);
    __1024bit_var val1_var = fake_neon::get_value_1024(val.val[1].id, val.val[1].type);
    __1024bit_var val2_var = fake_neon::get_value_1024(val.val[2].id, val.val[2].type);
    __1024bit_var val3_var = fake_neon::get_value_1024(val.val[3].id, val.val[3].type);
    *(ptr + 0) = val0_var.us[lane];
    fake_neon::new_operation("ST4", val.val[0].id, val.val[0].type, (void const *)ptr);
    *(ptr + 1) = val1_var.us[lane];
    fake_neon::new_operation("ST4", val.val[1].id, val.val[1].type, (void const *)ptr);
    *(ptr + 2) = val2_var.us[lane];
    fake_neon::new_operation("ST4", val.val[2].id, val.val[2].type, (void const *)ptr);
    *(ptr + 3) = val3_var.us[lane];
    fake_neon::new_operation("ST4", val.val[3].id, val.val[3].type, (void const *)ptr);
}

// Duplicate
uint8x8_t vdup_n_u8(uint8_t value) {
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint8x16_t vdupq_n_u8(uint8_t value) {
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint8x32_t vdupdq_n_u8(uint8_t value) {
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint8x64_t vdupqq_n_u8(uint8_t value) {
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint8x128_t vdupoq_n_u8(uint8_t value) {
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}

uint16x4_t vdup_n_u16(uint16_t value) {
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint16x8_t vdupq_n_u16(uint16_t value) {
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint16x16_t vdupdq_n_u16(uint16_t value) {
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint16x32_t vdupqq_n_u16(uint16_t value) {
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint16x64_t vdupoq_n_u16(uint16_t value) {
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}

int16x4_t vdup_n_s16(int16_t value) {
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int16x8_t vdupq_n_s16(int16_t value) {
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int16x16_t vdupdq_n_s16(int16_t value) {
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int16x32_t vdupqq_n_s16(int16_t value) {
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int16x64_t vdupoq_n_s16(int16_t value) {
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}

uint32x2_t vdup_n_u32(uint32_t value) {
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint32x4_t vdupq_n_u32(uint32_t value) {
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint32x8_t vdupdq_n_u32(uint32_t value) {
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint32x16_t vdupqq_n_u32(uint32_t value) {
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
uint32x32_t vdupoq_n_u32(uint32_t value) {
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}

int32x2_t vdup_n_s32(int32_t value) {
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int32x4_t vdupq_n_s32(int32_t value) {
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int32x8_t vdupdq_n_s32(int32_t value) {
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int32x16_t vdupqq_n_s32(int32_t value) {
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int32x32_t vdupoq_n_s32(int32_t value) {
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}

float32x2_t vdup_n_f32(float value) {
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
float32x4_t vdupq_n_f32(float value) {
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
float32x8_t vdupdq_n_f32(float value) {
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
float32x16_t vdupqq_n_f32(float value) {
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
float32x32_t vdupoq_n_f32(float value) {
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}

int64x1_t vdup_n_s64(int64_t value) {
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int64x2_t vdupq_n_s64(int64_t value) {
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int64x4_t vdupdq_n_s64(int64_t value) {
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int64x8_t vdupqq_n_s64(int64_t value) {
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}
int64x16_t vdupoq_n_s64(int64_t value) {
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = value;
    }
    o.id = fake_neon::new_operation("DUP", o_var, o.type);
    return o;
}

uint8x8_t vdup_lane_u8(uint8x8_t vec, const int lane) {
    uint8x8_t o;
    __64bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
uint8x16_t vdupq_lane_u8(uint8x8_t vec, const int lane) {
    uint8x16_t o;
    __128bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
uint8x32_t vdupdq_lane_u8(uint8x8_t vec, const int lane) {
    uint8x32_t o;
    __256bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
uint8x64_t vdupqq_lane_u8(uint8x8_t vec, const int lane) {
    uint8x64_t o;
    __512bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
uint8x128_t vdupoq_lane_u8(uint8x8_t vec, const int lane) {
    uint8x128_t o;
    __1024bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
uint8x16_t vdupq_laneq_u8(uint8x16_t vec, const int lane) {
    uint8x16_t o;
    __128bit_var o_var;
    __128bit_var vec_var = fake_neon::get_value_128(vec.id, vec.type);
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
uint8x32_t vdupdq_lanedq_u8(uint8x32_t vec, const int lane) {
    uint8x32_t o;
    __256bit_var o_var;
    __256bit_var vec_var = fake_neon::get_value_256(vec.id, vec.type);
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
uint8x64_t vdupqq_laneqq_u8(uint8x64_t vec, const int lane) {
    uint8x64_t o;
    __512bit_var o_var;
    __512bit_var vec_var = fake_neon::get_value_512(vec.id, vec.type);
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
uint8x128_t vdupoq_laneoq_u8(uint8x128_t vec, const int lane) {
    uint8x128_t o;
    __1024bit_var o_var;
    __1024bit_var vec_var = fake_neon::get_value_1024(vec.id, vec.type);
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = vec_var.ub[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}

int16x4_t vdup_lane_s16(int16x4_t vec, const int lane) {
    int16x4_t o;
    __64bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
int16x8_t vdupq_lane_s16(int16x4_t vec, const int lane) {
    int16x8_t o;
    __128bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
int16x16_t vdupdq_lane_s16(int16x4_t vec, const int lane) {
    int16x16_t o;
    __256bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
int16x32_t vdupqq_lane_s16(int16x4_t vec, const int lane) {
    int16x32_t o;
    __512bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
int16x64_t vdupoq_lane_s16(int16x4_t vec, const int lane) {
    int16x64_t o;
    __1024bit_var o_var;
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
int16x8_t vdupq_laneq_s16(int16x8_t vec, const int lane) {
    int16x8_t o;
    __128bit_var o_var;
    __128bit_var vec_var = fake_neon::get_value_128(vec.id, vec.type);
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
int16x16_t vdupdq_lanedq_s16(int16x16_t vec, const int lane) {
    int16x16_t o;
    __256bit_var o_var;
    __256bit_var vec_var = fake_neon::get_value_256(vec.id, vec.type);
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
int16x32_t vdupqq_laneqq_s16(int16x32_t vec, const int lane) {
    int16x32_t o;
    __512bit_var o_var;
    __512bit_var vec_var = fake_neon::get_value_512(vec.id, vec.type);
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}
int16x64_t vdupoq_laneoq_s16(int16x64_t vec, const int lane) {
    int16x64_t o;
    __1024bit_var o_var;
    __1024bit_var vec_var = fake_neon::get_value_1024(vec.id, vec.type);
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = vec_var.sh[lane];
    }
    o.id = fake_neon::new_operation("DUP", vec.id, vec.type, o_var, o.type);
    return o;
}

// Load / Duplicate
uint8x8_t vld1_dup_u8(uint8_t const *ptr) {
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint8x16_t vld1q_dup_u8(uint8_t const *ptr) {
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint8x32_t vld1dq_dup_u8(uint8_t const *ptr) {
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint8x64_t vld1qq_dup_u8(uint8_t const *ptr) {
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint8x128_t vld1oq_dup_u8(uint8_t const *ptr) {
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}

int8x8_t vld1_dup_s8(int8_t const *ptr) {
    int8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sb[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int8x16_t vld1q_dup_s8(int8_t const *ptr) {
    int8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sb[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int8x32_t vld1dq_dup_s8(int8_t const *ptr) {
    int8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sb[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int8x64_t vld1qq_dup_s8(int8_t const *ptr) {
    int8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sb[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int8x128_t vld1oq_dup_s8(int8_t const *ptr) {
    int8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.sb[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}

uint16x4_t vld1_dup_u16(uint16_t const *ptr) {
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint16x8_t vld1q_dup_u16(uint16_t const *ptr) {
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint16x16_t vld1dq_dup_u16(uint16_t const *ptr) {
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint16x32_t vld1qq_dup_u16(uint16_t const *ptr) {
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint16x64_t vld1oq_dup_u16(uint16_t const *ptr) {
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}

int16x4_t vld1_dup_s16(int16_t const *ptr) {
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int16x8_t vld1q_dup_s16(int16_t const *ptr) {
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int16x16_t vld1dq_dup_s16(int16_t const *ptr) {
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int16x32_t vld1qq_dup_s16(int16_t const *ptr) {
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int16x64_t vld1oq_dup_s16(int16_t const *ptr) {
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}

uint32x2_t vld1_dup_u32(uint32_t const *ptr) {
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint32x4_t vld1q_dup_u32(uint32_t const *ptr) {
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint32x8_t vld1dq_dup_u32(uint32_t const *ptr) {
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint32x16_t vld1qq_dup_u32(uint32_t const *ptr) {
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
uint32x32_t vld1oq_dup_u32(uint32_t const *ptr) {
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}

int32x2_t vld1_dup_s32(int32_t const *ptr) {
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int32x4_t vld1q_dup_s32(int32_t const *ptr) {
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int32x8_t vld1dq_dup_s32(int32_t const *ptr) {
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int32x16_t vld1qq_dup_s32(int32_t const *ptr) {
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
int32x32_t vld1oq_dup_s32(int32_t const *ptr) {
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}

float32x2_t vld1_dup_f32(float const *ptr) {
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
float32x4_t vld1q_dup_f32(float const *ptr) {
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
float32x8_t vld1dq_dup_f32(float const *ptr) {
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
float32x16_t vld1qq_dup_f32(float const *ptr) {
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}
float32x32_t vld1oq_dup_f32(float const *ptr) {
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = *ptr;
    }
    o.id = fake_neon::new_operation("LD1R", (void const *)ptr, o_var, o.type);
    return o;
}

// Move
uint16x8_t vmovl_u8(uint8x8_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("USHLL", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vmovldq_u8(uint8x16_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("USHLL", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vmovlqq_u8(uint8x32_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("USHLL", a.id, a.type, o_var, o.type);
    return o;
}
uint16x64_t vmovloq_u8(uint8x64_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("USHLL", a.id, a.type, o_var, o.type);
    return o;
}

uint8x8_t vmovn_u16(uint16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x16_t vmovndq_u16(uint16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x32_t vmovnqq_u16(uint16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x64_t vmovnoq_u16(uint16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vmovn_s32(int32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vmovnq_s32(int32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vmovndq_s32(int32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vmovnqq_s32(int32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vmovn_s64(int64x2_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vmovnq_s64(int64x4_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vmovndq_s64(int64x8_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vmovnqq_s64(int64x16_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("XTN", a.id, a.type, o_var, o.type);
    return o;
}

uint8x8_t vqmovun_s16(int16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = (a_var.sh[i] > 255) ? 255 : (a_var.sh[i] < 0) ? 0
                                                                    : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SQXTUN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x16_t vqmovundq_s16(int16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = (a_var.sh[i] > 255) ? 255 : (a_var.sh[i] < 0) ? 0
                                                                    : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SQXTUN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x32_t vqmovunqq_s16(int16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = (a_var.sh[i] > 255) ? 255 : (a_var.sh[i] < 0) ? 0
                                                                    : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SQXTUN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x64_t vqmovunoq_s16(int16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = (a_var.sh[i] > 255) ? 255 : (a_var.sh[i] < 0) ? 0
                                                                    : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SQXTUN", a.id, a.type, o_var, o.type);
    return o;
}

// Get Low
uint8x8_t vget_low_u8(uint8x16_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x16_t vgetdq_low_u8(uint8x32_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x32_t vgetqq_low_u8(uint8x64_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x64_t vgetoq_low_u8(uint8x128_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

uint16x4_t vget_low_u16(uint16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vgetdq_low_u16(uint16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vgetqq_low_u16(uint16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vgetoq_low_u16(uint16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vget_low_s16(int16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vgetdq_low_s16(int16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vgetqq_low_s16(int16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vgetoq_low_s16(int16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

uint32x2_t vget_low_u32(uint32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x4_t vgetdq_low_u32(uint32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x8_t vgetqq_low_u32(uint32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x16_t vgetoq_low_u32(uint32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vget_low_s32(int32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vgetdq_low_s32(int32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vgetqq_low_s32(int32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vgetoq_low_s32(int32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

float32x2_t vget_low_f32(float32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
float32x4_t vgetdq_low_f32(float32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
float32x8_t vgetqq_low_f32(float32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
float32x16_t vgetoq_low_f32(float32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

int64x1_t vget_low_s64(int64x2_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int64x2_t vgetdq_low_s64(int64x4_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int64x4_t vgetqq_low_s64(int64x8_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int64x8_t vgetoq_low_s64(int64x16_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

// Get High
uint8x8_t vget_high_u8(uint8x16_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = a_var.ub[8 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x16_t vgetdq_high_u8(uint8x32_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = a_var.ub[16 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x32_t vgetqq_high_u8(uint8x64_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = a_var.ub[32 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x64_t vgetoq_high_u8(uint8x128_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = a_var.ub[64 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

uint16x4_t vget_high_u16(uint16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[4 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vgetdq_high_u16(uint16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[8 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vgetqq_high_u16(uint16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[16 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vgetoq_high_u16(uint16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[32 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vget_high_s16(int16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[4 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vgetdq_high_s16(int16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[8 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vgetqq_high_s16(int16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[16 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vgetoq_high_s16(int16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[32 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

uint32x2_t vget_high_u32(uint32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[2 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x4_t vgetdq_high_u32(uint32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[4 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x8_t vgetqq_high_u32(uint32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[8 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x16_t vgetoq_high_u32(uint32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[16 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vget_high_s32(int32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[2 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vgetdq_high_s32(int32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[4 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vgetqq_high_s32(int32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[8 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vgetoq_high_s32(int32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[16 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

float32x2_t vget_high_f32(float32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[2 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
float32x4_t vgetdq_high_f32(float32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[4 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
float32x8_t vgetqq_high_f32(float32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[8 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
float32x16_t vgetoq_high_f32(float32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[16 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

int64x1_t vget_high_s64(int64x2_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = a_var.sd[1 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int64x2_t vgetdq_high_s64(int64x4_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = a_var.sd[2 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int64x4_t vgetqq_high_s64(int64x8_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = a_var.sd[4 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}
int64x8_t vgetoq_high_s64(int64x16_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = a_var.sd[8 + i];
    }
    o.id = fake_neon::new_operation("DUP", a.id, a.type, o_var, o.type);
    return o;
}

// Get Lane
uint32_t vget_lane_u32(uint32x2_t v, const int lane) {
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.us[lane];
}
uint32_t vgetq_lane_u32(uint32x4_t v, const int lane) {
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.us[lane];
}
uint32_t vgetdq_lane_u32(uint32x8_t v, const int lane) {
    __256bit_var v_var = fake_neon::get_value_256(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.us[lane];
}
uint32_t vgetqq_lane_u32(uint32x16_t v, const int lane) {
    __512bit_var v_var = fake_neon::get_value_512(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.us[lane];
}
uint32_t vgetoq_lane_u32(uint32x32_t v, const int lane) {
    __1024bit_var v_var = fake_neon::get_value_1024(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.us[lane];
}

int32_t vget_lane_s32(int32x2_t v, const int lane) {
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.ss[lane];
}
int32_t vgetq_lane_s32(int32x4_t v, const int lane) {
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.ss[lane];
}
int32_t vgetdq_lane_s32(int32x8_t v, const int lane) {
    __256bit_var v_var = fake_neon::get_value_256(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.ss[lane];
}
int32_t vgetqq_lane_s32(int32x16_t v, const int lane) {
    __512bit_var v_var = fake_neon::get_value_512(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.ss[lane];
}
int32_t vgetoq_lane_s32(int32x32_t v, const int lane) {
    __1024bit_var v_var = fake_neon::get_value_1024(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.ss[lane];
}

float vget_lane_f32(float32x2_t v, const int lane) {
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.fs[lane];
}
float vgetq_lane_f32(float32x4_t v, const int lane) {
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.fs[lane];
}
float vgetdq_lane_f32(float32x8_t v, const int lane) {
    __256bit_var v_var = fake_neon::get_value_256(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.fs[lane];
}
float vgetqq_lane_f32(float32x16_t v, const int lane) {
    __512bit_var v_var = fake_neon::get_value_512(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.fs[lane];
}
float vgetoq_lane_f32(float32x32_t v, const int lane) {
    __1024bit_var v_var = fake_neon::get_value_1024(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.fs[lane];
}

int64_t vget_lane_s64(int64x1_t v, const int lane) {
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.sd[lane];
}
int64_t vgetq_lane_s64(int64x2_t v, const int lane) {
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.sd[lane];
}
int64_t vgetdq_lane_s64(int64x4_t v, const int lane) {
    __256bit_var v_var = fake_neon::get_value_256(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.sd[lane];
}
int64_t vgetqq_lane_s64(int64x8_t v, const int lane) {
    __512bit_var v_var = fake_neon::get_value_512(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.sd[lane];
}
int64_t vgetoq_lane_s64(int64x16_t v, const int lane) {
    __1024bit_var v_var = fake_neon::get_value_1024(v.id, v.type);
    fake_neon::new_operation("DUP", v.id, v.type);
    return v_var.sd[lane];
}

// Multiply Accumulate
uint32x4_t vmlal_u16(uint32x4_t a, uint16x4_t b, uint16x4_t c) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __64bit_var c_var = fake_neon::get_value_64(c.id, c.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * c_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint32x8_t vmlaldq_u16(uint32x8_t a, uint16x8_t b, uint16x8_t c) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __128bit_var c_var = fake_neon::get_value_128(c.id, c.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * c_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint32x16_t vmlalqq_u16(uint32x16_t a, uint16x16_t b, uint16x16_t c) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __256bit_var c_var = fake_neon::get_value_256(c.id, c.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * c_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint32x32_t vmlaloq_u16(uint32x32_t a, uint16x32_t b, uint16x32_t c) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __512bit_var c_var = fake_neon::get_value_512(c.id, c.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * c_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}

int32x4_t vmlal_s16(int32x4_t a, int16x4_t b, int16x4_t c) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __64bit_var c_var = fake_neon::get_value_64(c.id, c.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] + ((int32_t)(b_var.sh[i]) * (int32_t)(c_var.sh[i]));
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
int32x8_t vmlaldq_s16(int32x8_t a, int16x8_t b, int16x8_t c) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __128bit_var c_var = fake_neon::get_value_128(c.id, c.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] + ((int32_t)(b_var.sh[i]) * (int32_t)(c_var.sh[i]));
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
int32x16_t vmlalqq_s16(int32x16_t a, int16x16_t b, int16x16_t c) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __256bit_var c_var = fake_neon::get_value_256(c.id, c.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] + ((int32_t)(b_var.sh[i]) * (int32_t)(c_var.sh[i]));
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
int32x32_t vmlaloq_s16(int32x32_t a, int16x32_t b, int16x32_t c) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __512bit_var c_var = fake_neon::get_value_512(c.id, c.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] + ((int32_t)(b_var.sh[i]) * (int32_t)(c_var.sh[i]));
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}

float32x2_t vmla_f32(float32x2_t a, float32x2_t b, float32x2_t c) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __64bit_var c_var = fake_neon::get_value_64(c.id, c.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i] * c_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMLA", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
float32x4_t vmlaq_f32(float32x4_t a, float32x4_t b, float32x4_t c) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __128bit_var c_var = fake_neon::get_value_128(c.id, c.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i] * c_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMLA", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
float32x8_t vmladq_f32(float32x8_t a, float32x8_t b, float32x8_t c) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __256bit_var c_var = fake_neon::get_value_256(c.id, c.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i] * c_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMLA", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
float32x16_t vmlaqq_f32(float32x16_t a, float32x16_t b, float32x16_t c) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __512bit_var c_var = fake_neon::get_value_512(c.id, c.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i] * c_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMLA", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
float32x32_t vmlaoq_f32(float32x32_t a, float32x32_t b, float32x32_t c) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    __1024bit_var c_var = fake_neon::get_value_1024(c.id, c.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i] * c_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMLA", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}

uint32x4_t vmlal_n_u16(uint32x4_t a, uint16x4_t b, uint16_t c) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * c;
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vmlaldq_n_u16(uint32x8_t a, uint16x8_t b, uint16_t c) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * c;
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vmlalqq_n_u16(uint32x16_t a, uint16x16_t b, uint16_t c) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * c;
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vmlaloq_n_u16(uint32x32_t a, uint16x32_t b, uint16_t c) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * c;
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x4_t vmlal_n_s16(int32x4_t a, int16x4_t b, int16_t c) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[i] * c;
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vmlaldq_n_s16(int32x8_t a, int16x8_t b, int16_t c) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[i] * c;
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vmlalqq_n_s16(int32x16_t a, int16x16_t b, int16_t c) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[i] * c;
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vmlaloq_n_s16(int32x32_t a, int16x32_t b, int16_t c) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[i] * c;
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x4_t vmlal_laneq_u16(uint32x4_t a, uint16x4_t b, uint16x8_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * v_var.uh[lane];
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x8_t vmlaldq_laneq_u16(uint32x8_t a, uint16x8_t b, uint16x8_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * v_var.uh[lane];
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x16_t vmlalqq_laneq_u16(uint32x16_t a, uint16x16_t b, uint16x8_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * v_var.uh[lane];
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x32_t vmlaloq_laneq_u16(uint32x32_t a, uint16x32_t b, uint16x8_t v, const int lane) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[i] * v_var.uh[lane];
    }
    o.id = fake_neon::new_operation("UMLAL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}

int32x4_t vmlal_lane_s16(int32x4_t a, int16x4_t b, int16x4_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[i] * v_var.sh[lane];
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x8_t vmlaldq_lane_s16(int32x8_t a, int16x8_t b, int16x4_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[i] * v_var.sh[lane];
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x16_t vmlalqq_lane_s16(int32x16_t a, int16x16_t b, int16x4_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[i] * v_var.sh[lane];
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x32_t vmlaloq_lane_s16(int32x32_t a, int16x32_t b, int16x4_t v, const int lane) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[i] * v_var.sh[lane];
    }
    o.id = fake_neon::new_operation("SMLAL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}

float32x2_t vmla_lane_f32(float32x2_t a, float32x2_t b, float32x2_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    float32x2_t o1, o2;
    __64bit_var o1_var, o2_var;
    for (int i = 0; i < 2; i++) {
        o1_var.fs[i] = b_var.fs[i] * v_var.fs[lane];
        o2_var.fs[i] = a_var.fs[i] + o1_var.fs[i];
    }
    o1.id = fake_neon::new_operation("FMUL", b.id, b.type, v.id, v.type, o1_var, o1.type);
    o2.id = fake_neon::new_operation("FADD", a.id, a.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
float32x4_t vmlaq_lane_f32(float32x4_t a, float32x4_t b, float32x2_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    float32x4_t o1, o2;
    __128bit_var o1_var, o2_var;
    for (int i = 0; i < 4; i++) {
        o1_var.fs[i] = b_var.fs[i] * v_var.fs[lane];
        o2_var.fs[i] = a_var.fs[i] + o1_var.fs[i];
    }
    o1.id = fake_neon::new_operation("FMUL", b.id, b.type, v.id, v.type, o1_var, o1.type);
    o2.id = fake_neon::new_operation("FADD", a.id, a.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
float32x8_t vmladq_lane_f32(float32x8_t a, float32x8_t b, float32x2_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    float32x8_t o1, o2;
    __256bit_var o1_var, o2_var;
    for (int i = 0; i < 8; i++) {
        o1_var.fs[i] = b_var.fs[i] * v_var.fs[lane];
        o2_var.fs[i] = a_var.fs[i] + o1_var.fs[i];
    }
    o1.id = fake_neon::new_operation("FMUL", b.id, b.type, v.id, v.type, o1_var, o1.type);
    o2.id = fake_neon::new_operation("FADD", a.id, a.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
float32x16_t vmlaqq_lane_f32(float32x16_t a, float32x16_t b, float32x2_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    float32x16_t o1, o2;
    __512bit_var o1_var, o2_var;
    for (int i = 0; i < 16; i++) {
        o1_var.fs[i] = b_var.fs[i] * v_var.fs[lane];
        o2_var.fs[i] = a_var.fs[i] + o1_var.fs[i];
    }
    o1.id = fake_neon::new_operation("FMUL", b.id, b.type, v.id, v.type, o1_var, o1.type);
    o2.id = fake_neon::new_operation("FADD", a.id, a.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
float32x32_t vmlaoq_lane_f32(float32x32_t a, float32x32_t b, float32x2_t v, const int lane) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    float32x32_t o1, o2;
    __1024bit_var o1_var, o2_var;
    for (int i = 0; i < 32; i++) {
        o1_var.fs[i] = b_var.fs[i] * v_var.fs[lane];
        o2_var.fs[i] = a_var.fs[i] + o1_var.fs[i];
    }
    o1.id = fake_neon::new_operation("FMUL", b.id, b.type, v.id, v.type, o1_var, o1.type);
    o2.id = fake_neon::new_operation("FADD", a.id, a.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}

int32x4_t vmlsl_n_s16(int32x4_t a, int16x4_t b, int16_t c) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.sh[i] * c;
    }
    o.id = fake_neon::new_operation("SMLSL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vmlsldq_n_s16(int32x8_t a, int16x8_t b, int16_t c) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.sh[i] * c;
    }
    o.id = fake_neon::new_operation("SMLSL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vmlslqq_n_s16(int32x16_t a, int16x16_t b, int16_t c) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.sh[i] * c;
    }
    o.id = fake_neon::new_operation("SMLSL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vmlsloq_n_s16(int32x32_t a, int16x32_t b, int16_t c) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.sh[i] * c;
    }
    o.id = fake_neon::new_operation("SMLSL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x4_t vmlsl_laneq_u16(uint32x4_t a, uint16x4_t b, uint16x8_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] - b_var.uh[i] * v_var.uh[lane];
    }
    o.id = fake_neon::new_operation("UMLSL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x8_t vmlsldq_laneq_u16(uint32x8_t a, uint16x8_t b, uint16x8_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] - b_var.uh[i] * v_var.uh[lane];
    }
    o.id = fake_neon::new_operation("UMLSL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x16_t vmlslqq_laneq_u16(uint32x16_t a, uint16x16_t b, uint16x8_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] - b_var.uh[i] * v_var.uh[lane];
    }
    o.id = fake_neon::new_operation("UMLSL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x32_t vmlsloq_laneq_u16(uint32x32_t a, uint16x32_t b, uint16x8_t v, const int lane) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] - b_var.uh[i] * v_var.uh[lane];
    }
    o.id = fake_neon::new_operation("UMLSL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}

int32x4_t vmlsl_lane_s16(int32x4_t a, int16x4_t b, int16x4_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.sh[i] * v_var.sh[lane];
    }
    o.id = fake_neon::new_operation("SMLSL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x8_t vmlsldq_lane_s16(int32x8_t a, int16x8_t b, int16x4_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.sh[i] * v_var.sh[lane];
    }
    o.id = fake_neon::new_operation("SMLSL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x16_t vmlslqq_lane_s16(int32x16_t a, int16x16_t b, int16x4_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.sh[i] * v_var.sh[lane];
    }
    o.id = fake_neon::new_operation("SMLSL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x32_t vmlsloq_lane_s16(int32x32_t a, int16x32_t b, int16x4_t v, const int lane) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.sh[i] * v_var.sh[lane];
    }
    o.id = fake_neon::new_operation("SMLSL", a.id, a.type, b.id, b.type, v.id, v.type, o_var, o.type);
    return o;
}

// Accumulate
uint16x4_t vpadal_u8(uint16x4_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[2 * i] + b_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vpadalq_u8(uint16x8_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[2 * i] + b_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vpadaldq_u8(uint16x16_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[2 * i] + b_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vpadalqq_u8(uint16x32_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[2 * i] + b_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vpadaloq_u8(uint16x64_t a, uint8x128_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[2 * i] + b_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vpadal_u16(uint32x2_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[2 * i] + b_var.uh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vpadalq_u16(uint32x4_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[2 * i] + b_var.uh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vpadaldq_u16(uint32x8_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[2 * i] + b_var.uh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vpadalqq_u16(uint32x16_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[2 * i] + b_var.uh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vpadaloq_u16(uint32x32_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] + b_var.uh[2 * i] + b_var.uh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vpadal_s16(int32x2_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[2 * i] + b_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vpadalq_s16(int32x4_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[2 * i] + b_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vpadaldq_s16(int32x8_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[2 * i] + b_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vpadalqq_s16(int32x16_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[2 * i] + b_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vpadaloq_s16(int32x32_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.sh[2 * i] + b_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADALP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

// Absolute Accumulate
uint16x8_t vabal_u8(uint16x8_t a, uint8x8_t b, uint8x8_t c) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __64bit_var c_var = fake_neon::get_value_64(c.id, c.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.ub[i] > c_var.ub[i]) ? (b_var.ub[i] - c_var.ub[i]) : (c_var.ub[i] - b_var.ub[i]));
    }
    o.id = fake_neon::new_operation("UABAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint16x16_t vabaldq_u8(uint16x16_t a, uint8x16_t b, uint8x16_t c) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __128bit_var c_var = fake_neon::get_value_128(c.id, c.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.ub[i] > c_var.ub[i]) ? (b_var.ub[i] - c_var.ub[i]) : (c_var.ub[i] - b_var.ub[i]));
    }
    o.id = fake_neon::new_operation("UABAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint16x32_t vabalqq_u8(uint16x32_t a, uint8x32_t b, uint8x32_t c) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __256bit_var c_var = fake_neon::get_value_256(c.id, c.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.ub[i] > c_var.ub[i]) ? (b_var.ub[i] - c_var.ub[i]) : (c_var.ub[i] - b_var.ub[i]));
    }
    o.id = fake_neon::new_operation("UABAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint16x64_t vabaloq_u8(uint16x64_t a, uint8x64_t b, uint8x64_t c) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __512bit_var c_var = fake_neon::get_value_512(c.id, c.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.ub[i] > c_var.ub[i]) ? (b_var.ub[i] - c_var.ub[i]) : (c_var.ub[i] - b_var.ub[i]));
    }
    o.id = fake_neon::new_operation("UABAL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}

// Add
uint8x8_t vadd_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x16_t vaddq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x32_t vadddq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x64_t vaddqq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x128_t vaddoq_u8(uint8x128_t a, uint8x128_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vadd_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] + b_var.sh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vaddq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] + b_var.sh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vadddq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] + b_var.sh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vaddqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] + b_var.sh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vaddoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] + b_var.sh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vadd_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.uh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vaddq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.uh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vadddq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.uh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vaddqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.uh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vaddoq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.uh[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vadd_u32(uint32x2_t a, uint32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] + b_var.us[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vaddq_u32(uint32x4_t a, uint32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] + b_var.us[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vadddq_u32(uint32x8_t a, uint32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] + b_var.us[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vaddqq_u32(uint32x16_t a, uint32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] + b_var.us[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vaddoq_u32(uint32x32_t a, uint32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] + b_var.us[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vadd_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.ss[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vaddq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.ss[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vadddq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.ss[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vaddqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.ss[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vaddoq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] + b_var.ss[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vadd_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x4_t vaddq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x8_t vadddq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x16_t vaddqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x32_t vaddoq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = a_var.fs[i] + b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int64x1_t vadd_s64(int64x1_t a, int64x1_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = a_var.sd[i] + b_var.sd[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x2_t vaddq_s64(int64x2_t a, int64x2_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = a_var.sd[i] + b_var.sd[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x4_t vadddq_s64(int64x4_t a, int64x4_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = a_var.sd[i] + b_var.sd[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x8_t vaddqq_s64(int64x8_t a, int64x8_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = a_var.sd[i] + b_var.sd[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x16_t vaddoq_s64(int64x16_t a, int64x16_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = a_var.sd[i] + b_var.sd[i];
    }
    o.id = fake_neon::new_operation("ADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x8_t vaddl_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UADDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vaddldq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UADDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vaddlqq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UADDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vaddloq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.ub[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UADDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x4_t vaddl_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) + (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SADDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vaddldq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) + (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SADDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vaddlqq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) + (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SADDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vaddloq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) + (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SADDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x8_t vaddw_u8(uint16x8_t a, uint8x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UADDW", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vaddwdq_u8(uint16x16_t a, uint8x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UADDW", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vaddwqq_u8(uint16x32_t a, uint8x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UADDW", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vaddwoq_u8(uint16x64_t a, uint8x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] + b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UADDW", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int64x2_t vaddw_s32(int64x2_t a, int32x2_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = a_var.sd[i] + (int64_t)(b_var.ss[i]);
    }
    o.id = fake_neon::new_operation("SADDW", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x4_t vaddwdq_s32(int64x4_t a, int32x4_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = a_var.sd[i] + (int64_t)(b_var.ss[i]);
    }
    o.id = fake_neon::new_operation("SADDW", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x8_t vaddwqq_s8(int64x8_t a, int32x8_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = a_var.sd[i] + (int64_t)(b_var.ss[i]);
    }
    o.id = fake_neon::new_operation("SADDW", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x16_t vaddwoq_s32(int64x16_t a, int32x16_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = a_var.sd[i] + (int64_t)(b_var.ss[i]);
    }
    o.id = fake_neon::new_operation("SADDW", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vpadd_u32(uint32x2_t a, uint32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.us[i] = a_var.us[2 * i] + a_var.us[2 * i + 1];
    }
    for (int i = 0; i < 1; i++) {
        o_var.us[i + 1] = b_var.us[2 * i] + b_var.us[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vpaddq_u32(uint32x4_t a, uint32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[2 * i] + a_var.us[2 * i + 1];
    }
    for (int i = 0; i < 2; i++) {
        o_var.us[i + 2] = b_var.us[2 * i] + b_var.us[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vpadddq_u32(uint32x8_t a, uint32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[2 * i] + a_var.us[2 * i + 1];
    }
    for (int i = 0; i < 4; i++) {
        o_var.us[i + 4] = b_var.us[2 * i] + b_var.us[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vpaddqq_u32(uint32x16_t a, uint32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[2 * i] + a_var.us[2 * i + 1];
    }
    for (int i = 0; i < 8; i++) {
        o_var.us[i + 8] = b_var.us[2 * i] + b_var.us[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vpaddoq_u32(uint32x32_t a, uint32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[2 * i] + a_var.us[2 * i + 1];
    }
    for (int i = 0; i < 16; i++) {
        o_var.us[i + 16] = b_var.us[2 * i] + b_var.us[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vpadd_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.ss[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    for (int i = 0; i < 1; i++) {
        o_var.ss[i + 1] = b_var.ss[2 * i] + b_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vpaddq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    for (int i = 0; i < 2; i++) {
        o_var.ss[i + 2] = b_var.ss[2 * i] + b_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vpadddq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    for (int i = 0; i < 4; i++) {
        o_var.ss[i + 4] = b_var.ss[2 * i] + b_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vpaddqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    for (int i = 0; i < 8; i++) {
        o_var.ss[i + 8] = b_var.ss[2 * i] + b_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vpaddoq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    for (int i = 0; i < 16; i++) {
        o_var.ss[i + 16] = b_var.ss[2 * i] + b_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("ADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vpadd_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.fs[i] = a_var.fs[2 * i] + a_var.fs[2 * i + 1];
    }
    for (int i = 0; i < 1; i++) {
        o_var.fs[i + 1] = b_var.fs[2 * i] + b_var.fs[2 * i + 1];
    }
    o.id = fake_neon::new_operation("FADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x4_t vpaddq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[2 * i] + a_var.fs[2 * i + 1];
    }
    for (int i = 0; i < 2; i++) {
        o_var.fs[i + 2] = b_var.fs[2 * i] + b_var.fs[2 * i + 1];
    }
    o.id = fake_neon::new_operation("FADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x8_t vpadddq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[2 * i] + a_var.fs[2 * i + 1];
    }
    for (int i = 0; i < 4; i++) {
        o_var.fs[i + 4] = b_var.fs[2 * i] + b_var.fs[2 * i + 1];
    }
    o.id = fake_neon::new_operation("FADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x16_t vpaddqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[2 * i] + a_var.fs[2 * i + 1];
    }
    for (int i = 0; i < 8; i++) {
        o_var.fs[i + 8] = b_var.fs[2 * i] + b_var.fs[2 * i + 1];
    }
    o.id = fake_neon::new_operation("FADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x32_t vpaddoq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[2 * i] + a_var.fs[2 * i + 1];
    }
    for (int i = 0; i < 16; i++) {
        o_var.fs[i + 16] = b_var.fs[2 * i] + b_var.fs[2 * i + 1];
    }
    o.id = fake_neon::new_operation("FADDP", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vpaddl_u8(uint8x8_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.ub[2 * i] + a_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADDLP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vpaddlq_u8(uint8x16_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.ub[2 * i] + a_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADDLP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vpaddldq_u8(uint8x32_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.ub[2 * i] + a_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADDLP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vpaddlqq_u8(uint8x64_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.ub[2 * i] + a_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADDLP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x64_t vpaddloq_u8(uint8x128_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.ub[2 * i] + a_var.ub[2 * i + 1];
    }
    o.id = fake_neon::new_operation("UADDLP", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vpaddl_s16(int16x4_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.sh[2 * i] + a_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vpaddlq_s16(int16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.sh[2 * i] + a_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vpaddldq_s16(int16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.sh[2 * i] + a_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vpaddlqq_s16(int16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.sh[2 * i] + a_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vpaddloq_s16(int16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.sh[2 * i] + a_var.sh[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}

int64x1_t vpaddl_s32(int32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}
int64x2_t vpaddlq_s32(int32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}
int64x4_t vpaddldq_s32(int32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}
int64x8_t vpaddlqq_s32(int32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}
int64x16_t vpaddloq_s32(int32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = a_var.ss[2 * i] + a_var.ss[2 * i + 1];
    }
    o.id = fake_neon::new_operation("SADDLP", a.id, a.type, o_var, o.type);
    return o;
}

uint16_t vaddlv_u8(uint8x8_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 8; i++) {
        o += a_var.ub[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}
uint16_t vaddlvq_u8(uint8x16_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 16; i++) {
        o += a_var.ub[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}
uint16_t vaddlvdq_u8(uint8x32_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 32; i++) {
        o += a_var.ub[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}
uint16_t vaddlvqq_u8(uint8x64_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 64; i++) {
        o += a_var.ub[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}
uint16_t vaddlvoq_u8(uint8x128_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 128; i++) {
        o += a_var.ub[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}

uint32_t vaddlv_u16(uint16x4_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint32_t o = 0;
    for (int i = 0; i < 4; i++) {
        o += a_var.uh[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}
uint32_t vaddlvq_u16(uint16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32_t o = 0;
    for (int i = 0; i < 8; i++) {
        o += a_var.uh[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}
uint32_t vaddlvdq_u16(uint16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32_t o = 0;
    for (int i = 0; i < 16; i++) {
        o += a_var.uh[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}
uint32_t vaddlvqq_u16(uint16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32_t o = 0;
    for (int i = 0; i < 32; i++) {
        o += a_var.uh[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}
uint32_t vaddlvoq_u16(uint16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32_t o = 0;
    for (int i = 0; i < 64; i++) {
        o += a_var.uh[i];
    }
    fake_neon::new_operation("UADDL", a.id, a.type);
    return o;
}

uint8x8_t vhadd_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = (a_var.ub[i] + b_var.ub[i]) >> 1;
    }
    o.id = fake_neon::new_operation("UHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x16_t vhaddq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = (a_var.ub[i] + b_var.ub[i]) >> 1;
    }
    o.id = fake_neon::new_operation("UHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x32_t vhadddq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = (a_var.ub[i] + b_var.ub[i]) >> 1;
    }
    o.id = fake_neon::new_operation("UHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x64_t vhaddqq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = (a_var.ub[i] + b_var.ub[i]) >> 1;
    }
    o.id = fake_neon::new_operation("UHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x128_t vhaddoq_u8(uint8x128_t a, uint8x128_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = (a_var.ub[i] + b_var.ub[i]) >> 1;
    }
    o.id = fake_neon::new_operation("UHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vqadd_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        int32_t val = a_var.sh[i] + b_var.sh[i];
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vqaddq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        int32_t val = a_var.sh[i] + b_var.sh[i];
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vqadddq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        int32_t val = a_var.sh[i] + b_var.sh[i];
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vqaddqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        int32_t val = a_var.sh[i] + b_var.sh[i];
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vqaddoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        int32_t val = a_var.sh[i] + b_var.sh[i];
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vrhadd_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = (a_var.sh[i] + b_var.sh[i] + 1) >> 1;
    }
    o.id = fake_neon::new_operation("SRHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vrhaddq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = (a_var.sh[i] + b_var.sh[i] + 1) >> 1;
    }
    o.id = fake_neon::new_operation("SRHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vrhadddq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = (a_var.sh[i] + b_var.sh[i] + 1) >> 1;
    }
    o.id = fake_neon::new_operation("SRHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vrhaddqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = (a_var.sh[i] + b_var.sh[i] + 1) >> 1;
    }
    o.id = fake_neon::new_operation("SRHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vrhaddoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = (a_var.sh[i] + b_var.sh[i] + 1) >> 1;
    }
    o.id = fake_neon::new_operation("SRHADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

// Sub
uint8x8_t vsub_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x16_t vsubq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x32_t vsubdq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x64_t vsubqq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x128_t vsuboq_u8(uint8x128_t a, uint8x128_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vsub_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] - b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vsubq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] - b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vsubdq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] - b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vsubqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] - b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vsuboq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] - b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vsub_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] - b_var.uh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vsubq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] - b_var.uh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vsubdq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] - b_var.uh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vsubqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] - b_var.uh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vsuboq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] - b_var.uh[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vsub_u32(uint32x2_t a, uint32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] - b_var.us[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vsubq_u32(uint32x4_t a, uint32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] - b_var.us[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vsubdq_u32(uint32x8_t a, uint32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] - b_var.us[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vsubqq_u32(uint32x16_t a, uint32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] - b_var.us[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vsuboq_u32(uint32x32_t a, uint32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] - b_var.us[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vsub_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vsubq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vsubdq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vsubqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vsuboq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] - b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vsub_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[i] - b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x4_t vsubq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[i] - b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x8_t vsubdq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[i] - b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x16_t vsubqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[i] - b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x32_t vsuboq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = a_var.fs[i] - b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FADD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x8_t vsubl_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("USUBL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vsubldq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("USUBL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vsublqq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("USUBL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vsubloq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.ub[i] - b_var.ub[i];
    }
    o.id = fake_neon::new_operation("USUBL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x4_t vsubl_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) - (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SSUBL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vsubldq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) - (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SSUBL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vsublqq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) - (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SSUBL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vsubloq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) - (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SSUBL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vqsub_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        int64_t val = a_var.ss[i] - b_var.ss[i];
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vqsubq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        int64_t val = a_var.ss[i] - b_var.ss[i];
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vqsubdq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        int64_t val = a_var.ss[i] - b_var.ss[i];
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vqsubqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        int64_t val = a_var.ss[i] - b_var.ss[i];
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vqsuboq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        int64_t val = a_var.ss[i] - b_var.ss[i];
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vhsub_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = (a_var.sh[i] - b_var.sh[i]) >> 1;
    }
    o.id = fake_neon::new_operation("SHSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vhsubq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = (a_var.sh[i] - b_var.sh[i]) >> 1;
    }
    o.id = fake_neon::new_operation("SHSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vhsubdq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = (a_var.sh[i] - b_var.sh[i]) >> 1;
    }
    o.id = fake_neon::new_operation("SHSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vhsubqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = (a_var.sh[i] - b_var.sh[i]) >> 1;
    }
    o.id = fake_neon::new_operation("SHSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vhsuboq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = (a_var.sh[i] - b_var.sh[i]) >> 1;
    }
    o.id = fake_neon::new_operation("SHSUB", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32_t vqsubs_s32(int32_t a, int32_t b) {
    return a - b;
}

// Mul
uint16x4_t vmul_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vmulq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vmuldq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vmulqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vmuloq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vmul_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] * b_var.sh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vmulq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] * b_var.sh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vmuldq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] * b_var.sh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vmulqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] * b_var.sh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vmuloq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] * b_var.sh[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vmul_u32(uint32x2_t a, uint32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] * b_var.us[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vmulq_u32(uint32x4_t a, uint32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] * b_var.us[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vmuldq_u32(uint32x8_t a, uint32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] * b_var.us[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vmulqq_u32(uint32x16_t a, uint32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] * b_var.us[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vmuloq_u32(uint32x32_t a, uint32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] * b_var.us[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vmul_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i] * b_var.ss[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vmulq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] * b_var.ss[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vmuldq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] * b_var.ss[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vmulqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] * b_var.ss[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vmuloq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] * b_var.ss[i];
    }
    o.id = fake_neon::new_operation("MUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vmul_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[i] * b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x4_t vmulq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[i] * b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x8_t vmuldq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[i] * b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x16_t vmulqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[i] * b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x32_t vmuloq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = a_var.fs[i] * b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vmul_n_f32(float32x2_t a, float b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[i] * b;
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, o_var, o.type);
    return o;
}
float32x4_t vmulq_n_f32(float32x4_t a, float b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[i] * b;
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, o_var, o.type);
    return o;
}
float32x8_t vmuldq_n_f32(float32x8_t a, float b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[i] * b;
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, o_var, o.type);
    return o;
}
float32x16_t vmulqq_n_f32(float32x16_t a, float b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[i] * b;
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, o_var, o.type);
    return o;
}
float32x32_t vmuloq_n_f32(float32x32_t a, float b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = a_var.fs[i] * b;
    }
    o.id = fake_neon::new_operation("FMUL", a.id, a.type, o_var, o.type);
    return o;
}

uint16x8_t vmull_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.ub[i] * b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vmulldq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.ub[i] * b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vmullqq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.ub[i] * b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vmulloq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.ub[i] * b_var.ub[i];
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x4_t vmull_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vmulldq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vmullqq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vmulloq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.uh[i] * b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x4_t vmull_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vmulldq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vmullqq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vmulloq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int64x2_t vmull_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x4_t vmulldq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x8_t vmullqq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x16_t vmulloq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x4_t vmull_n_u16(uint16x4_t a, uint16_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(b);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, o_var, o.type);
    return o;
}
uint32x8_t vmulldq_n_u16(uint16x8_t a, uint16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(b);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, o_var, o.type);
    return o;
}
uint32x16_t vmullqq_n_u16(uint16x16_t a, uint16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(b);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, o_var, o.type);
    return o;
}
uint32x32_t vmulloq_n_u16(uint16x32_t a, uint16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(b);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, o_var, o.type);
    return o;
}

int32x4_t vmull_n_s16(int16x4_t a, int16_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(b);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vmulldq_n_s16(int16x8_t a, int16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(b);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vmullqq_n_s16(int16x16_t a, int16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(b);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vmulloq_n_s16(int16x32_t a, int16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(b);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, o_var, o.type);
    return o;
}

uint32x4_t vmull_lane_u16(uint16x4_t a, uint16x4_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(v_var.uh[lane]);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x4_t vmull_laneq_u16(uint16x4_t a, uint16x8_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(v_var.uh[lane]);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x4_t vmull_lanedq_u16(uint16x4_t a, uint16x16_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __256bit_var v_var = fake_neon::get_value_256(v.id, v.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(v_var.uh[lane]);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x4_t vmull_laneqq_u16(uint16x4_t a, uint16x32_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __512bit_var v_var = fake_neon::get_value_512(v.id, v.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(v_var.uh[lane]);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x4_t vmull_laneoq_u16(uint16x4_t a, uint16x64_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __1024bit_var v_var = fake_neon::get_value_1024(v.id, v.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(v_var.uh[lane]);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x8_t vmullq_laneq_u16(uint16x8_t a, uint16x8_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(v_var.uh[lane]);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x16_t vmulldq_laneq_u16(uint16x16_t a, uint16x8_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(v_var.uh[lane]);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
uint32x32_t vmullqq_laneq_u16(uint16x32_t a, uint16x8_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = (uint32_t)(a_var.uh[i]) * (uint32_t)(v_var.uh[lane]);
    }
    o.id = fake_neon::new_operation("UMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}

int32x4_t vmull_lane_s16(int16x4_t a, int16x4_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x4_t vmull_laneq_s16(int16x4_t a, int16x8_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x4_t vmull_lanedq_s16(int16x4_t a, int16x16_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __256bit_var v_var = fake_neon::get_value_256(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x4_t vmull_laneqq_s16(int16x4_t a, int16x32_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __512bit_var v_var = fake_neon::get_value_512(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x4_t vmull_laneoq_s16(int16x4_t a, int16x64_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __1024bit_var v_var = fake_neon::get_value_1024(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x8_t vmullq_laneq_s16(int16x8_t a, int16x8_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x16_t vmulldq_laneq_s16(int16x16_t a, int16x8_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x32_t vmullqq_laneq_s16(int16x32_t a, int16x8_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __128bit_var v_var = fake_neon::get_value_128(v.id, v.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]);
    }
    o.id = fake_neon::new_operation("SMULL", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}

int16x4_t vqdmulh_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i])) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vqdmulhq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i])) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vqdmulhdq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i])) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vqdmulhqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i])) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vqdmulhoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i])) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vqdmulh_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vqdmulhq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vqdmulhdq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vqdmulhqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vqdmulhoq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vqrdmulh_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 4; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vqrdmulhq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 8; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vqrdmulhdq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 16; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vqrdmulhqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 32; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vqrdmulhoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 64; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(b_var.sh[i]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vqrdmulh_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 2; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vqrdmulhq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 4; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vqrdmulhdq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 8; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vqrdmulhqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 16; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vqrdmulhoq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 32; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(b_var.ss[i]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vqrdmulh_lane_s16(int16x4_t a, int16x4_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int16x4_t o;
    __64bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 4; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int16x8_t vqrdmulhq_lane_s16(int16x8_t a, int16x4_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int16x8_t o;
    __128bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 8; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int16x16_t vqrdmulhdq_lane_s16(int16x16_t a, int16x4_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int16x16_t o;
    __256bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 16; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int16x32_t vqrdmulhqq_lane_s16(int16x32_t a, int16x4_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int16x32_t o;
    __512bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 32; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int16x64_t vqrdmulhoq_lane_s16(int16x64_t a, int16x4_t v, const int lane) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int16x64_t o;
    __1024bit_var o_var;
    int32_t round_const = 1 << 15;
    for (int i = 0; i < 64; i++) {
        int32_t val = (2 * (int32_t)(a_var.sh[i]) * (int32_t)(v_var.sh[lane]) + round_const) >> 16;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}

int32x2_t vqrdmulh_lane_s32(int32x2_t a, int32x2_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x2_t o;
    __64bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 2; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x4_t vqrdmulhq_lane_s32(int32x4_t a, int32x2_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 4; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x8_t vqrdmulhdq_lane_s32(int32x8_t a, int32x2_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x8_t o;
    __256bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 8; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x16_t vqrdmulhqq_lane_s32(int32x16_t a, int32x2_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x16_t o;
    __512bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 16; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x32_t vqrdmulhoq_lane_s32(int32x32_t a, int32x2_t v, const int lane) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x32_t o;
    __1024bit_var o_var;
    int64_t round_const = 1 << 31;
    for (int i = 0; i < 32; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane]) + round_const) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQRDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}

int32x2_t vqdmulh_lane_s32(int32x2_t a, int32x2_t v, const int lane) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x4_t vqdmulhq_lane_s32(int32x4_t a, int32x2_t v, const int lane) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x8_t vqdmulhdq_lane_s32(int32x8_t a, int32x2_t v, const int lane) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x16_t vqdmulhqq_lane_s32(int32x16_t a, int32x2_t v, const int lane) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}
int32x32_t vqdmulhoq_lane_s32(int32x32_t a, int32x2_t v, const int lane) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __64bit_var v_var = fake_neon::get_value_64(v.id, v.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        int64_t val = (2 * (int64_t)(a_var.ss[i]) * (int64_t)(v_var.ss[lane])) >> 32;
        o_var.ss[i] = SATQ_S32(val);
    }
    o.id = fake_neon::new_operation("SQDMULH", a.id, a.type, v.id, v.type, o_var, o.type);
    return o;
}

// Absolute
int16x4_t vabs_s16(int16x4_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = (a_var.sh[i] > 0) ? (a_var.sh[i]) : (-a_var.sh[i]);
    }
    o.id = fake_neon::new_operation("ABS", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vabsq_s16(int16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = (a_var.sh[i] > 0) ? (a_var.sh[i]) : (-(a_var.sh[i]));
    }
    o.id = fake_neon::new_operation("ABS", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vabsdq_s16(int16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = (a_var.sh[i] > 0) ? (a_var.sh[i]) : (-a_var.sh[i]);
    }
    o.id = fake_neon::new_operation("ABS", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vabsqq_s16(int16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = (a_var.sh[i] > 0) ? (a_var.sh[i]) : (-a_var.sh[i]);
    }
    o.id = fake_neon::new_operation("ABS", a.id, a.type, o_var, o.type);
    return o;
}
int16x64_t vabsoq_s16(int16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = (a_var.sh[i] > 0) ? (a_var.sh[i]) : (-a_var.sh[i]);
    }
    o.id = fake_neon::new_operation("ABS", a.id, a.type, o_var, o.type);
    return o;
}

uint16x8_t vabdl_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.ub[i] > b_var.ub[i]) ? (a_var.ub[i] - b_var.ub[i]) : (b_var.ub[i] - a_var.ub[i]);
    }
    o.id = fake_neon::new_operation("UABDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vabdldq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.ub[i] > b_var.ub[i]) ? (a_var.ub[i] - b_var.ub[i]) : (b_var.ub[i] - a_var.ub[i]);
    }
    o.id = fake_neon::new_operation("UABDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vabdlqq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.ub[i] > b_var.ub[i]) ? (a_var.ub[i] - b_var.ub[i]) : (b_var.ub[i] - a_var.ub[i]);
    }
    o.id = fake_neon::new_operation("UABDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vabdloq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.ub[i] > b_var.ub[i]) ? (a_var.ub[i] - b_var.ub[i]) : (b_var.ub[i] - a_var.ub[i]);
    }
    o.id = fake_neon::new_operation("UABDL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vabd_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? (a_var.uh[i] - b_var.uh[i]) : (b_var.uh[i] - a_var.uh[i]);
    }
    o.id = fake_neon::new_operation("UABD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vabdq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? (a_var.uh[i] - b_var.uh[i]) : (b_var.uh[i] - a_var.uh[i]);
    }
    o.id = fake_neon::new_operation("UABD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vabddq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? (a_var.uh[i] - b_var.uh[i]) : (b_var.uh[i] - a_var.uh[i]);
    }
    o.id = fake_neon::new_operation("UABD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vabdqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? (a_var.uh[i] - b_var.uh[i]) : (b_var.uh[i] - a_var.uh[i]);
    }
    o.id = fake_neon::new_operation("UABD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vabdoq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? (a_var.uh[i] - b_var.uh[i]) : (b_var.uh[i] - a_var.uh[i]);
    }
    o.id = fake_neon::new_operation("UABD", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

// Min/Max
int16x4_t vmin_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? b_var.sh[i] : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vminq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? b_var.sh[i] : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vmindq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? b_var.sh[i] : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vminqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? b_var.sh[i] : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vminoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? b_var.sh[i] : a_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vmin_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? b_var.ss[i] : a_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vminq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? b_var.ss[i] : a_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vmindq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? b_var.ss[i] : a_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vminqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? b_var.ss[i] : a_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vminoq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? b_var.ss[i] : a_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vmin_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? b_var.fs[i] : a_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x4_t vminq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? b_var.fs[i] : a_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x8_t vmindq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? b_var.fs[i] : a_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x16_t vminqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? b_var.fs[i] : a_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x32_t vminoq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? b_var.fs[i] : a_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMIN", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vmax_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? a_var.uh[i] : b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vmaxq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? a_var.uh[i] : b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vmaxdq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? a_var.uh[i] : b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vmaxqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? a_var.uh[i] : b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vmaxoq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.uh[i] > b_var.uh[i]) ? a_var.uh[i] : b_var.uh[i];
    }
    o.id = fake_neon::new_operation("UMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vmax_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? a_var.sh[i] : b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vmaxq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? a_var.sh[i] : b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vmaxdq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? a_var.sh[i] : b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vmaxqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? a_var.sh[i] : b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vmaxoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = (a_var.sh[i] > b_var.sh[i]) ? a_var.sh[i] : b_var.sh[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vmax_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? a_var.ss[i] : b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vmaxq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? a_var.ss[i] : b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vmaxdq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? a_var.ss[i] : b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vmaxqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? a_var.ss[i] : b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vmaxoq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = (a_var.ss[i] > b_var.ss[i]) ? a_var.ss[i] : b_var.ss[i];
    }
    o.id = fake_neon::new_operation("SMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vmax_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? a_var.fs[i] : b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x4_t vmaxq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? a_var.fs[i] : b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x8_t vmaxdq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? a_var.fs[i] : b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x16_t vmaxqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? a_var.fs[i] : b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x32_t vmaxoq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = (a_var.fs[i] > b_var.fs[i]) ? a_var.fs[i] : b_var.fs[i];
    }
    o.id = fake_neon::new_operation("FMAX", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16_t vmaxv_u16(uint16x4_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 4; i++) {
        o = (a_var.uh[i] > o) ? a_var.uh[i] : o;
    }
    fake_neon::new_operation("UMAXV", a.id, a.type);
    return o;
}
uint16_t vmaxvq_u16(uint16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 8; i++) {
        o = (a_var.uh[i] > o) ? a_var.uh[i] : o;
    }
    fake_neon::new_operation("UMAXV", a.id, a.type);
    return o;
}
uint16_t vmaxvdq_u16(uint16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 16; i++) {
        o = (a_var.uh[i] > o) ? a_var.uh[i] : o;
    }
    fake_neon::new_operation("UMAXV", a.id, a.type);
    return o;
}
uint16_t vmaxvqq_u16(uint16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 32; i++) {
        o = (a_var.uh[i] > o) ? a_var.uh[i] : o;
    }
    fake_neon::new_operation("UMAXV", a.id, a.type);
    return o;
}
uint16_t vmaxvoq_u16(uint16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16_t o = 0;
    for (int i = 0; i < 64; i++) {
        o = (a_var.uh[i] > o) ? a_var.uh[i] : o;
    }
    fake_neon::new_operation("UMAXV", a.id, a.type);
    return o;
}

// Combine
uint8x16_t vcombine_u8(uint8x8_t low, uint8x8_t high) {
    __64bit_var low_var = fake_neon::get_value_64(low.id, low.type);
    __64bit_var high_var = fake_neon::get_value_64(high.id, high.type);
    uint8x16_t o1;
    uint8x16_t o2;
    __128bit_var o1_var;
    __128bit_var o2_var;
    for (int i = 0; i < 8; i++) {
        o1_var.ub[i] = low_var.ub[i];
        o2_var.ub[i] = low_var.ub[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 8; i++) {
        o2_var.ub[8 + i] = high_var.ub[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint8x32_t vcombineq_u8(uint8x16_t low, uint8x16_t high) {
    __128bit_var low_var = fake_neon::get_value_128(low.id, low.type);
    __128bit_var high_var = fake_neon::get_value_128(high.id, high.type);
    uint8x32_t o1;
    uint8x32_t o2;
    __256bit_var o1_var;
    __256bit_var o2_var;
    for (int i = 0; i < 16; i++) {
        o1_var.ub[i] = low_var.ub[i];
        o2_var.ub[i] = low_var.ub[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 16; i++) {
        o2_var.ub[16 + i] = high_var.ub[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint8x64_t vcombinedq_u8(uint8x32_t low, uint8x32_t high) {
    __256bit_var low_var = fake_neon::get_value_256(low.id, low.type);
    __256bit_var high_var = fake_neon::get_value_256(high.id, high.type);
    uint8x64_t o1;
    uint8x64_t o2;
    __512bit_var o1_var;
    __512bit_var o2_var;
    for (int i = 0; i < 32; i++) {
        o1_var.ub[i] = low_var.ub[i];
        o2_var.ub[i] = low_var.ub[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 32; i++) {
        o2_var.ub[32 + i] = high_var.ub[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint8x128_t vcombineqq_u8(uint8x64_t low, uint8x64_t high) {
    __512bit_var low_var = fake_neon::get_value_512(low.id, low.type);
    __512bit_var high_var = fake_neon::get_value_512(high.id, high.type);
    uint8x128_t o1;
    uint8x128_t o2;
    __1024bit_var o1_var;
    __1024bit_var o2_var;
    for (int i = 0; i < 64; i++) {
        o1_var.ub[i] = low_var.ub[i];
        o2_var.ub[i] = low_var.ub[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 64; i++) {
        o2_var.ub[64 + i] = high_var.ub[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}

int8x16_t vcombine_s8(int8x8_t low, int8x8_t high) {
    __64bit_var low_var = fake_neon::get_value_64(low.id, low.type);
    __64bit_var high_var = fake_neon::get_value_64(high.id, high.type);
    int8x16_t o1;
    int8x16_t o2;
    __128bit_var o1_var;
    __128bit_var o2_var;
    for (int i = 0; i < 8; i++) {
        o1_var.sb[i] = low_var.sb[i];
        o2_var.sb[i] = low_var.sb[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 8; i++) {
        o2_var.sb[8 + i] = high_var.sb[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int8x32_t vcombineq_s8(int8x16_t low, int8x16_t high) {
    __128bit_var low_var = fake_neon::get_value_128(low.id, low.type);
    __128bit_var high_var = fake_neon::get_value_128(high.id, high.type);
    int8x32_t o1;
    int8x32_t o2;
    __256bit_var o1_var;
    __256bit_var o2_var;
    for (int i = 0; i < 16; i++) {
        o1_var.sb[i] = low_var.sb[i];
        o2_var.sb[i] = low_var.sb[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 16; i++) {
        o2_var.sb[16 + i] = high_var.sb[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int8x64_t vcombinedq_s8(int8x32_t low, int8x32_t high) {
    __256bit_var low_var = fake_neon::get_value_256(low.id, low.type);
    __256bit_var high_var = fake_neon::get_value_256(high.id, high.type);
    int8x64_t o1;
    int8x64_t o2;
    __512bit_var o1_var;
    __512bit_var o2_var;
    for (int i = 0; i < 32; i++) {
        o1_var.sb[i] = low_var.sb[i];
        o2_var.sb[i] = low_var.sb[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 32; i++) {
        o2_var.sb[32 + i] = high_var.sb[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int8x128_t vcombineqq_s8(int8x64_t low, int8x64_t high) {
    __512bit_var low_var = fake_neon::get_value_512(low.id, low.type);
    __512bit_var high_var = fake_neon::get_value_512(high.id, high.type);
    int8x128_t o1;
    int8x128_t o2;
    __1024bit_var o1_var;
    __1024bit_var o2_var;
    for (int i = 0; i < 64; i++) {
        o1_var.sb[i] = low_var.sb[i];
        o2_var.sb[i] = low_var.sb[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 64; i++) {
        o2_var.sb[64 + i] = high_var.sb[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}

uint16x8_t vcombine_u16(uint16x4_t low, uint16x4_t high) {
    __64bit_var low_var = fake_neon::get_value_64(low.id, low.type);
    __64bit_var high_var = fake_neon::get_value_64(high.id, high.type);
    uint16x8_t o1;
    uint16x8_t o2;
    __128bit_var o1_var;
    __128bit_var o2_var;
    for (int i = 0; i < 4; i++) {
        o1_var.uh[i] = low_var.uh[i];
        o2_var.uh[i] = low_var.uh[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 4; i++) {
        o2_var.uh[4 + i] = high_var.uh[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint16x16_t vcombineq_u16(uint16x8_t low, uint16x8_t high) {
    __128bit_var low_var = fake_neon::get_value_128(low.id, low.type);
    __128bit_var high_var = fake_neon::get_value_128(high.id, high.type);
    uint16x16_t o1;
    uint16x16_t o2;
    __256bit_var o1_var;
    __256bit_var o2_var;
    for (int i = 0; i < 8; i++) {
        o1_var.uh[i] = low_var.uh[i];
        o2_var.uh[i] = low_var.uh[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 8; i++) {
        o2_var.uh[8 + i] = high_var.uh[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint16x32_t vcombinedq_u16(uint16x16_t low, uint16x16_t high) {
    __256bit_var low_var = fake_neon::get_value_256(low.id, low.type);
    __256bit_var high_var = fake_neon::get_value_256(high.id, high.type);
    uint16x32_t o1;
    uint16x32_t o2;
    __512bit_var o1_var;
    __512bit_var o2_var;
    for (int i = 0; i < 16; i++) {
        o1_var.uh[i] = low_var.uh[i];
        o2_var.uh[i] = low_var.uh[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 16; i++) {
        o2_var.uh[16 + i] = high_var.uh[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint16x64_t vcombineqq_u16(uint16x32_t low, uint16x32_t high) {
    __512bit_var low_var = fake_neon::get_value_512(low.id, low.type);
    __512bit_var high_var = fake_neon::get_value_512(high.id, high.type);
    uint16x64_t o1;
    uint16x64_t o2;
    __1024bit_var o1_var;
    __1024bit_var o2_var;
    for (int i = 0; i < 32; i++) {
        o1_var.uh[i] = low_var.uh[i];
        o2_var.uh[i] = low_var.uh[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 32; i++) {
        o2_var.uh[32 + i] = high_var.uh[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}

int16x8_t vcombine_s16(int16x4_t low, int16x4_t high) {
    __64bit_var low_var = fake_neon::get_value_64(low.id, low.type);
    __64bit_var high_var = fake_neon::get_value_64(high.id, high.type);
    int16x8_t o1;
    int16x8_t o2;
    __128bit_var o1_var;
    __128bit_var o2_var;
    for (int i = 0; i < 4; i++) {
        o1_var.sh[i] = low_var.sh[i];
        o2_var.sh[i] = low_var.sh[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 4; i++) {
        o2_var.sh[4 + i] = high_var.sh[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int16x16_t vcombineq_s16(int16x8_t low, int16x8_t high) {
    __128bit_var low_var = fake_neon::get_value_128(low.id, low.type);
    __128bit_var high_var = fake_neon::get_value_128(high.id, high.type);
    int16x16_t o1;
    int16x16_t o2;
    __256bit_var o1_var;
    __256bit_var o2_var;
    for (int i = 0; i < 8; i++) {
        o1_var.sh[i] = low_var.sh[i];
        o2_var.sh[i] = low_var.sh[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 8; i++) {
        o2_var.sh[8 + i] = high_var.sh[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int16x32_t vcombinedq_s16(int16x16_t low, int16x16_t high) {
    __256bit_var low_var = fake_neon::get_value_256(low.id, low.type);
    __256bit_var high_var = fake_neon::get_value_256(high.id, high.type);
    int16x32_t o1;
    int16x32_t o2;
    __512bit_var o1_var;
    __512bit_var o2_var;
    for (int i = 0; i < 16; i++) {
        o1_var.sh[i] = low_var.sh[i];
        o2_var.sh[i] = low_var.sh[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 16; i++) {
        o2_var.sh[16 + i] = high_var.sh[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int16x64_t vcombineqq_s16(int16x32_t low, int16x32_t high) {
    __512bit_var low_var = fake_neon::get_value_512(low.id, low.type);
    __512bit_var high_var = fake_neon::get_value_512(high.id, high.type);
    int16x64_t o1;
    int16x64_t o2;
    __1024bit_var o1_var;
    __1024bit_var o2_var;
    for (int i = 0; i < 32; i++) {
        o1_var.sh[i] = low_var.sh[i];
        o2_var.sh[i] = low_var.sh[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 32; i++) {
        o2_var.sh[32 + i] = high_var.sh[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}

uint32x4_t vcombine_u32(uint32x2_t low, uint32x2_t high) {
    __64bit_var low_var = fake_neon::get_value_64(low.id, low.type);
    __64bit_var high_var = fake_neon::get_value_64(high.id, high.type);
    uint32x4_t o1;
    uint32x4_t o2;
    __128bit_var o1_var;
    __128bit_var o2_var;
    for (int i = 0; i < 2; i++) {
        o1_var.us[i] = low_var.us[i];
        o2_var.us[i] = low_var.us[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 2; i++) {
        o2_var.us[2 + i] = high_var.us[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint32x8_t vcombineq_u32(uint32x4_t low, uint32x4_t high) {
    __128bit_var low_var = fake_neon::get_value_128(low.id, low.type);
    __128bit_var high_var = fake_neon::get_value_128(high.id, high.type);
    uint32x8_t o1;
    uint32x8_t o2;
    __256bit_var o1_var;
    __256bit_var o2_var;
    for (int i = 0; i < 4; i++) {
        o1_var.us[i] = low_var.us[i];
        o2_var.us[i] = low_var.us[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 4; i++) {
        o2_var.us[4 + i] = high_var.us[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint32x16_t vcombinedq_u32(uint32x8_t low, uint32x8_t high) {
    __256bit_var low_var = fake_neon::get_value_256(low.id, low.type);
    __256bit_var high_var = fake_neon::get_value_256(high.id, high.type);
    uint32x16_t o1;
    uint32x16_t o2;
    __512bit_var o1_var;
    __512bit_var o2_var;
    for (int i = 0; i < 8; i++) {
        o1_var.us[i] = low_var.us[i];
        o2_var.us[i] = low_var.us[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 8; i++) {
        o2_var.us[8 + i] = high_var.us[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
uint32x32_t vcombineqq_u32(uint32x16_t low, uint32x16_t high) {
    __512bit_var low_var = fake_neon::get_value_512(low.id, low.type);
    __512bit_var high_var = fake_neon::get_value_512(high.id, high.type);
    uint32x32_t o1;
    uint32x32_t o2;
    __1024bit_var o1_var;
    __1024bit_var o2_var;
    for (int i = 0; i < 16; i++) {
        o1_var.us[i] = low_var.us[i];
        o2_var.us[i] = low_var.us[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 16; i++) {
        o2_var.us[16 + i] = high_var.us[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}

int32x4_t vcombine_s32(int32x2_t low, int32x2_t high) {
    __64bit_var low_var = fake_neon::get_value_64(low.id, low.type);
    __64bit_var high_var = fake_neon::get_value_64(high.id, high.type);
    int32x4_t o1;
    int32x4_t o2;
    __128bit_var o1_var;
    __128bit_var o2_var;
    for (int i = 0; i < 2; i++) {
        o1_var.ss[i] = low_var.ss[i];
        o2_var.ss[i] = low_var.ss[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 2; i++) {
        o2_var.ss[2 + i] = high_var.ss[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int32x8_t vcombineq_s32(int32x4_t low, int32x4_t high) {
    __128bit_var low_var = fake_neon::get_value_128(low.id, low.type);
    __128bit_var high_var = fake_neon::get_value_128(high.id, high.type);
    int32x8_t o1;
    int32x8_t o2;
    __256bit_var o1_var;
    __256bit_var o2_var;
    for (int i = 0; i < 4; i++) {
        o1_var.ss[i] = low_var.ss[i];
        o2_var.ss[i] = low_var.ss[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 4; i++) {
        o2_var.ss[4 + i] = high_var.ss[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int32x16_t vcombinedq_s32(int32x8_t low, int32x8_t high) {
    __256bit_var low_var = fake_neon::get_value_256(low.id, low.type);
    __256bit_var high_var = fake_neon::get_value_256(high.id, high.type);
    int32x16_t o1;
    int32x16_t o2;
    __512bit_var o1_var;
    __512bit_var o2_var;
    for (int i = 0; i < 8; i++) {
        o1_var.ss[i] = low_var.ss[i];
        o2_var.ss[i] = low_var.ss[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 8; i++) {
        o2_var.ss[8 + i] = high_var.ss[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
int32x32_t vcombineqq_s32(int32x16_t low, int32x16_t high) {
    __512bit_var low_var = fake_neon::get_value_512(low.id, low.type);
    __512bit_var high_var = fake_neon::get_value_512(high.id, high.type);
    int32x32_t o1;
    int32x32_t o2;
    __1024bit_var o1_var;
    __1024bit_var o2_var;
    for (int i = 0; i < 16; i++) {
        o1_var.ss[i] = low_var.ss[i];
        o2_var.ss[i] = low_var.ss[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 16; i++) {
        o2_var.ss[16 + i] = high_var.ss[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}

float32x4_t vcombine_f32(float32x2_t low, float32x2_t high) {
    __64bit_var low_var = fake_neon::get_value_64(low.id, low.type);
    __64bit_var high_var = fake_neon::get_value_64(high.id, high.type);
    float32x4_t o1;
    float32x4_t o2;
    __128bit_var o1_var;
    __128bit_var o2_var;
    for (int i = 0; i < 2; i++) {
        o1_var.fs[i] = low_var.fs[i];
        o2_var.fs[i] = low_var.fs[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 2; i++) {
        o2_var.fs[2 + i] = high_var.fs[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
float32x8_t vcombineq_f32(float32x4_t low, float32x4_t high) {
    __128bit_var low_var = fake_neon::get_value_128(low.id, low.type);
    __128bit_var high_var = fake_neon::get_value_128(high.id, high.type);
    float32x8_t o1;
    float32x8_t o2;
    __256bit_var o1_var;
    __256bit_var o2_var;
    for (int i = 0; i < 4; i++) {
        o1_var.fs[i] = low_var.fs[i];
        o2_var.fs[i] = low_var.fs[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 4; i++) {
        o2_var.fs[4 + i] = high_var.fs[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
float32x16_t vcombinedq_f32(float32x8_t low, float32x8_t high) {
    __256bit_var low_var = fake_neon::get_value_256(low.id, low.type);
    __256bit_var high_var = fake_neon::get_value_256(high.id, high.type);
    float32x16_t o1;
    float32x16_t o2;
    __512bit_var o1_var;
    __512bit_var o2_var;
    for (int i = 0; i < 8; i++) {
        o1_var.fs[i] = low_var.fs[i];
        o2_var.fs[i] = low_var.fs[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 8; i++) {
        o2_var.fs[8 + i] = high_var.fs[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}
float32x32_t vcombineqq_f32(float32x16_t low, float32x16_t high) {
    __512bit_var low_var = fake_neon::get_value_512(low.id, low.type);
    __512bit_var high_var = fake_neon::get_value_512(high.id, high.type);
    float32x32_t o1;
    float32x32_t o2;
    __1024bit_var o1_var;
    __1024bit_var o2_var;
    for (int i = 0; i < 16; i++) {
        o1_var.fs[i] = low_var.fs[i];
        o2_var.fs[i] = low_var.fs[i];
    }
    o1.id = fake_neon::new_operation("DUP", low.id, low.type, o1_var, o1.type);
    for (int i = 0; i < 16; i++) {
        o2_var.fs[16 + i] = high_var.fs[i];
    }
    o2.id = fake_neon::new_operation("INS", high.id, high.type, o1.id, o1.type, o2_var, o2.type);
    return o2;
}

// Shift
uint16x4_t vshr_n_u16(uint16x4_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vshrq_n_u16(uint16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vshrdq_n_u16(uint16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vshrqq_n_u16(uint16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}
uint16x64_t vshroq_n_u16(uint16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vshr_n_s16(int16x4_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vshrq_n_s16(int16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vshrdq_n_s16(int16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vshrqq_n_s16(int16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int16x64_t vshroq_n_s16(int16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}

uint32x2_t vshr_n_u32(uint32x2_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}
uint32x4_t vshrq_n_u32(uint32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}
uint32x8_t vshrdq_n_u32(uint32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}
uint32x16_t vshrqq_n_u32(uint32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}
uint32x32_t vshroq_n_u32(uint32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("USHR", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vshr_n_s32(int32x2_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vshrq_n_s32(int32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vshrdq_n_s32(int32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vshrqq_n_s32(int32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vshroq_n_s32(int32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}

int64x1_t vshr_n_s64(int64x1_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int64x2_t vshrq_n_s64(int64x2_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int64x4_t vshrdq_n_s64(int64x4_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int64x8_t vshrqq_n_s64(int64x8_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}
int64x16_t vshroq_n_s64(int64x16_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SSHR", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vrshr_n_s16(int16x4_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 4; i++) {
        int16_t val = (a_var.sh[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SRSHR", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vrshrq_n_s16(int16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        int16_t val = (a_var.sh[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SRSHR", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vrshrdq_n_s16(int16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        int16_t val = (a_var.sh[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SRSHR", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vrshrqq_n_s16(int16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        int16_t val = (a_var.sh[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SRSHR", a.id, a.type, o_var, o.type);
    return o;
}
int16x64_t vrshroq_n_s16(int16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x64_t o;
    __1024bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 64; i++) {
        int16_t val = (a_var.sh[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(val);
    }
    o.id = fake_neon::new_operation("SRSHR", a.id, a.type, o_var, o.type);
    return o;
}

uint8x8_t vrshrn_n_u16(uint16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint8x8_t o;
    __64bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = (a_var.uh[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x16_t vrshrndq_n_u16(uint16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint8x16_t o;
    __128bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = (a_var.uh[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x32_t vrshrnqq_n_u16(uint16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint8x32_t o;
    __256bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = (a_var.uh[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x64_t vrshrnoq_n_u16(uint16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint8x64_t o;
    __512bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = (a_var.uh[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}

int8x8_t vrshrn_n_s16(int16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int8x8_t o;
    __64bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        o_var.sb[i] = (a_var.sh[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int8x16_t vrshrndq_n_s16(int16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int8x16_t o;
    __128bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        o_var.sb[i] = (a_var.sh[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int8x32_t vrshrnqq_n_s16(int16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int8x32_t o;
    __256bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        o_var.sb[i] = (a_var.sh[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int8x64_t vrshrnoq_n_s16(int16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int8x64_t o;
    __512bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 64; i++) {
        o_var.sb[i] = (a_var.sh[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}

uint16x4_t vrshrn_n_u32(uint32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.us[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vrshrndq_n_u32(uint32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.us[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vrshrnqq_n_u32(uint32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.us[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vrshrnoq_n_u32(uint32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.us[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vrshrn_n_s32(int32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = (a_var.ss[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vrshrndq_n_s32(int32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = (a_var.ss[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vrshrnqq_n_s32(int32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = (a_var.ss[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vrshrnoq_n_s32(int32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = (a_var.ss[i] + round_const) >> n;
    }
    o.id = fake_neon::new_operation("RSHRN", a.id, a.type, o_var, o.type);
    return o;
}

uint8x8_t vshrn_n_u16(uint16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x16_t vshrndq_n_u16(uint16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x32_t vshrnqq_n_u16(uint16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint8x64_t vshrnoq_n_u16(uint16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = a_var.uh[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}

int8x8_t vshrn_n_s16(int16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sb[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
int8x16_t vshrndq_n_s16(int16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sb[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
int8x32_t vshrnqq_n_s16(int16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sb[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
int8x64_t vshrnoq_n_s16(int16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sb[i] = a_var.sh[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}

uint16x4_t vshrn_n_u32(uint32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vshrndq_n_u32(uint32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vshrnqq_n_u32(uint32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vshrnoq_n_u32(uint32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.us[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vshrn_n_s64(int64x2_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vshrndq_n_s64(int64x4_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vshrnqq_n_s64(int64x8_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vshrnoq_n_s64(int64x16_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.sd[i] >> n;
    }
    o.id = fake_neon::new_operation("SHRN", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vqshrn_n_s32(int32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        int32_t temp = a_var.ss[i] >> n;
        o_var.sh[i] = SATQ_S16(temp);
    }
    o.id = fake_neon::new_operation("SQSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vqshrndq_n_s32(int32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        int32_t temp = a_var.ss[i] >> n;
        o_var.sh[i] = SATQ_S16(temp);
    }
    o.id = fake_neon::new_operation("SQSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vqshrnqq_n_s32(int32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        int32_t temp = a_var.ss[i] >> n;
        o_var.sh[i] = SATQ_S16(temp);
    }
    o.id = fake_neon::new_operation("SQSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vqshrnoq_n_s32(int32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        int32_t temp = a_var.ss[i] >> n;
        o_var.sh[i] = SATQ_S16(temp);
    }
    o.id = fake_neon::new_operation("SQSHRN", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vqrshrn_n_s32(int32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    int32_t round_const = 1 << (n - 1);
    for (int i = 0; i < 4; i++) {
        int32_t temp = (a_var.ss[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(temp);
    }
    o.id = fake_neon::new_operation("SQRSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vqrshrndq_n_s32(int32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    int32_t round_const = 1 << (n - 1);
    for (int i = 0; i < 8; i++) {
        int32_t temp = (a_var.ss[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(temp);
    }
    o.id = fake_neon::new_operation("SQRSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vqrshrnqq_n_s32(int32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    int32_t round_const = 1 << (n - 1);
    for (int i = 0; i < 16; i++) {
        int32_t temp = (a_var.ss[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(temp);
    }
    o.id = fake_neon::new_operation("SQRSHRN", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vqrshrnoq_n_s32(int32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    int32_t round_const = 1 << (n - 1);
    for (int i = 0; i < 32; i++) {
        int32_t temp = (a_var.ss[i] + round_const) >> n;
        o_var.sh[i] = SATQ_S16(temp);
    }
    o.id = fake_neon::new_operation("SQRSHRN", a.id, a.type, o_var, o.type);
    return o;
}

uint16x4_t vshl_n_u16(uint16x4_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vshlq_n_u16(uint16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vshldq_n_u16(uint16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vshlqq_n_u16(uint16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
uint16x64_t vshloq_n_u16(uint16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vshl_n_s16(int16x4_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vshlq_n_s16(int16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vshldq_n_s16(int16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vshlqq_n_s16(int16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
int16x64_t vshloq_n_s16(int16x64_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}

uint32x2_t vshl_n_u32(uint32x2_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
uint32x4_t vshlq_n_u32(uint32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
uint32x8_t vshldq_n_u32(uint32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
uint32x16_t vshlqq_n_u32(uint32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
uint32x32_t vshloq_n_u32(uint32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vshl_n_s32(int32x2_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vshlq_n_s32(int32x4_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vshldq_n_s32(int32x8_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vshlqq_n_s32(int32x16_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vshloq_n_s32(int32x32_t a, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] << n;
    }
    o.id = fake_neon::new_operation("SHL", a.id, a.type, o_var, o.type);
    return o;
}

int32x4_t vshll_n_s16(int16x4_t a, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = ((int32_t)(a_var.sh[i])) << n;
    }
    o.id = fake_neon::new_operation("SSHLL", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vshllq_n_s16(int16x8_t a, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = ((int32_t)(a_var.sh[i])) << n;
    }
    o.id = fake_neon::new_operation("SSHLL", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vshlldq_n_s16(int16x16_t a, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = ((int32_t)(a_var.sh[i])) << n;
    }
    o.id = fake_neon::new_operation("SSHLL", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vshllqq_n_s16(int16x32_t a, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = ((int32_t)(a_var.sh[i])) << n;
    }
    o.id = fake_neon::new_operation("SSHLL", a.id, a.type, o_var, o.type);
    return o;
}

int64x1_t vrshl_s64(int64x1_t a, int64x1_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            int64_t round_const = 1 << (shift - 1);
            o_var.sd[i] = (a_var.sd[i] + round_const) >> shift;
        }
    }
    o.id = fake_neon::new_operation("SRSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x2_t vrshlq_s64(int64x2_t a, int64x2_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            int64_t round_const = 1 << (shift - 1);
            o_var.sd[i] = (a_var.sd[i] + round_const) >> shift;
        }
    }
    o.id = fake_neon::new_operation("SRSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x4_t vrshldq_s64(int64x4_t a, int64x4_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            int64_t round_const = 1 << (shift - 1);
            o_var.sd[i] = (a_var.sd[i] + round_const) >> shift;
        }
    }
    o.id = fake_neon::new_operation("SRSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x8_t vrshlqq_s64(int64x8_t a, int64x8_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            int64_t round_const = 1 << (shift - 1);
            o_var.sd[i] = (a_var.sd[i] + round_const) >> shift;
        }
    }
    o.id = fake_neon::new_operation("SRSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x16_t vrshloq_s64(int64x16_t a, int64x16_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            int64_t round_const = 1 << (shift - 1);
            o_var.sd[i] = (a_var.sd[i] + round_const) >> shift;
        }
    }
    o.id = fake_neon::new_operation("SRSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int64x1_t vshl_s64(int64x1_t a, int64x1_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            o_var.sd[i] = a_var.sd[i] >> shift;
        }
    }
    o.id = fake_neon::new_operation("SSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x2_t vshlq_s64(int64x2_t a, int64x2_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift >= 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
            // printf("[neon] %ld << %d = %ld\n", a_var.sd[i], shift, o_var.sd[i]);
        } else {
            shift = -shift;
            o_var.sd[i] = a_var.sd[i] >> shift;
            // printf("[neon] %ld >> %d = %ld\n", a_var.sd[i], shift, o_var.sd[i]);
        }
    }
    o.id = fake_neon::new_operation("SSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x4_t vshldq_s64(int64x4_t a, int64x4_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            o_var.sd[i] = a_var.sd[i] >> shift;
        }
    }
    o.id = fake_neon::new_operation("SSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x8_t vshlqq_s64(int64x8_t a, int64x8_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            o_var.sd[i] = a_var.sd[i] >> shift;
        }
    }
    o.id = fake_neon::new_operation("SSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x16_t vshloq_s64(int64x16_t a, int64x16_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        int8_t shift = b_var.sb[i * 8];
        if (shift > 0) {
            o_var.sd[i] = a_var.sd[i] << shift;
        } else {
            shift = -shift;
            o_var.sd[i] = a_var.sd[i] >> shift;
        }
    }
    o.id = fake_neon::new_operation("SSHL", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

// Shift and Accumulate
uint16x4_t vsra_n_u16(uint16x4_t a, uint16x4_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] + (b_var.uh[i] >> n);
    }
    o.id = fake_neon::new_operation("USRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vsraq_n_u16(uint16x8_t a, uint16x8_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] + (b_var.uh[i] >> n);
    }
    o.id = fake_neon::new_operation("USRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vsradq_n_u16(uint16x16_t a, uint16x16_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] + (b_var.uh[i] >> n);
    }
    o.id = fake_neon::new_operation("USRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vsraqq_n_u16(uint16x32_t a, uint16x32_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] + (b_var.uh[i] >> n);
    }
    o.id = fake_neon::new_operation("USRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vsraoq_n_u16(uint16x64_t a, uint16x64_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] + (b_var.uh[i] >> n);
    }
    o.id = fake_neon::new_operation("USRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vsra_n_s16(int16x4_t a, int16x4_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] + (b_var.sh[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vsraq_n_s16(int16x8_t a, int16x8_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] + (b_var.sh[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vsradq_n_s16(int16x16_t a, int16x16_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] + (b_var.sh[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vsraqq_n_s16(int16x32_t a, int16x32_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] + (b_var.sh[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vsraoq_n_s16(int16x64_t a, int16x64_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] + (b_var.sh[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int64x1_t vsra_n_s64(int64x1_t a, int64x1_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = a_var.sd[i] + (b_var.sd[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x2_t vsraq_n_s64(int64x2_t a, int64x2_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = a_var.sd[i] + (b_var.sd[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x4_t vsradq_n_s64(int64x4_t a, int64x4_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = a_var.sd[i] + (b_var.sd[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x8_t vsraqq_n_s64(int64x8_t a, int64x8_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = a_var.sd[i] + (b_var.sd[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int64x16_t vsraoq_n_s64(int64x16_t a, int64x16_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = a_var.sd[i] + (b_var.sd[i] >> n);
    }
    o.id = fake_neon::new_operation("SSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vrsra_n_u16(uint16x4_t a, uint16x4_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.uh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vrsraq_n_u16(uint16x8_t a, uint16x8_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.uh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vrsradq_n_u16(uint16x16_t a, uint16x16_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.uh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vrsraqq_n_u16(uint16x32_t a, uint16x32_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.uh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vrsraoq_n_u16(uint16x64_t a, uint16x64_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    uint16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] + ((b_var.uh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vrsra_n_s16(int16x4_t a, int16x4_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] + ((b_var.sh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vrsraq_n_s16(int16x8_t a, int16x8_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] + ((b_var.sh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vrsradq_n_s16(int16x16_t a, int16x16_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] + ((b_var.sh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vrsraqq_n_s16(int16x32_t a, int16x32_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] + ((b_var.sh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vrsraoq_n_s16(int16x64_t a, int16x64_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    int16_t round_const = (1 << (n - 1));
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] + ((b_var.sh[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vrsra_n_u32(uint32x2_t a, uint32x2_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] + ((b_var.us[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vrsraq_n_u32(uint32x4_t a, uint32x4_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] + ((b_var.us[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vrsradq_n_u32(uint32x8_t a, uint32x8_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] + ((b_var.us[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vrsraqq_n_u32(uint32x16_t a, uint32x16_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] + ((b_var.us[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vrsraoq_n_u32(uint32x32_t a, uint32x32_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    uint32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] + ((b_var.us[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("URSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vrsra_n_s32(int32x2_t a, int32x2_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i] + ((b_var.ss[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vrsraq_n_s32(int32x4_t a, int32x4_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i] + ((b_var.ss[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vrsradq_n_s32(int32x8_t a, int32x8_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i] + ((b_var.ss[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vrsraqq_n_s32(int32x16_t a, int32x16_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i] + ((b_var.ss[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vrsraoq_n_s32(int32x32_t a, int32x32_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    int32_t round_const = (1 << (n - 1));
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i] + ((b_var.ss[i] + round_const) >> n);
    }
    o.id = fake_neon::new_operation("SRSRA", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

// Negate
int32x2_t vneg_s32(int32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = -(a_var.ss[i]);
    }
    o.id = fake_neon::new_operation("NEG", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vnegq_s32(int32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = -(a_var.ss[i]);
    }
    o.id = fake_neon::new_operation("NEG", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vnegdq_s32(int32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = -(a_var.ss[i]);
    }
    o.id = fake_neon::new_operation("NEG", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vnegqq_s32(int32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = -(a_var.ss[i]);
    }
    o.id = fake_neon::new_operation("NEG", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vnegoq_s32(int32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = -(a_var.ss[i]);
    }
    o.id = fake_neon::new_operation("NEG", a.id, a.type, o_var, o.type);
    return o;
}

// Re-interpret
uint16x4_t vreinterpret_u16_s16(int16x4_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vreinterpretq_u16_s16(int16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vreinterpretdq_u16_s16(int16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vreinterpretqq_u16_s16(int16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x64_t vreinterpretoq_u16_s16(int16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vreinterpret_s16_u16(uint16x4_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vreinterpretq_s16_u16(uint16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vreinterpretdq_s16_u16(uint16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vreinterpretqq_s16_u16(uint16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int16x64_t vreinterpretoq_s16_u16(uint16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vreinterpret_s32_u32(uint32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vreinterpretq_s32_u32(uint32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vreinterpretdq_s32_u32(uint32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vreinterpretqq_s32_u32(uint32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vreinterpretoq_s32_u32(uint32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

uint32x2_t vreinterpret_u32_s32(int32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x4_t vreinterpretq_u32_s32(int32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x8_t vreinterpretdq_u32_s32(int32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x16_t vreinterpretqq_u32_s32(int32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x32_t vreinterpretoq_u32_s32(int32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

uint32x2_t vreinterpret_u32_f32(float32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x4_t vreinterpretq_u32_f32(float32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x8_t vreinterpretdq_u32_f32(float32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x16_t vreinterpretqq_u32_f32(float32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x32_t vreinterpretoq_u32_f32(float32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

float32x2_t vreinterpret_f32_u32(uint32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
float32x4_t vreinterpretq_f32_u32(uint32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
float32x8_t vreinterpretdq_f32_u32(uint32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
float32x16_t vreinterpretqq_f32_u32(uint32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
float32x32_t vreinterpretoq_f32_u32(uint32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.fs[i] = a_var.fs[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

uint32x2_t vreinterpret_u32_u8(uint8x8_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x4_t vreinterpretq_u32_u8(uint8x16_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x8_t vreinterpretdq_u32_u8(uint8x32_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x16_t vreinterpretqq_u32_u8(uint8x64_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint32x32_t vreinterpretoq_u32_u8(uint8x128_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

uint8x8_t vreinterpret_u8_u32(uint32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x16_t vreinterpretq_u8_u32(uint32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x32_t vreinterpretdq_u8_u32(uint32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x64_t vreinterpretqq_u8_u32(uint32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint8x128_t vreinterpretoq_u8_u32(uint32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = a_var.ub[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

uint16x4_t vreinterpret_u16_u32(uint32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x8_t vreinterpretq_u16_u32(uint32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x16_t vreinterpretdq_u16_u32(uint32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x32_t vreinterpretqq_u16_u32(uint32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
uint16x64_t vreinterpretoq_u16_u32(uint32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

int16x4_t vreinterpret_s16_s32(int32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int16x8_t vreinterpretq_s16_s32(int32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int16x16_t vreinterpretdq_s16_s32(int32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int16x32_t vreinterpretqq_s16_s32(int32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int16x64_t vreinterpretoq_s16_s32(int32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vreinterpret_s32_s16(int16x4_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vreinterpretq_s32_s16(int16x8_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vreinterpretdq_s32_s16(int16x16_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vreinterpretqq_s32_s16(int16x32_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vreinterpretoq_s32_s16(int16x64_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

int64x1_t vreinterpret_s64_s32(int32x2_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int64x1_t o;
    __64bit_var o_var;
    for (int i = 0; i < 1; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int64x2_t vreinterpretq_s64_s32(int32x4_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int64x2_t o;
    __128bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int64x4_t vreinterpretdq_s64_s32(int32x8_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int64x4_t o;
    __256bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int64x8_t vreinterpretqq_s64_s32(int32x16_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int64x8_t o;
    __512bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int64x16_t vreinterpretoq_s64_s32(int32x32_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int64x16_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sd[i] = a_var.sd[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

int32x2_t vreinterpret_s32_s64(int64x1_t a) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x4_t vreinterpretq_s32_s64(int64x2_t a) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x8_t vreinterpretdq_s32_s64(int64x4_t a) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x16_t vreinterpretqq_s32_s64(int64x8_t a) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}
int32x32_t vreinterpretoq_s32_s64(int64x16_t a) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ss[i] = a_var.ss[i];
    }
    o.id = fake_neon::new_operation("NOP", a.id, a.type, o_var, o.type);
    return o;
}

// Bitwise
// Compare less than or equal
uint16x4_t vcle_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.uh[i] <= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vcleq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.uh[i] <= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vcledq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.uh[i] <= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vcleqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.uh[i] <= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vcleoq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.uh[i] <= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vcle_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.sh[i] <= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vcleq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.sh[i] <= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vcledq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.sh[i] <= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vcleqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.sh[i] <= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vcleoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.sh[i] <= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vceq_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.uh[i] == b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vceqq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.uh[i] == b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vceqdq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.uh[i] == b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vceqqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.uh[i] == b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vceqoq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.uh[i] == b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vceq_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.sh[i] == b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vceqq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.sh[i] == b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vceqdq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.sh[i] == b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vceqqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.sh[i] == b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vceqoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.sh[i] == b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vceq_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = (a_var.fs[i] == b_var.fs[i]) ? 0xffffffff : 0x00000000;
    }
    o.id = fake_neon::new_operation("FCMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x4_t vceqq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = (a_var.fs[i] == b_var.fs[i]) ? 0xffffffff : 0x00000000;
    }
    o.id = fake_neon::new_operation("FCMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x8_t vceqdq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = (a_var.fs[i] == b_var.fs[i]) ? 0xffffffff : 0x00000000;
    }
    o.id = fake_neon::new_operation("FCMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x16_t vceqqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = (a_var.fs[i] == b_var.fs[i]) ? 0xffffffff : 0x00000000;
    }
    o.id = fake_neon::new_operation("FCMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x32_t vceqoq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = (a_var.fs[i] == b_var.fs[i]) ? 0xffffffff : 0x00000000;
    }
    o.id = fake_neon::new_operation("FCMEQ", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vcge_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.uh[i] >= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vcgeq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.uh[i] >= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vcgedq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.uh[i] >= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vcgeqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.uh[i] >= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vcgeoq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.uh[i] >= b_var.uh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMHS", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vcge_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.sh[i] >= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vcgeq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.sh[i] >= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vcgedq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.sh[i] >= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vcgeqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.sh[i] >= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vcgeoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.sh[i] >= b_var.sh[i]) ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMGE", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

// Bitwise
uint8x8_t vand_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = a_var.ub[i] & b_var.ub[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x16_t vandq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = a_var.ub[i] & b_var.ub[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x32_t vanddq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = a_var.ub[i] & b_var.ub[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x64_t vandqq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = a_var.ub[i] & b_var.ub[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x128_t vandoq_u8(uint8x128_t a, uint8x128_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = a_var.ub[i] & b_var.ub[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint16x4_t vand_u16(uint16x4_t a, uint16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = a_var.uh[i] & b_var.uh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x8_t vandq_u16(uint16x8_t a, uint16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = a_var.uh[i] & b_var.uh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x16_t vanddq_u16(uint16x16_t a, uint16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = a_var.uh[i] & b_var.uh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x32_t vandqq_u16(uint16x32_t a, uint16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = a_var.uh[i] & b_var.uh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint16x64_t vandoq_u16(uint16x64_t a, uint16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = a_var.uh[i] & b_var.uh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vand_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] & b_var.sh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vandq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] & b_var.sh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vanddq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] & b_var.sh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vandqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] & b_var.sh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vandoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] & b_var.sh[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vand_u32(uint32x2_t a, uint32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] & b_var.us[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vandq_u32(uint32x4_t a, uint32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] & b_var.us[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vanddq_u32(uint32x8_t a, uint32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] & b_var.us[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vandqq_u32(uint32x16_t a, uint32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] & b_var.us[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vandoq_u32(uint32x32_t a, uint32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] & b_var.us[i];
    }
    o.id = fake_neon::new_operation("AND", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vorr_u32(uint32x2_t a, uint32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] | b_var.us[i];
    }
    o.id = fake_neon::new_operation("ORR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vorrq_u32(uint32x4_t a, uint32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] | b_var.us[i];
    }
    o.id = fake_neon::new_operation("ORR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vorrdq_u32(uint32x8_t a, uint32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] | b_var.us[i];
    }
    o.id = fake_neon::new_operation("ORR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vorrqq_u32(uint32x16_t a, uint32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] | b_var.us[i];
    }
    o.id = fake_neon::new_operation("ORR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vorroq_u32(uint32x32_t a, uint32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] | b_var.us[i];
    }
    o.id = fake_neon::new_operation("ORR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t veor_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.sh[i] = a_var.sh[i] ^ b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EOR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t veorq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.sh[i] = a_var.sh[i] ^ b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EOR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t veordq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.sh[i] = a_var.sh[i] ^ b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EOR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t veorqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.sh[i] = a_var.sh[i] ^ b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EOR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t veoroq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.sh[i] = a_var.sh[i] ^ b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EOR", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

uint32x2_t vbic_u32(uint32x2_t a, uint32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2; i++) {
        o_var.us[i] = a_var.us[i] & ~(b_var.us[i]);
    }
    o.id = fake_neon::new_operation("BIC", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x4_t vbicq_u32(uint32x4_t a, uint32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.us[i] = a_var.us[i] & ~(b_var.us[i]);
    }
    o.id = fake_neon::new_operation("BIC", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x8_t vbicdq_u32(uint32x8_t a, uint32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.us[i] = a_var.us[i] & ~(b_var.us[i]);
    }
    o.id = fake_neon::new_operation("BIC", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x16_t vbicqq_u32(uint32x16_t a, uint32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.us[i] = a_var.us[i] & ~(b_var.us[i]);
    }
    o.id = fake_neon::new_operation("BIC", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint32x32_t vbicoq_u32(uint32x32_t a, uint32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.us[i] = a_var.us[i] & ~(b_var.us[i]);
    }
    o.id = fake_neon::new_operation("BIC", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vtst_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4; i++) {
        o_var.uh[i] = (a_var.sh[i] & b_var.sh[i]) != 0 ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMTST", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vtstq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.uh[i] = (a_var.sh[i] & b_var.sh[i]) != 0 ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMTST", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vtstdq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.uh[i] = (a_var.sh[i] & b_var.sh[i]) != 0 ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMTST", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vtstqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.uh[i] = (a_var.sh[i] & b_var.sh[i]) != 0 ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMTST", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vtstoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.uh[i] = (a_var.sh[i] & b_var.sh[i]) != 0 ? 0xffff : 0x0000;
    }
    o.id = fake_neon::new_operation("CMTST", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

// Select
uint8x8_t vbsl_u8(uint8x8_t a, uint8x8_t b, uint8x8_t c) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    __64bit_var c_var = fake_neon::get_value_64(c.id, c.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8; i++) {
        o_var.ub[i] = (a_var.ub[i] & b_var.ub[i]) | (~(a_var.ub[i]) & c_var.ub[i]);
    }
    o.id = fake_neon::new_operation("BSL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint8x16_t vbslq_u8(uint8x16_t a, uint8x16_t b, uint8x16_t c) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    __128bit_var c_var = fake_neon::get_value_128(c.id, c.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16; i++) {
        o_var.ub[i] = (a_var.ub[i] & b_var.ub[i]) | (~(a_var.ub[i]) & c_var.ub[i]);
    }
    o.id = fake_neon::new_operation("BSL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint8x32_t vbsldq_u8(uint8x32_t a, uint8x32_t b, uint8x32_t c) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    __256bit_var c_var = fake_neon::get_value_256(c.id, c.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32; i++) {
        o_var.ub[i] = (a_var.ub[i] & b_var.ub[i]) | (~(a_var.ub[i]) & c_var.ub[i]);
    }
    o.id = fake_neon::new_operation("BSL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint8x64_t vbslqq_u8(uint8x64_t a, uint8x64_t b, uint8x64_t c) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    __512bit_var c_var = fake_neon::get_value_512(c.id, c.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64; i++) {
        o_var.ub[i] = (a_var.ub[i] & b_var.ub[i]) | (~(a_var.ub[i]) & c_var.ub[i]);
    }
    o.id = fake_neon::new_operation("BSL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}
uint8x128_t vbsloq_u8(uint8x128_t a, uint8x128_t b, uint8x128_t c) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    __1024bit_var c_var = fake_neon::get_value_1024(c.id, c.type);
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128; i++) {
        o_var.ub[i] = (a_var.ub[i] & b_var.ub[i]) | (~(a_var.ub[i]) & c_var.ub[i]);
    }
    o.id = fake_neon::new_operation("BSL", a.id, a.type, b.id, b.type, c.id, c.type, o_var, o.type);
    return o;
}

// Extract
uint8x8_t vext_u8(uint8x8_t a, uint8x8_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint8x8_t o;
    __64bit_var o_var;
    for (int i = 0; i < 8 - n; i++) {
        o_var.ub[i] = a_var.ub[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ub[8 - n + i] = b_var.ub[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x16_t vextq_u8(uint8x16_t a, uint8x16_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint8x16_t o;
    __128bit_var o_var;
    for (int i = 0; i < 16 - n; i++) {
        o_var.ub[i] = a_var.ub[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ub[16 - n + i] = b_var.ub[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x32_t vextdq_u8(uint8x32_t a, uint8x32_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint8x32_t o;
    __256bit_var o_var;
    for (int i = 0; i < 32 - n; i++) {
        o_var.ub[i] = a_var.ub[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ub[32 - n + i] = b_var.ub[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x64_t vextqq_u8(uint8x64_t a, uint8x64_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint8x64_t o;
    __512bit_var o_var;
    for (int i = 0; i < 64 - n; i++) {
        o_var.ub[i] = a_var.ub[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ub[64 - n + i] = b_var.ub[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
uint8x128_t vextoq_u8(uint8x128_t a, uint8x128_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint8x128_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 128 - n; i++) {
        o_var.ub[i] = a_var.ub[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ub[128 - n + i] = b_var.ub[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int16x4_t vext_s16(int16x4_t a, int16x4_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4_t o;
    __64bit_var o_var;
    for (int i = 0; i < 4 - n; i++) {
        o_var.sh[i] = a_var.sh[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.sh[4 - n + i] = b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x8_t vextq_s16(int16x8_t a, int16x8_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8_t o;
    __128bit_var o_var;
    for (int i = 0; i < 8 - n; i++) {
        o_var.sh[i] = a_var.sh[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.sh[8 - n + i] = b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x16_t vextdq_s16(int16x16_t a, int16x16_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16_t o;
    __256bit_var o_var;
    for (int i = 0; i < 16 - n; i++) {
        o_var.sh[i] = a_var.sh[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.sh[16 - n + i] = b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x32_t vextqq_s16(int16x32_t a, int16x32_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32_t o;
    __512bit_var o_var;
    for (int i = 0; i < 32 - n; i++) {
        o_var.sh[i] = a_var.sh[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.sh[32 - n + i] = b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int16x64_t vextoq_s16(int16x64_t a, int16x64_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 64 - n; i++) {
        o_var.sh[i] = a_var.sh[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.sh[64 - n + i] = b_var.sh[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

int32x2_t vext_s32(int32x2_t a, int32x2_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2 - n; i++) {
        o_var.ss[i] = a_var.ss[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ss[2 - n + i] = b_var.ss[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x4_t vextq_s32(int32x4_t a, int32x4_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4 - n; i++) {
        o_var.ss[i] = a_var.ss[n + i];
        // printf("vext[%d] = a[%d]\n", i, n + i);
    }
    for (int i = 0; i < n; i++) {
        o_var.ss[4 - n + i] = b_var.ss[i];
        // printf("vext[%d] = b[%d]\n", 4 - n + i, i);
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x8_t vextdq_s32(int32x8_t a, int32x8_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8 - n; i++) {
        o_var.ss[i] = a_var.ss[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ss[8 - n + i] = b_var.ss[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x16_t vextqq_s32(int32x16_t a, int32x16_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16 - n; i++) {
        o_var.ss[i] = a_var.ss[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ss[16 - n + i] = b_var.ss[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
int32x32_t vextoq_s32(int32x32_t a, int32x32_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32 - n; i++) {
        o_var.ss[i] = a_var.ss[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.ss[32 - n + i] = b_var.ss[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

float32x2_t vext_f32(float32x2_t a, float32x2_t b, const int n) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2_t o;
    __64bit_var o_var;
    for (int i = 0; i < 2 - n; i++) {
        o_var.fs[i] = a_var.fs[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.fs[2 - n + i] = b_var.fs[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x4_t vextq_f32(float32x4_t a, float32x4_t b, const int n) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4_t o;
    __128bit_var o_var;
    for (int i = 0; i < 4 - n; i++) {
        o_var.fs[i] = a_var.fs[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.fs[4 - n + i] = b_var.fs[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x8_t vextdq_f32(float32x8_t a, float32x8_t b, const int n) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8_t o;
    __256bit_var o_var;
    for (int i = 0; i < 8 - n; i++) {
        o_var.fs[i] = a_var.fs[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.fs[8 - n + i] = b_var.fs[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x16_t vextqq_f32(float32x16_t a, float32x16_t b, const int n) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16_t o;
    __512bit_var o_var;
    for (int i = 0; i < 16 - n; i++) {
        o_var.fs[i] = a_var.fs[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.fs[16 - n + i] = b_var.fs[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}
float32x32_t vextoq_f32(float32x32_t a, float32x32_t b, const int n) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32_t o;
    __1024bit_var o_var;
    for (int i = 0; i < 32 - n; i++) {
        o_var.fs[i] = a_var.fs[n + i];
    }
    for (int i = 0; i < n; i++) {
        o_var.fs[32 - n + i] = b_var.fs[i];
    }
    o.id = fake_neon::new_operation("EXT", a.id, a.type, b.id, b.type, o_var, o.type);
    return o;
}

// Shuffle
int32x2_t vrev64_s32(int32x2_t vec) {
    __64bit_var vec_var = fake_neon::get_value_64(vec.id, vec.type);
    uint32x2_t o;
    __64bit_var o_var;
    int reg_size = 64;
    int element_size = 32;
    int container_size = 64;
    int elements_per_container = container_size / element_size;
    int containers_per_register = reg_size / container_size;
    int element = 0;
    for (int i = 0; i < containers_per_register; i++) {
        int rev_element = element + elements_per_container - 1;
        for (int j = 0; j < elements_per_container; j++) {
            o_var.ss[rev_element--] = vec_var.ss[element++];
        }
    }
    o.id = fake_neon::new_operation("REV64", vec.id, vec.type, o_var, o.type);
    return o;
}
int32x4_t vrev64q_s32(int32x4_t vec) {
    __128bit_var vec_var = fake_neon::get_value_128(vec.id, vec.type);
    uint32x4_t o;
    __128bit_var o_var;
    int reg_size = 128;
    int element_size = 32;
    int container_size = 64;
    int elements_per_container = container_size / element_size;
    int containers_per_register = reg_size / container_size;
    int element = 0;
    for (int i = 0; i < containers_per_register; i++) {
        int rev_element = element + elements_per_container - 1;
        for (int j = 0; j < elements_per_container; j++) {
            o_var.ss[rev_element--] = vec_var.ss[element++];
            // printf("vrev64[%d] = in[%d]\n", rev_element + 1, element - 1);
        }
    }
    o.id = fake_neon::new_operation("REV64", vec.id, vec.type, o_var, o.type);
    return o;
}
int32x8_t vrev64dq_s32(int32x8_t vec) {
    __256bit_var vec_var = fake_neon::get_value_256(vec.id, vec.type);
    uint32x8_t o;
    __256bit_var o_var;
    int reg_size = 256;
    int element_size = 32;
    int container_size = 64;
    int elements_per_container = container_size / element_size;
    int containers_per_register = reg_size / container_size;
    int element = 0;
    for (int i = 0; i < containers_per_register; i++) {
        int rev_element = element + elements_per_container - 1;
        for (int j = 0; j < elements_per_container; j++) {
            o_var.ss[rev_element--] = vec_var.ss[element++];
        }
    }
    o.id = fake_neon::new_operation("REV64", vec.id, vec.type, o_var, o.type);
    return o;
}
int32x16_t vrev64qq_s32(int32x16_t vec) {
    __512bit_var vec_var = fake_neon::get_value_512(vec.id, vec.type);
    uint32x16_t o;
    __512bit_var o_var;
    int reg_size = 512;
    int element_size = 32;
    int container_size = 64;
    int elements_per_container = container_size / element_size;
    int containers_per_register = reg_size / container_size;
    int element = 0;
    for (int i = 0; i < containers_per_register; i++) {
        int rev_element = element + elements_per_container - 1;
        for (int j = 0; j < elements_per_container; j++) {
            o_var.ss[rev_element--] = vec_var.ss[element++];
        }
    }
    o.id = fake_neon::new_operation("REV64", vec.id, vec.type, o_var, o.type);
    return o;
}
int32x32_t vrev64oq_s32(int32x32_t vec) {
    __1024bit_var vec_var = fake_neon::get_value_1024(vec.id, vec.type);
    uint32x32_t o;
    __1024bit_var o_var;
    int reg_size = 1024;
    int element_size = 32;
    int container_size = 64;
    int elements_per_container = container_size / element_size;
    int containers_per_register = reg_size / container_size;
    int element = 0;
    for (int i = 0; i < containers_per_register; i++) {
        int rev_element = element + elements_per_container - 1;
        for (int j = 0; j < elements_per_container; j++) {
            o_var.ss[rev_element--] = vec_var.ss[element++];
        }
    }
    o.id = fake_neon::new_operation("REV64", vec.id, vec.type, o_var, o.type);
    return o;
}

int16x4x2_t vtrn_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4x2_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    for (int i = 0; i < 2; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[2 * i];
        o_var0.sh[2 * i + 1] = b_var.sh[2 * i];
        o_var1.sh[2 * i + 0] = a_var.sh[2 * i + 1];
        o_var1.sh[2 * i + 1] = b_var.sh[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int16x8x2_t vtrnq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8x2_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    for (int i = 0; i < 4; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[2 * i];
        o_var0.sh[2 * i + 1] = b_var.sh[2 * i];
        o_var1.sh[2 * i + 0] = a_var.sh[2 * i + 1];
        o_var1.sh[2 * i + 1] = b_var.sh[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int16x16x2_t vtrndq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16x2_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    for (int i = 0; i < 8; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[2 * i];
        o_var0.sh[2 * i + 1] = b_var.sh[2 * i];
        o_var1.sh[2 * i + 0] = a_var.sh[2 * i + 1];
        o_var1.sh[2 * i + 1] = b_var.sh[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int16x32x2_t vtrnqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32x2_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    for (int i = 0; i < 16; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[2 * i];
        o_var0.sh[2 * i + 1] = b_var.sh[2 * i];
        o_var1.sh[2 * i + 0] = a_var.sh[2 * i + 1];
        o_var1.sh[2 * i + 1] = b_var.sh[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int16x64x2_t vtrnoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64x2_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    for (int i = 0; i < 32; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[2 * i];
        o_var0.sh[2 * i + 1] = b_var.sh[2 * i];
        o_var1.sh[2 * i + 0] = a_var.sh[2 * i + 1];
        o_var1.sh[2 * i + 1] = b_var.sh[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}

int32x2x2_t vtrn_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2x2_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    for (int i = 0; i < 1; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[2 * i];
        o_var0.ss[2 * i + 1] = b_var.ss[2 * i];
        o_var1.ss[2 * i + 0] = a_var.ss[2 * i + 1];
        o_var1.ss[2 * i + 1] = b_var.ss[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int32x4x2_t vtrnq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4x2_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    for (int i = 0; i < 2; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[2 * i];
        o_var0.ss[2 * i + 1] = b_var.ss[2 * i];
        o_var1.ss[2 * i + 0] = a_var.ss[2 * i + 1];
        o_var1.ss[2 * i + 1] = b_var.ss[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int32x8x2_t vtrndq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8x2_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    for (int i = 0; i < 4; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[2 * i];
        o_var0.ss[2 * i + 1] = b_var.ss[2 * i];
        o_var1.ss[2 * i + 0] = a_var.ss[2 * i + 1];
        o_var1.ss[2 * i + 1] = b_var.ss[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int32x16x2_t vtrnqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16x2_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    for (int i = 0; i < 8; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[2 * i];
        o_var0.ss[2 * i + 1] = b_var.ss[2 * i];
        o_var1.ss[2 * i + 0] = a_var.ss[2 * i + 1];
        o_var1.ss[2 * i + 1] = b_var.ss[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int32x32x2_t vtrnoq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32x2_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    for (int i = 0; i < 16; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[2 * i];
        o_var0.ss[2 * i + 1] = b_var.ss[2 * i];
        o_var1.ss[2 * i + 0] = a_var.ss[2 * i + 1];
        o_var1.ss[2 * i + 1] = b_var.ss[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("TRN1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("TRN2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}

uint8x8x2_t vzip_u8(uint8x8_t a, uint8x8_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    uint8x8x2_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    for (int i = 0; i < 4; i++) {
        o_var0.ub[2 * i + 0] = a_var.ub[i];
        o_var0.ub[2 * i + 1] = b_var.ub[i];
        o_var1.ub[2 * i + 0] = a_var.ub[i + 4];
        o_var1.ub[2 * i + 1] = b_var.ub[i + 4];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
uint8x16x2_t vzipq_u8(uint8x16_t a, uint8x16_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    uint8x16x2_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    for (int i = 0; i < 8; i++) {
        o_var0.ub[2 * i + 0] = a_var.ub[i];
        o_var0.ub[2 * i + 1] = b_var.ub[i];
        o_var1.ub[2 * i + 0] = a_var.ub[i + 8];
        o_var1.ub[2 * i + 1] = b_var.ub[i + 8];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
uint8x32x2_t vzipdq_u8(uint8x32_t a, uint8x32_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    uint8x32x2_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    for (int i = 0; i < 16; i++) {
        o_var0.ub[2 * i + 0] = a_var.ub[i];
        o_var0.ub[2 * i + 1] = b_var.ub[i];
        o_var1.ub[2 * i + 0] = a_var.ub[i + 16];
        o_var1.ub[2 * i + 1] = b_var.ub[i + 16];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
uint8x64x2_t vzipqq_u8(uint8x64_t a, uint8x64_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    uint8x64x2_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    for (int i = 0; i < 32; i++) {
        o_var0.ub[2 * i + 0] = a_var.ub[i];
        o_var0.ub[2 * i + 1] = b_var.ub[i];
        o_var1.ub[2 * i + 0] = a_var.ub[i + 32];
        o_var1.ub[2 * i + 1] = b_var.ub[i + 32];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
uint8x128x2_t vzipoq_u8(uint8x128_t a, uint8x128_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    uint8x128x2_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    for (int i = 0; i < 64; i++) {
        o_var0.ub[2 * i + 0] = a_var.ub[i];
        o_var0.ub[2 * i + 1] = b_var.ub[i];
        o_var1.ub[2 * i + 0] = a_var.ub[i + 64];
        o_var1.ub[2 * i + 1] = b_var.ub[i + 64];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}

int16x4x2_t vzip_s16(int16x4_t a, int16x4_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int16x4x2_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    for (int i = 0; i < 2; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[i];
        o_var0.sh[2 * i + 1] = b_var.sh[i];
        o_var1.sh[2 * i + 0] = a_var.sh[i + 2];
        o_var1.sh[2 * i + 1] = b_var.sh[i + 2];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int16x8x2_t vzipq_s16(int16x8_t a, int16x8_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int16x8x2_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    for (int i = 0; i < 4; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[i];
        o_var0.sh[2 * i + 1] = b_var.sh[i];
        o_var1.sh[2 * i + 0] = a_var.sh[i + 4];
        o_var1.sh[2 * i + 1] = b_var.sh[i + 4];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int16x16x2_t vzipdq_s16(int16x16_t a, int16x16_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int16x16x2_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    for (int i = 0; i < 8; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[i];
        o_var0.sh[2 * i + 1] = b_var.sh[i];
        o_var1.sh[2 * i + 0] = a_var.sh[i + 8];
        o_var1.sh[2 * i + 1] = b_var.sh[i + 8];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int16x32x2_t vzipqq_s16(int16x32_t a, int16x32_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int16x32x2_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    for (int i = 0; i < 16; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[i];
        o_var0.sh[2 * i + 1] = b_var.sh[i];
        o_var1.sh[2 * i + 0] = a_var.sh[i + 16];
        o_var1.sh[2 * i + 1] = b_var.sh[i + 16];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int16x64x2_t vzipoq_s16(int16x64_t a, int16x64_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int16x64x2_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    for (int i = 0; i < 32; i++) {
        o_var0.sh[2 * i + 0] = a_var.sh[i];
        o_var0.sh[2 * i + 1] = b_var.sh[i];
        o_var1.sh[2 * i + 0] = a_var.sh[i + 32];
        o_var1.sh[2 * i + 1] = b_var.sh[i + 32];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}

int32x2x2_t vzip_s32(int32x2_t a, int32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    int32x2x2_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    for (int i = 0; i < 1; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[i];
        o_var0.ss[2 * i + 1] = b_var.ss[i];
        o_var1.ss[2 * i + 0] = a_var.ss[i + 1];
        o_var1.ss[2 * i + 1] = b_var.ss[i + 1];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int32x4x2_t vzipq_s32(int32x4_t a, int32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    int32x4x2_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    for (int i = 0; i < 2; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[i];
        o_var0.ss[2 * i + 1] = b_var.ss[i];
        o_var1.ss[2 * i + 0] = a_var.ss[i + 2];
        o_var1.ss[2 * i + 1] = b_var.ss[i + 2];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int32x8x2_t vzipdq_s32(int32x8_t a, int32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    int32x8x2_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    for (int i = 0; i < 4; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[i];
        o_var0.ss[2 * i + 1] = b_var.ss[i];
        o_var1.ss[2 * i + 0] = a_var.ss[i + 4];
        o_var1.ss[2 * i + 1] = b_var.ss[i + 4];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int32x16x2_t vzipqq_s32(int32x16_t a, int32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    int32x16x2_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    for (int i = 0; i < 8; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[i];
        o_var0.ss[2 * i + 1] = b_var.ss[i];
        o_var1.ss[2 * i + 0] = a_var.ss[i + 8];
        o_var1.ss[2 * i + 1] = b_var.ss[i + 8];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
int32x32x2_t vzipoq_s32(int32x32_t a, int32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    int32x32x2_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    for (int i = 0; i < 16; i++) {
        o_var0.ss[2 * i + 0] = a_var.ss[i];
        o_var0.ss[2 * i + 1] = b_var.ss[i];
        o_var1.ss[2 * i + 0] = a_var.ss[i + 16];
        o_var1.ss[2 * i + 1] = b_var.ss[i + 16];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}

float32x2x2_t vzip_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2x2_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    for (int i = 0; i < 1; i++) {
        o_var0.fs[2 * i + 0] = a_var.fs[i];
        o_var0.fs[2 * i + 1] = b_var.fs[i];
        o_var1.fs[2 * i + 0] = a_var.fs[i + 1];
        o_var1.fs[2 * i + 1] = b_var.fs[i + 1];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
float32x4x2_t vzipq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4x2_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    for (int i = 0; i < 2; i++) {
        o_var0.fs[2 * i + 0] = a_var.fs[i];
        o_var0.fs[2 * i + 1] = b_var.fs[i];
        o_var1.fs[2 * i + 0] = a_var.fs[i + 2];
        o_var1.fs[2 * i + 1] = b_var.fs[i + 2];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
float32x8x2_t vzipdq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8x2_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    for (int i = 0; i < 4; i++) {
        o_var0.fs[2 * i + 0] = a_var.fs[i];
        o_var0.fs[2 * i + 1] = b_var.fs[i];
        o_var1.fs[2 * i + 0] = a_var.fs[i + 4];
        o_var1.fs[2 * i + 1] = b_var.fs[i + 4];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
float32x16x2_t vzipqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16x2_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    for (int i = 0; i < 8; i++) {
        o_var0.fs[2 * i + 0] = a_var.fs[i];
        o_var0.fs[2 * i + 1] = b_var.fs[i];
        o_var1.fs[2 * i + 0] = a_var.fs[i + 8];
        o_var1.fs[2 * i + 1] = b_var.fs[i + 8];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
float32x32x2_t vzipoq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32x2_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    for (int i = 0; i < 16; i++) {
        o_var0.fs[2 * i + 0] = a_var.fs[i];
        o_var0.fs[2 * i + 1] = b_var.fs[i];
        o_var1.fs[2 * i + 0] = a_var.fs[i + 16];
        o_var1.fs[2 * i + 1] = b_var.fs[i + 16];
    }
    o.val[0].id = fake_neon::new_operation("ZIP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("ZIP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}

float32x2x2_t vuzp_f32(float32x2_t a, float32x2_t b) {
    __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
    __64bit_var b_var = fake_neon::get_value_64(b.id, b.type);
    float32x2x2_t o;
    __64bit_var o_var0;
    __64bit_var o_var1;
    for (int i = 0; i < 1; i++) {
        o_var0.fs[i + 0] = a_var.fs[2 * i + 0];
        o_var1.fs[i + 0] = a_var.fs[2 * i + 1];
        o_var0.fs[i + 1] = b_var.fs[2 * i + 0];
        o_var1.fs[i + 1] = b_var.fs[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("UZP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("UZP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
float32x4x2_t vuzpq_f32(float32x4_t a, float32x4_t b) {
    __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
    __128bit_var b_var = fake_neon::get_value_128(b.id, b.type);
    float32x4x2_t o;
    __128bit_var o_var0;
    __128bit_var o_var1;
    for (int i = 0; i < 2; i++) {
        o_var0.fs[i + 0] = a_var.fs[2 * i + 0];
        o_var1.fs[i + 0] = a_var.fs[2 * i + 1];
        o_var0.fs[i + 2] = b_var.fs[2 * i + 0];
        o_var1.fs[i + 2] = b_var.fs[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("UZP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("UZP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
float32x8x2_t vuzpdq_f32(float32x8_t a, float32x8_t b) {
    __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
    __256bit_var b_var = fake_neon::get_value_256(b.id, b.type);
    float32x8x2_t o;
    __256bit_var o_var0;
    __256bit_var o_var1;
    for (int i = 0; i < 4; i++) {
        o_var0.fs[i + 0] = a_var.fs[2 * i + 0];
        o_var1.fs[i + 0] = a_var.fs[2 * i + 1];
        o_var0.fs[i + 4] = b_var.fs[2 * i + 0];
        o_var1.fs[i + 4] = b_var.fs[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("UZP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("UZP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
float32x16x2_t vuzpqq_f32(float32x16_t a, float32x16_t b) {
    __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
    __512bit_var b_var = fake_neon::get_value_512(b.id, b.type);
    float32x16x2_t o;
    __512bit_var o_var0;
    __512bit_var o_var1;
    for (int i = 0; i < 8; i++) {
        o_var0.fs[i + 0] = a_var.fs[2 * i + 0];
        o_var1.fs[i + 0] = a_var.fs[2 * i + 1];
        o_var0.fs[i + 8] = b_var.fs[2 * i + 0];
        o_var1.fs[i + 8] = b_var.fs[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("UZP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("UZP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}
float32x32x2_t vuzpoq_f32(float32x32_t a, float32x32_t b) {
    __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
    __1024bit_var b_var = fake_neon::get_value_1024(b.id, b.type);
    float32x32x2_t o;
    __1024bit_var o_var0;
    __1024bit_var o_var1;
    for (int i = 0; i < 16; i++) {
        o_var0.fs[i + 0] = a_var.fs[2 * i + 0];
        o_var1.fs[i + 0] = a_var.fs[2 * i + 1];
        o_var0.fs[i + 16] = b_var.fs[2 * i + 0];
        o_var1.fs[i + 16] = b_var.fs[2 * i + 1];
    }
    o.val[0].id = fake_neon::new_operation("UZP1", a.id, a.type, b.id, b.type, o_var0, o.val[0].type);
    o.val[1].id = fake_neon::new_operation("UZP2", a.id, a.type, b.id, b.type, o_var1, o.val[1].type);
    return o;
}