#include "utility.hpp"

std::map<std::string, std::map<std::string, kernel_utility_t>> utility_functions;

void register_utilities() {
    utility_functions["zlib"]["adler32"] = adler32_utility;

    utility_functions["libjpeg"]["rgb_to_gray"] = rgb_to_gray_utility;
    utility_functions["libjpeg"]["ycbcr_to_rgb"] = ycbcr_to_rgb_utility;
    utility_functions["libjpeg"]["rgb_to_ycbcr"] = rgb_to_ycbcr_utility;
    utility_functions["libjpeg"]["upsample"] = upsample_utility;
    utility_functions["libjpeg"]["downsample"] = downsample_utility;

    utility_functions["libpng"]["read_sub"] = read_sub_utility;
    utility_functions["libpng"]["read_up"] = read_up_utility;
    utility_functions["libpng"]["read_avg"] = read_avg_utility;
    utility_functions["libpng"]["read_paeth"] = read_paeth_utility;
    utility_functions["libpng"]["expand_palette"] = expand_palette_utility;

    utility_functions["libwebp"]["sharp_update_rgb"] = sharp_update_rgb_utility;
    utility_functions["libwebp"]["sharp_filter_row"] = sharp_filter_row_utility;
    utility_functions["libwebp"]["apply_alpha_multiply"] = apply_alpha_multiply_utility;
    utility_functions["libwebp"]["dispatch_alpha"] = dispatch_alpha_utility;
    utility_functions["libwebp"]["tm_prediction"] = tm_prediction_utility;
    utility_functions["libwebp"]["dc_prediction"] = dc_prediction_utility;
    utility_functions["libwebp"]["he_prediction"] = he_prediction_utility;
    utility_functions["libwebp"]["ve_prediction"] = ve_prediction_utility;
    utility_functions["libwebp"]["vertical_filter"] = vertical_filter_utility;
    utility_functions["libwebp"]["gradient_filter"] = gradient_filter_utility;

    utility_functions["skia"]["convolve_horizontally"] = convolve_horizontally_utility;
    utility_functions["skia"]["convolve_vertically"] = convolve_vertically_utility;
    utility_functions["skia"]["row_blend"] = row_blend_utility;
    utility_functions["skia"]["row_opaque"] = row_opaque_utility;

    utility_functions["webaudio"]["is_audible"] = is_audible_utility;
    utility_functions["webaudio"]["copy_with_gain"] = copy_with_gain_utility;
    utility_functions["webaudio"]["copy_with_sample"] = copy_with_sample_utility;
    utility_functions["webaudio"]["sum_from"] = sum_from_utility;
    utility_functions["webaudio"]["handle_nan"] = handle_nan_utility;

    utility_functions["xnnpack"]["gemm_fp32"] = gemm_fp32_utility;
    utility_functions["xnnpack"]["gemm_int32"] = gemm_int32_utility;
    utility_functions["xnnpack"]["gemm_int16"] = gemm_int16_utility;
    utility_functions["xnnpack"]["spmm_fp32"] = spmm_fp32_utility;
    utility_functions["xnnpack"]["spmm_int32"] = spmm_int32_utility;
    utility_functions["xnnpack"]["spmm_int16"] = spmm_int16_utility;

    utility_functions["libopus"]["biquad_alt"] = biquad_alt_utility;
    utility_functions["libopus"]["lpc_prediction"] = lpc_prediction_utility;
    utility_functions["libopus"]["pitch_xcorr"] = pitch_xcorr_utility;
    utility_functions["libopus"]["warped_autocorrelation"] = warped_autocorrelation_utility;

    utility_functions["libvpx"]["forward_dct"] = forward_dct_utility;
    utility_functions["libvpx"]["inverse_dct"] = inverse_dct_utility;
    utility_functions["libvpx"]["sad"] = sad_utility;
    utility_functions["libvpx"]["quant"] = quant_utility;

#ifndef SWAN_SIMULATION

    // boringssl is not supported in simulation mode
    utility_functions["boringssl"]["aes"] = aes_utility;
    utility_functions["boringssl"]["des"] = des_utility;
    utility_functions["boringssl"]["chacha20"] = chacha20_utility;
    utility_functions["boringssl"]["sha256"] = sha256_utility;

    // crc32 is not supported in simulation mode
    utility_functions["zlib"]["crc32"] = crc32_utility;

    // optroutines is not supported in simulation mode
    utility_functions["optroutines"]["memchr"] = memchr_utility;
    utility_functions["optroutines"]["memcmp"] = memcmp_utility;
    utility_functions["optroutines"]["memset"] = memset_utility;
    utility_functions["optroutines"]["strlen"] = strlen_utility;

    // pffft is not supported in simulation mode
    utility_functions["pffft"]["fft_forward_real"] = fft_forward_real_utility;
    utility_functions["pffft"]["fft_backward_real"] = fft_backward_real_utility;
    utility_functions["pffft"]["fft_forward_complex"] = fft_forward_complex_utility;
    utility_functions["pffft"]["fft_backward_complex"] = fft_backward_complex_utility;

    // FP16 is not supported in simulation mode
    utility_functions["xnnpack"]["gemm_fp16"] = gemm_fp16_utility;
    utility_functions["xnnpack"]["spmm_fp16"] = spmm_fp16_utility;

#endif
}

