#include "kernel.h"
#include <riscv_vector.h>
#include <algorithm>
#include <cmath>

// Stage the serial float recurrence in lane order. Chunk boundaries start at
// the exact scalar successor, rather than reconstructing gx/gy from lane * step.
static uint32_t lut_index(float t, hb_paint_extend_t extend) {
    if (!std::isfinite(t)) t = 0.f;
    else if (extend == HB_PAINT_EXTEND_PAD) t = hb_clamp(t, 0.f, 1.f);
    else if (extend == HB_PAINT_EXTEND_REPEAT) {
        t = t - floorf(t);
        if (t < 0.f) t += 1.f;
    } else {
        t = fmodf(fabsf(t), 2.f);
        t = t > 1.f ? 2.f - t : t;
    }
    if (!std::isfinite(t)) t = 0.f;
    else t = hb_clamp(t, 0.f, 1.f);
    return (unsigned)(t * 255.f + 0.5f);
}

static vuint32m1_t channel(vuint32m1_t src, vuint32m1_t dst,
                            vuint32m1_t inv, unsigned shift, size_t vl) {
    auto s = __riscv_vand_vx_u32m1(__riscv_vsrl_vx_u32m1(src, shift, vl), 255, vl);
    auto d = __riscv_vand_vx_u32m1(__riscv_vsrl_vx_u32m1(dst, shift, vl), 255, vl);
    auto scaled = __riscv_vsrl_vx_u32m1(
        __riscv_vadd_vx_u32m1(__riscv_vmul_vv_u32m1(d, inv, vl), 255, vl), 8, vl);
    auto sum = __riscv_vand_vx_u32m1(__riscv_vadd_vv_u32m1(s, scaled, vl), 255, vl);
    return __riscv_vsll_vx_u32m1(sum, shift, vl);
}

extern "C" void hb_139_linear_gradient_region_rvv(
    hb_packed_t<uint32_t> *row, unsigned min_x, unsigned max_x,
    float gx, float gy, float gx0, float gy0, float dx, float dy,
    float inv_denom, float inv_xx, float inv_yx, const uint32_t *lut,
    hb_paint_extend_t extend) {
    for (unsigned px = min_x; px < max_x;) {
        size_t vl = __riscv_vsetvl_e32m1(std::min<size_t>(max_x - px, 64));
        float xs[64], ys[64], projections[64];
        uint32_t indices[64], dst[64], out[64];
        for (size_t i = 0; i < vl; ++i) {
            xs[i] = gx;
            ys[i] = gy;
            dst[i] = (uint32_t)row[px + i];
            gx += inv_xx;
            gy += inv_yx;
        }
        auto vx = __riscv_vle32_v_f32m1(xs, vl);
        auto vy = __riscv_vle32_v_f32m1(ys, vl);
        // Preserve the upstream parenthesization, with FP contraction disabled.
        vx = __riscv_vfmul_vf_f32m1(__riscv_vfsub_vf_f32m1(vx, gx0, vl), dx, vl);
        vy = __riscv_vfmul_vf_f32m1(__riscv_vfsub_vf_f32m1(vy, gy0, vl), dy, vl);
        auto proj = __riscv_vfmul_vf_f32m1(__riscv_vfadd_vv_f32m1(vx, vy, vl),
                                            inv_denom, vl);
        __riscv_vse32_v_f32m1(projections, proj, vl);
        for (size_t i = 0; i < vl; ++i)
            indices[i] = lut_index(projections[i], extend) * 4;
        auto offsets = __riscv_vle32_v_u32m1(indices, vl);
        auto src = __riscv_vluxei32_v_u32m1(lut, offsets, vl);
        auto old = __riscv_vle32_v_u32m1(dst, vl);
        auto sa = __riscv_vsrl_vx_u32m1(src, 24, vl);
        auto inv = __riscv_vrsub_vx_u32m1(sa, 255, vl);
        auto result = __riscv_vor_vv_u32m1(channel(src, old, inv, 0, vl),
                                             channel(src, old, inv, 8, vl), vl);
        result = __riscv_vor_vv_u32m1(result, channel(src, old, inv, 16, vl), vl);
        result = __riscv_vor_vv_u32m1(result, channel(src, old, inv, 24, vl), vl);
        result = __riscv_vmerge_vvm_u32m1(result, old,
                                            __riscv_vmseq_vx_u32m1_b32(sa, 0, vl), vl);
        result = __riscv_vmerge_vvm_u32m1(result, src,
                                            __riscv_vmseq_vx_u32m1_b32(sa, 255, vl), vl);
        __riscv_vse32_v_u32m1(out, result, vl);
        for (size_t i = 0; i < vl; ++i) row[px + i] = hb_packed_t<uint32_t>(out[i]);
        px += vl;
    }
}
