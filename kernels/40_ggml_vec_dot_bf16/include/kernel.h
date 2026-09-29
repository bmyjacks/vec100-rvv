/****************************************************************************
 *
 *
 *  Project: llama.cpp 0.4.1
 *  Source files:
 *    ggml/include/ggml.h
 *    ggml/src/ggml-impl.h
 *    ggml/src/ggml-cpu/vec.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ggml/include/ggml.h
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
 *
 * ggml/src/ggml-cpu/vec.h
 *
 *   Vectorized functions for fundamental operations
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

#ifndef KERNELS_40_GGML_VEC_DOT_BF16_INCLUDE_KERNEL_H_
#define KERNELS_40_GGML_VEC_DOT_BF16_INCLUDE_KERNEL_H_

#include <cassert>
#include <cstddef>
#include <cstdint>

/*
 * ggml/include/ggml.h:258
 */
#define GGML_UNUSED(x) (void)(x)

/*
 * ggml/include/ggml.h:377
 */
typedef struct {
    uint16_t bits;
} ggml_bf16_t;

/* ggml/include/ggml.h:2945-2949 (GCC/Clang C++ configuration) */
#define GGML_RESTRICT __restrict__

/*
 * ggml/src/ggml-impl.h:606-613
 */
static inline float ggml_compute_bf16_to_fp32(ggml_bf16_t h) {
    union {
        float f;
        uint32_t i;
    } u;
    u.i = (uint32_t)h.bits << 16;
    return u.f;
}

/*
 * ggml/src/ggml-impl.h:639
 */
#define GGML_BF16_TO_FP32(x) ggml_compute_bf16_to_fp32(x)

/*
 * ggml/src/ggml-cpu/vec.h:15
 */
typedef double ggml_float;

/*
 * ggml/src/ggml-cpu/vec.h:53-54
 */
extern "C" {

/*
 * ggml/src/ggml-cpu/vec.h:72
 */
void ggml_vec_dot_bf16_isolated(int n, float *GGML_RESTRICT s, size_t bs,
                        ggml_bf16_t *GGML_RESTRICT x, size_t bx,
                        ggml_bf16_t *GGML_RESTRICT y, size_t by, int nrc);
void ggml_vec_dot_bf16_rvv(int n, float *GGML_RESTRICT s, size_t bs,
                           ggml_bf16_t *GGML_RESTRICT x, size_t bx,
                           ggml_bf16_t *GGML_RESTRICT y, size_t by, int nrc);

/*
 * ggml/src/ggml-cpu/vec.h:1568-1570
 */
}

#endif // KERNELS_40_GGML_VEC_DOT_BF16_INCLUDE_KERNEL_H_
