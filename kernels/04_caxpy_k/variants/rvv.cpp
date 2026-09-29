#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

// The scalar kernel permits arbitrary increments, including repeated elements
// and overlapping x/y. Preserve loop-carried dependencies in those cases.
int caxpy_k_rvv(BLASLONG n, BLASLONG, BLASLONG, FLOAT ar, FLOAT ai,
                FLOAT *x, BLASLONG sx, FLOAT *y, BLASLONG sy,
                FLOAT *, BLASLONG) {
    if (n <= 0 || (ar == 0.0f && ai == 0.0f))
        return 0;
    const auto xfirst = reinterpret_cast<uintptr_t>(x + (sx < 0 ? 2 * (n - 1) * sx : 0));
    const auto xlast = reinterpret_cast<uintptr_t>(x + (sx > 0 ? 2 * (n - 1) * sx : 0) + 2);
    const auto yfirst = reinterpret_cast<uintptr_t>(y + (sy < 0 ? 2 * (n - 1) * sy : 0));
    const auto ylast = reinterpret_cast<uintptr_t>(y + (sy > 0 ? 2 * (n - 1) * sy : 0) + 2);
    if (sx == 0 || sy == 0 || (xfirst < ylast && yfirst < xlast)) {
        BLASLONG ix = 0, iy = 0;
        for (BLASLONG i = 0; i < n; ++i) {
            y[iy] += ar * x[ix] - ai * x[ix + 1];
            y[iy + 1] += ar * x[ix + 1] + ai * x[ix];
            ix += 2 * sx;
            iy += 2 * sy;
        }
        return 0;
    }
    BLASLONG i = 0;
    while (i < n) {
        const size_t vl = __riscv_vsetvl_e32m1(n - i);
        const auto xr = __riscv_vlse32_v_f32m1(x + 2 * i * sx, 8 * sx, vl);
        const auto xi = __riscv_vlse32_v_f32m1(x + 2 * i * sx + 1, 8 * sx, vl);
        const auto yr = __riscv_vlse32_v_f32m1(y + 2 * i * sy, 8 * sy, vl);
        const auto yi = __riscv_vlse32_v_f32m1(y + 2 * i * sy + 1, 8 * sy, vl);
        const auto real = __riscv_vfadd_vv_f32m1(
            yr, __riscv_vfmsub_vf_f32m1(
                xr, ar, __riscv_vfmul_vf_f32m1(xi, ai, vl), vl), vl);
        const auto imag = __riscv_vfadd_vv_f32m1(
            yi, __riscv_vfmadd_vf_f32m1(
                xi, ar, __riscv_vfmul_vf_f32m1(xr, ai, vl), vl), vl);
        __riscv_vsse32_v_f32m1(y + 2 * i * sy, 8 * sy, real, vl);
        __riscv_vsse32_v_f32m1(y + 2 * i * sy + 1, 8 * sy, imag, vl);
        i += vl;
    }
    return 0;
}
