#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

static bool overlap(const void *a, size_t an, const void *b, size_t bn) {
    uintptr_t x = reinterpret_cast<uintptr_t>(a);
    uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x < y + bn && y < x + an;
}

void estimateCUPropagateCost_bench_rvv(int *dst, const uint16_t *propagateIn,
                                   const int32_t *intraCosts,
                                   const uint16_t *interCosts,
                                   const int32_t *invQscales,
                                   const double *fpsFactor, int len) {
    if (len <= 0) return;
    const size_t n = size_t(len);
    if (overlap(dst, n * 4, propagateIn, n * 2) ||
        overlap(dst, n * 4, intraCosts, n * 4) ||
        overlap(dst, n * 4, interCosts, n * 2) ||
        overlap(dst, n * 4, invQscales, n * 4) ||
        overlap(dst, n * 4, fpsFactor, sizeof(double))) {
        estimateCUPropagateCost_bench_isolated(dst, propagateIn, intraCosts,
                                              interCosts, invQscales, fpsFactor, len);
        return;
    }
    const double fps = *fpsFactor / 256;
    for (int i = 0; i < len;) {
        size_t vl = __riscv_vsetvl_e32m1(len - i);
        vint32m1_t intra = __riscv_vle32_v_i32m1(intraCosts + i, vl);
        vuint16mf2_t inter16 = __riscv_vand_vx_u16mf2(
            __riscv_vle16_v_u16mf2(interCosts + i, vl), LOWRES_COST_MASK, vl);
        vint32m1_t inter = __riscv_vmin_vv_i32m1(intra,
            __riscv_vreinterpret_v_u32m1_i32m1(
                __riscv_vwaddu_vx_u32m1(inter16, 0, vl)), vl);
        vint32m1_t diff = __riscv_vsub_vv_i32m1(intra, inter, vl);
        vfloat64m2_t dIntra = __riscv_vfcvt_f_x_v_f64m2(
            __riscv_vwadd_vx_i64m2(intra, 0, vl), vl);
        vfloat64m2_t q = __riscv_vfcvt_f_x_v_f64m2(
            __riscv_vwadd_vx_i64m2(
                __riscv_vle32_v_i32m1(invQscales + i, vl), 0, vl), vl);
        vuint16mf2_t prop16 = __riscv_vle16_v_u16mf2(propagateIn + i, vl);
        vfloat64m2_t prop = __riscv_vfcvt_f_xu_v_f64m2(
            __riscv_vwaddu_vx_u64m2(
                __riscv_vwaddu_vx_u32m1(prop16, 0, vl), 0, vl), vl);
        vfloat64m2_t amount = __riscv_vfmul_vv_f64m2(dIntra, q, vl);
        amount = __riscv_vfmul_vf_f64m2(amount, fps, vl);
        amount = __riscv_vfadd_vv_f64m2(prop, amount, vl);
        vfloat64m2_t num = __riscv_vfcvt_f_x_v_f64m2(
            __riscv_vwadd_vx_i64m2(diff, 0, vl), vl);
        vfloat64m2_t result = __riscv_vfmul_vv_f64m2(amount, num, vl);
        result = __riscv_vfdiv_vv_f64m2(result, dIntra, vl);
        result = __riscv_vfadd_vf_f64m2(result, 0.5, vl);
        // C++'s int conversion truncates towards zero, regardless of frm.
        vint32m1_t out = __riscv_vfncvt_rtz_x_f_w_i32m1(result, vl);
        __riscv_vse32_v_i32m1(dst + i, out, vl);
        i += vl;
    }
}
