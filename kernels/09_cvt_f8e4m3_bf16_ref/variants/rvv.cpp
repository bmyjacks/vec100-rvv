#include "kernel.h"

#include <riscv_vector.h>

void skl_cvt_f8e4m3_bf16_rvv(__bf16 *pDst, const uint8_t *pSrc, size_t n) {
    if (n == 0)
        return;

    // A scalar iteration reads one byte before writing two. Keep that exact
    // order when a store can change any source byte in this call.
    const uintptr_t dst = reinterpret_cast<uintptr_t>(pDst);
    const uintptr_t src = reinterpret_cast<uintptr_t>(pSrc);
    if ((src <= dst && dst - src < n) ||
        (dst < src && src - dst < 2 * n)) {
        skl_cvt_f8e4m3_bf16_ref(pDst, pSrc, n);
        return;
    }

    // Exponent-zero encodings are exact BF16 values too; the table is the
    // magnitude only. Both FP8 NaN encodings become positive canonical NaN.
    alignas(16) static const uint16_t subnormal[8] = {
        0x0000, 0x3b00, 0x3b80, 0x3bc0,
        0x3c00, 0x3c20, 0x3c40, 0x3c60};
    const vuint16m2_t table = __riscv_vle16_v_u16m2(subnormal, 8);

    while (n != 0) {
        const size_t vl = __riscv_vsetvl_e8m1(n);
        const vuint8m1_t bytes = __riscv_vle8_v_u8m1(pSrc, vl);
        const vuint16m2_t bits = __riscv_vzext_vf2_u16m2(bytes, vl);
        const vuint16m2_t exp = __riscv_vand_vx_u16m2(
            __riscv_vsrl_vx_u16m2(bits, 3, vl), 15, vl);
        const vuint16m2_t mant = __riscv_vand_vx_u16m2(bits, 7, vl);
        const vuint16m2_t sign = __riscv_vsll_vx_u16m2(
            __riscv_vand_vx_u16m2(bits, 0x80, vl), 8, vl);
        const vuint16m2_t exponent = __riscv_vsll_vx_u16m2(
            __riscv_vadd_vx_u16m2(exp, 120, vl), 7, vl);
        const vuint16m2_t fraction = __riscv_vsll_vx_u16m2(mant, 4, vl);
        vuint16m2_t result = __riscv_vor_vv_u16m2(
            __riscv_vor_vv_u16m2(exponent, fraction, vl), sign, vl);
        const vuint16m2_t sub = __riscv_vor_vv_u16m2(
            __riscv_vrgather_vv_u16m2(table, mant, vl), sign, vl);
        result = __riscv_vmerge_vvm_u16m2(
            result, sub, __riscv_vmseq_vx_u16m2_b8(exp, 0, vl), vl);
        const vbool8_t nan = __riscv_vmand_mm_b8(
            __riscv_vmseq_vx_u16m2_b8(exp, 15, vl),
            __riscv_vmseq_vx_u16m2_b8(mant, 7, vl), vl);
        result = __riscv_vmerge_vxm_u16m2(result, 0x7fc0, nan, vl);
        __riscv_vse16_v_u16m2(reinterpret_cast<uint16_t *>(pDst), result, vl);
        pSrc += vl;
        pDst += vl;
        n -= vl;
    }
}
