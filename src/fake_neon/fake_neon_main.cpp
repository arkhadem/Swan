#include "fake_neon_include.hpp"
#include <assert.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#define CLEAR_ALL_REGISTER_FILES      \
    fake_neon::registers_64.clear();  \
    fake_neon::registers_128.clear(); \
    fake_neon::registers_256.clear(); \
    fake_neon::registers_512.clear(); \
    fake_neon::registers_1024.clear();

#ifdef FAKE_NEON_COMPARE
std::vector<__64bit_var> fake_neon::registers_64;
std::vector<__128bit_var> fake_neon::registers_128;
std::vector<__256bit_var> fake_neon::registers_256;
std::vector<__512bit_var> fake_neon::registers_512;
std::vector<__1024bit_var> fake_neon::registers_1024;
#endif

std::vector<operation> fake_neon::operations;

bool fake_neon::analysis_enabled = false;
int fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_MAX] = {0, 0, 0, 0, 0};
long fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_MAX] = {0, 0, 0, 0, 0};

std::ofstream fake_neon::graph_f;

operation::operation(std::string t, double c_i, double c_o, int a, int b, int c, int o, void const *m, void const *n) {
    src_address = m;
    dst_address = n;
    constant_input = c_i;
    constant_output = c_o;
    first_operand = a;
    second_operand = b;
    third_operand = c;
    result_operand = o;
    type = t;
}

__int64_t fake_neon::new_register_64(__64bit_var o) {
    int id = fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_64] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_64];
#ifdef FAKE_NEON_COMPARE
    fake_neon::registers_64.push_back(o);
#endif
    fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_64] += 1;
    return id;
}

__int64_t fake_neon::new_register_128(__128bit_var o) {
    int id = fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_128] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_128];
#ifdef FAKE_NEON_COMPARE
    fake_neon::registers_128.push_back(o);
#endif
    fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_128] += 1;
    return id;
}

__int64_t fake_neon::new_register_256(__256bit_var o) {
    int id = fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_256] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_256];
#ifdef FAKE_NEON_COMPARE
    fake_neon::registers_256.push_back(o);
#endif
    fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_256] += 1;
    return id;
}

__int64_t fake_neon::new_register_512(__512bit_var o) {
    int id = fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_512] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_512];
#ifdef FAKE_NEON_COMPARE
    fake_neon::registers_512.push_back(o);
#endif
    fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_512] += 1;
    return id;
}

__int64_t fake_neon::new_register_1024(__1024bit_var o) {
    int id = fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_1024] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_1024];
#ifdef FAKE_NEON_COMPARE
    fake_neon::registers_1024.push_back(o);
#endif
    fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_1024] += 1;
    return id;
}

