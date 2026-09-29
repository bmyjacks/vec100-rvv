#ifndef KERNELS_90_WHISPER_MEL_FILTERBANK_INCLUDE_KERNEL_H_
#define KERNELS_90_WHISPER_MEL_FILTERBANK_INCLUDE_KERNEL_H_

// Focused projection of one already-computed FFT frame. The input arrays are
// disjoint from the output; n_fft >= 0, n_mel >= 0, 0 <= i < mel_len.
void whisper_mel_filterbank_projection(const float *fft_out,
                                       const float *filters, float *mel,
                                       int n_fft, int n_mel, int mel_len, int i);
void whisper_mel_filterbank_projection_rvv(const float *fft_out,
                                           const float *filters, float *mel,
                                           int n_fft, int n_mel, int mel_len,
                                           int i);

#endif // KERNELS_90_WHISPER_MEL_FILTERBANK_INCLUDE_KERNEL_H_
