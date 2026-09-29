#include "kernel.h"
#include <riscv_vector.h>

void GB_bitmap_subref_rvv(GB_void *Cx, const GB_void *Ax, size_t csize,
                          const uint32_t *I32, const uint64_t *I64, int Ikind,
                          const int64_t *Icolon, int64_t first, int64_t end,
                          int64_t pC0, int64_t pA0) {
    // Source address is pC0 + I[iA], destination is pA0 + iA.
    // For an overlapping view, retain the original ordered memcpy calls.
    const auto index = [&](int64_t i) -> int64_t {
        if (Ikind == GB_ALL) return i;
        if (Ikind == GB_RANGE) return Icolon[GxB_BEGIN] + i;
        if (Ikind == GB_STRIDE)
            return Icolon[GxB_BEGIN] + i * Icolon[GxB_INC];
        return I32 ? I32[i] : I64[i];
    };
    bool overlap = false;
    for (int64_t i = first; i < end; ++i) {
        auto *src = Ax + (pC0 + index(i)) * csize;
        auto *dst = Cx + (pA0 + first) * csize;
        auto *dst_end = Cx + (pA0 + end) * csize;
        if (reinterpret_cast<uintptr_t>(src) < reinterpret_cast<uintptr_t>(dst_end) &&
            reinterpret_cast<uintptr_t>(src + csize) > reinterpret_cast<uintptr_t>(dst)) {
            overlap = true;
            break;
        }
    }
    if (overlap) {
        for (int64_t i = first; i < end; ++i)
            memcpy(Cx + (pA0 + i) * csize,
                   Ax + (pC0 + index(i)) * csize, csize);
        return;
    }
    // Byte-wise indexed loads support every entry size, index kind and
    // repeated source index without modifying the untouched output bytes.
    for (int64_t i = first; i < end;) {
        size_t vl = __riscv_vsetvl_e64m1(end - i);
        vuint64m1_t offsets;
        if (Ikind == GB_LIST) {
            offsets = I32 ? __riscv_vwcvtu_x_x_v_u64m1(
                __riscv_vle32_v_u32mf2(I32 + i, vl), vl)
                : __riscv_vle64_v_u64m1(I64 + i, vl);
        } else {
            offsets = __riscv_vid_v_u64m1(vl);
            offsets = __riscv_vadd_vx_u64m1(offsets, i, vl);
            if (Ikind == GB_STRIDE)
                offsets = __riscv_vmul_vx_u64m1(offsets, Icolon[GxB_INC], vl);
            if (Ikind != GB_ALL)
                offsets = __riscv_vadd_vx_u64m1(offsets, Icolon[GxB_BEGIN], vl);
        }
        offsets = __riscv_vmul_vx_u64m1(
            __riscv_vadd_vx_u64m1(offsets, pC0, vl), csize, vl);
        for (size_t b = 0; b < csize; ++b) {
            vuint64m1_t pos = __riscv_vadd_vx_u64m1(offsets, b, vl);
            vuint8mf8_t bytes = __riscv_vluxei64_v_u8mf8(Ax, pos, vl);
            __riscv_vsse8_v_u8mf8(Cx + (pA0 + i) * csize + b,
                                    csize, bytes, vl);
        }
        i += vl;
    }
}
