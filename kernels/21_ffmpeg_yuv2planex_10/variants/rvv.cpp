#include "kernel.h"

#include <cstddef>
#include <cstdint>
#include <riscv_vector.h>

// Independently linkable vector implementation of the pixel-outer reduction.
// Preserve the upstream pixel-by-pixel read/write order if inputs overlap output.
static bool overlaps(const void *a, size_t an, const void *b, size_t bn) {
    if (!an || !bn) return false;
    uintptr_t x = reinterpret_cast<uintptr_t>(a);
    uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x >= y ? x - y < bn : y - x < an;
}

void yuv2planeX_10_c_template_rvv(const int16_t *filter, int filterSize,
                                  const int16_t **src, uint16_t *dest, int dstW,
                                  int big_endian, int output_bits) {
    const int shift = 27 - output_bits;
    const size_t bytes = static_cast<size_t>(dstW) * sizeof(uint16_t);
    bool alias = overlaps(dest, bytes, filter,
                          static_cast<size_t>(filterSize) * sizeof(int16_t)) ||
                 overlaps(dest, bytes, src,
                          static_cast<size_t>(filterSize) * sizeof(*src));
    for (int j = 0; j < filterSize && !alias; ++j)
        alias = overlaps(dest, bytes, src[j], bytes);
    if (alias) {
        for (int i = 0; i < dstW; ++i) {
            int val = 1 << (shift - 1);
            for (int j = 0; j < filterSize; ++j)
                val += src[j][i] * filter[j];
            if (big_endian)
                AV_WB16(&dest[i], av_clip_uintp2(val >> shift, output_bits));
            else
                AV_WL16(&dest[i], av_clip_uintp2(val >> shift, output_bits));
        }
        return;
    }
    for (int i = 0; i < dstW;) {
        size_t vl = __riscv_vsetvl_e16m1(static_cast<size_t>(dstW - i));
        vint32m2_t acc = __riscv_vmv_v_x_i32m2(1 << (shift - 1), vl);
        for (int j = 0; j < filterSize; ++j) {
            vint16m1_t pixels = __riscv_vle16_v_i16m1(src[j] + i, vl);
            acc = __riscv_vwmacc_vx_i32m2(acc, filter[j], pixels, vl);
        }
        acc = __riscv_vsra_vx_i32m2(acc, shift, vl);
        acc = __riscv_vmax_vx_i32m2(acc, 0, vl);
        acc = __riscv_vmin_vx_i32m2(acc, (1 << output_bits) - 1, vl);
        vuint16m1_t result = __riscv_vreinterpret_v_i16m1_u16m1(
            __riscv_vncvt_x_x_w_i16m1(acc, vl));
        if (big_endian) {
            vuint16m1_t hi = __riscv_vsll_vx_u16m1(result, 8, vl);
            result = __riscv_vor_vv_u16m1(
                hi, __riscv_vsrl_vx_u16m1(result, 8, vl), vl);
        }
        __riscv_vse16_v_u16m1(dest + i, result, vl);
        i += static_cast<int>(vl);
    }
}
