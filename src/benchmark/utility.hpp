#ifndef A01AC121_98C5_4E77_85B1_4D93BFDB2DBA
#define A01AC121_98C5_4E77_85B1_4D93BFDB2DBA
#include "swan.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <math.h>
#include <random>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

extern std::map<std::string, std::map<std::string, kernel_utility_t>> utility_functions;

template <typename T>
T abs(T input1, T input2) {
    if (input1 > input2)
        return input1 - input2;
    return input2 - input1;
}

template <typename T>
void compare_1D(int count0, T *data1, T *data2, char *obj_name, T delta) {
    bool is_equal = true;
    for (int i = 0; i < count0; i++) {
        if (abs<T>(data1[i], data2[i]) > delta) {
            if ((std::is_same<T, float>::value == false) && ((std::is_same<T, double>::value == false))
#ifndef __fp16
                && (std::is_same<T, __fp16>::value == false)
#endif
            )
                std::cout << std::hex;
            if (sizeof(T) == 1)
                std::cout << "Error: " << obj_name << "[scalar][" << i << "](" << (unsigned)data1[i] << ") != " << obj_name << "[neon][" << i << "](" << (unsigned)data2[i] << ")" << std::endl;
            else
                std::cout << "Error: " << obj_name << "[scalar][" << i << "](" << data1[i] << ") != " << obj_name << "[neon][" << i << "](" << data2[i] << ")" << std::endl;
            is_equal = false;
            // exit(-1);
        }
    }
    if (is_equal)
        std::cout << obj_name << "[scalar] == " << obj_name << "[neon]" << std::endl;
    else
        std::cout << obj_name << "[scalar] != " << obj_name << "[neon]" << std::endl;
}

template <typename T>
void compare_1D(int count0, T *data1, T *data2, char *obj_name) {
    compare_1D<T>(count0, data1, data2, obj_name, 0);
}

template <typename T>
void compare_2D(int count1, int count0, T **data1, T **data2, char *obj_name, T delta) {
    bool is_equal = true;
    for (int i = 0; i < count1; i++) {
        for (int j = 0; j < count0; j++) {
            if (abs<T>(data1[i][j], data2[i][j]) > delta) {
                if ((std::is_same<T, float>::value == false) && ((std::is_same<T, double>::value == false))
#ifndef __fp16
                    && (std::is_same<T, __fp16>::value == false)
#endif
                )
                    std::cout << std::hex;
                if (sizeof(T) == 1)
                    std::cout << "Error: " << obj_name << "[scalar][" << i << "][" << j << "](" << (unsigned)data1[i][j] << ") != " << obj_name << "[neon][" << i << "][" << j << "](" << (unsigned)data2[i][j] << ")" << std::endl;
                else
                    std::cout << "Error: " << obj_name << "[scalar][" << i << "][" << j << "](" << data1[i][j] << ") != " << obj_name << "[neon][" << i << "][" << j << "](" << data2[i][j] << ")" << std::endl;
                is_equal = false;
                // exit(-1);
            }
        }
    }
    if (is_equal)
        std::cout << obj_name << "[scalar] == " << obj_name << "[neon]" << std::endl;
    else
        std::cout << obj_name << "[scalar] != " << obj_name << "[neon]" << std::endl;
}

template <typename T>
void compare_2D(int count1, int count0, T **data1, T **data2, char *obj_name) {
    compare_2D<T>(count1, count0, data1, data2, obj_name, 0);
}

template <typename T>
void compare_3D(int count2, int count1, int count0, T ***data1, T ***data2, char *obj_name, T delta) {
    bool is_equal = true;
    for (int i = 0; i < count2; i++) {
        for (int j = 0; j < count1; j++) {
            for (int k = 0; k < count0; k++) {
                if (abs<T>(data1[i][j][k], data2[i][j][k]) > delta) {
                    if ((std::is_same<T, float>::value == false) && ((std::is_same<T, double>::value == false))
#ifndef __fp16
                        && (std::is_same<T, __fp16>::value == false)
#endif
                    )
                        std::cout << std::hex;
                    if (sizeof(T) == 1)
                        std::cout << "Error: " << obj_name << "[scalar][" << i << "][" << j << "][" << k << "](" << (unsigned)data1[i][j][k] << ") != " << obj_name << "[neon][" << i << "][" << j << "][" << k << "](" << (unsigned)data2[i][j][k] << ")" << std::endl;
                    else
                        std::cout << "Error: " << obj_name << "[scalar][" << i << "][" << j << "][" << k << "](" << data1[i][j][k] << ") != " << obj_name << "[neon][" << i << "][" << j << "][" << k << "](" << data2[i][j][k] << ")" << std::endl;
                    is_equal = false;
                    // exit(-1);
                }
            }
        }
    }
    if (is_equal)
        std::cout << obj_name << "[scalar] == " << obj_name << "[neon]" << std::endl;
    else
        std::cout << obj_name << "[scalar] != " << obj_name << "[neon]" << std::endl;
}

