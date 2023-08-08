#include "swan.hpp"

#include "runner.hpp"
#include "scalar.hpp"
#include "utility.hpp"
#include <stddef.h>
#include <stdio.h>

void register_scalar_kernels() {
#ifdef SWAN_SIMULATION
    kernel_functions["libjpeg"]["rgb_to_ycbcr"][platform_t::SCALAR_PLAT] = rgb_to_ycbcr_scalar;
    kernel_functions["libopus"]["pitch_xcorr"][platform_t::SCALAR_PLAT] = pitch_xcorr_scalar;
    kernel_functions["libwebp"]["tm_prediction"][platform_t::SCALAR_PLAT] = tm_prediction_scalar;
    kernel_functions["zlib"]["adler32"][platform_t::SCALAR_PLAT] = adler32_scalar;
    kernel_functions["skia"]["convolve_horizontally"][platform_t::SCALAR_PLAT] = convolve_horizontally_scalar;
    kernel_functions["webaudio"]["is_audible"][platform_t::SCALAR_PLAT] = is_audible_scalar;
    kernel_functions["xnnpack"]["gemm_fp32"][platform_t::SCALAR_PLAT] = gemm_fp32_scalar;
    kernel_functions["libvpx"]["sad"][platform_t::SCALAR_PLAT] = sad_scalar;
#else
    kernel_functions["libjpeg"]["rgb_to_gray"][platform_t::SCALAR_PLAT] = rgb_to_gray_scalar;
    kernel_functions["libjpeg"]["ycbcr_to_rgb"][platform_t::SCALAR_PLAT] = ycbcr_to_rgb_scalar;
    kernel_functions["libjpeg"]["rgb_to_ycbcr"][platform_t::SCALAR_PLAT] = rgb_to_ycbcr_scalar;
    kernel_functions["libjpeg"]["upsample"][platform_t::SCALAR_PLAT] = upsample_scalar;
    kernel_functions["libjpeg"]["downsample"][platform_t::SCALAR_PLAT] = downsample_scalar;

    kernel_functions["libpng"]["read_sub"][platform_t::SCALAR_PLAT] = read_sub_scalar;
    kernel_functions["libpng"]["read_up"][platform_t::SCALAR_PLAT] = read_up_scalar;
    kernel_functions["libpng"]["read_avg"][platform_t::SCALAR_PLAT] = read_avg_scalar;
    kernel_functions["libpng"]["read_paeth"][platform_t::SCALAR_PLAT] = read_paeth_scalar;
    kernel_functions["libpng"]["expand_palette"][platform_t::SCALAR_PLAT] = expand_palette_scalar;

    kernel_functions["libwebp"]["sharp_update_rgb"][platform_t::SCALAR_PLAT] = sharp_update_rgb_scalar;
    kernel_functions["libwebp"]["sharp_filter_row"][platform_t::SCALAR_PLAT] = sharp_filter_row_scalar;
    kernel_functions["libwebp"]["apply_alpha_multiply"][platform_t::SCALAR_PLAT] = apply_alpha_multiply_scalar;
    kernel_functions["libwebp"]["dispatch_alpha"][platform_t::SCALAR_PLAT] = dispatch_alpha_scalar;
    kernel_functions["libwebp"]["tm_prediction"][platform_t::SCALAR_PLAT] = tm_prediction_scalar;
    kernel_functions["libwebp"]["dc_prediction"][platform_t::SCALAR_PLAT] = dc_prediction_scalar;
    kernel_functions["libwebp"]["he_prediction"][platform_t::SCALAR_PLAT] = he_prediction_scalar;
    kernel_functions["libwebp"]["ve_prediction"][platform_t::SCALAR_PLAT] = ve_prediction_scalar;
    kernel_functions["libwebp"]["vertical_filter"][platform_t::SCALAR_PLAT] = vertical_filter_scalar;
    kernel_functions["libwebp"]["gradient_filter"][platform_t::SCALAR_PLAT] = gradient_filter_scalar;

    kernel_functions["boringssl"]["aes"][platform_t::SCALAR_PLAT] = aes_scalar;
    kernel_functions["boringssl"]["des"][platform_t::SCALAR_PLAT] = des_scalar;
    kernel_functions["boringssl"]["chacha20"][platform_t::SCALAR_PLAT] = chacha20_scalar;
    kernel_functions["boringssl"]["sha256"][platform_t::SCALAR_PLAT] = sha256_scalar;

    kernel_functions["zlib"]["adler32"][platform_t::SCALAR_PLAT] = adler32_scalar;
    kernel_functions["zlib"]["crc32"][platform_t::SCALAR_PLAT] = crc32_scalar;

    kernel_functions["skia"]["convolve_horizontally"][platform_t::SCALAR_PLAT] = convolve_horizontally_scalar;
    kernel_functions["skia"]["convolve_vertically"][platform_t::SCALAR_PLAT] = convolve_vertically_scalar;
    kernel_functions["skia"]["row_blend"][platform_t::SCALAR_PLAT] = row_blend_scalar;
    kernel_functions["skia"]["row_opaque"][platform_t::SCALAR_PLAT] = row_opaque_scalar;

    kernel_functions["webaudio"]["is_audible"][platform_t::SCALAR_PLAT] = is_audible_scalar;
    kernel_functions["webaudio"]["copy_with_gain"][platform_t::SCALAR_PLAT] = copy_with_gain_scalar;
    kernel_functions["webaudio"]["copy_with_sample"][platform_t::SCALAR_PLAT] = copy_with_sample_scalar;
    kernel_functions["webaudio"]["sum_from"][platform_t::SCALAR_PLAT] = sum_from_scalar;
    kernel_functions["webaudio"]["handle_nan"][platform_t::SCALAR_PLAT] = handle_nan_scalar;

    kernel_functions["optroutines"]["memchr"][platform_t::SCALAR_PLAT] = memchr_scalar;
    kernel_functions["optroutines"]["memcmp"][platform_t::SCALAR_PLAT] = memcmp_scalar;
    kernel_functions["optroutines"]["memset"][platform_t::SCALAR_PLAT] = memset_scalar;
    kernel_functions["optroutines"]["strlen"][platform_t::SCALAR_PLAT] = strlen_scalar;

    kernel_functions["xnnpack"]["gemm_fp32"][platform_t::SCALAR_PLAT] = gemm_fp32_scalar;
    kernel_functions["xnnpack"]["gemm_int32"][platform_t::SCALAR_PLAT] = gemm_int32_scalar;
    kernel_functions["xnnpack"]["gemm_fp16"][platform_t::SCALAR_PLAT] = gemm_fp16_scalar;
    kernel_functions["xnnpack"]["gemm_int16"][platform_t::SCALAR_PLAT] = gemm_int16_scalar;
    kernel_functions["xnnpack"]["spmm_fp32"][platform_t::SCALAR_PLAT] = spmm_fp32_scalar;
    kernel_functions["xnnpack"]["spmm_int32"][platform_t::SCALAR_PLAT] = spmm_int32_scalar;
    kernel_functions["xnnpack"]["spmm_fp16"][platform_t::SCALAR_PLAT] = spmm_fp16_scalar;
    kernel_functions["xnnpack"]["spmm_int16"][platform_t::SCALAR_PLAT] = spmm_int16_scalar;

    kernel_functions["libopus"]["biquad_alt"][platform_t::SCALAR_PLAT] = biquad_alt_scalar;
    kernel_functions["libopus"]["lpc_prediction"][platform_t::SCALAR_PLAT] = lpc_prediction_scalar;
    kernel_functions["libopus"]["pitch_xcorr"][platform_t::SCALAR_PLAT] = pitch_xcorr_scalar;
    kernel_functions["libopus"]["warped_autocorrelation"][platform_t::SCALAR_PLAT] = warped_autocorrelation_scalar;

    kernel_functions["libvpx"]["forward_dct"][platform_t::SCALAR_PLAT] = forward_dct_scalar;
    kernel_functions["libvpx"]["inverse_dct"][platform_t::SCALAR_PLAT] = inverse_dct_scalar;
    kernel_functions["libvpx"]["sad"][platform_t::SCALAR_PLAT] = sad_scalar;
    kernel_functions["libvpx"]["quant"][platform_t::SCALAR_PLAT] = quant_scalar;

    kernel_functions["pffft"]["fft_forward_real"][platform_t::SCALAR_PLAT] = pffft_scalar;
    kernel_functions["pffft"]["fft_backward_real"][platform_t::SCALAR_PLAT] = pffft_scalar;
    kernel_functions["pffft"]["fft_forward_complex"][platform_t::SCALAR_PLAT] = pffft_scalar;
    kernel_functions["pffft"]["fft_backward_complex"][platform_t::SCALAR_PLAT] = pffft_scalar;
#endif
}