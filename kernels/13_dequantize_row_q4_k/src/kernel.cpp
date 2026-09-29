/****************************************************************************
 *
 *
 *  Project: llama.cpp 0.4.1
 *  Source files:
 *    ggml/src/ggml-impl.h
 *    ggml/src/ggml-quants.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ggml/src/ggml-impl.h
 *
 * ggml/src/ggml-quants.c
 *
 */

#include "kernel.h"

/*
 * ggml/src/ggml-impl.h:378-385
 */
static inline float fp32_from_bits(uint32_t w) {
    union {
        uint32_t as_bits;
        float as_value;
    } fp32;
    fp32.as_bits = w;
    return fp32.as_value;
}

/*
 * ggml/src/ggml-impl.h:387-394
 */
static inline uint32_t fp32_to_bits(float f) {
    union {
        float as_value;
        uint32_t as_bits;
    } fp32;
    fp32.as_value = f;
    return fp32.as_bits;
}

/*
 * ggml/src/ggml-impl.h:396-417
 */
static inline float ggml_compute_fp16_to_fp32(ggml_fp16_t h) {
    const uint32_t w = (uint32_t)h << 16;
    const uint32_t sign = w & UINT32_C(0x80000000);
    const uint32_t two_w = w + w;

    const uint32_t exp_offset = UINT32_C(0xE0) << 23;
    const float exp_scale = fp32_from_bits(UINT32_C(0x7800000));
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

/*
 * ggml/src/ggml-impl.h:445
 */
#define GGML_COMPUTE_FP16_TO_FP32(x) ggml_compute_fp16_to_fp32(x)

/*
 * ggml/src/ggml-impl.h:448
 */
#define GGML_FP16_TO_FP32(x) GGML_COMPUTE_FP16_TO_FP32(x)

/*
 * ggml/src/ggml-quants.c:880-887
 */
static inline void get_scale_min_k4(int j, const uint8_t *GGML_RESTRICT q,
                                    uint8_t *GGML_RESTRICT d,
                                    uint8_t *GGML_RESTRICT m) {
    if (j < 4) {
        *d = q[j] & 63;
        *m = q[j + 4] & 63;
    } else {
        *d = (q[j + 4] & 0xF) | ((q[j - 4] >> 6) << 4);
        *m = (q[j + 4] >> 4) | ((q[j - 0] >> 6) << 4);
    }
}

/*
 * ggml/src/ggml-quants.c:1529-1551
 */
void dequantize_row_q4_K(const block_q4_K *GGML_RESTRICT x,
                         float *GGML_RESTRICT y, int64_t k) {
    assert(k % QK_K == 0);
    const int nb = k / QK_K;

    for (int i = 0; i < nb; i++) {
        const uint8_t *q = x[i].qs;

        const float d = GGML_FP16_TO_FP32(x[i].d);
        const float min = GGML_FP16_TO_FP32(x[i].dmin);

        int is = 0;
        uint8_t sc, m;
        for (int j = 0; j < QK_K; j += 64) {
            get_scale_min_k4(is + 0, x[i].scales, &sc, &m);
            const float d1 = d * sc;
            const float m1 = min * m;
            get_scale_min_k4(is + 1, x[i].scales, &sc, &m);
            const float d2 = d * sc;
            const float m2 = min * m;
            for (int l = 0; l < 32; ++l)
                *y++ = d1 * (q[l] & 0xF) - m1;
            for (int l = 0; l < 32; ++l)
                *y++ = d2 * (q[l] >> 4) - m2;
            q += 32;
            is += 2;
        }
    }
}
