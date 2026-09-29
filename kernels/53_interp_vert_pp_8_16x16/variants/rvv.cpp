#include "kernel.h"

#include <riscv_vector.h>

void interp_vert_pp_8_16x16_rvv(const pixel *src, std::intptr_t srcStride,
                            pixel *dst, std::intptr_t dstStride, int coeffIdx) {
    const int16_t *c = X265_NS::g_lumaFilter[coeffIdx];
    const pixel *const first = src - 3 * srcStride;

    // The source reads span rows -3..19 and writes span rows 0..15.
    // On overlap the original reads each output's taps before that write;
    // retain precisely this order, including zero/negative strides.
    const intptr_t src_end = 22 * srcStride;
    const intptr_t dst_end = 15 * dstStride;
    const uintptr_t src_lo = reinterpret_cast<uintptr_t>(first + (src_end < 0 ? src_end : 0));
    const uintptr_t src_hi = reinterpret_cast<uintptr_t>(first + (src_end > 0 ? src_end : 0) + 16);
    const uintptr_t dst_lo = reinterpret_cast<uintptr_t>(dst + (dst_end < 0 ? dst_end : 0));
    const uintptr_t dst_hi = reinterpret_cast<uintptr_t>(dst + (dst_end > 0 ? dst_end : 0) + 16);
    if (src_lo < dst_hi && dst_lo < src_hi) {
        for (int row = 0; row < 16; ++row) {
            const pixel *s = first + row * srcStride;
            pixel *d = dst + row * dstStride;
            for (int col = 0; col < 16; ++col) {
                int sum = s[col] * c[0];
                for (int tap = 1; tap < 8; ++tap)
                    sum += s[col + tap * srcStride] * c[tap];
                int16_t val = static_cast<int16_t>((sum + 32) >> 6);
                val = val < 0 ? 0 : val;
                val = val > 255 ? 255 : val;
                d[col] = static_cast<pixel>(val);
            }
        }
        return;
    }

    for (int row = 0; row < 16; ++row) {
        const pixel *s = first + row * srcStride;
        pixel *d = dst + row * dstStride;
        for (int col = 0; col < 16;) {
            const size_t vl = __riscv_vsetvl_e8m1(16 - col);
            // Every filter's partial sums fit signed 16 bits for 8-bit pixels.
            vint16m2_t sum = __riscv_vmv_v_x_i16m2(0, vl);
            for (int tap = 0; tap < 8; ++tap) {
                const vuint8m1_t bytes = __riscv_vle8_v_u8m1(s + col + tap * srcStride, vl);
                const vint16m2_t samples = __riscv_vreinterpret_v_u16m2_i16m2(
                    __riscv_vzext_vf2_u16m2(bytes, vl));
                sum = __riscv_vmacc_vx_i16m2(sum, c[tap], samples, vl);
            }
            sum = __riscv_vadd_vx_i16m2(sum, 32, vl);
            sum = __riscv_vsra_vx_i16m2(sum, 6, vl);
            sum = __riscv_vmax_vx_i16m2(sum, 0, vl);
            sum = __riscv_vmin_vx_i16m2(sum, 255, vl);
            const vuint8m1_t out = __riscv_vnclipu_wx_u8m1(
                __riscv_vreinterpret_v_i16m2_u16m2(sum), 0, __RISCV_VXRM_RDN, vl);
            __riscv_vse8_v_u8m1(d + col, out, vl);
            col += static_cast<int>(vl);
        }
    }
}
