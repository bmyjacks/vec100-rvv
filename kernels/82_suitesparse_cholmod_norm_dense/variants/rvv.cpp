#include "kernel.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <riscv_vector.h>

namespace suitesparse {

double cholmod_norm_dense_rvv(Int nrow, Int ncol, Int d, const double *Xx) {
    double xnorm = 0;
    alignas(8) double magnitudes[64];
    for (Int j = 0; j < ncol; ++j) {
        double s = 0;
        for (Int i = 0; i < nrow;) {
            size_t vl = __riscv_vsetvl_e64m1(
                static_cast<size_t>(std::min<Int>(nrow - i, 64)));
            vfloat64m1_t values = __riscv_vle64_v_f64m1(Xx + j * d + i, vl);
            // Sign XOR with itself clears the sign bit, including on NaNs.
            vfloat64m1_t abs_values =
                __riscv_vfsgnjx_vv_f64m1(values, values, vl);
            __riscv_vse64_v_f64m1(magnitudes, abs_values, vl);
            // Do not use a vector reduction: each FP add must occur in i order.
            for (size_t k = 0; k < vl; ++k) {
                s += magnitudes[k];
            }
            i += static_cast<Int>(vl);
        }
        if ((std::isnan(s) || s > xnorm) && !std::isnan(xnorm)) {
            xnorm = s;
        }
    }
    return xnorm;
}

} // namespace suitesparse
