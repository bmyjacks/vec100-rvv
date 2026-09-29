#include "kernel.h"

#include <cmath>
#include <cstdint>
#include <riscv_vector.h>

static unsigned next_index(unsigned i, unsigned start, unsigned end) {
    return i >= end ? start : i + 1;
}

static float infer(const hb_array_t<contour_point_t> points,
                   const hb_array_t<contour_point_t> deltas,
                   unsigned target, unsigned prev, unsigned next,
                   float contour_point_t::*m) {
    float t = points.arrayZ[target].*m;
    float a = points.arrayZ[prev].*m;
    float b = points.arrayZ[next].*m;
    float da = deltas.arrayZ[prev].*m;
    float db = deltas.arrayZ[next].*m;
    if (a == b) return da == db ? da : 0.f;
    if (t <= hb_min(a, b)) return a < b ? da : db;
    if (t >= hb_max(a, b)) return a > b ? da : db;
    float r = (t - a) / (b - a);
    return da + r * (db - da);
}

static bool overlap(const contour_point_t *a, unsigned an,
                    const contour_point_t *b, unsigned bn) {
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    const size_t xn = size_t(an) * sizeof(contour_point_t);
    const size_t yn = size_t(bn) * sizeof(contour_point_t);
    return an && bn && x < y + yn && y < x + xn;
}

// Vectorize a whole, non-wrapping gap only if both coordinates interpolate
// strictly inside finite endpoints. Boundary choices, NaNs, signed zeros,
// overlaps, and wrapping gaps retain the scalar evaluation order.
static bool vector_gap(const hb_array_t<contour_point_t> orig,
                       hb_array_t<contour_point_t> deltas,
                       unsigned prev, unsigned next, unsigned count) {
    if (next <= prev || overlap(orig.arrayZ, count, deltas.arrayZ, count))
        return false;
    const auto &p = orig.arrayZ[prev];
    const auto &q = orig.arrayZ[next];
    const auto &a = deltas.arrayZ[prev];
    const auto &b = deltas.arrayZ[next];
    if (!std::isfinite(p.x) || !std::isfinite(p.y) ||
        !std::isfinite(q.x) || !std::isfinite(q.y) ||
        !std::isfinite(a.x) || !std::isfinite(a.y) ||
        !std::isfinite(b.x) || !std::isfinite(b.y) ||
        p.x == q.x || p.y == q.y)
        return false;
    // A gap never includes a referenced endpoint; verify the interpolation
    // domain before performing any vector stores (including partial strips).
    for (unsigned i = prev + 1; i < next; ++i) {
        const auto &t = orig.arrayZ[i];
        if (!std::isfinite(t.x) || !std::isfinite(t.y) ||
            !(t.x > hb_min(p.x, q.x) && t.x < hb_max(p.x, q.x)) ||
            !(t.y > hb_min(p.y, q.y) && t.y < hb_max(p.y, q.y)))
            return false;
    }
    constexpr ptrdiff_t stride = sizeof(contour_point_t);
    for (unsigned i = prev + 1; i < next;) {
        const size_t vl = __riscv_vsetvl_e32m1(next - i);
        const auto tx = __riscv_vlse32_v_f32m1(&orig.arrayZ[i].x, stride, vl);
        const auto ty = __riscv_vlse32_v_f32m1(&orig.arrayZ[i].y, stride, vl);
        const auto px = __riscv_vfsub_vf_f32m1(tx, p.x, vl);
        const auto py = __riscv_vfsub_vf_f32m1(ty, p.y, vl);
        const auto dx = __riscv_vfdiv_vf_f32m1(px, q.x - p.x, vl);
        const auto dy = __riscv_vfdiv_vf_f32m1(py, q.y - p.y, vl);
        const auto ax = __riscv_vfmv_v_f_f32m1(a.x, vl);
        const auto ay = __riscv_vfmv_v_f_f32m1(a.y, vl);
        const auto rx = __riscv_vfmacc_vf_f32m1(ax, b.x - a.x, dx, vl);
        const auto ry = __riscv_vfmacc_vf_f32m1(ay, b.y - a.y, dy, vl);
        __riscv_vsse32_v_f32m1(&deltas.arrayZ[i].x, stride, rx, vl);
        __riscv_vsse32_v_f32m1(&deltas.arrayZ[i].y, stride, ry, vl);
        i += static_cast<unsigned>(vl);
    }
    return true;
}

void gvar_iup_interpolate_rvv(const hb_array_t<contour_point_t> points,
                               hb_array_t<contour_point_t> deltas,
                               hb_array_t<contour_point_t> orig_points,
                               bool apply_to_all, bool phantom_only) {
    if (apply_to_all || phantom_only) return;
    unsigned start_point = 0, end_point = 0;
    const unsigned count = points.length;
    while (true) {
        while (end_point < count && !points.arrayZ[end_point].is_end_point)
            ++end_point;
        if (end_point == count) break;
        unsigned unref_count = 0;
        for (unsigned i = start_point; i < end_point + 1; ++i)
            unref_count += deltas.arrayZ[i].flag;
        unref_count = (end_point - start_point + 1) - unref_count;
        unsigned j = start_point;
        if (unref_count != 0 && unref_count <= end_point - start_point) {
            for (;;) {
                unsigned i, prev, next;
                do {
                    i = j;
                    j = next_index(i, start_point, end_point);
                } while (!(deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag));
                prev = j = i;
                do {
                    i = j;
                    j = next_index(i, start_point, end_point);
                } while (!(!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag));
                next = j;
                if (vector_gap(orig_points, deltas, prev, next, count)) {
                    unref_count -= next - prev - 1;
                    if (!unref_count) break;
                    continue;
                }
                i = prev;
                for (;;) {
                    i = next_index(i, start_point, end_point);
                    if (i == next) break;
                    deltas.arrayZ[i].x = infer(orig_points, deltas, i, prev, next,
                                                &contour_point_t::x);
                    deltas.arrayZ[i].y = infer(orig_points, deltas, i, prev, next,
                                                &contour_point_t::y);
                    if (--unref_count == 0) break;
                }
                if (!unref_count) break;
            }
        }
        start_point = end_point = end_point + 1;
    }
}