void sparse_init_1D(int count0, float sparsity, float *data, float min_val, float max_val) {
    std::random_device random_device;
    auto rng = std::mt19937(random_device());

    std::vector<float> temp(count0);

    // Filling out the required zeros
    size_t sparse_end = 0;
    if (sparsity > 0)
        sparse_end = std::max(std::min(size_t(float(count0) * sparsity), size_t(count0)), size_t(0));
    std::fill(temp.begin(), temp.begin() + sparse_end, 0);

    // Filling out the rest of the non-zero values
    auto f32_rng = std::bind(std::uniform_real_distribution<float>(min_val, max_val), std::ref(rng));
    auto my_rng = std::bind(zero_remover<float>, count0, sparsity, f32_rng, min_val, max_val);
    std::generate(temp.begin() + sparse_end, temp.end(), std::ref(my_rng));

    // Shuffling non-zeros
    std::shuffle(temp.begin(), temp.end(), rng);

    // Coppying data back to the array
    memcpy(data, temp.data(), count0 * sizeof(float));
}

void sparse_init_1D(int count0, double sparsity, double *data, double min_val, double max_val) {
    std::random_device random_device;
    auto rng = std::mt19937(random_device());

    std::vector<double> temp(count0);

    // Filling out the required zeros
    size_t sparse_end = 0;
    if (sparsity > 0)
        sparse_end = std::max(std::min(size_t(float(count0) * sparsity), size_t(count0)), size_t(0));
    std::fill(temp.begin(), temp.begin() + sparse_end, 0);

    // Filling out the rest of the non-zero values
    auto f32_rng = std::bind(std::uniform_real_distribution<double>(min_val, max_val), std::ref(rng));
    auto my_rng = std::bind(zero_remover<double>, count0, sparsity, f32_rng, min_val, max_val);
    std::generate(temp.begin() + sparse_end, temp.end(), std::ref(my_rng));

    // Shuffling non-zeros
    std::shuffle(temp.begin(), temp.end(), rng);

    // Coppying data back to the array
    memcpy(data, temp.data(), count0 * sizeof(double));
}

#ifndef SWAN_SIMULATION
void sparse_init_1D(int count0, float sparsity, __fp16 *data, __fp16 min_val, __fp16 max_val) {
    std::random_device random_device;
    auto rng = std::mt19937(random_device());

    std::vector<__fp16> temp(count0);

    // Filling out the required zeros
    size_t sparse_end = 0;
    if (sparsity > 0)
        sparse_end = std::max(std::min(size_t(float(count0) * sparsity), size_t(count0)), size_t(0));
    std::fill(temp.begin(), temp.begin() + sparse_end, 0);

    // Filling out the rest of the non-zero values
    auto f32_rng = std::bind(std::uniform_real_distribution<float>(min_val, max_val), std::ref(rng));
    auto f16_rng = std::bind(fp16_ieee_from_fp32_value, f32_rng);
    auto my_rng = std::bind(zero_remover<__fp16>, count0, sparsity, f16_rng, min_val, max_val);
    std::generate(temp.begin() + sparse_end, temp.end(), std::ref(my_rng));

    // Shuffling non-zeros
    std::shuffle(temp.begin(), temp.end(), rng);

    // Coppying data back to the array
    memcpy(data, temp.data(), count0 * sizeof(__fp16));
}
#endif

void *swan_aligned_malloc(size_t nb_bytes) {
    void *p, *p0 = malloc(nb_bytes + SWAN_MEM_ALIGNMENT);
    if (!p0)
        return (void *)0;
    p = (void *)(((size_t)p0 + SWAN_MEM_ALIGNMENT) & (~((size_t)(SWAN_MEM_ALIGNMENT - 1))));
    *((void **)p - 1) = p0;
    return p;
}