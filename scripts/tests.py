
libjpeg_test = []
libpng_test = []
libwebp_test = []
boringssl_test = []
optroutines_test = []
zlib_test = []
skia_test = []
webaudio_test = []
xnnpack_test = []
libopus_test = []
libvpx_test = []
pffft_test = []

libjpeg_test.append("rgb_to_gray")
libjpeg_test.append("ycbcr_to_rgb")
libjpeg_test.append("rgb_to_ycbcr")
libjpeg_test.append("upsample")
libjpeg_test.append("downsample")

libpng_test.append("read_sub")
libpng_test.append("read_up")
libpng_test.append("read_avg")
libpng_test.append("read_paeth")
libpng_test.append("expand_palette")

libwebp_test.append("sharp_update_rgb")
libwebp_test.append("sharp_filter_row")
libwebp_test.append("apply_alpha_multiply")
libwebp_test.append("dispatch_alpha")
libwebp_test.append("tm_prediction")
libwebp_test.append("dc_prediction")
libwebp_test.append("ve_prediction")
libwebp_test.append("he_prediction")
libwebp_test.append("vertical_filter")
libwebp_test.append("gradient_filter")

boringssl_test.append("aes")
boringssl_test.append("des")
boringssl_test.append("chacha20")
boringssl_test.append("sha256")

zlib_test.append("adler32")
zlib_test.append("crc32")

optroutines_test.append("memchr")
optroutines_test.append("memcmp")
optroutines_test.append("memset")
optroutines_test.append("strlen")

skia_test.append("convolve_horizontally")
skia_test.append("convolve_vertically")
skia_test.append("row_blend")
skia_test.append("row_opaque")

webaudio_test.append("is_audible")
webaudio_test.append("copy_with_gain")
webaudio_test.append("copy_with_sample")
webaudio_test.append("sum_from")
webaudio_test.append("handle_nan")

xnnpack_test.append("gemm_fp32")
xnnpack_test.append("gemm_int32")
xnnpack_test.append("gemm_fp16")
xnnpack_test.append("gemm_int16")
xnnpack_test.append("spmm_fp32")
xnnpack_test.append("spmm_int32")
xnnpack_test.append("spmm_fp16")
xnnpack_test.append("spmm_int16")

libopus_test.append("biquad_alt")
libopus_test.append("lpc_prediction")
libopus_test.append("pitch_xcorr")
libopus_test.append("warped_autocorrelation")

libvpx_test.append("forward_dct")
libvpx_test.append("inverse_dct")
libvpx_test.append("sad")
libvpx_test.append("quant")

pffft_test.append("fft_forward_real")
pffft_test.append("fft_backward_real")
pffft_test.append("fft_forward_complex")
pffft_test.append("fft_backward_complex")

tests_bench = {}

tests_bench["libjpeg"] = libjpeg_test
tests_bench["libpng"] = libpng_test
tests_bench["libwebp"] = libwebp_test
tests_bench["boringssl"] = boringssl_test
tests_bench["zlib"] = zlib_test
tests_bench["optroutines"] = optroutines_test
tests_bench["skia"] = skia_test
tests_bench["webaudio"] = webaudio_test
tests_bench["xnnpack"] = xnnpack_test
tests_bench["libopus"] = libopus_test
tests_bench["libvpx"] = libvpx_test
tests_bench["pffft"] = pffft_test

library_list = ["libjpeg",
                "libpng",
                "libwebp",
                "boringssl",
                "zlib",
                "optroutines",
                "skia",
                "webaudio",
                "xnnpack",
                "libopus",
                "libvpx",
                "pffft"]

platform_list = ["scalar", "neon"]