template <typename T>
void compare_3D(int count2, int count1, int count0, T ***data1, T ***data2, char *obj_name) {
    compare_3D<T>(count2, count1, count0, data1, data2, obj_name, 0);
}

#define SWAN_MEM_ALIGNMENT 64 // with a 64-byte alignment, we are even aligned on L2 cache lines...
void *swan_aligned_malloc(size_t nb_bytes);

template <typename T>
void alloc_1D(int count0, T *&data) {
    data = (T *)swan_aligned_malloc(count0 * sizeof(T));
}

template <typename T>
void alloc_2D(int count1, int count0, T **&data) {
    alloc_1D<T *>(count1, data);
    for (int i = 0; i < count1; i++)
        alloc_1D<T>(count0, data[i]);
}

template <typename T>
void alloc_3D(int count2, int count1, int count0, T ***&data) {
    alloc_1D<T **>(count2, data);
    for (int i = 0; i < count2; i++)
        alloc_2D<T>(count1, count0, data[i]);
}

static inline float fp32_from_bits(uint32_t w) {
    union {
        uint32_t as_bits;
        float as_value;
    } fp32 = {w};
    return fp32.as_value;
}

static inline uint32_t fp32_to_bits(float f) {
    union {
        float as_value;
        uint32_t as_bits;
    } fp32 = {f};
    return fp32.as_bits;
}

#ifndef __fp16
static inline uint16_t fp16_ieee_from_fp32_value(float f) {
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__GNUC__) && !defined(__STRICT_ANSI__)
    const float scale_to_inf = 0x1.0p+112f;
    const float scale_to_zero = 0x1.0p-110f;
#else
    const float scale_to_inf = fp32_from_bits(UINT32_C(0x77800000));
    const float scale_to_zero = fp32_from_bits(UINT32_C(0x08800000));
#endif
    float base = (fabsf(f) * scale_to_inf) * scale_to_zero;
    const uint32_t w = fp32_to_bits(f);
    const uint32_t shl1_w = w + w;
    const uint32_t sign = w & UINT32_C(0x80000000);
    uint32_t bias = shl1_w & UINT32_C(0xFF000000);
    if (bias < UINT32_C(0x71000000)) {
        bias = UINT32_C(0x71000000);
    }
    base = fp32_from_bits((bias >> 1) + UINT32_C(0x07800000)) + base;
    const uint32_t bits = fp32_to_bits(base);
    const uint32_t exp_bits = (bits >> 13) & UINT32_C(0x00007C00);
    const uint32_t mantissa_bits = bits & UINT32_C(0x00000FFF);
    const uint32_t nonsign = exp_bits + mantissa_bits;
    return (sign >> 16) | (shl1_w > UINT32_C(0xFF000000) ? UINT16_C(0x7E00) : nonsign);
}
#endif

template <typename T>
T zero_remover(int count0, float sparsity, T inp, T min_val, T max_val) {
    if (inp != 0)
        return inp;

    // negative sparsity shows sparsity is not important to the caller
    if (sparsity < 0)
        return inp;

    // It's a dummy call, output not used at all
    if (count0 == 0)
        return inp;

    // make a value close to zero (works for floats)
    T return_val = (max_val - min_val) / (T)(count0 + 1);

    // make a value close to zero (works for integers)
    if (return_val == (T)0) {
        return_val = 1;
    }

    // if it's signed, return a positive number
    if (min_val >= 0)
        return return_val;

    // Otherwise, 50-50 chance return positive or negative
    if (rand() % 2)
        return return_val;
    return -1 * return_val;
}

