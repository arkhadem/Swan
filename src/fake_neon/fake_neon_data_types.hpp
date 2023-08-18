#ifndef F860FC06_44A7_4323_8EE7_A2A3DF7AFBAF
#define F860FC06_44A7_4323_8EE7_A2A3DF7AFBAF

#include "fake_neon_include.hpp"
#include "fake_neon_variable_types.hpp"
#include <cstring>
#include <stdint.h>

// 64 bit
// 1 val
struct type_64bit {
    uint32_t id;
    FAKE_NEON_REG_TYPE_t type = FAKE_NEON_REG_TYPE_t::TYPE_64;
    // Assignment operator overleading
    type_64bit &operator=(const type_64bit &a) {
        __64bit_var a_var = fake_neon::get_value_64(a.id, a.type);
        __64bit_var o_var;
        for (int i = 0; i < 1; i++) {
            o_var.ud[i] = a_var.ud[i];
        }
        // NOP instruction is only used to connect assignment src to dst for DFG analysis
        this->id = fake_neon::new_operation("NOP", a.id, a.type, o_var, this->type);
        return *this;
    }
    // Load operator overloading
    template <typename T>
    type_64bit &operator=(const T *ptr) {
        __64bit_var o_var;
        memcpy(&o_var, ptr, sizeof(__64bit_var));
        this->id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, this->type);
        return *this;
    }
    // Store operator overleading
    template <typename T>
    operator T *() {
        T *tmp = (T *)malloc(sizeof(__64bit_var));
        __64bit_var my_var = fake_neon::get_value_64(this->id, this->type);
        memcpy(tmp, my_var, sizeof(__64bit_var));
        fake_neon::new_operation("ST1", this->id, this->type, (void const *)tmp);
        return tmp;
    }
};
typedef type_64bit uint8x8_t;
typedef type_64bit int8x8_t;
typedef type_64bit int16x4_t;
typedef type_64bit uint16x4_t;
typedef type_64bit int32x2_t;
typedef type_64bit uint32x2_t;
typedef type_64bit float32x2_t;
typedef type_64bit int64x1_t;
typedef type_64bit uint64x1_t;
// 2 vals
struct uint8x8x2_t {
    uint8x8_t val[2];
    uint8x8x2_t &operator=(const uint8x8x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int16x4x2_t {
    int16x4_t val[2];
    int16x4x2_t &operator=(const int16x4x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int32x2x2_t {
    int32x2_t val[2];
    int32x2x2_t &operator=(const int32x2x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct float32x2x2_t {
    float32x2_t val[2];
    float32x2x2_t &operator=(const float32x2x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
// 4 vals
struct uint32x2x4_t {
    uint32x2_t val[4];
    uint32x2x4_t &operator=(const uint32x2x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct uint8x8x4_t {
    uint8x8_t val[4];
    uint8x8x4_t &operator=(const uint8x8x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};

// 128 bit
// 1 val
struct type_128bit {
    uint32_t id;
    FAKE_NEON_REG_TYPE_t type = FAKE_NEON_REG_TYPE_t::TYPE_128;
    // Assignment operator overleading
    type_128bit &operator=(const type_128bit &a) {
        __128bit_var a_var = fake_neon::get_value_128(a.id, a.type);
        __128bit_var o_var;
        for (int i = 0; i < 2; i++) {
            o_var.ud[i] = a_var.ud[i];
        }
        // NOP instruction is only used to connect assignment src to dst for DFG analysis
        this->id = fake_neon::new_operation("NOP", a.id, a.type, o_var, this->type);
        return *this;
    }
    // Load operator overloading
    template <typename T>
    type_128bit &operator=(const T *ptr) {
        __128bit_var o_var;
        memcpy(&o_var, ptr, sizeof(__128bit_var));
        this->id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, this->type);
        return *this;
    }
    // Store operator overleading
    template <typename T>
    operator T *() {
        T *tmp = (T *)malloc(sizeof(__128bit_var));
        __128bit_var my_var = fake_neon::get_value_128(this->id, this->type);
        memcpy(tmp, my_var, sizeof(__128bit_var));
        fake_neon::new_operation("ST1", this->id, this->type, (void const *)tmp);
        return tmp;
    }
};
typedef type_128bit uint8x16_t;
typedef type_128bit int8x16_t;
typedef type_128bit uint16x8_t;
typedef type_128bit int16x8_t;
typedef type_128bit uint32x4_t;
typedef type_128bit int32x4_t;
typedef type_128bit float32x4_t;
typedef type_128bit uint64x2_t;
typedef type_128bit int64x2_t;
typedef type_128bit uint64x2_t;
// 2 vals
struct uint8x16x2_t {
    uint8x16_t val[2];
    uint8x16x2_t &operator=(const uint8x16x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int16x8x2_t {
    int16x8_t val[2];
    int16x8x2_t &operator=(const int16x8x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int32x4x2_t {
    int32x4_t val[2];
    int32x4x2_t &operator=(const int32x4x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct float32x4x2_t {
    float32x4_t val[4];
    float32x4x2_t &operator=(const float32x4x2_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
// 4 vals
struct uint8x16x4_t {
    uint8x16_t val[4];
    uint8x16x4_t &operator=(const uint8x16x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct uint32x4x4_t {
    uint32x4_t val[4];
    uint32x4x4_t &operator=(const uint32x4x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};

// 256 bits
struct type_256bit {
    uint32_t id;
    FAKE_NEON_REG_TYPE_t type = FAKE_NEON_REG_TYPE_t::TYPE_256;
    // Assignment operator overleading
    type_256bit &operator=(const type_256bit &a) {
        __256bit_var a_var = fake_neon::get_value_256(a.id, a.type);
        __256bit_var o_var;
        for (int i = 0; i < 4; i++) {
            o_var.ud[i] = a_var.ud[i];
        }
        // NOP instruction is only used to connect assignment src to dst for DFG analysis
        this->id = fake_neon::new_operation("NOP", a.id, a.type, o_var, this->type);
        return *this;
    }
    // Load operator overloading
    template <typename T>
    type_256bit &operator=(const T *ptr) {
        __256bit_var o_var;
        memcpy(&o_var, ptr, sizeof(__256bit_var));
        this->id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, this->type);
        return *this;
    }
    // Store operator overleading
    template <typename T>
    operator T *() {
        T *tmp = (T *)malloc(sizeof(__256bit_var));
        __256bit_var my_var = fake_neon::get_value_256(this->id, this->type);
        memcpy(tmp, my_var, sizeof(__256bit_var));
        fake_neon::new_operation("ST1", this->id, this->type, (void const *)tmp);
        return tmp;
    }
};
typedef type_256bit uint8x32_t;
typedef type_256bit int8x32_t;
typedef type_256bit uint16x16_t;
typedef type_256bit int16x16_t;
typedef type_256bit uint32x8_t;
typedef type_256bit int32x8_t;
typedef type_256bit float32x8_t;
typedef type_256bit int64x4_t;
typedef type_256bit uint64x4_t;
// 2 vals
struct uint8x32x2_t {
    uint8x32_t val[2];
    uint8x32x2_t &operator=(const uint8x32x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int16x16x2_t {
    int16x16_t val[2];
    int16x16x2_t &operator=(const int16x16x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int32x8x2_t {
    int32x8_t val[2];
    int32x8x2_t &operator=(const int32x8x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct float32x8x2_t {
    float32x8_t val[4];
    float32x8x2_t &operator=(const float32x8x2_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
// 4 vals
struct uint8x32x4_t {
    uint8x32_t val[4];
    uint8x32x4_t &operator=(const uint8x32x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct uint32x8x4_t {
    uint32x8_t val[4];
    uint32x8x4_t &operator=(const uint32x8x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};

// 512 bits
struct type_512bit {
    uint32_t id;
    FAKE_NEON_REG_TYPE_t type = FAKE_NEON_REG_TYPE_t::TYPE_512;
    // Assignment operator overleading
    type_512bit &operator=(const type_512bit &a) {
        __512bit_var a_var = fake_neon::get_value_512(a.id, a.type);
        __512bit_var o_var;
        for (int i = 0; i < 8; i++) {
            o_var.ud[i] = a_var.ud[i];
        }
        // NOP instruction is only used to connect assignment src to dst for DFG analysis
        this->id = fake_neon::new_operation("NOP", a.id, a.type, o_var, this->type);
        return *this;
    }
    // Load operator overloading
    template <typename T>
    type_512bit &operator=(const T *ptr) {
        __512bit_var o_var;
        memcpy(&o_var, ptr, sizeof(__512bit_var));
        this->id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, this->type);
        return *this;
    }
    // Store operator overleading
    template <typename T>
    operator T *() {
        T *tmp = (T *)malloc(sizeof(__512bit_var));
        __512bit_var my_var = fake_neon::get_value_512(this->id, this->type);
        memcpy(tmp, my_var, sizeof(__512bit_var));
        fake_neon::new_operation("ST1", this->id, this->type, (void const *)tmp);
        return tmp;
    }
};
typedef type_512bit int8x64_t;
typedef type_512bit uint8x64_t;
typedef type_512bit int16x32_t;
typedef type_512bit uint16x32_t;
typedef type_512bit int32x16_t;
typedef type_512bit uint32x16_t;
typedef type_512bit float32x16_t;
typedef type_512bit int64x8_t;
typedef type_512bit uint64x8_t;
// 2 vals
struct uint8x64x2_t {
    uint8x64_t val[2];
    uint8x64x2_t &operator=(const uint8x64x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int16x32x2_t {
    int16x32_t val[2];
    int16x32x2_t &operator=(const int16x32x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int32x16x2_t {
    int32x16_t val[2];
    int32x16x2_t &operator=(const int32x16x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct float32x16x2_t {
    float32x16_t val[4];
    float32x16x2_t &operator=(const float32x16x2_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
// 4 vals
struct uint8x64x4_t {
    uint8x64_t val[4];
    uint8x64x4_t &operator=(const uint8x64x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct uint32x16x4_t {
    uint32x16_t val[4];
    uint32x16x4_t &operator=(const uint32x16x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};

// 1024 bits
struct type_1024bit {
    uint32_t id;
    FAKE_NEON_REG_TYPE_t type = FAKE_NEON_REG_TYPE_t::TYPE_1024;
    // Assignment operator overleading
    type_1024bit &operator=(const type_1024bit &a) {
        __1024bit_var a_var = fake_neon::get_value_1024(a.id, a.type);
        __1024bit_var o_var;
        for (int i = 0; i < 8; i++) {
            o_var.ud[i] = a_var.ud[i];
        }
        // NOP instruction is only used to connect assignment src to dst for DFG analysis
        this->id = fake_neon::new_operation("NOP", a.id, a.type, o_var, this->type);
        return *this;
    }
    // Load operator overloading
    template <typename T>
    type_1024bit &operator=(const T *ptr) {
        __1024bit_var o_var;
        memcpy(&o_var, ptr, sizeof(__1024bit_var));
        this->id = fake_neon::new_operation("LD1", (void const *)ptr, o_var, this->type);
        return *this;
    }
    // Store operator overleading
    template <typename T>
    operator T *() {
        T *tmp = (T *)malloc(sizeof(__1024bit_var));
        __1024bit_var my_var = fake_neon::get_value_1024(this->id, this->type);
        memcpy(tmp, my_var, sizeof(__1024bit_var));
        fake_neon::new_operation("ST1", this->id, this->type, (void const *)tmp);
        return tmp;
    }
};
typedef type_1024bit int8x128_t;
typedef type_1024bit uint8x128_t;
typedef type_1024bit int16x64_t;
typedef type_1024bit uint16x64_t;
typedef type_1024bit int32x32_t;
typedef type_1024bit uint32x32_t;
typedef type_1024bit float32x32_t;
typedef type_1024bit int64x16_t;
typedef type_1024bit uint64x16_t;
// 2 vals
struct uint8x128x2_t {
    uint8x128_t val[2];
    uint8x128x2_t &operator=(const uint8x128x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int16x64x2_t {
    int16x64_t val[2];
    int16x64x2_t &operator=(const int16x64x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct int32x32x2_t {
    int32x32_t val[2];
    int32x32x2_t &operator=(const int32x32x2_t &a) {
        for (int i = 0; i < 2; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct float32x32x2_t {
    float32x32_t val[4];
    float32x32x2_t &operator=(const float32x32x2_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
// 4 vals
struct uint8x128x4_t {
    uint8x128_t val[4];
    uint8x128x4_t &operator=(const uint8x128x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
struct uint32x32x4_t {
    uint32x32_t val[4];
    uint32x32x4_t &operator=(const uint32x32x4_t &a) {
        for (int i = 0; i < 4; i++) {
            this->val[i] = a.val[i];
        }
        return *this;
    }
};
#endif /* F860FC06_44A7_4323_8EE7_A2A3DF7AFBAF */
