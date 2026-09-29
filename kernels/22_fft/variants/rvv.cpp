#include "kernel.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <riscv_vector.h>

namespace {
struct Cache {
    float sin_vals[WHISPER_N_FFT], cos_vals[WHISPER_N_FFT];
    Cache() {
        for (int i = 0; i < WHISPER_N_FFT; ++i) {
            double theta = (2 * M_PI * i) / WHISPER_N_FFT;
            sin_vals[i] = sinf(theta);
            cos_vals[i] = cosf(theta);
        }
    }
} cache;

void dft(const float *in, int N, float *out) {
    const int step = WHISPER_N_FFT / N;
    for (int k = 0; k < N; ++k) {
        float re = 0, im = 0;
        for (int n = 0; n < N; ++n) {
            int idx = (k * n * step) % WHISPER_N_FFT;
            re += in[n] * cache.cos_vals[idx];
            im -= in[n] * cache.sin_vals[idx];
        }
        out[2*k] = re;
        out[2*k+1] = im;
    }
}

void fft(float *in, int N, float *out) {
    if (N == 1) {
        out[0] = in[0]; out[1] = 0;
        return;
    }
    const int half = N / 2;
    if (N - half * 2 == 1) {
        dft(in, N, out);
        return;
    }
    float *even = in + N;
    for (int i = 0; i < half;) {
        size_t vl = __riscv_vsetvl_e32m1(half - i);
        auto v = __riscv_vlse32_v_f32m1(in + 2*i, 2*sizeof(float), vl);
        __riscv_vse32_v_f32m1(even + i, v, vl);
        i += vl;
    }
    float *even_fft = out + 2*N;
    fft(even, half, even_fft);
    float *odd = even;
    for (int i = 0; i < half;) {
        size_t vl = __riscv_vsetvl_e32m1(half - i);
        auto v = __riscv_vlse32_v_f32m1(in + 2*i + 1, 2*sizeof(float), vl);
        __riscv_vse32_v_f32m1(odd + i, v, vl);
        i += vl;
    }
    float *odd_fft = even_fft + N;
    fft(odd, half, odd_fft);

    // Preserve the source's ordered float operations: changing the arithmetic
    // to a vector butterfly can change rounding (and fused-multiply behavior).
    const int step = WHISPER_N_FFT / N;
    for (int k = 0; k < half; ++k) {
        int idx = k * step;
        float re = cache.cos_vals[idx];
        float im = -cache.sin_vals[idx];
        float re_odd = odd_fft[2*k];
        float im_odd = odd_fft[2*k+1];
        out[2*k] = even_fft[2*k] + re*re_odd - im*im_odd;
        out[2*k+1] = even_fft[2*k+1] + re*im_odd + im*re_odd;
        out[2*(k+half)] = even_fft[2*k] - re*re_odd + im*im_odd;
        out[2*(k+half)+1] = even_fft[2*k+1] - re*im_odd - im*re_odd;
    }
}
} // namespace

void fft_rvv(float *in, int N, float *out) { fft(in, N, out); }