void fake_neon::printer_reg(std::string t,
                            int dst_reg_count,
                            int dst_mem_count,
                            int src_reg_count,
                            int src_mem_count,
                            __int64_t dst_reg,
                            FAKE_NEON_REG_TYPE_t dst_reg_type,
                            const void *dst_mem,
                            __int64_t src_reg1,
                            FAKE_NEON_REG_TYPE_t src_reg1_type,
                            __int64_t src_reg2,
                            FAKE_NEON_REG_TYPE_t src_reg2_type,
                            __int64_t src_reg3,
                            FAKE_NEON_REG_TYPE_t src_reg3_type,
                            const void *src_mem) {

    if (dst_reg_count > 0) {
        assert(dst_reg != -1);
        assert(dst_reg_type != FAKE_NEON_REG_TYPE_t::TYPE_MAX);
    } else {
        assert(dst_reg == -1);
        assert(dst_reg_type == FAKE_NEON_REG_TYPE_t::TYPE_MAX);
    }

    if (dst_mem_count > 0) {
        assert(dst_mem != 0);
    } else {
        assert(dst_mem == 0);
    }

    if (src_reg_count > 0) {
        assert(src_reg1 != -1);
        assert(src_reg1_type != FAKE_NEON_REG_TYPE_t::TYPE_MAX);
    } else {
        assert(src_reg1 == -1);
        assert(src_reg1_type == FAKE_NEON_REG_TYPE_t::TYPE_MAX);
    }
    if (src_reg_count > 1) {
        assert(src_reg2 != -1);
        assert(src_reg2_type != FAKE_NEON_REG_TYPE_t::TYPE_MAX);
    } else {
        assert(src_reg2 == -1);
        assert(src_reg2_type == FAKE_NEON_REG_TYPE_t::TYPE_MAX);
    }
    if (src_reg_count > 2) {
        assert(src_reg3 != -1);
        assert(src_reg3_type != FAKE_NEON_REG_TYPE_t::TYPE_MAX);
    } else {
        assert(src_reg3 == -1);
        assert(src_reg3_type == FAKE_NEON_REG_TYPE_t::TYPE_MAX);
    }

    if (src_mem_count > 0) {
        assert(src_mem != 0);
    } else {
        assert(src_mem == 0);
    }

    graph_f << t << " ";
    graph_f << dst_reg_count << " " << dst_mem_count << " " << src_reg_count << " " << src_mem_count << " ";
    graph_f << dst_reg << " " << dst_reg_type << " ";
    graph_f << dst_mem << " ";
    graph_f << src_reg1 << " " << src_reg1_type << " ";
    graph_f << src_reg2 << " " << src_reg2_type << " ";
    graph_f << src_reg3 << " " << src_reg3_type << " ";
    graph_f << src_mem << " ";
    graph_f << endl;
}

__int64_t fake_neon::new_operation(std::string t, void const *m, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_64);
    int id = new_register_64(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 1, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, void const *m, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_128);
    int id = new_register_128(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 1, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, void const *m, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_256);
    int id = new_register_256(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 1, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, void const *m, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_512);
    int id = new_register_512(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 1, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, void const *m, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_1024);
    int id = new_register_1024(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 1, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}

__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load_lane
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_64);
    int id = new_register_64(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 1, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load_lane
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_128);
    int id = new_register_128(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 1, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load_lane
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_256);
    int id = new_register_256(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 1, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load_lane
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_512);
    int id = new_register_512(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 1, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // load_lane
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_1024);
    int id = new_register_1024(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 1, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, m);
    }
    return id;
}

__int64_t fake_neon::new_operation(std::string t, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // dup
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_64);
    int id = new_register_64(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 0, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // dup
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_128);
    int id = new_register_128(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 0, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // dup
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_256);
    int id = new_register_256(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 0, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // dup
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_512);
    int id = new_register_512(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 0, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // dup
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_1024);
    int id = new_register_1024(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 0, 0, id, o_type, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}

void fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type) { // get
    if (analysis_enabled) {
        printer_reg(t, 0, 0, 1, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
}

__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_64);
    int id = new_register_64(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 0, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_128);
    int id = new_register_128(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 0, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_256);
    int id = new_register_256(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 0, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_512);
    int id = new_register_512(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 0, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_1024);
    int id = new_register_1024(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 1, 0, id, o_type, 0, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}

__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_64);
    int id = new_register_64(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 2, 0, id, o_type, 0, a, a_type, b, b_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_128);
    int id = new_register_128(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 2, 0, id, o_type, 0, a, a_type, b, b_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_256);
    int id = new_register_256(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 2, 0, id, o_type, 0, a, a_type, b, b_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_512);
    int id = new_register_512(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 2, 0, id, o_type, 0, a, a_type, b, b_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_1024);
    int id = new_register_1024(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 2, 0, id, o_type, 0, a, a_type, b, b_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
    return id;
}

__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_64);
    int id = new_register_64(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 3, 0, id, o_type, 0, a, a_type, b, b_type, c, c_type, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_128);
    int id = new_register_128(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 3, 0, id, o_type, 0, a, a_type, b, b_type, c, c_type, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_256);
    int id = new_register_256(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 3, 0, id, o_type, 0, a, a_type, b, b_type, c, c_type, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_512);
    int id = new_register_512(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 3, 0, id, o_type, 0, a, a_type, b, b_type, c, c_type, 0);
    }
    return id;
}
__int64_t fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type) { // convert and copy
    assert(o_type == FAKE_NEON_REG_TYPE_t::TYPE_1024);
    int id = new_register_1024(o);
    if (analysis_enabled) {
        printer_reg(t, 1, 0, 3, 0, id, o_type, 0, a, a_type, b, b_type, c, c_type, 0);
    }
    return id;
}

