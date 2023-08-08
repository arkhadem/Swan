#ifndef FC597AA3_5A1C_4BCC_B0A0_18F269E78F27
#define FC597AA3_5A1C_4BCC_B0A0_18F269E78F27

#include "swan.hpp"

#ifdef SWAN_SIMULATION
void rgb_to_ycbcr_scalar(config_t *, input_t *, output_t *);
void pitch_xcorr_scalar(config_t *, input_t *, output_t *);
void tm_prediction_scalar(config_t *, input_t *, output_t *);
void adler32_scalar(config_t *, input_t *, output_t *);
void convolve_horizontally_scalar(config_t *, input_t *, output_t *);
void is_audible_scalar(config_t *, input_t *, output_t *);
void gemm_fp32_scalar(config_t *, input_t *, output_t *);
void sad_scalar(config_t *, input_t *, output_t *);
#else

void downsample_scalar(config_t *, input_t *, output_t *);
void ycbcr_to_rgb_scalar(config_t *, input_t *, output_t *);
void rgb_to_ycbcr_scalar(config_t *, input_t *, output_t *);
void upsample_scalar(config_t *, input_t *, output_t *);
void rgb_to_gray_scalar(config_t *, input_t *, output_t *);

void read_sub_scalar(config_t *, input_t *, output_t *);
void read_up_scalar(config_t *, input_t *, output_t *);
void read_avg_scalar(config_t *, input_t *, output_t *);
void read_paeth_scalar(config_t *, input_t *, output_t *);
void expand_palette_scalar(config_t *, input_t *, output_t *);

void sharp_update_rgb_scalar(config_t *, input_t *, output_t *);
void sharp_filter_row_scalar(config_t *, input_t *, output_t *);
void apply_alpha_multiply_scalar(config_t *, input_t *, output_t *);
void dispatch_alpha_scalar(config_t *, input_t *, output_t *);
void tm_prediction_scalar(config_t *, input_t *, output_t *);
void dc_prediction_scalar(config_t *, input_t *, output_t *);
void he_prediction_scalar(config_t *, input_t *, output_t *);
void ve_prediction_scalar(config_t *, input_t *, output_t *);
void vertical_filter_scalar(config_t *, input_t *, output_t *);
void gradient_filter_scalar(config_t *, input_t *, output_t *);

void aes_scalar(config_t *, input_t *, output_t *);
void des_scalar(config_t *, input_t *, output_t *);
void chacha20_scalar(config_t *, input_t *, output_t *);
void sha256_scalar(config_t *, input_t *, output_t *);

void adler32_scalar(config_t *, input_t *, output_t *);
void crc32_scalar(config_t *, input_t *, output_t *);

void convolve_horizontally_scalar(config_t *, input_t *, output_t *);
void convolve_vertically_scalar(config_t *, input_t *, output_t *);
void row_blend_scalar(config_t *, input_t *, output_t *);
void row_opaque_scalar(config_t *, input_t *, output_t *);

void is_audible_scalar(config_t *, input_t *, output_t *);
void copy_with_gain_scalar(config_t *, input_t *, output_t *);
void copy_with_sample_scalar(config_t *, input_t *, output_t *);
void sum_from_scalar(config_t *, input_t *, output_t *);
void handle_nan_scalar(config_t *, input_t *, output_t *);

void memchr_scalar(config_t *, input_t *, output_t *);
void memcmp_scalar(config_t *, input_t *, output_t *);
void memset_scalar(config_t *, input_t *, output_t *);
void strlen_scalar(config_t *, input_t *, output_t *);

void gemm_fp32_scalar(config_t *, input_t *, output_t *);
void gemm_int32_scalar(config_t *, input_t *, output_t *);
void gemm_fp16_scalar(config_t *, input_t *, output_t *);
void gemm_int16_scalar(config_t *, input_t *, output_t *);
void spmm_fp32_scalar(config_t *, input_t *, output_t *);
void spmm_int32_scalar(config_t *, input_t *, output_t *);
void spmm_fp16_scalar(config_t *, input_t *, output_t *);
void spmm_int16_scalar(config_t *, input_t *, output_t *);

void biquad_alt_scalar(config_t *, input_t *, output_t *);
void lpc_prediction_scalar(config_t *, input_t *, output_t *);
void pitch_xcorr_scalar(config_t *, input_t *, output_t *);
void warped_autocorrelation_scalar(config_t *, input_t *, output_t *);

void forward_dct_scalar(config_t *, input_t *, output_t *);
void inverse_dct_scalar(config_t *, input_t *, output_t *);
void sad_scalar(config_t *, input_t *, output_t *);
void quant_scalar(config_t *, input_t *, output_t *);

void pffft_scalar(config_t *, input_t *, output_t *);
#endif
#endif /* FC597AA3_5A1C_4BCC_B0A0_18F269E78F27 */
