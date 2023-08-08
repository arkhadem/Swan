#ifndef FC597AA3_5A1C_4BCC_B0A0_18F269E78F27
#define FC597AA3_5A1C_4BCC_B0A0_18F269E78F27

#include "swan.hpp"

#ifdef SWAN_SIMULATION
void rgb_to_ycbcr_neon(config_t *, input_t *, output_t *);
void pitch_xcorr_neon(config_t *, input_t *, output_t *);
void tm_prediction_neon(config_t *, input_t *, output_t *);
void adler32_neon(config_t *, input_t *, output_t *);
void convolve_horizontally_neon(config_t *, input_t *, output_t *);
void is_audible_neon(config_t *, input_t *, output_t *);
void gemm_fp32_neon(config_t *, input_t *, output_t *);
void sad_neon(config_t *, input_t *, output_t *);
#else
void downsample_neon(config_t *, input_t *, output_t *);
void ycbcr_to_rgb_neon(config_t *, input_t *, output_t *);
void rgb_to_ycbcr_neon(config_t *, input_t *, output_t *);
void upsample_neon(config_t *, input_t *, output_t *);
void rgb_to_gray_neon(config_t *, input_t *, output_t *);

void read_sub_neon(config_t *, input_t *, output_t *);
void read_up_neon(config_t *, input_t *, output_t *);
void read_avg_neon(config_t *, input_t *, output_t *);
void read_paeth_neon(config_t *, input_t *, output_t *);
void expand_palette_neon(config_t *, input_t *, output_t *);

void sharp_update_rgb_neon(config_t *, input_t *, output_t *);
void sharp_filter_row_neon(config_t *, input_t *, output_t *);
void apply_alpha_multiply_neon(config_t *, input_t *, output_t *);
void dispatch_alpha_neon(config_t *, input_t *, output_t *);
void tm_prediction_neon(config_t *, input_t *, output_t *);
void dc_prediction_neon(config_t *, input_t *, output_t *);
void he_prediction_neon(config_t *, input_t *, output_t *);
void ve_prediction_neon(config_t *, input_t *, output_t *);
void vertical_filter_neon(config_t *, input_t *, output_t *);
void gradient_filter_neon(config_t *, input_t *, output_t *);

void aes_neon(config_t *, input_t *, output_t *);
void des_neon(config_t *, input_t *, output_t *);
void chacha20_neon(config_t *, input_t *, output_t *);
void sha256_neon(config_t *, input_t *, output_t *);

void adler32_neon(config_t *, input_t *, output_t *);
void crc32_neon(config_t *, input_t *, output_t *);

void convolve_horizontally_neon(config_t *, input_t *, output_t *);
void convolve_vertically_neon(config_t *, input_t *, output_t *);
void row_blend_neon(config_t *, input_t *, output_t *);
void row_opaque_neon(config_t *, input_t *, output_t *);

void is_audible_neon(config_t *, input_t *, output_t *);
void copy_with_gain_neon(config_t *, input_t *, output_t *);
void copy_with_sample_neon(config_t *, input_t *, output_t *);
void sum_from_neon(config_t *, input_t *, output_t *);
void handle_nan_neon(config_t *, input_t *, output_t *);

void memchr_neon(config_t *, input_t *, output_t *);
void memcmp_neon(config_t *, input_t *, output_t *);
void memset_neon(config_t *, input_t *, output_t *);
void strlen_neon(config_t *, input_t *, output_t *);

void gemm_fp32_neon(config_t *, input_t *, output_t *);
void gemm_int32_neon(config_t *, input_t *, output_t *);
void gemm_fp16_neon(config_t *, input_t *, output_t *);
void gemm_int16_neon(config_t *, input_t *, output_t *);
void spmm_fp32_neon(config_t *, input_t *, output_t *);
void spmm_int32_neon(config_t *, input_t *, output_t *);
void spmm_fp16_neon(config_t *, input_t *, output_t *);
void spmm_int16_neon(config_t *, input_t *, output_t *);

void biquad_alt_neon(config_t *, input_t *, output_t *);
void lpc_prediction_neon(config_t *, input_t *, output_t *);
void pitch_xcorr_neon(config_t *, input_t *, output_t *);
void warped_autocorrelation_neon(config_t *, input_t *, output_t *);

void forward_dct_neon(config_t *, input_t *, output_t *);
void inverse_dct_neon(config_t *, input_t *, output_t *);
void sad_neon(config_t *, input_t *, output_t *);
void quant_neon(config_t *, input_t *, output_t *);

void pffft_neon(config_t *, input_t *, output_t *);
#endif
#endif /* FC597AA3_5A1C_4BCC_B0A0_18F269E78F27 */
