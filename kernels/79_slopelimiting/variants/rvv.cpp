#include "kernel.h"

#include <riscv_vector.h>

// Keep the upstream order: sample both anchors at their original positions,
// finish the first interval, then sample the second anchor and rewrite it.
static void write_interval(cmsUInt16Number *table, int begin, int end,
                           double slope, double beta) {
    for (int i = begin; i < end;) {
        const size_t vl = __riscv_vsetvl_e64m1(end - i);
        const auto indices = __riscv_vadd_vx_u64m1(
            __riscv_vid_v_u64m1(vl), static_cast<unsigned>(i), vl);
        const auto x = __riscv_vfcvt_f_xu_v_f64m1(indices, vl);
        const auto b = __riscv_vfmv_v_f_f64m1(beta, vl);
        const auto interpolated = __riscv_vfmacc_vf_f64m1(b, slope, x, vl);
        const auto rounded = __riscv_vfadd_vf_f64m1(interpolated, 0.5, vl);
        // The upstream fast floor rounds to 15.16 fixed point before taking
        // the integer part. A direct FP-to-integer truncation can differ by
        // one for results extremely close to an integer boundary.
        const auto fixed = __riscv_vfadd_vf_f64m1(
            __riscv_vfsub_vf_f64m1(rounded, 32767.0, vl),
            68719476736.0 * 1.5, vl);
        auto wide = __riscv_vadd_vx_u64m1(
            __riscv_vsrl_vx_u64m1(
                __riscv_vreinterpret_v_f64m1_u64m1(fixed), 16, vl),
            32767, vl);
        const auto low = __riscv_vmfle_vf_f64m1_b64(rounded, 0.0, vl);
        const auto high = __riscv_vmfge_vf_f64m1_b64(rounded, 65535.0, vl);
        wide = __riscv_vmerge_vxm_u64m1(wide, 0, low, vl);
        wide = __riscv_vmerge_vxm_u64m1(wide, 65535, high, vl);
        const auto word32 = __riscv_vnsrl_wx_u32mf2(wide, 0, vl);
        const auto word16 = __riscv_vnsrl_wx_u16mf4(word32, 0, vl);
        __riscv_vse16_v_u16mf4(table + i, word16, vl);
        i += static_cast<int>(vl);
    }
}

void SlopeLimiting_rvv(cmsToneCurve *g) {
    const int at_begin = static_cast<int>(floor(g->nEntries * 0.02 + 0.5));
    const int at_end = static_cast<int>(g->nEntries) - at_begin - 1;
    const bool descending = cmsIsToneCurveDescending(g);
    const int begin_val = descending ? 65535 : 0;
    const int end_val = descending ? 0 : 65535;

    // The upstream n<25 case divides by zero and can produce NaNs. Execute
    // the identical scalar operation there, including its fast-floor behavior.
    if (at_begin == 0) {
        const double val = g->Table16[at_begin];
        const double slope = (val - begin_val) / at_begin;
        const double beta = val - slope * at_begin;
        for (int i = 0; i < at_begin; ++i)
            g->Table16[i] = _cmsQuickSaturateWord(i * slope + beta);
        const double tail_val = g->Table16[at_end];
        const double tail_slope = (end_val - tail_val) / at_begin;
        const double tail_beta = tail_val - tail_slope * at_end;
        for (int i = at_end; i < static_cast<int>(g->nEntries); ++i)
            g->Table16[i] = _cmsQuickSaturateWord(i * tail_slope + tail_beta);
        return;
    }

    const double first_val = g->Table16[at_begin];
    const double first_slope = (first_val - begin_val) / at_begin;
    const double first_beta = first_val - first_slope * at_begin;
    write_interval(g->Table16, 0, at_begin, first_slope, first_beta);

    const double last_val = g->Table16[at_end];
    const double last_slope = (end_val - last_val) / at_begin;
    const double last_beta = last_val - last_slope * at_end;
    write_interval(g->Table16, at_end, static_cast<int>(g->nEntries),
                   last_slope, last_beta);
}
