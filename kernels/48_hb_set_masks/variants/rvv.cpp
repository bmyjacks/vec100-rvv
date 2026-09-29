#include "kernel.h"

#include <cstddef>
#include <riscv_vector.h>

static_assert(sizeof(hb_glyph_info_t) == 20 &&
                  offsetof(hb_glyph_info_t, mask) == 4 &&
                  offsetof(hb_glyph_info_t, cluster) == 8,
              "strided RVV field access requires the upstream glyph layout");

extern "C" void hb_017_buffer_set_masks_rvv(hb_glyph_info_t *info, unsigned int len,
                                        hb_mask_t value, hb_mask_t mask,
                                        unsigned int cluster_start,
                                        unsigned int cluster_end) {
    if (!mask)
        return;

    const hb_mask_t not_mask = ~mask;
    value &= mask;
    const bool whole = cluster_start == 0 && cluster_end == (unsigned int)-1;
    constexpr ptrdiff_t stride = sizeof(hb_glyph_info_t);

    unsigned int i = 0;
    while (i < len) {
        const size_t vl = __riscv_vsetvl_e32m1(len - i);
        const vuint32m1_t old_mask =
            __riscv_vlse32_v_u32m1(&info[i].mask, stride, vl);
        const vuint32m1_t cleared =
            __riscv_vand_vx_u32m1(old_mask, not_mask, vl);
        const vuint32m1_t updated = __riscv_vor_vx_u32m1(cleared, value, vl);

        if (whole) {
            __riscv_vsse32_v_u32m1(&info[i].mask, stride, updated, vl);
        } else {
            const vuint32m1_t cluster =
                __riscv_vlse32_v_u32m1(&info[i].cluster, stride, vl);
            const vbool32_t from =
                __riscv_vmsgeu_vx_u32m1_b32(cluster, cluster_start, vl);
            const vbool32_t until =
                __riscv_vmsltu_vx_u32m1_b32(cluster, cluster_end, vl);
            const vbool32_t selected = __riscv_vmand_mm_b32(from, until, vl);
            // Do not write records outside the half-open cluster interval.
            __riscv_vsse32_v_u32m1_m(selected, &info[i].mask, stride, updated,
                                     vl);
        }
        i += static_cast<unsigned int>(vl);
    }
}
