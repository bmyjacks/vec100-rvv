#include "kernel.h"

#include <riscv_vector.h>

static_assert(sizeof(HBINT8) == 1 && sizeof(HBINT16) == 2 &&
                  sizeof(HBINT32) == 4 && sizeof(contour_point_t) == 12 &&
                  offsetof(contour_point_t, x) == 0 &&
                  offsetof(contour_point_t, y) == 4,
              "HarfBuzz packed-number and point layouts required");

static bool overlap(const void *a, size_t n, const void *b, size_t m) {
    if (!n || !m) return false;
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x <= y ? y - x < n : x - y < m;
}

template <bool is_x>
static bool decode_axis(const HBUINT8 *&p, hb_array_t<contour_point_t> points,
                        float scalar, const HBUINT8 *end, unsigned start) {
    unsigned i = 0;
    const unsigned count = points.length;
    while (i < count) {
        if (unlikely(p + 1 > end)) return false;
        unsigned control = *p++;
        unsigned run_count = (control & TupleValues::VALUE_RUN_COUNT_MASK) + 1;
        unsigned stop = i + run_count;
        if (unlikely(stop > count)) return false;
        unsigned skip = i < start ? hb_min(start - i, run_count) : 0;
        i += skip;

        const unsigned kind = control & TupleValues::VALUES_SIZE_MASK;
        if (kind == TupleValues::VALUES_ARE_ZEROS) {
            i = stop;
            continue;
        }
        const unsigned width = kind == TupleValues::VALUES_ARE_WORDS ? 2 :
                               kind == TupleValues::VALUES_ARE_LONGS ? 4 : 1;
        if (unlikely(p + run_count * width > end)) return false;
        const HBUINT8 *run = p + skip * width;
        p += run_count * width;

        // Inputs can be views into the same storage as output points. In that
        // case preserve the source's ordered read/modify/write semantics.
        if (overlap(run, size_t(stop - i) * width, points.arrayZ,
                    size_t(count) * sizeof(contour_point_t))) {
            for (; i < stop; ++i) {
                float v;
                if (width == 1) v = float(*reinterpret_cast<const HBINT8 *>(run)) * scalar;
                else if (width == 2) v = float(*reinterpret_cast<const HBINT16 *>(run)) * scalar;
                else v = float(*reinterpret_cast<const HBINT32 *>(run)) * scalar;
                if (is_x) points.arrayZ[i].x += v;
                else points.arrayZ[i].y += v;
                run += width;
            }
            continue;
        }

        while (i < stop) {
            const size_t vl = __riscv_vsetvl_e8m1(stop - i);
            const ptrdiff_t stride = width;
            vuint8m1_t b0 = __riscv_vlse8_v_u8m1(
                reinterpret_cast<const uint8_t *>(run), stride, vl);
            vuint32m4_t u = __riscv_vzext_vf4_u32m4(b0, vl);
            if (width >= 2) {
                vuint8m1_t b1 = __riscv_vlse8_v_u8m1(
                    reinterpret_cast<const uint8_t *>(run) + 1, stride, vl);
                vuint32m4_t hi = __riscv_vzext_vf4_u32m4(b1, vl);
                u = __riscv_vor_vv_u32m4(__riscv_vsll_vx_u32m4(u, 8, vl), hi, vl);
            }
            if (width == 4) {
                for (unsigned byte = 2; byte < 4; ++byte) {
                    vuint8m1_t b = __riscv_vlse8_v_u8m1(
                        reinterpret_cast<const uint8_t *>(run) + byte, stride, vl);
                    u = __riscv_vor_vv_u32m4(
                        __riscv_vsll_vx_u32m4(u, 8, vl),
                        __riscv_vzext_vf4_u32m4(b, vl), vl);
                }
            }
            vint32m4_t signed_values = __riscv_vreinterpret_v_u32m4_i32m4(u);
            if (width != 4) {
                const unsigned shift = width == 1 ? 24 : 16;
                signed_values = __riscv_vsra_vx_i32m4(
                    __riscv_vsll_vx_i32m4(signed_values, shift, vl), shift, vl);
            }
            vfloat32m4_t delta = __riscv_vfcvt_f_x_v_f32m4(signed_values, vl);
            delta = __riscv_vfmul_vf_f32m4(delta, scalar, vl);
            float *dest = is_x ? &points.arrayZ[i].x : &points.arrayZ[i].y;
            vfloat32m4_t old = __riscv_vlse32_v_f32m4(dest, sizeof(contour_point_t), vl);
            __riscv_vsse32_v_f32m4(dest, sizeof(contour_point_t),
                                   __riscv_vfadd_vv_f32m4(old, delta, vl), vl);
            run += width * vl;
            i += vl;
        }
    }
    return true;
}

extern "C" bool decompile_deltas_add_to_points_rvv(
    const HBUINT8 *&p, hb_array_t<contour_point_t> points, float scalar,
    const HBUINT8 *end, unsigned start) {
    return decode_axis<true>(p, points, scalar, end, start) &&
           decode_axis<false>(p, points, scalar, end, start);
}
