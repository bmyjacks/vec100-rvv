#include "kernel.h"
#include <riscv_vector.h>
#include <vector>

// The reference defines E4M3 subnormals and the unsigned canonical NaN in
// software. Reuse that conversion instead of requiring the Xsfmm32a8f ISA.
extern "C" void skl_gemm_f8e4m3rc_f8e4m3rc_f32rc_ref_rvv(
    size_t m, size_t n, size_t k, float alpha, const uint8_t *a,
    size_t rsa, size_t csa, const uint8_t *b, size_t rsb, size_t csb,
    float beta, float *c, size_t rsc, size_t csc) {
    std::vector<float> converted(__riscv_vsetvlmax_e32m1());
    for (size_t ii = 0; ii < m; ++ii)
        for (size_t jj = 0; jj < n;) {
            size_t vl = __riscv_vsetvl_e32m1(n - jj);
            auto acc = __riscv_vfmv_v_f_f32m1(0.0f, vl);
            for (size_t kk = 0; kk < k; ++kk) {
                float av = skl_cvt_f8e4m3_f32(a[ii * rsa + kk * csa]);
                for (size_t lane = 0; lane < vl; ++lane)
                    converted[lane] = skl_cvt_f8e4m3_f32(b[kk * rsb + (jj + lane) * csb]);
                auto bv = __riscv_vle32_v_f32m1(converted.data(), vl);
                auto product = __riscv_vfmul_vf_f32m1(bv, av, vl);
                acc = __riscv_vfadd_vv_f32m1(acc, product, vl);
            }
            float *dst = c + ii * rsc + jj * csc;
            auto old = __riscv_vlse32_v_f32m1(dst, csc * sizeof(float), vl);
            old = __riscv_vfmul_vf_f32m1(old, beta, vl);
            acc = __riscv_vfmul_vf_f32m1(acc, alpha, vl);
            __riscv_vsse32_v_f32m1(dst, csc * sizeof(float),
                                   __riscv_vfadd_vv_f32m1(old, acc, vl), vl);
            jj += vl;
        }
}
