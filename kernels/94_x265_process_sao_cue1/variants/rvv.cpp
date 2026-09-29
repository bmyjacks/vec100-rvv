#include "kernel.h"
#include <riscv_vector.h>
#include <cstddef>
#include <cstdint>

static bool overlaps(const void *a, size_t an, const void *b, size_t bn) {
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x <= y ? y - x < an : x - y < bn;
}

void process_sao_cue1_2rows_rvv(pixel *rec, int8_t *upBuff1, int8_t *offsetEo,
                                 intptr_t stride, int width) {
    if (width <= 0)
        return;

    // A row must not overwrite another row's later reads, and neither the
    // signs nor the lookup table may change through pixel/sign writes. This
    // also covers zero/negative strides and all partial/offset aliases.
    if (stride < width ||
        overlaps(rec, size_t(width), upBuff1, size_t(width)) ||
        overlaps(rec + stride, size_t(width), upBuff1, size_t(width)) ||
        overlaps(rec + 2 * stride, size_t(width), upBuff1, size_t(width)) ||
        overlaps(rec, size_t(width), offsetEo, 5) ||
        overlaps(rec + stride, size_t(width), offsetEo, 5) ||
        overlaps(rec + 2 * stride, size_t(width), offsetEo, 5) ||
        overlaps(upBuff1, size_t(width), offsetEo, 5)) {
        process_sao_cue1_2rows(rec, upBuff1, offsetEo, stride, width);
        return;
    }

    // Each column carries its sign from row 0 to row 1. Process one entire
    // row before the next; columns and vector chunks are independent here.
    for (int y = 0; y < 2; ++y, rec += stride) {
        for (int x = 0; x < width;) {
            const size_t vl = __riscv_vsetvl_e8m1(width - x);
            const vuint8m1_t current = __riscv_vle8_v_u8m1(rec + x, vl);
            const vuint8m1_t below = __riscv_vle8_v_u8m1(rec + stride + x, vl);
            const vbool8_t gt = __riscv_vmsltu_vv_u8m1_b8(below, current, vl);
            const vbool8_t lt = __riscv_vmsltu_vv_u8m1_b8(current, below, vl);
            vuint8m1_t pos = __riscv_vmerge_vxm_u8m1(
                __riscv_vmv_v_x_u8m1(0, vl), 1, gt, vl);
            vuint8m1_t neg = __riscv_vmerge_vxm_u8m1(
                __riscv_vmv_v_x_u8m1(0, vl), 1, lt, vl);
            const vuint8m1_t sign = __riscv_vsub_vv_u8m1(pos, neg, vl);
            vuint8m1_t edge = __riscv_vadd_vx_u8m1(
                __riscv_vle8_v_u8m1(reinterpret_cast<const uint8_t *>(upBuff1 + x), vl), 2, vl);
            edge = __riscv_vadd_vv_u8m1(edge, sign, vl);
            const vuint8m1_t table = __riscv_vle8_v_u8m1(
                reinterpret_cast<const uint8_t *>(offsetEo), 5);
            const vuint8m1_t delta = __riscv_vrgather_vv_u8m1(table, edge, vl);
            const vint16m2_t pixels = __riscv_vreinterpret_v_u16m2_i16m2(
                __riscv_vzext_vf2_u16m2(current, vl));
            vint16m2_t sum = __riscv_vadd_vv_i16m2(pixels,
                __riscv_vsext_vf2_i16m2(__riscv_vreinterpret_v_u8m1_i8m1(delta), vl), vl);
            sum = __riscv_vmax_vx_i16m2(sum, 0, vl);
            sum = __riscv_vmin_vx_i16m2(sum, 255, vl);
            const vuint8m1_t output = __riscv_vnclipu_wx_u8m1(
                __riscv_vreinterpret_v_i16m2_u16m2(sum), 0, __RISCV_VXRM_RDN, vl);
            __riscv_vse8_v_u8m1(reinterpret_cast<uint8_t *>(upBuff1 + x),
                __riscv_vsub_vv_u8m1(neg, pos, vl), vl);
            __riscv_vse8_v_u8m1(rec + x, output, vl);
            x += static_cast<int>(vl);
        }
    }
}
