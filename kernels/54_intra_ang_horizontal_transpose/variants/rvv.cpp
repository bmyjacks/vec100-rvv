#include "kernel.h"

#include <riscv_vector.h>

void intra_ang_horizontal_transpose_rvv(pixel *dst, std::intptr_t dstStride,
                                    const pixel *srcPix) {
    // dirMode=10, width=16, bFilter=0: horizontal mode with angle zero.
    // The upstream neighbour flip snapshots these 16 source pixels before
    // touching dst, even when srcPix aliases dst.
    pixel top[16];
    for (int x = 0; x < 16; ++x)
        top[x] = srcPix[33 + x];

    if (dstStride > -16 && dstStride < 16) {
        // Rows can overlap: the prediction writes and upper-triangle swaps
        // must happen in precisely the upstream order.
        for (int y = 0; y < 16; ++y)
            for (int x = 0; x < 16; ++x)
                dst[y * dstStride + x] = top[x];
        for (int y = 0; y < 15; ++y)
            for (int x = y + 1; x < 16; ++x) {
                pixel tmp = dst[y * dstStride + x];
                dst[y * dstStride + x] = dst[x * dstStride + y];
                dst[x * dstStride + y] = tmp;
            }
        return;
    }

    // For disjoint rows the upstream prediction and transpose result in
    // row y containing 16 copies of the snapshotted neighbour top[y].
    for (int y = 0; y < 16; ++y) {
        size_t x = 0;
        while (x < 16) {
            const size_t vl = __riscv_vsetvl_e8m1(16 - x);
            vuint8m1_t v = __riscv_vmv_v_x_u8m1(top[y], vl);
            __riscv_vse8_v_u8m1(dst + y * dstStride + x, v, vl);
            x += vl;
        }
    }
}
