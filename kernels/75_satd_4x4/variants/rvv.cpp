#include "kernel.h"

#include <riscv_vector.h>

namespace {
vint16m1_t difference(const pixel *a, intptr_t sa, const pixel *b,
                       intptr_t sb, int col, size_t vl) {
    const vuint8mf2_t x = __riscv_vlse8_v_u8mf2(a + col, sa, vl);
    const vuint8mf2_t y = __riscv_vlse8_v_u8mf2(b + col, sb, vl);
    return __riscv_vsub_vv_i16m1(
        __riscv_vreinterpret_v_u16m1_i16m1(__riscv_vzext_vf2_u16m1(x, vl)),
        __riscv_vreinterpret_v_u16m1_i16m1(__riscv_vzext_vf2_u16m1(y, vl)), vl);
}

vint16m1_t add_abs(vint16m1_t total, vint16m1_t v, size_t vl) {
    return __riscv_vadd_vv_i16m1(total, __riscv_vmax_vv_i16m1(
        v, __riscv_vrsub_vx_i16m1(v, 0, vl), vl), vl);
}

vint16m1_t vertical(vint16m1_t total, vint16m1_t horizontal, size_t vl) {
    const vint16m1_t r0 = __riscv_vrgather_vx_i16m1(horizontal, 0, vl);
    const vint16m1_t r1 = __riscv_vrgather_vx_i16m1(horizontal, 1, vl);
    const vint16m1_t r2 = __riscv_vrgather_vx_i16m1(horizontal, 2, vl);
    const vint16m1_t r3 = __riscv_vrgather_vx_i16m1(horizontal, 3, vl);
    const vint16m1_t u0 = __riscv_vadd_vv_i16m1(r0, r1, vl);
    const vint16m1_t u1 = __riscv_vsub_vv_i16m1(r0, r1, vl);
    const vint16m1_t u2 = __riscv_vadd_vv_i16m1(r2, r3, vl);
    const vint16m1_t u3 = __riscv_vsub_vv_i16m1(r2, r3, vl);
    total = add_abs(total, __riscv_vadd_vv_i16m1(u0, u2, vl), vl);
    total = add_abs(total, __riscv_vadd_vv_i16m1(u1, u3, vl), vl);
    total = add_abs(total, __riscv_vsub_vv_i16m1(u0, u2, vl), vl);
    return add_abs(total, __riscv_vsub_vv_i16m1(u1, u3, vl), vl);
}
} // namespace

int satd_4x4_bench_rvv(const pixel *pix1, intptr_t stride_pix1, const pixel *pix2,
                       intptr_t stride_pix2) {
    const size_t vl = __riscv_vsetvl_e16m1(4);
    const vint16m1_t c0 = difference(pix1, stride_pix1, pix2, stride_pix2, 0, vl);
    const vint16m1_t c1 = difference(pix1, stride_pix1, pix2, stride_pix2, 1, vl);
    const vint16m1_t c2 = difference(pix1, stride_pix1, pix2, stride_pix2, 2, vl);
    const vint16m1_t c3 = difference(pix1, stride_pix1, pix2, stride_pix2, 3, vl);

    // Each vector lane holds one row. First transform across the columns.
    const vint16m1_t t0 = __riscv_vadd_vv_i16m1(c0, c1, vl);
    const vint16m1_t t1 = __riscv_vsub_vv_i16m1(c0, c1, vl);
    const vint16m1_t t2 = __riscv_vadd_vv_i16m1(c2, c3, vl);
    const vint16m1_t t3 = __riscv_vsub_vv_i16m1(c2, c3, vl);

    vint16m1_t total = __riscv_vmv_v_x_i16m1(0, vl);
    // All intermediate magnitudes and the final sum fit signed 16 bits.
    total = vertical(total, __riscv_vadd_vv_i16m1(t0, t2, vl), vl);
    total = vertical(total, __riscv_vadd_vv_i16m1(t1, t3, vl), vl);
    total = vertical(total, __riscv_vsub_vv_i16m1(t0, t2, vl), vl);
    total = vertical(total, __riscv_vsub_vv_i16m1(t1, t3, vl), vl);
    // Gather broadcasts each row, so every lane has the complete SATD sum.
    return __riscv_vmv_x_s_i16m1_i16(total) >> 1;
}
