/****************************************************************************
 *
 *
 *  Project: stable-diffusion.cpp (revision c92d73c)
 *  Source files:
 *    ggml/include/ggml.h
 *    ggml/src/ggml-cpu/ggml-cpu-impl.h
 *    ggml/src/ggml-cpu/vec.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ggml/include/ggml.h
 *
 * License origin: ggml/LICENSE
 *
 * MIT License
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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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
 * ggml/src/ggml-cpu/ggml-cpu-impl.h
 *
 * License origin: ggml/LICENSE
 *
 * MIT License
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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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
 * License origin: ggml/LICENSE
 *
 * MIT License
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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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

#ifndef KERNELS_39_GGML_COMPUTE_FORWARD_GROUP_NORM_F32_INCLUDE_KERNEL_H_
#define KERNELS_39_GGML_COMPUTE_FORWARD_GROUP_NORM_F32_INCLUDE_KERNEL_H_

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

/* ggml/include/ggml.h:176-188 (non-shared configuration) */
#define GGML_API extern

/* ggml/include/ggml.h:199-205 (GCC/Clang configuration) */
#define GGML_ATTRIBUTE_FORMAT(...) __attribute__((format(printf, __VA_ARGS__)))

/* ggml/include/ggml.h:222-226 */
#define GGML_MAX_DIMS 4
#define GGML_MAX_SRC 10
#define GGML_MAX_OP_PARAMS 64

/* ggml/include/ggml.h:228-230 */
#define GGML_MAX_NAME 64

/* ggml/include/ggml.h:258 */
#define GGML_UNUSED(x) (void)(x)

/* ggml/include/ggml.h:279-288 (C++ configuration) */
#define GGML_NORETURN [[noreturn]]
#define GGML_ABORT(...) ggml_abort(__FILE__, __LINE__, __VA_ARGS__)
#define GGML_ASSERT(x)                                                         \
    if (!(x))                                                                  \
    GGML_ABORT("GGML_ASSERT(%s) failed", #x)

/* ggml/include/ggml.h:298-318 */
#define GGML_TENSOR_LOCALS_1(type, prefix, pointer, array)                     \
    const type prefix##0 = (pointer) ? (pointer)->array[0] : 0;                \
    GGML_UNUSED(prefix##0);
#define GGML_TENSOR_LOCALS_2(type, prefix, pointer, array)                     \
    GGML_TENSOR_LOCALS_1(type, prefix, pointer, array)                         \
    const type prefix##1 = (pointer) ? (pointer)->array[1] : 0;                \
    GGML_UNUSED(prefix##1);
#define GGML_TENSOR_LOCALS_3(type, prefix, pointer, array)                     \
    GGML_TENSOR_LOCALS_2(type, prefix, pointer, array)                         \
    const type prefix##2 = (pointer) ? (pointer)->array[2] : 0;                \
    GGML_UNUSED(prefix##2);
#define GGML_TENSOR_LOCALS(type, prefix, pointer, array)                       \
    GGML_TENSOR_LOCALS_3(type, prefix, pointer, array)                         \
    const type prefix##3 = (pointer) ? (pointer)->array[3] : 0;                \
    GGML_UNUSED(prefix##3);

#define GGML_TENSOR_UNARY_OP_LOCALS                                            \
    GGML_TENSOR_LOCALS(int64_t, ne0, src0, ne)                                 \
    GGML_TENSOR_LOCALS(size_t, nb0, src0, nb)                                  \
    GGML_TENSOR_LOCALS(int64_t, ne, dst, ne)                                   \
    GGML_TENSOR_LOCALS(size_t, nb, dst, nb)

/* ggml/include/ggml.h:389-390 (selected tensor type) */
enum ggml_type { GGML_TYPE_F32 = 0 };

/* ggml/include/ggml.h:482-483 (selected operation value) */
enum ggml_op { GGML_OP_NONE = 0 };

/* ggml/include/ggml.h:678-713 */
struct ggml_tensor {
    enum ggml_type type;
    struct ggml_backend_buffer *buffer;
    int64_t ne[GGML_MAX_DIMS];
    size_t nb[GGML_MAX_DIMS];
    enum ggml_op op;
    int32_t op_params[GGML_MAX_OP_PARAMS / sizeof(int32_t)];
    int32_t flags;
    struct ggml_tensor *src[GGML_MAX_SRC];
    struct ggml_tensor *view_src;
    size_t view_offs;
    void *data;
    char name[GGML_MAX_NAME];
    void *extra;
    char padding[8];
};

/* ggml/include/ggml.h:2971 */
struct ggml_threadpool;

/* ggml/src/ggml-cpu/ggml-cpu-impl.h:17-30 */
struct ggml_compute_params {
    int ith, nth;
    size_t wsize;
    void *wdata;
    struct ggml_threadpool *threadpool;
    bool use_ref;
};

/* ggml/include/ggml.h:349 */
typedef void (*ggml_abort_callback_t)(const char *error_message);

/* ggml/include/ggml.h:355-356,805 (C++ linkage) */
extern "C" {
GGML_NORETURN GGML_ATTRIBUTE_FORMAT(3, 4) GGML_API
    void ggml_abort(const char *file, int line, const char *fmt, ...);
GGML_API bool ggml_are_same_shape(const struct ggml_tensor *t0,
                                  const struct ggml_tensor *t1);
}

/* ggml/src/ggml-cpu/vec.h:15 */
typedef double ggml_float;

/* Wrapper for invoking the extracted kernel. */
void ggml_compute_forward_group_norm_f32_bench(const float *input,
                                               float *output, int64_t ne00,
                                               int64_t ne01, int n_channels,
                                               int n_groups, float eps);

#endif // KERNELS_39_GGML_COMPUTE_FORWARD_GROUP_NORM_F32_INCLUDE_KERNEL_H_
