/****************************************************************************
 *
 *
 *  Project: whisper.cpp 1.9.4
 *  Source files:
 *    include/whisper.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * include/whisper.h
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

#ifndef KERNELS_22_FFT_INCLUDE_KERNEL_H_
#define KERNELS_22_FFT_INCLUDE_KERNEL_H_

/*
 * include/whisper.h:34
 */
#define WHISPER_N_FFT 400

/*
 * Wrapper for invoking the extracted kernel.
 */
void fft_isolated(float *in, int N, float *out);

#endif // KERNELS_22_FFT_INCLUDE_KERNEL_H_
