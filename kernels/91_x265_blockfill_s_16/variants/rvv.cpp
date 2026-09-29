#include "kernel.h"

#include <riscv_vector.h>

void blockfill_s_16_rvv(int16_t *dst, intptr_t dstride, int16_t val) {
    // Adjacent rows can be treated as one 256-element span only at stride 16.
    // Strip-mine that span: VLEN 128 and 256 (and larger VLENs) all work.
    if (dstride == 16) {
        for (size_t i = 0; i < 256;) {
            size_t vl = __riscv_vsetvl_e16m1(256 - i);
            vint16m1_t fill = __riscv_vmv_v_x_i16m1(val, vl);
            __riscv_vse16_v_i16m1(dst + i, fill, vl);
            i += vl;
        }
        return;
    }

    // Each logical row is written independently. In particular, zero, small,
    // and negative strides may make rows overlap or run backwards in memory.
    // No row is loaded: all aliases receive the same value as the scalar loop.
    for (int y = 0; y < 16; ++y) {
        int16_t *row = dst + y * dstride;
        for (size_t x = 0; x < 16;) {
            size_t vl = __riscv_vsetvl_e16m1(16 - x);
            vint16m1_t fill = __riscv_vmv_v_x_i16m1(val, vl);
            __riscv_vse16_v_i16m1(row + x, fill, vl);
            x += vl;
        }
    }
}
