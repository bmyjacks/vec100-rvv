#include "kernel.h"

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <riscv_vector.h>

// Preserve upstream's store order when neighborhoods from different pixels
// overlap (including zero/negative strides), or masks have non-boolean bits.
static void scalar(pixel* src, intptr_t srcStep, intptr_t offset, int32_t tc,
                   int32_t maskP, int32_t maskQ, int32_t maskP1, int32_t maskQ1)
{
    int32_t thrCut = tc * 10;
    int32_t tc2 = tc >> 1;
    maskP1 &= maskP;
    maskQ1 &= maskQ;
    for (int32_t i = 0; i < UNIT_SIZE; i++, src += srcStep)
    {
        int16_t m4 = (int16_t)src[0];
        int16_t m3 = (int16_t)src[-offset];
        int16_t m5 = (int16_t)src[offset];
        int16_t m2 = (int16_t)src[-offset * 2];
        int32_t delta = (9 * (m4 - m3) - 3 * (m5 - m2) + 8) >> 4;
        if (abs(delta) < thrCut)
        {
            delta = x265_clip3(-tc, tc, delta);
            src[-offset] = x265_clip(m3 + (delta & maskP));
            src[0] = x265_clip(m4 - (delta & maskQ));
            if (maskP1)
            {
                int16_t m1 = (int16_t)src[-offset * 3];
                int32_t delta1 = x265_clip3(-tc2, tc2, ((((m1 + m3 + 1) >> 1) - m2 + delta) >> 1));
                src[-offset * 2] = x265_clip(m2 + delta1);
            }
            if (maskQ1)
            {
                int16_t m6 = (int16_t)src[offset * 2];
                int32_t delta2 = x265_clip3(-tc2, tc2, ((((m6 + m4 + 1) >> 1) - m5 - delta) >> 1));
                src[offset] = x265_clip(m5 + delta2);
            }
        }
    }
}

static vint16m1_t load_pixel(const pixel* p, ptrdiff_t stride, size_t vl)
{
    return __riscv_vreinterpret_v_u16m1_i16m1(__riscv_vwcvtu_x_x_v_u16m1(
        __riscv_vlse8_v_u8mf2(p, stride, vl), vl));
}

static void store_pixel(pixel* p, ptrdiff_t stride, vint16m1_t v,
                        vbool16_t active, size_t vl)
{
    v = __riscv_vmax_vx_i16m1(v, 0, vl);
    v = __riscv_vmin_vx_i16m1(v, 255, vl);
    __riscv_vsse8_v_u8mf2_m(active, p, stride,
        __riscv_vncvt_x_x_w_u8mf2(__riscv_vreinterpret_v_i16m1_u16m1(v), vl), vl);
}

void pelFilterLuma_bench_rvv(pixel* src, intptr_t srcStep, intptr_t offset, int32_t tc,
                             int32_t maskP, int32_t maskQ, int32_t maskP1, int32_t maskQ1)
{
    // The accessed neighborhood is [-3*offset, 2*offset]. For positive
    // geometry, either separation proves disjointness without multiplication
    // overflow. The second case covers horizontal edges (step=1).
    bool independent = srcStep > 0 && offset > 0 &&
        ((offset <= INTPTR_MAX / 5 && srcStep > 5 * offset) ||
         (srcStep <= INTPTR_MAX / 3 && offset > 3 * srcStep));
    if (!independent || tc < 0 || tc > 32767 ||
        (maskP != 0 && maskP != -1) || (maskQ != 0 && maskQ != -1) ||
        (maskP1 != 0 && maskP1 != -1) || (maskQ1 != 0 && maskQ1 != -1))
    {
        scalar(src, srcStep, offset, tc, maskP, maskQ, maskP1, maskQ1);
        return;
    }

    maskP1 &= maskP;
    maskQ1 &= maskQ;
    const int32_t tc2 = tc >> 1;
    const int32_t threshold = tc * 10 > 32767 ? 32767 : tc * 10;
    const ptrdiff_t stride = srcStep;
    for (int32_t i = 0; i < UNIT_SIZE; )
    {
        size_t vl = __riscv_vsetvl_e16m1(UNIT_SIZE - i);
        pixel* p = src + i * srcStep;
        vint16m1_t m4 = load_pixel(p, stride, vl);
        vint16m1_t m3 = load_pixel(p - offset, stride, vl);
        vint16m1_t m5 = load_pixel(p + offset, stride, vl);
        vint16m1_t m2 = load_pixel(p - 2 * offset, stride, vl);
        vint16m1_t delta = __riscv_vsub_vv_i16m1(m4, m3, vl);
        delta = __riscv_vmul_vx_i16m1(delta, 9, vl);
        vint16m1_t other = __riscv_vsub_vv_i16m1(m5, m2, vl);
        delta = __riscv_vsub_vv_i16m1(delta, __riscv_vmul_vx_i16m1(other, 3, vl), vl);
        delta = __riscv_vadd_vx_i16m1(delta, 8, vl);
        delta = __riscv_vsra_vx_i16m1(delta, 4, vl);
        vint16m1_t mag = __riscv_vmax_vv_i16m1(delta, __riscv_vneg_v_i16m1(delta, vl), vl);
        vbool16_t active = __riscv_vmslt_vx_i16m1_b16(mag, threshold, vl);
        delta = __riscv_vmax_vx_i16m1(delta, -tc, vl);
        delta = __riscv_vmin_vx_i16m1(delta, tc, vl);

        vint16m1_t p0 = maskP ? __riscv_vadd_vv_i16m1(m3, delta, vl) : m3;
        vint16m1_t q0 = maskQ ? __riscv_vsub_vv_i16m1(m4, delta, vl) : m4;
        store_pixel(p - offset, stride, p0, active, vl);
        store_pixel(p, stride, q0, active, vl);
        if (maskP1)
        {
            vint16m1_t m1 = load_pixel(p - 3 * offset, stride, vl);
            vint16m1_t d1 = __riscv_vadd_vv_i16m1(m1, m3, vl);
            d1 = __riscv_vsra_vx_i16m1(__riscv_vadd_vx_i16m1(d1, 1, vl), 1, vl);
            d1 = __riscv_vadd_vv_i16m1(__riscv_vsub_vv_i16m1(d1, m2, vl), delta, vl);
            d1 = __riscv_vsra_vx_i16m1(d1, 1, vl);
            d1 = __riscv_vmin_vx_i16m1(__riscv_vmax_vx_i16m1(d1, -tc2, vl), tc2, vl);
            store_pixel(p - 2 * offset, stride, __riscv_vadd_vv_i16m1(m2, d1, vl), active, vl);
        }
        if (maskQ1)
        {
            vint16m1_t m6 = load_pixel(p + 2 * offset, stride, vl);
            vint16m1_t d2 = __riscv_vadd_vv_i16m1(m6, m4, vl);
            d2 = __riscv_vsra_vx_i16m1(__riscv_vadd_vx_i16m1(d2, 1, vl), 1, vl);
            d2 = __riscv_vsub_vv_i16m1(__riscv_vsub_vv_i16m1(d2, m5, vl), delta, vl);
            d2 = __riscv_vsra_vx_i16m1(d2, 1, vl);
            d2 = __riscv_vmin_vx_i16m1(__riscv_vmax_vx_i16m1(d2, -tc2, vl), tc2, vl);
            store_pixel(p + offset, stride, __riscv_vadd_vv_i16m1(m5, d2, vl), active, vl);
        }
        i += vl;
    }
}
