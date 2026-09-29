/****************************************************************************
 *
 *
 *  Project: whisper.cpp 1.9.4
 *  Source files:
 *    src/whisper.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/whisper.cpp
 *
 * Copyright (c) 2023-2026 The ggml authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "kernel.h"

/*
 * src/whisper.cpp:25
 */
#define _USE_MATH_DEFINES
#include <cmath>

/*
 * src/whisper.cpp:3046-3082
 */
#define SIN_COS_N_COUNT WHISPER_N_FFT
namespace {
struct whisper_global_cache {
    float sin_vals[SIN_COS_N_COUNT];
    float cos_vals[SIN_COS_N_COUNT];

    float hann_window[WHISPER_N_FFT];

    whisper_global_cache() {
        fill_sin_cos_table();
        fill_hann_window(sizeof(hann_window) / sizeof(hann_window[0]), true,
                         hann_window);
    }

    void fill_sin_cos_table() {
        for (int i = 0; i < SIN_COS_N_COUNT; i++) {
            double theta = (2 * M_PI * i) / SIN_COS_N_COUNT;
            sin_vals[i] = sinf(theta);
            cos_vals[i] = cosf(theta);
        }
    }

    void fill_hann_window(int length, bool periodic, float *output) {
        int offset = -1;
        if (periodic) {
            offset = 0;
        }
        for (int i = 0; i < length; i++) {
            output[i] =
                0.5 * (1.0 - cosf((2.0 * M_PI * i) / (length + offset)));
        }
    }
} global_cache;
} // namespace

/*
 * src/whisper.cpp:3087-3103
 */
static void dft(const float *in, int N, float *out) {
    const int sin_cos_step = SIN_COS_N_COUNT / N;

    for (int k = 0; k < N; k++) {
        float re = 0;
        float im = 0;

        for (int n = 0; n < N; n++) {
            int idx = (k * n * sin_cos_step) % (SIN_COS_N_COUNT);
            re += in[n] * global_cache.cos_vals[idx];
            im -= in[n] * global_cache.sin_vals[idx];
        }

        out[k * 2 + 0] = re;
        out[k * 2 + 1] = im;
    }
}

/*
 * src/whisper.cpp:3109-3151
 */
static void fft(float *in, int N, float *out) {
    if (N == 1) {
        out[0] = in[0];
        out[1] = 0;
        return;
    }

    const int half_N = N / 2;
    if (N - half_N * 2 == 1) {
        dft(in, N, out);
        return;
    }

    float *even = in + N;
    for (int i = 0; i < half_N; ++i) {
        even[i] = in[2 * i];
    }
    float *even_fft = out + 2 * N;
    fft(even, half_N, even_fft);

    float *odd = even;
    for (int i = 0; i < half_N; ++i) {
        odd[i] = in[2 * i + 1];
    }
    float *odd_fft = even_fft + N;
    fft(odd, half_N, odd_fft);

    const int sin_cos_step = SIN_COS_N_COUNT / N;
    for (int k = 0; k < half_N; k++) {
        int idx = k * sin_cos_step;
        float re = global_cache.cos_vals[idx];
        float im = -global_cache.sin_vals[idx];

        float re_odd = odd_fft[2 * k + 0];
        float im_odd = odd_fft[2 * k + 1];

        out[2 * k + 0] = even_fft[2 * k + 0] + re * re_odd - im * im_odd;
        out[2 * k + 1] = even_fft[2 * k + 1] + re * im_odd + im * re_odd;

        out[2 * (k + half_N) + 0] =
            even_fft[2 * k + 0] - re * re_odd + im * im_odd;
        out[2 * (k + half_N) + 1] =
            even_fft[2 * k + 1] - re * im_odd - im * re_odd;
    }
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void fft_isolated(float *in, int N, float *out) { fft(in, N, out); }
