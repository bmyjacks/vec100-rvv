#include "kernel.h"

#include <cstddef>
#include <riscv_vector.h>

int sscal_k_rvv(BLASLONG n, BLASLONG, BLASLONG, FLOAT da, FLOAT *x, BLASLONG inc_x,
                FLOAT *, BLASLONG, FLOAT *, BLASLONG dummy2) {
    if (n <= 0 || inc_x <= 0)
        return 0;

    const ptrdiff_t byte_stride = inc_x * static_cast<BLASLONG>(sizeof(FLOAT));
    BLASLONG i = 0;
    while (i < n) {
        const size_t vl = __riscv_vsetvl_e32m1(static_cast<size_t>(n - i));
        FLOAT *const at = x + i * inc_x;

        if (da == 0.0f && dummy2 != 1) {
            __riscv_vsse32_v_f32m1(at, byte_stride,
                                   __riscv_vfmv_v_f_f32m1(0.0f, vl), vl);
        } else {
            const vfloat32m1_t input =
                __riscv_vlse32_v_f32m1(at, byte_stride, vl);
            vfloat32m1_t output;
            if (da == 0.0f) {
                // isfinite tests the exponent bits; both infinities and NaNs
                // must become the source's NAN constant, including -infinity.
                const vuint32m1_t bits =
                    __riscv_vreinterpret_v_f32m1_u32m1(input);
                const vuint32m1_t exponent =
                    __riscv_vand_vx_u32m1(bits, 0x7f800000u, vl);
                const vbool32_t nonfinite =
                    __riscv_vmseq_vx_u32m1_b32(exponent, 0x7f800000u, vl);
                const vuint32m1_t result = __riscv_vmerge_vvm_u32m1(
                    __riscv_vmv_v_x_u32m1(0, vl),
                    __riscv_vmv_v_x_u32m1(0x7fc00000u, vl), nonfinite, vl);
                output = __riscv_vreinterpret_v_u32m1_f32m1(result);
            } else {
                output = __riscv_vfmul_vf_f32m1(input, da, vl);
            }
            __riscv_vsse32_v_f32m1(at, byte_stride, output, vl);
        }
        i += static_cast<BLASLONG>(vl);
    }
    return 0;
}