template <typename T>
void sparse_init_1D(int count0, float sparsity, T *data, T min_val, T max_val) {
    std::random_device random_device;
    auto rng = std::mt19937(random_device());

    std::vector<T> temp(count0);

    // Filling out the required zeros
    size_t sparse_end = 0;
    if (sparsity > 0)
        sparse_end = std::max(std::min(size_t(float(count0) * sparsity), size_t(count0)), size_t(0));
    std::fill(temp.begin(), temp.begin() + sparse_end, 0);

    // Filling out the rest of the non-zero values
    if ((std::is_same<T, float>::value) || ((std::is_same<T, double>::value))) {
        auto f32_rng = std::bind(std::uniform_real_distribution<float>(min_val, max_val), std::ref(rng));
        auto my_rng = std::bind(zero_remover<T>, count0, sparsity, f32_rng, min_val, max_val);
        std::generate(temp.begin() + sparse_end, temp.end(), std::ref(my_rng));
#ifndef __fp16
    } else if (std::is_same<T, __fp16>::value) {
        auto f32_rng = std::bind(std::uniform_real_distribution<float>(min_val, max_val), std::ref(rng));
        auto f16_rng = std::bind(fp16_ieee_from_fp32_value, f32_rng);
        auto my_rng = std::bind(zero_remover<T>, count0, sparsity, f16_rng, min_val, max_val);
        std::generate(temp.begin() + sparse_end, temp.end(), std::ref(my_rng));
#endif
    } else {
        auto other_rng = std::bind(std::uniform_int_distribution<T>(min_val, max_val), std::ref(rng));
        auto my_rng = std::bind(zero_remover<T>, count0, sparsity, other_rng, min_val, max_val);
        std::generate(temp.begin() + sparse_end, temp.end(), std::ref(my_rng));
    }

    // Shuffling non-zeros
    std::shuffle(temp.begin(), temp.end(), rng);

    // Coppying data back to the array
    memcpy(data, temp.data(), count0 * sizeof(T));
}

void sparse_init_1D(int count0, float sparsity, float *data, float min_val, float max_val);

void sparse_init_1D(int count0, double sparsity, double *data, double min_val, double max_val);

#ifndef __fp16
void sparse_init_1D(int count0, float sparsity, __fp16 *data, __fp16 min_val, __fp16 max_val);
#endif

template <typename T>
void sparse_init_1D(int count0, float sparsity, T *data) {
    if ((std::is_same<T, float>::value) || ((std::is_same<T, double>::value))) {
        sparse_init_1D(count0, sparsity, data, (T)-100, (T)100);
#ifndef __fp16
    } else if (std::is_same<T, __fp16>::value) {
        sparse_init_1D(count0, sparsity, data, (T)-10, (T)10);
#endif
    } else if (std::is_signed<T>::value) {
        sparse_init_1D(count0, sparsity, data, (T)-50, (T)50);
    } else {
        sparse_init_1D(count0, sparsity, data, (T)0, (T)100);
    }
}

template <typename T>
void sparse_init_alloc_1D(int count0, float sparsity, T *&data) {
    alloc_1D<T>(count0, data);
    sparse_init_1D<T>(count0, sparsity, data);
}

template <typename T>
void sparse_init_alloc_1D(int count0, float sparsity, T *&data, T min_val, T max_val) {
    alloc_1D<T>(count0, data);
    sparse_init_1D(count0, sparsity, data, min_val, max_val);
}

template <typename T>
void sparse_init_alloc_2D(int count1, int count0, float sparsity, T **&data) {
    alloc_1D<T *>(count1, data);
    for (int i = 0; i < count1; i++)
        sparse_init_alloc_1D<T>(count0, sparsity, data[i]);
}

template <typename T>
void sparse_init_alloc_2D(int count1, int count0, float sparsity, T **&data, T min_val, T max_val) {
    alloc_1D<T *>(count1, data);
    for (int i = 0; i < count1; i++)
        sparse_init_alloc_1D<T>(count0, sparsity, data[i], min_val, max_val);
}

template <typename T>
void sparse_init_alloc_3D(int count2, int count1, int count0, float sparsity, T ***&data) {
    alloc_1D<T **>(count2, data);
    for (int i = 0; i < count2; i++)
        sparse_init_alloc_2D<T>(count1, count0, sparsity, data[i]);
}

template <typename T>
void sparse_init_alloc_3D(int count2, int count1, int count0, float sparsity, T ***&data, T min_val, T max_val) {
    alloc_1D<T **>(count2, data);
    for (int i = 0; i < count2; i++)
        sparse_init_alloc_2D<T>(count1, count0, sparsity, data[i], min_val, max_val);
}

template <typename T>
void init_alloc_1D(int count0, T *&data) {
    sparse_init_alloc_1D<T>(count0, -1, data);
}

template <typename T>
void init_alloc_1D(int count0, T *&data, T min_val, T max_val) {
    sparse_init_alloc_1D<T>(count0, -1, data, min_val, max_val);
}

