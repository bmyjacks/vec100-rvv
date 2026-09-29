#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

// The upstream 64-bit contract has 0 <= Sl <= 63 and 0 <= Al <= 15.
// Preserve scalar traversal for overlapping arrays, including bits/values.
static bool overlap(const void *a, size_t an, const void *b, size_t bn) {
    uintptr_t x = reinterpret_cast<uintptr_t>(a);
    uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x < y + bn && y < x + an;
}

void encode_mcu_AC_first_prepare_rvv(const JCOEF *block, const int *order, int Sl,
                                 int Al, UJCOEF *values, size_t *bits) {
    if (Sl < 0 || Sl > 63 || Al < 0 || Al > 15 ||
        overlap(values, 128 * sizeof(UJCOEF), block, 64 * sizeof(JCOEF)) ||
        overlap(values, 128 * sizeof(UJCOEF), order, size_t(Sl) * sizeof(int)) ||
        overlap(bits, sizeof(size_t), values, 128 * sizeof(UJCOEF)) ||
        overlap(bits, sizeof(size_t), block, 64 * sizeof(JCOEF)) ||
        overlap(bits, sizeof(size_t), order, size_t(Sl) * sizeof(int))) {
        encode_mcu_AC_first_prepare_isolated(block, order, Sl, Al, values, bits);
        return;
    }
    size_t mask = 0;
    for (int k = 0; k < Sl;) {
        size_t vl = __riscv_vsetvl_e16m1(Sl - k);
        vint32m2_t indices = __riscv_vle32_v_i32m2(order + k, vl);
        vuint32m2_t offsets = __riscv_vsll_vx_u32m2(
            __riscv_vreinterpret_v_i32m2_u32m2(indices), 1, vl);
        vint16m1_t coef = __riscv_vluxei32_v_i16m1(block, offsets, vl);
        vint32m2_t wide = __riscv_vwadd_vx_i32m2(coef, 0, vl);
        vint32m2_t sign = __riscv_vsra_vx_i32m2(wide, 31, vl);
        vint32m2_t magnitude = __riscv_vsub_vv_i32m2(
            __riscv_vxor_vv_i32m2(wide, sign, vl), sign, vl);
        magnitude = __riscv_vsra_vx_i32m2(magnitude, Al, vl);
        vint32m2_t encoded = __riscv_vxor_vv_i32m2(sign, magnitude, vl);
        uint32_t mags[64], codes[64];
        __riscv_vse32_v_u32m2(mags,
            __riscv_vreinterpret_v_i32m2_u32m2(magnitude), vl);
        __riscv_vse32_v_u32m2(codes,
            __riscv_vreinterpret_v_i32m2_u32m2(encoded), vl);
        for (size_t j = 0; j < vl; ++j) {
            if (mags[j]) {
                values[k + j] = static_cast<UJCOEF>(mags[j]);
                values[k + j + DCTSIZE2] = static_cast<UJCOEF>(codes[j]);
                mask |= size_t(1) << (k + j);
            }
        }
        k += vl;
    }
    bits[0] = mask;
}
