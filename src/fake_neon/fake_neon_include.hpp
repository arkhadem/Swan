#ifndef FC09DE07_F4FD_4E5B_B523_2F91B915AEB4
#define FC09DE07_F4FD_4E5B_B523_2F91B915AEB4

#include <fstream>
#include <math.h>
#include <stdbool.h>
#include <string>
#include <vector>

// #define DEBUG

using namespace std;

#include "fake_neon_variable_types.hpp"

#define FAKE_NEON_COMPARE

class operation {
public:
    void const *src_address;
    void const *dst_address;
    double constant_input;
    double constant_output;
    int first_operand;
    int second_operand;
    int third_operand;
    int result_operand;
    std::string type;
    operation(std::string t, double c_i, double c_o, int a, int b, int c, int o, void const *m, void const *n);
};

class fake_neon {
    static bool analysis_enabled;
    static int flushed_registers[FAKE_NEON_REG_TYPE_t::TYPE_MAX];
    static long registers_size[FAKE_NEON_REG_TYPE_t::TYPE_MAX];

public:
#ifdef FAKE_NEON_COMPARE
    static std::vector<__64bit_var> registers_64;
    static std::vector<__128bit_var> registers_128;
    static std::vector<__256bit_var> registers_256;
    static std::vector<__512bit_var> registers_512;
    static std::vector<__1024bit_var> registers_1024;
#endif
    static std::vector<operation> operations;
    static std::ofstream graph_f;
    fake_neon();
    static __int64_t new_register_64(__64bit_var o);
    static __int64_t new_register_128(__128bit_var o);
    static __int64_t new_register_256(__256bit_var o);
    static __int64_t new_register_512(__512bit_var o);
    static __int64_t new_register_1024(__1024bit_var o);

    static void printer_reg(std::string t,
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
                            const void *src_mem);

    static __int64_t new_operation(std::string t, void const *m, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, void const *m, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, void const *m, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, void const *m, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, void const *m, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *m, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static void new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __64bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __128bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __256bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __512bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static __int64_t new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, __int64_t b, FAKE_NEON_REG_TYPE_t b_type, __int64_t c, FAKE_NEON_REG_TYPE_t c_type, __1024bit_var o, FAKE_NEON_REG_TYPE_t o_type);
    static void new_operation(std::string t, __int64_t a, FAKE_NEON_REG_TYPE_t a_type, void const *n);

    static __64bit_var get_value_64(int id, FAKE_NEON_REG_TYPE_t type);
    static __128bit_var get_value_128(int id, FAKE_NEON_REG_TYPE_t type);
    static __256bit_var get_value_256(int id, FAKE_NEON_REG_TYPE_t type);
    static __512bit_var get_value_512(int id, FAKE_NEON_REG_TYPE_t type);
    static __1024bit_var get_value_1024(int id, FAKE_NEON_REG_TYPE_t type);

    static void initializer(char *exp_name);
    static void finisher();
    static void flusher();
    static void start_analysis(void);
    static void end_analysis(void);
    static void dump_graph();
};

#endif /* FC09DE07_F4FD_4E5B_B523_2F91B915AEB4 */
