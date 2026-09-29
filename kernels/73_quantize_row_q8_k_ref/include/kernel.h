/****************************************************************************
 *
 *
 *  Project: llama.cpp 0.4.1
 *  Source files:
 *    ggml/include/ggml.h
 *    ggml/src/ggml-impl.h
 *    ggml/src/ggml-common.h
 *    ggml/src/ggml-quants.h
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
 * ggml/src/ggml-common.h
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
 * ggml/src/ggml-quants.h
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
 */

#ifndef KERNELS_73_QUANTIZE_ROW_Q8_K_REF_INCLUDE_KERNEL_H_
#define KERNELS_73_QUANTIZE_ROW_Q8_K_REF_INCLUDE_KERNEL_H_

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

/*
 * ggml/include/ggml.h:2947-2948 (GCC/Clang C++ configuration)
 */
#define GGML_RESTRICT __restrict__

/*
 * ggml/include/ggml.h:187 (static-library configuration)
 */
#define GGML_API extern

/*
 * ggml/src/ggml-impl.h:35-37
 */
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/*
 * ggml/src/ggml-common.h:16 (C++ configuration)
 */
typedef uint16_t ggml_half;

/*
 * ggml/src/ggml-common.h:89
 */
#define QK_K 256

/*
 * ggml/src/ggml-common.h:371-376
 */
typedef struct {
    float d;
    int8_t qs[QK_K];
    int16_t bsums[QK_K / 16];
} block_q8_K;
static_assert(sizeof(block_q8_K) ==
                  sizeof(float) + QK_K + QK_K / 16 * sizeof(int16_t),
              "wrong q8_K block size/padding");

/*
 * ggml/src/ggml-quants.h:10-12
 */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * ggml/src/ggml-quants.h:34
 */
GGML_API void quantize_row_q8_K_ref(const float *GGML_RESTRICT x,
                                    block_q8_K *GGML_RESTRICT y, int64_t k);

/*
 * ggml/src/ggml-quants.h:113-115
 */
#ifdef __cplusplus
}
#endif

/*
 * Wrapper for invoking the extracted kernel.
 */
void quantize_row_q8_K_bench(const float *x, block_q8_K *y, int64_t k);

#endif // KERNELS_73_QUANTIZE_ROW_Q8_K_REF_INCLUDE_KERNEL_H_
