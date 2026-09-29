/****************************************************************************
 *
 *
 *  Project: stable-diffusion.cpp (revision c92d73c)
 *  Source files:
 *    ggml/src/ggml-quants.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ggml/src/ggml-quants.c
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
 * ggml/src/ggml-quants.c:148-183
 */
void quantize_row_q4_0_ref(const float *GGML_RESTRICT x,
                           block_q4_0 *GGML_RESTRICT y, int64_t k) {
    static const int qk = QK4_0;

    assert(k % qk == 0);

    const int nb = k / qk;

    for (int i = 0; i < nb; i++) {
        float amax = 0.0f;
        float max = 0.0f;

        for (int j = 0; j < qk; j++) {
            const float v = x[i * qk + j];
            if (amax < fabsf(v)) {
                amax = fabsf(v);
                max = v;
            }
        }

        const float d = max / -8;
        const float id = d ? 1.0f / d : 0.0f;

        y[i].d = GGML_FP32_TO_FP16(d);

        for (int j = 0; j < qk / 2; ++j) {
            const float x0 = x[i * qk + 0 + j] * id;
            const float x1 = x[i * qk + qk / 2 + j] * id;

            const uint8_t xi0 = MIN(15, (int8_t)(x0 + 8.5f));
            const uint8_t xi1 = MIN(15, (int8_t)(x1 + 8.5f));

            y[i].qs[j] = xi0;
            y[i].qs[j] |= xi1 << 4;
        }
    }
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void quantize_row_q4_0_bench(const float *x, uint8_t *out, int64_t k) {
    quantize_row_q4_0_ref(x, (block_q4_0 *)out, k);
}
