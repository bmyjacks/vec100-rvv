#include "kernel.h"

#include <riscv_vector.h>

// Same block layout and first-maximum tie rule as the scalar reference.
void quantize_row_q4_0_rvv(const float *GGML_RESTRICT x,
                           block_q4_0 *GGML_RESTRICT y, int64_t k) {
    assert(k % QK4_0 == 0);
    const int nb = k / QK4_0;
    for (int i = 0; i < nb; ++i) {
        const float *xb = x + i * QK4_0;
        float amax = 0.0f;
        float max = 0.0f;
        for (int j = 0; j < QK4_0; ++j) {
            const float v = xb[j];
            if (amax < fabsf(v)) {
                amax = fabsf(v);
                max = v;
            }
        }
        const float d = max / -8;
        const float id = d ? 1.0f / d : 0.0f;
        y[i].d = GGML_FP32_TO_FP16(d);

        int32_t lo[QK4_0 / 2], hi[QK4_0 / 2];
        for (size_t j = 0; j < QK4_0 / 2;) {
            const size_t vl = __riscv_vsetvl_e32m1(QK4_0 / 2 - j);
            vfloat32m1_t x0 = __riscv_vle32_v_f32m1(xb + j, vl);
            vfloat32m1_t x1 = __riscv_vle32_v_f32m1(xb + QK4_0 / 2 + j, vl);
            x0 = __riscv_vfadd_vf_f32m1(__riscv_vfmul_vf_f32m1(x0, id, vl),
                                         8.5f, vl);
            x1 = __riscv_vfadd_vf_f32m1(__riscv_vfmul_vf_f32m1(x1, id, vl),
                                         8.5f, vl);
            __riscv_vse32_v_i32m1(lo + j,
                __riscv_vfcvt_rtz_x_f_v_i32m1(x0, vl), vl);
            __riscv_vse32_v_i32m1(hi + j,
                __riscv_vfcvt_rtz_x_f_v_i32m1(x1, vl), vl);
            j += vl;
        }
        for (int j = 0; j < QK4_0 / 2; ++j) {
            const uint8_t xi0 = MIN(15, static_cast<int8_t>(lo[j]));
            const uint8_t xi1 = MIN(15, static_cast<int8_t>(hi[j]));
            y[i].qs[j] = xi0;
            y[i].qs[j] |= xi1 << 4;
        }
    }
}
