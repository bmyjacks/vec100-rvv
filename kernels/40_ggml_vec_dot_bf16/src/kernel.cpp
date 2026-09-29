/****************************************************************************
 *
 *
 *  Project: llama.cpp 0.4.1
 *  Source files:
 *    ggml/src/ggml-cpu/vec.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ggml/src/ggml-cpu/vec.cpp
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

/* ggml/src/ggml-cpu/vec.cpp:139-146 (scalar configuration) */
void ggml_vec_dot_bf16_isolated(int n, float *GGML_RESTRICT s, size_t bs,
                       ggml_bf16_t *GGML_RESTRICT x, size_t bx,
                       ggml_bf16_t *GGML_RESTRICT y, size_t by, int nrc) {
    assert(nrc == 1);
    GGML_UNUSED(nrc);
    GGML_UNUSED(bx);
    GGML_UNUSED(by);
    GGML_UNUSED(bs);
    int i = 0;
    ggml_float sumf = 0;

    /* ggml/src/ggml-cpu/vec.cpp:257-262 */
    for (; i < n; ++i) {
        sumf += (ggml_float)(GGML_BF16_TO_FP32(x[i]) * GGML_BF16_TO_FP32(y[i]));
    }
    *s = sumf;
}
