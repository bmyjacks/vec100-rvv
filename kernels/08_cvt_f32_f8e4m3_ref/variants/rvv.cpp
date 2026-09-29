#include "kernel.h"

#include <riscv_vector.h>

void skl_cvt_f32_f8e4m3_rvv(uint8_t *pDst, const float *pSrc,
                            float scaling_factor, size_t n) {
    if (n == 0)
        return;

    // The reference reads a float and writes a byte on every iteration.
    // Vector stores would change future reads in either overlap direction.
    const uintptr_t dst = reinterpret_cast<uintptr_t>(pDst);
    const uintptr_t src = reinterpret_cast<uintptr_t>(pSrc);
    if ((src <= dst && dst - src < 4 * n) ||
        (dst < src && src - dst < n)) {
        skl_cvt_f32_f8e4m3_ref(pDst, pSrc, scaling_factor, n);
        return;
    }

    while (n != 0) {
        // Fixed-size scratch bounds the scalar repair work independently of
        // VLEN. A vector multiplication reproduces pSrc[i] * scaling_factor.
        const size_t vl = __riscv_vsetvl_e32m1(n < 8 ? n : 8);
        const vfloat32m1_t values = __riscv_vle32_v_f32m1(pSrc, vl);
        const vfloat32m1_t scaled =
            __riscv_vfmul_vf_f32m1(values, scaling_factor, vl);
        const vuint32m1_t bits = __riscv_vreinterpret_v_f32m1_u32m1(scaled);
        const vuint32m1_t mant = __riscv_vand_vx_u32m1(bits, 0x7fffff, vl);
        const vuint32m1_t exp = __riscv_vand_vx_u32m1(
            __riscv_vsrl_vx_u32m1(bits, 23, vl), 255, vl);

        // Far from binade boundaries, floor(log2f(abs(x))) is the binary
        // exponent, powf(2,-exp) is exact, and the reference rounds mant/2^20
        // with nearbyintf. vfcvt uses the same active FP rounding mode.
        const vfloat32m1_t fraction = __riscv_vfmul_vf_f32m1(
            __riscv_vfcvt_f_xu_v_f32m1(mant, vl), 0x1p-20f, vl);
        const vint32m1_t rounded = __riscv_vfcvt_x_f_v_i32m1(fraction, vl);
        const vuint32m1_t base = __riscv_vsll_vx_u32m1(
            __riscv_vsub_vx_u32m1(exp, 120, vl), 3, vl);
        const vuint32m1_t magnitude = __riscv_vadd_vv_u32m1(
            base, __riscv_vreinterpret_v_i32m1_u32m1(rounded), vl);
        const vuint32m1_t signed_value = __riscv_vor_vv_u32m1(
            magnitude, __riscv_vsrl_vx_u32m1(
                           __riscv_vand_vx_u32m1(bits, 0x80000000u, vl),
                           24, vl), vl);
        // The reference's nonsaturating overflow is *positive* 0x7f,
        // even for a negative operand.
        const vuint32m1_t output = __riscv_vmerge_vxm_u32m1(
            signed_value, 0x7f,
            __riscv_vmsgtu_vx_u32m1_b32(magnitude, 0x7e, vl), vl);
        const vuint16mf2_t half = __riscv_vncvt_x_x_w_u16mf2(output, vl);
        const vuint8mf4_t bytes = __riscv_vncvt_x_x_w_u8mf4(half, vl);
        __riscv_vse8_v_u8mf4(pDst, bytes, vl);

        uint32_t scaled_bits[8];
        __riscv_vse32_v_u32m1(scaled_bits, bits, vl);
        for (size_t i = 0; i < vl; ++i) {
            const uint32_t e = (scaled_bits[i] >> 23) & 255;
            const uint32_t m = scaled_bits[i] & 0x7fffff;
            if (e < 121 || e > 135 || m < 0x100000 || m > 0x700000) {
                float x;
                __builtin_memcpy(&x, &scaled_bits[i], sizeof(x));
                pDst[i] = skl_cvt_f32_f8e4m3(x, false);
            }
        }
        pDst += vl;
        pSrc += vl;
        n -= vl;
    }
}
