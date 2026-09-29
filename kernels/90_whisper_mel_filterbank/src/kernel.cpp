#include "kernel.h"

#include <algorithm>
#include <cmath>

// whisper.cpp 1.9.4 src/whisper.cpp:3188-3206, after the frame FFT and
// modulus computation. fft_out/filters/mel are the .data() of the upstream
// vectors; n_fft, n_mel, mel_len and i are their original runtime values.
void whisper_mel_filterbank_projection(const float *fft_out,
                                       const float *filters, float *mel,
                                       int n_fft, int n_mel, int mel_len, int i) {
    for (int j = 0; j < n_mel; j++) {
        double sum = 0.0;
        int k = 0;
        for (k = 0; k < n_fft - 3; k += 4) {
            sum +=
                    fft_out[k + 0] * filters[j * n_fft + k + 0] +
                    fft_out[k + 1] * filters[j * n_fft + k + 1] +
                    fft_out[k + 2] * filters[j * n_fft + k + 2] +
                    fft_out[k + 3] * filters[j * n_fft + k + 3];
        }
        for (; k < n_fft; k++) {
            sum += fft_out[k] * filters[j * n_fft + k];
        }
        sum = log10(std::max(sum, 1e-10));
        mel[j * mel_len + i] = sum;
    }
}
