#include "kernel.h"

#include <riscv_vector.h>

// ggml/src/ggml-impl.h:396-417: retain ggml's software half conversion,
// including its subnormal and exceptional-value behavior (no Zvfh required).
static float half_to_float(ggml_fp16_t h) {
    union Bits { uint32_t u; float f; } bits;
    const uint32_t w = uint32_t(h) << 16;
    const uint32_t sign = w & UINT32_C(0x80000000);
    const uint32_t twice = w + w;
    bits.u = UINT32_C(0x07800000);
    const float scale = bits.f;
    bits.u = (twice >> 4) + (UINT32_C(0xe0) << 23);
    const float normal = bits.f * scale;
    bits.u = (twice >> 17) | (UINT32_C(126) << 23);
    const float subnormal = bits.f - 0.5f;
    bits.f = twice < (UINT32_C(1) << 27) ? subnormal : normal;
    bits.u |= sign;
    return bits.f;
}

static void scale_min(int j, const uint8_t *q, uint8_t &sc, uint8_t &m) {
    if (j < 4) {
        sc = q[j] & 63;
        m = q[j + 4] & 63;
    } else {
        sc = (q[j + 4] & 15) | ((q[j - 4] >> 6) << 4);
        m = (q[j + 4] >> 4) | ((q[j] >> 6) << 4);
    }
}

static void output_half(const uint8_t *q, float *out, float d, float m,
                        bool upper) {
    for (size_t l = 0; l < 32;) {
        const size_t vl = __riscv_vsetvl_e8m1(32 - l);
        vuint8m1_t packed = __riscv_vle8_v_u8m1(q + l, vl);
        vuint8m1_t nibble = upper ? __riscv_vsrl_vx_u8m1(packed, 4, vl)
                                  : __riscv_vand_vx_u8m1(packed, 15, vl);
        vuint32m4_t widened = __riscv_vzext_vf4_u32m4(nibble, vl);
        vfloat32m4_t values = __riscv_vfcvt_f_xu_v_f32m4(widened, vl);
        values = __riscv_vfmul_vf_f32m4(values, d, vl);
        values = __riscv_vfsub_vf_f32m4(values, m, vl);
        __riscv_vse32_v_f32m4(out + l, values, vl);
        l += vl;
    }
}

extern "C" void dequantize_row_q4_K_rvv(const block_q4_K *GGML_RESTRICT x,
                                         float *GGML_RESTRICT y, int64_t k) {
    assert(k % QK_K == 0);
    const int nb = k / QK_K; // match the extracted implementation's int bound
    for (int i = 0; i < nb; ++i) {
        const float d = half_to_float(x[i].d);
        const float min = half_to_float(x[i].dmin);
        for (int j = 0; j < 4; ++j) {
            uint8_t sc, m;
            scale_min(2 * j, x[i].scales, sc, m);
            const float d1 = d * sc, m1 = min * m;
            scale_min(2 * j + 1, x[i].scales, sc, m);
            const float d2 = d * sc, m2 = min * m;
            output_half(x[i].qs + 32 * j, y + i * QK_K + j * 64, d1, m1, false);
            output_half(x[i].qs + 32 * j, y + i * QK_K + j * 64 + 32, d2, m2, true);
        }
    }
}
