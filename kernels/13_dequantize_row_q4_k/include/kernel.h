/****************************************************************************
 *
 *
 *  Project: llama.cpp 0.4.1
 *  Source files:
 *    ggml/include/ggml.h
 *    ggml/src/ggml-common.h
 *    ggml/src/ggml-quants.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ggml/include/ggml.h
 *
 * ggml/src/ggml-common.h
 *
 * ggml/src/ggml-quants.h
 *
 */

#ifndef KERNELS_13_DEQUANTIZE_ROW_Q4_K_INCLUDE_KERNEL_H_
#define KERNELS_13_DEQUANTIZE_ROW_Q4_K_INCLUDE_KERNEL_H_

#include <cassert>
#include <cstdint>

/*
 * ggml/include/ggml.h:370
 */
typedef uint16_t ggml_fp16_t;

/*
 * ggml/include/ggml.h:2945-2962
 */
#define GGML_RESTRICT __restrict__

/*
 * ggml/src/ggml-common.h:3-12
 */
typedef uint16_t ggml_half;
typedef uint32_t ggml_half2;

/*
 * ggml/src/ggml-common.h:89-90
 */
#define QK_K 256
#define K_SCALE_SIZE 12

/*
 * ggml/src/ggml-common.h:174-178
 */
#define GGML_EXTENSION __extension__

/*
 * ggml/src/ggml-common.h:327-338
 */
typedef struct {
    GGML_EXTENSION union {
        struct {
            ggml_half d;
            ggml_half dmin;
        };
        ggml_half2 dm;
    };
    uint8_t scales[K_SCALE_SIZE];
    uint8_t qs[QK_K / 2];
} block_q4_K;
static_assert(sizeof(block_q4_K) ==
                  2 * sizeof(ggml_half) + K_SCALE_SIZE + QK_K / 2,
              "wrong q4_K block size/padding");

/*
 * ggml/src/ggml-quants.h:10-12
 */
extern "C" {

/*
 * ggml/src/ggml-quants.h:60
 */
void dequantize_row_q4_K(const block_q4_K *GGML_RESTRICT x,
                         float *GGML_RESTRICT y, int64_t k);

/*
 * ggml/src/ggml-quants.h:113-115
 */
}

#endif // KERNELS_13_DEQUANTIZE_ROW_Q4_K_INCLUDE_KERNEL_H_
