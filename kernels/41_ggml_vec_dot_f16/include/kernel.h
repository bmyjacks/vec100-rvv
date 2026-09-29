/****************************************************************************
 *
 *
 *  Project: whisper.cpp 1.9.4
 *  Source files:
 *    ggml/include/ggml.h
 *    ggml/src/ggml-cpu/vec.h
 *    ggml/src/ggml-cpu/simd-mappings.h
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
 *
 * ggml/src/ggml-cpu/simd-mappings.h
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

#ifndef KERNELS_41_GGML_VEC_DOT_F16_INCLUDE_KERNEL_H_
#define KERNELS_41_GGML_VEC_DOT_F16_INCLUDE_KERNEL_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

/* ggml/include/ggml.h:258 */
#define GGML_UNUSED(x) (void)(x)

/* ggml/include/ggml.h:370 */
typedef uint16_t ggml_fp16_t;

/* ggml/include/ggml.h:2896-2900 (GCC/Clang C++ configuration) */
#define GGML_RESTRICT __restrict__

/* ggml/src/ggml-cpu/vec.h:15 */
typedef double ggml_float;

/* ggml/src/ggml-cpu/simd-mappings.h:119 */
extern float ggml_table_f32_f16[1 << 16];

/* ggml/src/ggml-cpu/simd-mappings.h:146-154 */
inline static float ggml_lookup_fp16_to_fp32(ggml_fp16_t f) {
    uint16_t s;
    memcpy(&s, &f, sizeof(uint16_t));
    return ggml_table_f32_f16[s];
}

#define GGML_CPU_FP16_TO_FP32(x) ggml_lookup_fp16_to_fp32(x)

/* ggml/src/ggml-cpu/vec.h:53-55,73,1568-1570 */
extern "C" void ggml_vec_dot_f16_rvv(int n, float *GGML_RESTRICT s, size_t bs,
                                 ggml_fp16_t *GGML_RESTRICT x, size_t bx,
                                 ggml_fp16_t *GGML_RESTRICT y, size_t by,
                                 int nrc);

/* Wrapper for invoking the extracted kernel with its lookup table initialized.
 */
void ggml_vec_dot_f16_isolated(int n, float *s, size_t bs, ggml_fp16_t *x,
                               size_t bx, ggml_fp16_t *y, size_t by, int nrc);

#endif // KERNELS_41_GGML_VEC_DOT_F16_INCLUDE_KERNEL_H_
