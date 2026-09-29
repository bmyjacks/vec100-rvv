#include "kernel.h"

#include <cstddef>
#include <riscv_vector.h>

// One lane per byte. A nonempty mask corresponds to the original early FALSE;
// only read within the caller's guaranteed 32-byte bitmaps.
BOOL pcre2_009_class_intersect_rvv(const uint8_t *set1, const uint8_t *set2,
                                   BOOL invert_bits) {
    size_t offset = 0;
    while (offset < 32) {
        const size_t vl = __riscv_vsetvl_e8m1(32 - offset);
        const vuint8m1_t a = __riscv_vle8_v_u8m1(set1 + offset, vl);
        const vuint8m1_t b = __riscv_vle8_v_u8m1(set2 + offset, vl);
        const vuint8m1_t selected =
            invert_bits ? __riscv_vnot_v_u8m1(b, vl) : b;
        const vuint8m1_t both = __riscv_vand_vv_u8m1(a, selected, vl);
        const vbool8_t hit = __riscv_vmsne_vx_u8m1_b8(both, 0, vl);
        if (__riscv_vfirst_m_b8(hit, vl) >= 0)
            return FALSE;
        offset += vl;
    }
    return TRUE;
}
