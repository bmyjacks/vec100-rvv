#include "kernel.h"
#include <riscv_vector.h>

void ssim_distortion_dc_rvv(const pixel *fenc, uint32_t fStride,
                            const pixel *recon, std::intptr_t rstride, int trSize,
                            uint64_t *ssDcOut, uint64_t *dcOut) {
    uint64_t ssDc = 0, dc_k = 0;
    for (int y = 0; y < trSize; y += 4) {
        for (int x = 0; x < trSize; ) {
            const size_t vl = __riscv_vsetvl_e8m1((trSize - x + 3) / 4);
            const vuint8m1_t a = __riscv_vlse8_v_u8m1(fenc + y * fStride + x, 4, vl);
            const vuint8m1_t b = __riscv_vlse8_v_u8m1(recon + y * rstride + x, 4, vl);
            const vuint16m2_t aw = __riscv_vzext_vf2_u16m2(a, vl);
            const vuint16m2_t bw = __riscv_vzext_vf2_u16m2(b, vl);
            const vint16m2_t delta = __riscv_vreinterpret_v_u16m2_i16m2(
                __riscv_vsub_vv_u16m2(aw, bw, vl));
            const vint32m4_t square = __riscv_vwmul_vv_i32m4(delta, delta, vl);
            const vint32m1_t zero = __riscv_vmv_v_x_i32m1(0, vl);
            const vint32m1_t sum = __riscv_vredsum_vs_i32m4_i32m1(square, zero, vl);
            ssDc += static_cast<uint32_t>(__riscv_vmv_x_s_i32m1_i32(sum));
            x += static_cast<int>(4 * vl);
        }
    }
    for (int y = 0; y < trSize; y += 4) {
        for (int x = 0; x < trSize; ) {
            const size_t vl = __riscv_vsetvl_e8m1((trSize - x + 3) / 4);
            const vuint8m1_t a = __riscv_vlse8_v_u8m1(fenc + y * fStride + x, 4, vl);
            const vuint16m2_t aw = __riscv_vzext_vf2_u16m2(a, vl);
            const vuint32m4_t square = __riscv_vwmulu_vv_u32m4(aw, aw, vl);
            const vuint32m1_t zero = __riscv_vmv_v_x_u32m1(0, vl);
            const vuint32m1_t sum = __riscv_vredsum_vs_u32m4_u32m1(square, zero, vl);
            dc_k += __riscv_vmv_x_s_u32m1_u32(sum);
            x += static_cast<int>(4 * vl);
        }
    }
    *ssDcOut = ssDc;
    *dcOut = dc_k;
}
