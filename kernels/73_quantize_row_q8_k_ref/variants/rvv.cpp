#include "kernel.h"

#include <riscv_vector.h>

// Scalar reduction retains the first signed maximum on equal magnitudes.
// On a zero block the reference leaves bsums untouched.
void quantize_row_q8_K_rvv(const float *GGML_RESTRICT x,
                           block_q8_K *GGML_RESTRICT y, int64_t k) {
    assert(k % QK_K == 0);
    const int64_t nb = k / QK_K;
    for (int64_t i = 0; i < nb; ++i) {
        float max = 0;
        float amax = 0;
        for (int j = 0; j < QK_K; ++j) {
            float ax = fabsf(x[j]);
            if (ax > amax) {
                amax = ax;
                max = x[j];
            }
        }
        if (!amax) {
            y[i].d = 0;
            memset(y[i].qs, 0, QK_K);
            x += QK_K;
            continue;
        }
        const float iscale = -127.f / max;
        int32_t rounded[QK_K];
        for (size_t j = 0; j < QK_K;) {
            const size_t vl = __riscv_vsetvl_e32m1(QK_K - j);
            vfloat32m1_t vf = __riscv_vle32_v_f32m1(x + j, vl);
            vf = __riscv_vfmul_vf_f32m1(vf, iscale, vl);
            // nearest_int uses the float addition itself (including the current
            // FP rounding mode), then extracts the mantissa bits.
            vf = __riscv_vfadd_vf_f32m1(vf, 12582912.f, vl);
            vint32m1_t vi = __riscv_vreinterpret_v_f32m1_i32m1(vf);
            vi = __riscv_vand_vx_i32m1(vi, 0x007fffff, vl);
            vi = __riscv_vsub_vx_i32m1(vi, 0x00400000, vl);
            vi = __riscv_vmin_vx_i32m1(vi, 127, vl);
            __riscv_vse32_v_i32m1(rounded + j, vi, vl);
            j += vl;
        }
        for (int j = 0; j < QK_K; ++j)
            y[i].qs[j] = rounded[j];
        for (int j = 0; j < QK_K / 16; ++j) {
            int sum = 0;
            for (int ii = 0; ii < 16; ++ii)
                sum += y[i].qs[j * 16 + ii];
            y[i].bsums[j] = sum;
        }
        y[i].d = 1 / iscale;
        x += QK_K;
    }
}
