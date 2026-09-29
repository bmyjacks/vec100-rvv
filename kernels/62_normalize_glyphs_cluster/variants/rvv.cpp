#include "kernel.h"

#include <riscv_vector.h>

static int compare_info_codepoint(const hb_glyph_info_t *a,
                                  const hb_glyph_info_t *b) {
    return (int)b->codepoint - (int)a->codepoint;
}

extern "C" void normalize_glyphs_cluster_rvv(hb_glyph_info_t *info,
                                         hb_glyph_position_t *pos,
                                         unsigned int start, unsigned int end,
                                         bool backward) {
    hb_position_t total_x = 0, total_y = 0;
    for (unsigned i = start; i < end; ++i) {
        total_x = hb_saturate_add(total_x, pos[i].x_advance);
        total_y = hb_saturate_add(total_y, pos[i].y_advance);
    }

    hb_position_t x = 0, y = 0;
    for (unsigned i = start; i < end;) {
        // The saturating prefix is order-dependent (it may hit either bound
        // and recover). Form its inputs in scalar order; apply independent
        // saturated offsets and clear advances with strided RVV operations.
        const size_t vl = __riscv_vsetvl_e32m1(
            (end - i) < 64 ? end - i : 64);
        int32_t px[64], py[64];
        for (size_t j = 0; j < vl; ++j) {
            px[j] = x;
            py[j] = y;
            x = hb_saturate_add(x, pos[i + j].x_advance);
            y = hb_saturate_add(y, pos[i + j].y_advance);
        }
        constexpr ptrdiff_t stride = sizeof(hb_glyph_position_t);
        int32_t *ox = &pos[i].x_offset, *oy = &pos[i].y_offset;
        vint32m1_t vx = __riscv_vlse32_v_i32m1(ox, stride, vl);
        vint32m1_t vy = __riscv_vlse32_v_i32m1(oy, stride, vl);
        vx = __riscv_vsadd_vv_i32m1(vx, __riscv_vle32_v_i32m1(px, vl), vl);
        vy = __riscv_vsadd_vv_i32m1(vy, __riscv_vle32_v_i32m1(py, vl), vl);
        __riscv_vsse32_v_i32m1(ox, stride, vx, vl);
        __riscv_vsse32_v_i32m1(oy, stride, vy, vl);
        const vint32m1_t zero = __riscv_vmv_v_x_i32m1(0, vl);
        __riscv_vsse32_v_i32m1(&pos[i].x_advance, stride, zero, vl);
        __riscv_vsse32_v_i32m1(&pos[i].y_advance, stride, zero, vl);
        i += vl;
    }

    if (backward) {
        pos[end - 1].x_advance = total_x;
        pos[end - 1].y_advance = total_y;
        hb_stable_sort(info + start, end - start - 1,
                       compare_info_codepoint, pos + start);
    } else {
        pos[start].x_advance = hb_saturate_add(pos[start].x_advance, total_x);
        pos[start].y_advance = hb_saturate_add(pos[start].y_advance, total_y);
        for (unsigned i = start + 1; i < end;) {
            const size_t vl = __riscv_vsetvl_e32m1(end - i);
            constexpr ptrdiff_t stride = sizeof(hb_glyph_position_t);
            int32_t *ox = &pos[i].x_offset, *oy = &pos[i].y_offset;
            vint32m1_t vx = __riscv_vlse32_v_i32m1(ox, stride, vl);
            vint32m1_t vy = __riscv_vlse32_v_i32m1(oy, stride, vl);
            __riscv_vsse32_v_i32m1(ox, stride,
                                   __riscv_vssub_vx_i32m1(vx, total_x, vl), vl);
            __riscv_vsse32_v_i32m1(oy, stride,
                                   __riscv_vssub_vx_i32m1(vy, total_y, vl), vl);
            i += vl;
        }
        hb_stable_sort(info + start + 1, end - start - 1,
                       compare_info_codepoint, pos + start + 1);
    }
}