template <typename T>
void init_alloc_2D(int count1, int count0, T **&data) {
    sparse_init_alloc_2D<T>(count1, count0, -1, data);
}

template <typename T>
void init_alloc_2D(int count1, int count0, T **&data, T min_val, T max_val) {
    sparse_init_alloc_2D<T>(count1, count0, -1, data, min_val, max_val);
}

template <typename T>
void init_alloc_3D(int count2, int count1, int count0, T ***&data) {
    sparse_init_alloc_3D<T>(count2, count1, count0, -1, data);
}

template <typename T>
void init_alloc_3D(int count2, int count1, int count0, T ***&data, T min_val, T max_val) {
    sparse_init_alloc_3D<T>(count2, count1, count0, -1, data, min_val, max_val);
}

void register_utilities();

#ifdef SWAN_SIMULATION
extern kernel_utility_t rgb_to_ycbcr_utility;
extern kernel_utility_t pitch_xcorr_utility;
extern kernel_utility_t tm_prediction_utility;
extern kernel_utility_t adler32_utility;
extern kernel_utility_t convolve_horizontally_utility;
extern kernel_utility_t is_audible_utility;
extern kernel_utility_t gemm_fp32_utility;
extern kernel_utility_t sad_utility;
#else

extern kernel_utility_t downsample_utility;
extern kernel_utility_t ycbcr_to_rgb_utility;
extern kernel_utility_t rgb_to_ycbcr_utility;
extern kernel_utility_t upsample_utility;
extern kernel_utility_t rgb_to_gray_utility;

extern kernel_utility_t read_sub_utility;
extern kernel_utility_t read_up_utility;
extern kernel_utility_t read_avg_utility;
extern kernel_utility_t read_paeth_utility;
extern kernel_utility_t expand_palette_utility;

extern kernel_utility_t sharp_update_rgb_utility;
extern kernel_utility_t sharp_filter_row_utility;
extern kernel_utility_t apply_alpha_multiply_utility;
extern kernel_utility_t dispatch_alpha_utility;
extern kernel_utility_t tm_prediction_utility;
extern kernel_utility_t dc_prediction_utility;
extern kernel_utility_t he_prediction_utility;
extern kernel_utility_t ve_prediction_utility;
extern kernel_utility_t vertical_filter_utility;
extern kernel_utility_t gradient_filter_utility;

extern kernel_utility_t aes_utility;
extern kernel_utility_t des_utility;
extern kernel_utility_t chacha20_utility;
extern kernel_utility_t sha256_utility;

extern kernel_utility_t adler32_utility;
extern kernel_utility_t crc32_utility;

extern kernel_utility_t convolve_horizontally_utility;
extern kernel_utility_t convolve_vertically_utility;
extern kernel_utility_t row_blend_utility;
extern kernel_utility_t row_opaque_utility;

extern kernel_utility_t is_audible_utility;
extern kernel_utility_t copy_with_gain_utility;
extern kernel_utility_t copy_with_sample_utility;
extern kernel_utility_t sum_from_utility;
extern kernel_utility_t handle_nan_utility;

extern kernel_utility_t memchr_utility;
extern kernel_utility_t memcmp_utility;
extern kernel_utility_t memset_utility;
extern kernel_utility_t strlen_utility;

extern kernel_utility_t gemm_fp32_utility;
extern kernel_utility_t gemm_int32_utility;
extern kernel_utility_t gemm_fp16_utility;
extern kernel_utility_t gemm_int16_utility;
extern kernel_utility_t spmm_fp32_utility;
extern kernel_utility_t spmm_int32_utility;
extern kernel_utility_t spmm_fp16_utility;
extern kernel_utility_t spmm_int16_utility;

extern kernel_utility_t biquad_alt_utility;
extern kernel_utility_t lpc_prediction_utility;
extern kernel_utility_t pitch_xcorr_utility;
extern kernel_utility_t warped_autocorrelation_utility;

extern kernel_utility_t forward_dct_utility;
extern kernel_utility_t inverse_dct_utility;
extern kernel_utility_t sad_utility;
extern kernel_utility_t quant_utility;

extern kernel_utility_t fft_forward_real_utility;
extern kernel_utility_t fft_backward_real_utility;
extern kernel_utility_t fft_forward_complex_utility;
extern kernel_utility_t fft_backward_complex_utility;

#endif

#endif /* A01AC121_98C5_4E77_85B1_4D93BFDB2DBA */
