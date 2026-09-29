#include "kernel.h"
#include <riscv_vector.h>

extern "C" void skl_gemm_f32rcprc_f32rcprc_f32rcprc_ref_rvv(
    size_t m0, size_t n0, size_t k0, size_t m1, size_t n1, size_t k1,
    float alpha, const float *a_pack, size_t rsa0, size_t csa0, size_t rsa1,
    size_t csa1, const float *b_pack, size_t rsb0, size_t csb0, size_t rsb1,
    size_t csb1, float beta, float *c_pack, size_t rsc0, size_t csc0,
    size_t rsc1, size_t csc1) {
    for (size_t ii1 = 0; ii1 < m1; ++ii1)
        for (size_t jj1 = 0; jj1 < n1; ++jj1)
            for (size_t ii0 = 0; ii0 < m0; ++ii0)
                for (size_t jj0 = 0; jj0 < n0;) {
                    size_t vl = __riscv_vsetvl_e32m1(n0 - jj0);
                    vfloat32m1_t acc = __riscv_vfmv_v_f_f32m1(0.0f, vl);
                    for (size_t kk1 = 0; kk1 < k1; ++kk1) {
                        const float *a = a_pack + ii1 * rsa1 + kk1 * csa1 + ii0 * rsa0;
                        const float *b = b_pack + kk1 * rsb1 + jj1 * csb1 + jj0 * csb0;
                        for (size_t kk0 = 0; kk0 < k0; ++kk0) {
                            vfloat32m1_t bv = __riscv_vlse32_v_f32m1(b + kk0 * rsb0, csb0 * sizeof(float), vl);
                            acc = __riscv_vfmacc_vf_f32m1(acc, a[kk0 * csa0], bv, vl);
                        }
                    }
                    float *c = c_pack + ii1 * rsc1 + jj1 * csc1 + ii0 * rsc0 + jj0 * csc0;
                    vfloat32m1_t cv = __riscv_vlse32_v_f32m1(c, csc0 * sizeof(float), vl);
                    acc = __riscv_vfmul_vf_f32m1(acc, alpha, vl);
                    cv = __riscv_vfmacc_vf_f32m1(acc, beta, cv, vl);
                    __riscv_vsse32_v_f32m1(c, csc0 * sizeof(float), cv, vl);
                    jj0 += vl;
                }
}
