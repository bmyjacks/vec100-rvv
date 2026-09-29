#ifndef KERNELS_58_LLAMA_QUANTIZE_Q8_0_INCLUDE_KERNEL_H_
#define KERNELS_58_LLAMA_QUANTIZE_Q8_0_INCLUDE_KERNEL_H_

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

// llama.cpp 0.4.1 ggml/include/ggml.h:2947-2948 (GCC/Clang C++).
#define GGML_RESTRICT __restrict__
// ggml/src/ggml-common.h:16,251-256.
typedef uint16_t ggml_half;
#define QK8_0 32
typedef struct {
    ggml_half d;
    int8_t qs[QK8_0];
} block_q8_0;
static_assert(sizeof(block_q8_0) == sizeof(ggml_half) + QK8_0,
              "wrong q8_0 block size/padding");
// ggml/src/ggml-impl.h:39-41.
#define MAX(a, b) ((a) > (b) ? (a) : (b))

extern "C" void quantize_row_q8_0_ref(const float *GGML_RESTRICT x,
                                       block_q8_0 *GGML_RESTRICT y, int64_t k);
void quantize_row_q8_0_rvv(const float *GGML_RESTRICT x,
                           block_q8_0 *GGML_RESTRICT y, int64_t k);

#endif // KERNELS_58_LLAMA_QUANTIZE_Q8_0_INCLUDE_KERNEL_H_
