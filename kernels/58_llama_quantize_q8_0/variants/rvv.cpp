#include "kernel.h"

#include <riscv_vector.h>

// Standalone copy of the selected ggml/src/ggml-impl.h:378-394,419-449
// conversion; this object must link without the scalar reference object.
static inline uint32_t fp32_to_bits(float f) {
    union { float as_value; uint32_t as_bits; } fp32;
    fp32.as_value = f;
    return fp32.as_bits;
}
static inline float fp32_from_bits(uint32_t w) {
    union { uint32_t as_bits; float as_value; } fp32;
    fp32.as_bits = w;
    return fp32.as_value;
}
static inline ggml_half ggml_compute_fp32_to_fp16(float f) {
    const float scale_to_inf = 0x1.0p+112f;
    const float scale_to_zero = 0x1.0p-110f;
    float base = (fabsf(f) * scale_to_inf) * scale_to_zero;
    const uint32_t w = fp32_to_bits(f);
    const uint32_t shl1_w = w + w;
    const uint32_t sign = w & UINT32_C(0x80000000);
    uint32_t bias = shl1_w & UINT32_C(0xFF000000);
    if (bias < UINT32_C(0x71000000)) bias = UINT32_C(0x71000000);
    base = fp32_from_bits((bias >> 1) + UINT32_C(0x07800000)) + base;
    const uint32_t bits = fp32_to_bits(base);
    const uint32_t exp_bits = (bits >> 13) & UINT32_C(0x00007C00);
    const uint32_t mantissa_bits = bits & UINT32_C(0x00000FFF);
    const uint32_t nonsign = exp_bits + mantissa_bits;
    return (sign >> 16) | (shl1_w > UINT32_C(0xFF000000) ? UINT16_C(0x7E00) : nonsign);
}

void quantize_row_q8_0_rvv(const float *GGML_RESTRICT x,
                           block_q8_0 *GGML_RESTRICT y, int64_t k) {
    assert(k % QK8_0 == 0);
    const int nb = k / QK8_0;
    for (int i = 0; i < nb; ++i) {
        float amax = 0.0f;
        for (int j = 0; j < QK8_0; ++j) {
            const float v = x[i * QK8_0 + j];
            amax = MAX(amax, fabsf(v));
        }
        const float d = amax / ((1 << 7) - 1);
        const float id = d ? 1.0f / d : 0.0f;
        y[i].d = ggml_compute_fp32_to_fp16(d);

        float scaled[QK8_0];
        for (int j = 0; j < QK8_0;) {
            size_t vl = __riscv_vsetvl_e32m1(QK8_0 - j);
            vfloat32m1_t v = __riscv_vle32_v_f32m1(x + i * QK8_0 + j, vl);
            __riscv_vse32_v_f32m1(scaled + j,
                                  __riscv_vfmul_vf_f32m1(v, id, vl), vl);
            j += vl;
        }
        // roundf is ties-away independent of the current FP rounding mode;
        // scalar conversion also preserves the reference's float-to-int8 rule.
        for (int j = 0; j < QK8_0; ++j) y[i].qs[j] = roundf(scaled[j]);
    }
}
