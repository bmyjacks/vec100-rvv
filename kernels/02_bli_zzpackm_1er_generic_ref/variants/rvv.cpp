#include "kernel.h"

#include <cstddef>
#include <cstdint>
#include <riscv_vector.h>

static void zero_edges(double *p, dim_t used, dim_t total,
                       dim_t n, dim_t n_max, inc_t ldp) {
    for (dim_t col = 0; col < 2 * n; ++col)
        for (dim_t row = used; row < total; ++row)
            p[col * ldp + row] = 0.0;
    for (dim_t col = 2 * n; col < 2 * n_max; ++col)
        for (dim_t row = 0; row < total; ++row)
            p[col * ldp + row] = 0.0;
}

static bool disjoint(const void *a, size_t a_bytes, const void *b, size_t b_bytes) {
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return !a_bytes || !b_bytes || x + a_bytes <= y || y + b_bytes <= x;
}

void bli_zzpackm_1er_generic_ref_rvv(
    conj_t conja, pack_t schema, dim_t cdim, dim_t cdim_max, dim_t cdim_bcast,
    dim_t n, dim_t n_max, const void *kappa, const void *a, inc_t inca,
    inc_t lda, void *p, inc_t ldp, const void *params, const cntx_t *cntx) {
    (void)params;
    (void)cntx;
    const bool one_e = (schema & BLIS_PACK_FORMAT_BITS) == BLIS_BITVAL_1E;
    const bool conj = conja == BLIS_CONJUGATE;
    const auto *src = static_cast<const double *>(a);
    auto *dst = static_cast<double *>(p);
    const auto *scale = static_cast<const double *>(kappa);
    const double kr = scale[0], ki = scale[1];
    const dim_t used = (one_e ? 2 : 1) * cdim * cdim_bcast;
    const dim_t total = (one_e ? 2 : 1) * cdim_max * cdim_bcast;

    // The original macro has ordered stores and no restrict guarantee. The
    // fast path requires distinct source and output spans; all other layouts
    // execute the same per-element write order as the scalar reference.
    const bool fast = cdim > 0 && n > 0 && cdim_bcast == 1 && inca == 1 &&
                      lda >= cdim && ldp >= total &&
                      disjoint(src, size_t(2 * ((n - 1) * lda + cdim)) * sizeof(double),
                               dst, size_t((2 * n_max - 1) * ldp + total) * sizeof(double));

    for (dim_t k = 0; k < n; ++k) {
        if (fast) {
            dim_t mn = 0;
            while (mn < cdim) {
                const size_t vl = __riscv_vsetvl_e64m1(cdim - mn);
                const double *row = src + 2 * (k * lda + mn);
                const auto ar = __riscv_vlse64_v_f64m1(row, 2 * sizeof(double), vl);
                auto ai = __riscv_vlse64_v_f64m1(row + 1, 2 * sizeof(double), vl);
                if (conj) ai = __riscv_vfneg_v_f64m1(ai, vl);
                const auto arkr = __riscv_vfmul_vf_f64m1(ar, kr, vl);
                const auto aiki = __riscv_vfmul_vf_f64m1(ai, ki, vl);
                const auto aiki2 = __riscv_vfmul_vf_f64m1(ar, ki, vl);
                const auto aikr = __riscv_vfmul_vf_f64m1(ai, kr, vl);
                const auto real = __riscv_vfsub_vv_f64m1(arkr, aiki, vl);
                const auto imag = __riscv_vfadd_vv_f64m1(aiki2, aikr, vl);
                if (one_e) {
                    double *ri = dst + 2 * k * ldp + 2 * mn;
                    double *ir = ri + ldp;
                    __riscv_vsse64_v_f64m1(ri, 2 * sizeof(double), real, vl);
                    __riscv_vsse64_v_f64m1(ri + 1, 2 * sizeof(double), imag, vl);
                    const auto negative_imag = __riscv_vfneg_v_f64m1(imag, vl);
                    __riscv_vsse64_v_f64m1(ir, 2 * sizeof(double), negative_imag, vl);
                    __riscv_vsse64_v_f64m1(ir + 1, 2 * sizeof(double), real, vl);
                } else {
                    double *real_out = dst + 2 * k * ldp + mn;
                    __riscv_vse64_v_f64m1(real_out, real, vl);
                    __riscv_vse64_v_f64m1(real_out + ldp, imag, vl);
                }
                mn += static_cast<dim_t>(vl);
            }
        } else {
            for (dim_t mn = 0; mn < cdim; ++mn) {
                const double ar = src[2 * (k * lda + mn * inca)];
                const double raw_ai = src[2 * (k * lda + mn * inca) + 1];
                const double ai = conj ? -raw_ai : raw_ai;
                const double real = (kr * ar) - (ki * ai);
                const double imag = (ki * ar) + (kr * ai);
                if (one_e) {
                    for (dim_t d = 0; d < cdim_bcast; ++d) {
                        double *ri = dst + 2 * k * ldp + 2 * mn * cdim_bcast + d;
                        double *ir = ri + ldp;
                        ri[0] = real;
                        ri[cdim_bcast] = imag;
                        ir[0] = -imag;
                        ir[cdim_bcast] = real;
                    }
                } else {
                    for (dim_t d = 0; d < cdim_bcast; ++d) {
                        double *ri = dst + 2 * k * ldp + mn * cdim_bcast + d;
                        ri[0] = real;
                        ri[ldp] = imag;
                    }
                }
            }
        }
    }
    zero_edges(dst, used, total, n, n_max, ldp);
}
