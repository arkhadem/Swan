#include "swan.hpp"

#include "neon.hpp"
#include "runner.hpp"
#include "utility.hpp"
#include <stddef.h>
#include <stdio.h>

void register_neon_kernels() {
    kernel_functions["zlib"]["adler32"][platform_t::NEON_PLAT] = adler32_neon;

    kernel_functions["libjpeg"]["rgb_to_gray"][platform_t::NEON_PLAT] = rgb_to_gray_neon;
    kernel_functions["libjpeg"]["ycbcr_to_rgb"][platform_t::NEON_PLAT] = ycbcr_to_rgb_neon;
    kernel_functions["libjpeg"]["rgb_to_ycbcr"][platform_t::NEON_PLAT] = rgb_to_ycbcr_neon;
    kernel_functions["libjpeg"]["upsample"][platform_t::NEON_PLAT] = upsample_neon;
    kernel_functions["libjpeg"]["downsample"][platform_t::NEON_PLAT] = downsample_neon;

    kernel_functions["libpng"]["read_sub"][platform_t::NEON_PLAT] = read_sub_neon;
    kernel_functions["libpng"]["read_up"][platform_t::NEON_PLAT] = read_up_neon;
    kernel_functions["libpng"]["read_avg"][platform_t::NEON_PLAT] = read_avg_neon;
    kernel_functions["libpng"]["read_paeth"][platform_t::NEON_PLAT] = read_paeth_neon;
    kernel_functions["libpng"]["expand_palette"][platform_t::NEON_PLAT] = expand_palette_neon;

    kernel_functions["libwebp"]["sharp_update_rgb"][platform_t::NEON_PLAT] = sharp_update_rgb_neon;
    kernel_functions["libwebp"]["sharp_filter_row"][platform_t::NEON_PLAT] = sharp_filter_row_neon;
    kernel_functions["libwebp"]["apply_alpha_multiply"][platform_t::NEON_PLAT] = apply_alpha_multiply_neon;
    kernel_functions["libwebp"]["dispatch_alpha"][platform_t::NEON_PLAT] = dispatch_alpha_neon;
    kernel_functions["libwebp"]["tm_prediction"][platform_t::NEON_PLAT] = tm_prediction_neon;
    kernel_functions["libwebp"]["dc_prediction"][platform_t::NEON_PLAT] = dc_prediction_neon;
    kernel_functions["libwebp"]["he_prediction"][platform_t::NEON_PLAT] = he_prediction_neon;
    kernel_functions["libwebp"]["ve_prediction"][platform_t::NEON_PLAT] = ve_prediction_neon;
    kernel_functions["libwebp"]["vertical_filter"][platform_t::NEON_PLAT] = vertical_filter_neon;
    kernel_functions["libwebp"]["gradient_filter"][platform_t::NEON_PLAT] = gradient_filter_neon;

    kernel_functions["skia"]["convolve_horizontally"][platform_t::NEON_PLAT] = convolve_horizontally_neon;
    kernel_functions["skia"]["convolve_vertically"][platform_t::NEON_PLAT] = convolve_vertically_neon;
    kernel_functions["skia"]["row_blend"][platform_t::NEON_PLAT] = row_blend_neon;
    kernel_functions["skia"]["row_opaque"][platform_t::NEON_PLAT] = row_opaque_neon;

    kernel_functions["webaudio"]["is_audible"][platform_t::NEON_PLAT] = is_audible_neon;
    kernel_functions["webaudio"]["copy_with_gain"][platform_t::NEON_PLAT] = copy_with_gain_neon;
    kernel_functions["webaudio"]["copy_with_sample"][platform_t::NEON_PLAT] = copy_with_sample_neon;
    kernel_functions["webaudio"]["sum_from"][platform_t::NEON_PLAT] = sum_from_neon;
    kernel_functions["webaudio"]["handle_nan"][platform_t::NEON_PLAT] = handle_nan_neon;

    kernel_functions["xnnpack"]["gemm_fp32"][platform_t::NEON_PLAT] = gemm_fp32_neon;
    kernel_functions["xnnpack"]["gemm_int32"][platform_t::NEON_PLAT] = gemm_int32_neon;
    kernel_functions["xnnpack"]["gemm_int16"][platform_t::NEON_PLAT] = gemm_int16_neon;
    kernel_functions["xnnpack"]["spmm_fp32"][platform_t::NEON_PLAT] = spmm_fp32_neon;
    kernel_functions["xnnpack"]["spmm_int32"][platform_t::NEON_PLAT] = spmm_int32_neon;
    kernel_functions["xnnpack"]["spmm_int16"][platform_t::NEON_PLAT] = spmm_int16_neon;

    kernel_functions["libopus"]["biquad_alt"][platform_t::NEON_PLAT] = biquad_alt_neon;
    kernel_functions["libopus"]["lpc_prediction"][platform_t::NEON_PLAT] = lpc_prediction_neon;
    kernel_functions["libopus"]["pitch_xcorr"][platform_t::NEON_PLAT] = pitch_xcorr_neon;
    kernel_functions["libopus"]["warped_autocorrelation"][platform_t::NEON_PLAT] = warped_autocorrelation_neon;

    kernel_functions["libvpx"]["forward_dct"][platform_t::NEON_PLAT] = forward_dct_neon;
    kernel_functions["libvpx"]["inverse_dct"][platform_t::NEON_PLAT] = inverse_dct_neon;
    kernel_functions["libvpx"]["sad"][platform_t::NEON_PLAT] = sad_neon;
    kernel_functions["libvpx"]["quant"][platform_t::NEON_PLAT] = quant_neon;

#ifndef SWAN_SIMULATION

    // boringssl is not supported in simulation mode
    kernel_functions["boringssl"]["aes"][platform_t::NEON_PLAT] = aes_neon;
    kernel_functions["boringssl"]["des"][platform_t::NEON_PLAT] = des_neon;
    kernel_functions["boringssl"]["chacha20"][platform_t::NEON_PLAT] = chacha20_neon;
    kernel_functions["boringssl"]["sha256"][platform_t::NEON_PLAT] = sha256_neon;

    // crc32 is not supported in simulation mode
    kernel_functions["zlib"]["crc32"][platform_t::NEON_PLAT] = crc32_neon;

    // optroutines is not supported in simulation mode
    kernel_functions["optroutines"]["memchr"][platform_t::NEON_PLAT] = memchr_neon;
    kernel_functions["optroutines"]["memcmp"][platform_t::NEON_PLAT] = memcmp_neon;
    kernel_functions["optroutines"]["memset"][platform_t::NEON_PLAT] = memset_neon;
    kernel_functions["optroutines"]["strlen"][platform_t::NEON_PLAT] = strlen_neon;

    // pffft is not supported in simulation mode
    kernel_functions["pffft"]["fft_forward_real"][platform_t::NEON_PLAT] = pffft_neon;
    kernel_functions["pffft"]["fft_backward_real"][platform_t::NEON_PLAT] = pffft_neon;
    kernel_functions["pffft"]["fft_forward_complex"][platform_t::NEON_PLAT] = pffft_neon;
    kernel_functions["pffft"]["fft_backward_complex"][platform_t::NEON_PLAT] = pffft_neon;

    // FP16 is not supported in simulation mode
    kernel_functions["xnnpack"]["gemm_fp16"][platform_t::NEON_PLAT] = gemm_fp16_neon;
    kernel_functions["xnnpack"]["spmm_fp16"][platform_t::NEON_PLAT] = spmm_fp16_neon;

#endif
}
