#include "kernel.h"

#include <riscv_vector.h>

uint32_t quant_c_rvv(const int16_t *coef, const int32_t *quantCoeff,
                       int32_t *deltaU, int16_t *qCoef, int qBits, int add,
                       int numCoeff) {
    // The upstream preconditions require qBits >= 8 and a multiple-of-16
    // count; signed intermediates must remain in int32_t range.
    uint32_t numSig = 0;
    for (int i = 0; i < numCoeff;) {
        size_t vl = __riscv_vsetvl_e32m2(static_cast<size_t>(numCoeff - i));
        vint16m1_t input = __riscv_vle16_v_i16m1(coef + i, vl);
        vint32m2_t value = __riscv_vwcvt_x_x_v_i32m2(input, vl);
        vbool16_t negative = __riscv_vmslt_vx_i32m2_b16(value, 0, vl);
        vint32m2_t magnitude = __riscv_vmerge_vvm_i32m2(
            value, __riscv_vneg_v_i32m2(value, vl), negative, vl);
        vint32m2_t coeff = __riscv_vle32_v_i32m2(quantCoeff + i, vl);
        vint32m2_t product = __riscv_vmul_vv_i32m2(magnitude, coeff, vl);
        vint32m2_t rounded = __riscv_vadd_vx_i32m2(product, add, vl);
        vint32m2_t level = __riscv_vsra_vx_i32m2(rounded, qBits, vl);
        vbool16_t significant = __riscv_vmsne_vx_i32m2_b16(level, 0, vl);
        numSig += __riscv_vcpop_m_b16(significant, vl);
        vint32m2_t scaled = __riscv_vsll_vx_i32m2(level, qBits, vl);
        vint32m2_t error = __riscv_vsub_vv_i32m2(product, scaled, vl);
        error = __riscv_vsra_vx_i32m2(error, qBits - 8, vl);
        __riscv_vse32_v_i32m2(deltaU + i, error, vl);
        vint32m2_t negated = __riscv_vneg_v_i32m2(level, vl);
        level = __riscv_vmerge_vvm_i32m2(level, negated, negative, vl);
        level = __riscv_vmax_vx_i32m2(level, -32768, vl);
        level = __riscv_vmin_vx_i32m2(level, 32767, vl);
        vint16m1_t result = __riscv_vncvt_x_x_w_i16m1(level, vl);
        __riscv_vse16_v_i16m1(qCoef + i, result, vl);
        i += static_cast<int>(vl);
    }
    return numSig;
}