void fake_neon::new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *n) { // store
    if (analysis_enabled) {
        printer_reg(t, 0, 1, 1, 0, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, n, a, a_type, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, -1, FAKE_NEON_REG_TYPE_t::TYPE_MAX, 0);
    }
}

__64bit_var fake_neon::get_value_64(int id, FAKE_NEON_REG_TYPE_t type) {
    assert(type == FAKE_NEON_REG_TYPE_t::TYPE_64);
    if (id >= fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_64] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_64]) {
        cout << "Error: requesting register ID#" << id << " while there are " << fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_64] << " registers!" << endl;
        assert(false);
    } else if (id < fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_64]) {
        // cout << "Warning: requesting register ID#" << id << " is between " << fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_64] << " flsuhed registers! Returning 0!" << endl;
        __64bit_var temp;
        temp.sd[0] = 0;
        return temp;
    }
#ifdef FAKE_NEON_COMPARE
    return registers_64[id - fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_64]];
#else
    __64bit_var temp;
    return temp;
#endif
}
__128bit_var fake_neon::get_value_128(int id, FAKE_NEON_REG_TYPE_t type) {
    assert(type == FAKE_NEON_REG_TYPE_t::TYPE_128);
    if (id >= fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_128] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_128]) {
        cout << "Error: requesting register ID#" << id << " while there are " << fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_128] << " registers!" << endl;
        assert(false);
    } else if (id < fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_128]) {
        // cout << "Warning: requesting register ID#" << id << " is between " << fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_128] << " flsuhed registers! Returning 0!" << endl;
        __128bit_var temp;
        temp.sd[0] = 0;
        temp.sd[1] = 0;
        return temp;
    }
#ifdef FAKE_NEON_COMPARE
    return registers_128[id - fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_128]];
#else
    __128bit_var temp;
    return temp;
#endif
}
__256bit_var fake_neon::get_value_256(int id, FAKE_NEON_REG_TYPE_t type) {
    assert(type == FAKE_NEON_REG_TYPE_t::TYPE_256);
    if (id >= fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_256] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_256]) {
        cout << "Error: requesting register ID#" << id << " while there are " << fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_256] << " registers!" << endl;
        assert(false);
    } else if (id < fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_256]) {
        // cout << "Warning: requesting register ID#" << id << " is between " << fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_256] << " flsuhed registers! Returning 0!" << endl;
        __256bit_var temp;
        temp.sd[0] = 0;
        temp.sd[1] = 0;
        temp.sd[2] = 0;
        temp.sd[3] = 0;
        return temp;
    }
#ifdef FAKE_NEON_COMPARE
    return registers_256[id - fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_256]];
#else
    __256bit_var temp;
    return temp;
#endif
}
__512bit_var fake_neon::get_value_512(int id, FAKE_NEON_REG_TYPE_t type) {
    assert(type == FAKE_NEON_REG_TYPE_t::TYPE_512);
    if (id >= fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_512] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_512]) {
        cout << "Error: requesting register ID#" << id << " while there are " << fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_512] << " registers!" << endl;
        assert(false);
    } else if (id < fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_512]) {
        // cout << "Warning: requesting register ID#" << id << " is between " << fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_512] << " flsuhed registers! Returning 0!" << endl;
        __512bit_var temp;
        temp.sd[0] = 0;
        temp.sd[1] = 0;
        temp.sd[2] = 0;
        temp.sd[3] = 0;
        temp.sd[4] = 0;
        temp.sd[5] = 0;
        temp.sd[6] = 0;
        temp.sd[7] = 0;
        return temp;
    }
#ifdef FAKE_NEON_COMPARE
    return registers_512[id - fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_512]];
