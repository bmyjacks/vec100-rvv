#include "kernel.h"
#include <riscv_vector.h>

extern "C" FT_Error FT_Outline_Get_BBox_rvv(FT_Outline *outline, FT_BBox *abbox) {
    if (!abbox) return FT_Err_Invalid_Argument;
    if (!outline) return FT_Err_Invalid_Outline;
    if (!outline->n_points || !outline->n_contours) {
        *abbox = {0, 0, 0, 0};
        return FT_Err_Ok;
    }

    // Same initial extrema and the same ON-tag test as the pinned ftbbox.c.
    // If any off-curve control lies beyond the on-curve box, the exact conic
    // and cubic extrema must be computed by the original decomposition path.
    FT_BBox cbox = {0x7fffffffL, 0x7fffffffL, -0x7fffffffL, -0x7fffffffL};
    FT_BBox bbox = cbox;
    for (size_t i = 0; i < outline->n_points;) {
        const size_t vl = __riscv_vsetvl_e64m1(outline->n_points - i);
        const auto x = __riscv_vlse64_v_i64m1(&outline->points[i].x, sizeof(FT_Vector), vl);
        const auto y = __riscv_vlse64_v_i64m1(&outline->points[i].y, sizeof(FT_Vector), vl);
        const auto tags = __riscv_vlse8_v_u8mf8(outline->tags + i, 1, vl);
        const auto on = __riscv_vmseq_vx_u8mf8_b64(
            __riscv_vand_vx_u8mf8(tags, 3, vl), FT_CURVE_TAG_ON, vl);
        const auto initmin = __riscv_vmv_v_x_i64m1(0x7fffffffL, vl);
        const auto initmax = __riscv_vmv_v_x_i64m1(-0x7fffffffL, vl);
        const auto onxlo = __riscv_vmerge_vvm_i64m1(initmin, x, on, vl);
        const auto onxhi = __riscv_vmerge_vvm_i64m1(initmax, x, on, vl);
        const auto onylo = __riscv_vmerge_vvm_i64m1(initmin, y, on, vl);
        const auto onyhi = __riscv_vmerge_vvm_i64m1(initmax, y, on, vl);
        cbox.xMin = __riscv_vmv_x_s_i64m1_i64(__riscv_vredmin_vs_i64m1_i64m1(x, __riscv_vmv_s_x_i64m1(cbox.xMin, vl), vl));
        cbox.xMax = __riscv_vmv_x_s_i64m1_i64(__riscv_vredmax_vs_i64m1_i64m1(x, __riscv_vmv_s_x_i64m1(cbox.xMax, vl), vl));
        cbox.yMin = __riscv_vmv_x_s_i64m1_i64(__riscv_vredmin_vs_i64m1_i64m1(y, __riscv_vmv_s_x_i64m1(cbox.yMin, vl), vl));
        cbox.yMax = __riscv_vmv_x_s_i64m1_i64(__riscv_vredmax_vs_i64m1_i64m1(y, __riscv_vmv_s_x_i64m1(cbox.yMax, vl), vl));
        bbox.xMin = __riscv_vmv_x_s_i64m1_i64(__riscv_vredmin_vs_i64m1_i64m1(onxlo, __riscv_vmv_s_x_i64m1(bbox.xMin, vl), vl));
        bbox.xMax = __riscv_vmv_x_s_i64m1_i64(__riscv_vredmax_vs_i64m1_i64m1(onxhi, __riscv_vmv_s_x_i64m1(bbox.xMax, vl), vl));
        bbox.yMin = __riscv_vmv_x_s_i64m1_i64(__riscv_vredmin_vs_i64m1_i64m1(onylo, __riscv_vmv_s_x_i64m1(bbox.yMin, vl), vl));
        bbox.yMax = __riscv_vmv_x_s_i64m1_i64(__riscv_vredmax_vs_i64m1_i64m1(onyhi, __riscv_vmv_s_x_i64m1(bbox.yMax, vl), vl));
        i += vl;
    }
    if (cbox.xMin < bbox.xMin || cbox.xMax > bbox.xMax ||
        cbox.yMin < bbox.yMin || cbox.yMax > bbox.yMax)
        return FT_Outline_Get_BBox(outline, abbox);
    *abbox = bbox;
    return FT_Err_Ok;
}
