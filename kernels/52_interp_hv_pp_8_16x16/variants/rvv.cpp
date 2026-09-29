#include "kernel.h"

#include <riscv_vector.h>

void interp_hv_pp_8_16x16_rvv(const pixel *src, std::intptr_t srcStride, pixel *dst,
                          std::intptr_t dstStride, int idxX, int idxY) {
    constexpr int width = 16, height = 16, taps = 8;
    alignas(32) int16_t intermediate[width * (height + taps - 1)];
    const int16_t *cx = X265_NS::g_lumaFilter[idxX];
    const int16_t *cy = X265_NS::g_lumaFilter[idxY];
    // Same two-pass order as upstream: all horizontal reads precede any
    // output writes, even when source and destination storage overlap.
    const pixel *first = src - 3 * srcStride - 3;
    for (int row = 0; row < height + taps - 1; ++row) {
        const pixel *s = first + row * srcStride;
        for (int col = 0; col < width;) {
            const size_t vl = __riscv_vsetvl_e8m1(width - col);
            vint32m4_t sum = __riscv_vmv_v_x_i32m4(0, vl);
            for (int tap = 0; tap < taps; ++tap) {
                vuint8m1_t bytes = __riscv_vle8_v_u8m1(s + col + tap, vl);
                vint16m2_t samples = __riscv_vreinterpret_v_u16m2_i16m2(
                    __riscv_vzext_vf2_u16m2(bytes, vl));
                sum = __riscv_vwmacc_vx_i32m4(sum, cx[tap], samples, vl);
            }
            sum = __riscv_vsub_vx_i32m4(sum, IF_INTERNAL_OFFS, vl);
            // At depth 8 the horizontal shift is zero; upstream converts
            // the resulting value to int16_t before the vertical pass.
            vint16m2_t val = __riscv_vnsra_wx_i16m2(sum, 0, vl);
            __riscv_vse16_v_i16m2(intermediate + row * width + col, val, vl);
            col += static_cast<int>(vl);
        }
    }

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width;) {
            const size_t vl = __riscv_vsetvl_e16m2(width - col);
            vint32m4_t sum = __riscv_vmv_v_x_i32m4(0, vl);
            for (int tap = 0; tap < taps; ++tap) {
                vint16m2_t samples = __riscv_vle16_v_i16m2(
                    intermediate + (row + tap) * width + col, vl);
                sum = __riscv_vwmacc_vx_i32m4(sum, cy[tap], samples, vl);
            }
            constexpr int shift = IF_FILTER_PREC + IF_INTERNAL_PREC - 8;
            constexpr int offset = (1 << (shift - 1)) +
                                   (IF_INTERNAL_OFFS << IF_FILTER_PREC);
            sum = __riscv_vadd_vx_i32m4(sum, offset, vl);
            sum = __riscv_vsra_vx_i32m4(sum, shift, vl);
            vint16m2_t val = __riscv_vnsra_wx_i16m2(sum, 0, vl);
            val = __riscv_vmax_vx_i16m2(val, 0, vl);
            val = __riscv_vmin_vx_i16m2(val, 255, vl);
            vuint8m1_t out = __riscv_vnclipu_wx_u8m1(
                __riscv_vreinterpret_v_i16m2_u16m2(val), 0, __RISCV_VXRM_RDN, vl);
            __riscv_vse8_v_u8m1(dst + row * dstStride + col, out, vl);
            col += static_cast<int>(vl);
        }
    }
}