#else
    __512bit_var temp;
    return temp;
#endif
}
__1024bit_var fake_neon::get_value_1024(int id, FAKE_NEON_REG_TYPE_t type) {
    assert(type == FAKE_NEON_REG_TYPE_t::TYPE_1024);
    if (id >= fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_1024] + fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_1024]) {
        cout << "Error: requesting register ID#" << id << " while there are " << fake_neon::registers_size[FAKE_NEON_REG_TYPE_t::TYPE_1024] << " registers!" << endl;
        assert(false);
    } else if (id < fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_1024]) {
        // cout << "Warning: requesting register ID#" << id << " is between " << fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_1024] << " flsuhed registers! Returning 0!" << endl;
        __1024bit_var temp;
        temp.sd[0] = 0;
        temp.sd[1] = 0;
        temp.sd[2] = 0;
        temp.sd[3] = 0;
        temp.sd[4] = 0;
        temp.sd[5] = 0;
        temp.sd[6] = 0;
        temp.sd[7] = 0;
        temp.sd[8] = 0;
        temp.sd[9] = 0;
        temp.sd[10] = 0;
        temp.sd[11] = 0;
        temp.sd[12] = 0;
        temp.sd[13] = 0;
        temp.sd[14] = 0;
        temp.sd[15] = 0;
        return temp;
    }
#ifdef FAKE_NEON_COMPARE
    return registers_1024[id - fake_neon::flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_1024]];
#else
    __1024bit_var temp;
    return temp;
#endif
}

inline bool file_exists(const std::string &name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void fake_neon::initializer(char *exp_name) {
    string graph_name;
    string full_name;
    graph_name = (string)exp_name + (string) "_DFG";
    full_name = graph_name + ".txt";
    if (file_exists(full_name)) {
        int dup_num;
        for (dup_num = 1; dup_num != -1; dup_num++) {
            full_name = graph_name + "_(" + to_string(dup_num) + ").txt";
            if (file_exists(full_name)) {
                continue;
            }
            break;
        }
    }
    for (uint32_t t = 0; t < FAKE_NEON_REG_TYPE_t::TYPE_MAX; t++) {
        fake_neon::flushed_registers[t] += fake_neon::registers_size[t];
        fake_neon::registers_size[t] = 0;
    }
#ifdef FAKE_NEON_COMPARE
    CLEAR_ALL_REGISTER_FILES
#endif
    fake_neon::graph_f.open(full_name);
    cout << "file " << full_name << endl;
    fake_neon::start_analysis();
}

void fake_neon::finisher() {
    cout << "Finishing GPIC3 computation" << endl;
    fake_neon::end_analysis();
    if (fake_neon::graph_f.is_open()) {
        fake_neon::graph_f.close();
    } else {
        cout << "Warning: closing an already closed file!" << endl;
    }
    for (uint32_t t = 0; t < FAKE_NEON_REG_TYPE_t::TYPE_MAX; t++) {
        fake_neon::flushed_registers[t] = 0;
    }
#ifdef FAKE_NEON_COMPARE
    CLEAR_ALL_REGISTER_FILES
#endif
    for (uint32_t t = 0; t < FAKE_NEON_REG_TYPE_t::TYPE_MAX; t++) {
        fake_neon::registers_size[t] = 0;
    }
    fake_neon::operations.clear();
}

void fake_neon::flusher() {
    for (uint32_t t = 0; t < FAKE_NEON_REG_TYPE_t::TYPE_MAX; t++) {
        fake_neon::flushed_registers[t] += fake_neon::registers_size[t];
    }
#ifdef FAKE_NEON_COMPARE
    CLEAR_ALL_REGISTER_FILES
#endif
    for (uint32_t t = 0; t < FAKE_NEON_REG_TYPE_t::TYPE_MAX; t++) {
        fake_neon::registers_size[t] = 0;
    }
    graph_f << "flushed" << endl;
}

void fake_neon::start_analysis(void) { analysis_enabled = true; }

void fake_neon::end_analysis(void) { analysis_enabled = false; }