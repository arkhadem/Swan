#ifndef B4A1AF31_89E5_42D4_9A07_334E337B747B
#define B4A1AF31_89E5_42D4_9A07_334E337B747B

#include "swan.hpp"
#include <map>
#include <string>

extern std::map<std::string, std::map<std::string, std::map<platform_e, kernel_func>>> kernel_functions;

void register_scalar_kernels();
void register_neon_kernels();

#endif /* B4A1AF31_89E5_42D4_9A07_334E337B747B */
