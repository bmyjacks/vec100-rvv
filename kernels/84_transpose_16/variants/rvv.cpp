#include "kernel.h"

#include <riscv_vector.h>

void transpose_16_rvv(pixel *dst, const pixel *src, intptr_t stride) {
    const intptr_t last_row = 15 * stride;
    const intptr_t low = last_row < 0 ? last_row : 0;
    const intptr_t high = (last_row > 0 ? last_row : 0) + 16;
    const uintptr_t dst_low = reinterpret_cast<uintptr_t>(dst);
    const uintptr_t src_low = reinterpret_cast<uintptr_t>(src + low);
    const uintptr_t src_high = reinterpret_cast<uintptr_t>(src + high);

    // The upstream loops read each pixel immediately before writing it.
    // Vectorizing overlapping buffers can change subsequent source reads.
    if (dst_low < src_high && src_low < dst_low + 256) {
        for (int k = 0; k < 16; ++k)
            for (int l = 0; l < 16; ++l)
                dst[k * 16 + l] = src[l * stride + k];
        return;
    }

    for (int k = 0; k < 16; ++k) {
        int l = 0;
        while (l < 16) {
            const size_t vl = __riscv_vsetvl_e8m1(16 - l);
            const vuint8m1_t row =
                __riscv_vlse8_v_u8m1(src + l * stride + k, stride, vl);
            __riscv_vse8_v_u8m1(dst + k * 16 + l, row, vl);
            l += static_cast<int>(vl);
        }
    }
}
