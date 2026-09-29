/****************************************************************************
 *
 *
 *  Project: whisper.cpp 1.9.4
 *  Source files:
 *    ggml/src/ggml-impl.h
 *    ggml/src/ggml-cpu/ggml-cpu.c
 *    ggml/src/ggml-cpu/vec.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ggml/src/ggml-impl.h
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
 * ggml/src/ggml-cpu/ggml-cpu.c
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

/* ggml/src/ggml-impl.h:366-382 */
static inline float fp32_from_bits(uint32_t w) {
    union {
        uint32_t as_bits;
        float as_value;
    } fp32;
    fp32.as_bits = w;
    return fp32.as_value;
}

static inline uint32_t fp32_to_bits(float f) {
    union {
        float as_value;
        uint32_t as_bits;
    } fp32;
    fp32.as_value = f;
    return fp32.as_bits;
}

/* ggml/src/ggml-impl.h:384-405 (C++17 scalar configuration) */
static inline float ggml_compute_fp16_to_fp32(ggml_fp16_t h) {
    const uint32_t w = (uint32_t)h << 16;
    const uint32_t sign = w & UINT32_C(0x80000000);
    const uint32_t two_w = w + w;

    const uint32_t exp_offset = UINT32_C(0xE0) << 23;
    const float exp_scale = 0x1.0p-112f;
    const float normalized_value =
        fp32_from_bits((two_w >> 4) + exp_offset) * exp_scale;

    const uint32_t magic_mask = UINT32_C(126) << 23;
    const float magic_bias = 0.5f;
    const float denormalized_value =
        fp32_from_bits((two_w >> 17) | magic_mask) - magic_bias;

    const uint32_t denormalized_cutoff = UINT32_C(1) << 27;
    const uint32_t result =
        sign | (two_w < denormalized_cutoff ? fp32_to_bits(denormalized_value)
                                            : fp32_to_bits(normalized_value));
    return fp32_from_bits(result);
}

/* ggml/src/ggml-cpu/ggml-cpu.c:81 */
float ggml_table_f32_f16[1 << 16];

/* ggml/src/ggml-cpu/vec.cpp:264-274 (scalar configuration) */
static void ggml_vec_dot_f16_impl(int n, float *GGML_RESTRICT s, size_t bs,
                      ggml_fp16_t *GGML_RESTRICT x, size_t bx,
                      ggml_fp16_t *GGML_RESTRICT y, size_t by, int nrc) {
    assert(nrc == 1);
    GGML_UNUSED(nrc);
    GGML_UNUSED(bx);
    GGML_UNUSED(by);
    GGML_UNUSED(bs);

    ggml_float sumf = 0.0;

    /* ggml/src/ggml-cpu/vec.cpp:371-378 */
    for (int i = 0; i < n; ++i) {
        sumf += (ggml_float)(GGML_CPU_FP16_TO_FP32(x[i]) *
                             GGML_CPU_FP16_TO_FP32(y[i]));
    }

    *s = sumf;
}

/* Wrapper for invoking the extracted kernel; initializes the lookup table as
 * in ggml_cpu_init (ggml/src/ggml-cpu/ggml-cpu.c:3869-3882). */
void ggml_vec_dot_f16_isolated(int n, float *s, size_t bs, ggml_fp16_t *x,
                               size_t bx, ggml_fp16_t *y, size_t by, int nrc) {
    static const bool initialized = [] {
        for (int i = 0; i < (1 << 16); ++i) {
            union {
                uint16_t u16;
                ggml_fp16_t fp16;
            } u = {(uint16_t)i};
            float f = ggml_compute_fp16_to_fp32(u.fp16);
            ggml_table_f32_f16[i] = f;
        }
        return true;
    }();
    (void)initialized;
    ggml_vec_dot_f16_impl(n, s, bs, x, bx, y, by, nrc);
}
