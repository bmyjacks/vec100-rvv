#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

static_assert(sizeof(contour_point_t) == 12 &&
                  offsetof(contour_point_t, x) == 0 &&
                  offsetof(contour_point_t, y) == 4,
              "the upstream point layout is required for strided access");

// Detect byte-range overlap without forming out-of-bounds pointers. All input
// and output views must be live for their declared lengths, as upstream expects.
static bool overlaps(const void *a, size_t as, const void *b, size_t bs) {
    if (!as || !bs) return false;
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x <= y ? y - x < as : x - y < bs;
}

// The source's sequential reads/writes are essential when user-provided
// vector storage overlaps another view. This is an independent local fallback,
// not a reference to the scalar oracle object.
static bool ordered(const hb_array_t<const contour_point_t> points,
                    const hb_array_t<const int> xd,
                    const hb_array_t<const int> yd,
                    const contour_point_t &p1, const contour_point_t &p2,
                    int p1_dx, int p2_dx, int p1_dy, int p2_dy,
                    double tolerance_sq, double *ox, double *oy) {
    const unsigned n = points.length;
    for (unsigned j = 0; j < 2; ++j) {
        float contour_point_t::*xp;
        double x1, x2, d1, d2;
        const int *in;
        double *out;
        if (j == 0) {
            xp = &contour_point_t::x;
            x1 = static_cast<double>(p1.x);
            x2 = static_cast<double>(p2.x);
            d1 = p1_dx; d2 = p2_dx; in = xd.arrayZ; out = ox;
        } else {
            xp = &contour_point_t::y;
            x1 = static_cast<double>(p1.y);
            x2 = static_cast<double>(p2.y);
            d1 = p1_dy; d2 = p2_dy; in = yd.arrayZ; out = oy;
        }
        if (x1 == x2) {
            if (d1 == d2) {
                for (unsigned i = 0; i < n; ++i) out[i] = d1;
            } else {
                for (unsigned i = 0; i < n; ++i) out[i] = 0.0;
            }
            continue;
        }
        if (x1 > x2) { hb_swap(x1, x2); hb_swap(d1, d2); }
        double scale = (d2 - d1) / (x2 - x1);
        for (unsigned i = 0; i < n; ++i) {
            double x = (double)(points.arrayZ[i].*xp);
            double d;
            if (x <= x1) d = d1;
            else if (x >= x2) d = d2;
            else d = d1 + (x - x1) * scale;
            out[i] = d;
            double err = d - in[i];
            if (err * err > tolerance_sq) return false;
        }
    }
    return true;
}

extern "C" bool hb_012_iup_segment_interpolate_rvv(
    const hb_array_t<const contour_point_t> points,
    const hb_array_t<const int> xd, const hb_array_t<const int> yd,
    const contour_point_t &p1, const contour_point_t &p2, int p1_dx, int p2_dx,
    int p1_dy, int p2_dy, double tolerance_sq,
    hb_vector_t<double> &ox, hb_vector_t<double> &oy) {
    const unsigned n = points.length;
    if (unlikely(!ox.resize_dirty(n) || !oy.resize_dirty(n))) return false;
    if (!n) return true;

    const size_t out_bytes = size_t(n) * sizeof(double);
    const size_t point_bytes = size_t(n) * sizeof(contour_point_t);
    const size_t in_bytes = size_t(n) * sizeof(int);
    if (overlaps(ox.arrayZ, out_bytes, oy.arrayZ, out_bytes) ||
        overlaps(ox.arrayZ, out_bytes, points.arrayZ, point_bytes) ||
        overlaps(oy.arrayZ, out_bytes, points.arrayZ, point_bytes) ||
        overlaps(ox.arrayZ, out_bytes, xd.arrayZ, in_bytes) ||
        overlaps(ox.arrayZ, out_bytes, yd.arrayZ, in_bytes) ||
        overlaps(oy.arrayZ, out_bytes, xd.arrayZ, in_bytes) ||
        overlaps(oy.arrayZ, out_bytes, yd.arrayZ, in_bytes))
        return ordered(points, xd, yd, p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
                       tolerance_sq, ox.arrayZ, oy.arrayZ);

    for (unsigned j = 0; j < 2; ++j) {
        double x1, x2, d1, d2;
        const int *in;
        double *out;
        if (j == 0) {
            x1 = static_cast<double>(p1.x);
            x2 = static_cast<double>(p2.x);
            d1 = p1_dx; d2 = p2_dx;
            in = xd.arrayZ; out = ox.arrayZ;
        } else {
            x1 = static_cast<double>(p1.y);
            x2 = static_cast<double>(p2.y);
            d1 = p1_dy; d2 = p2_dy;
            in = yd.arrayZ; out = oy.arrayZ;
        }

        if (x1 == x2) {
            const double fill = d1 == d2 ? d1 : 0.0;
            for (unsigned i = 0; i < n;) {
                size_t vl = __riscv_vsetvl_e64m2(n - i);
                __riscv_vse64_v_f64m2(out + i,
                    __riscv_vfmv_v_f_f64m2(fill, vl), vl);
                i += vl;
            }
            continue;
        }
        if (x1 > x2) { hb_swap(x1, x2); hb_swap(d1, d2); }
        double scale = (d2 - d1) / (x2 - x1);
        for (unsigned i = 0; i < n;) {
            size_t vl = __riscv_vsetvl_e32m1(n - i);
            vfloat32m1_t fx = __riscv_vlse32_v_f32m1(
                j == 0 ? &points.arrayZ[i].x : &points.arrayZ[i].y,
                ptrdiff_t(sizeof(contour_point_t)), vl);
            vfloat64m2_t x = __riscv_vfwcvt_f_f_v_f64m2(fx, vl);
            vbool32_t lo = __riscv_vmfle_vf_f64m2_b32(x, x1, vl);
            vbool32_t hi = __riscv_vmfge_vf_f64m2_b32(x, x2, vl);
            vfloat64m2_t diff = __riscv_vfsub_vf_f64m2(x, x1, vl);
            vfloat64m2_t middle = __riscv_vfmul_vf_f64m2(diff, scale, vl);
            middle = __riscv_vfadd_vv_f64m2(
                __riscv_vfmv_v_f_f64m2(d1, vl), middle, vl);
            vfloat64m2_t d = __riscv_vfmerge_vfm_f64m2(middle, d2, hi, vl);
            d = __riscv_vfmerge_vfm_f64m2(d, d1, lo, vl);
            vint32m1_t actual = __riscv_vle32_v_i32m1(in + i, vl);
            vfloat64m2_t expected = __riscv_vfwcvt_f_x_v_f64m2(actual, vl);
            vfloat64m2_t err = __riscv_vfsub_vv_f64m2(d, expected, vl);
            vfloat64m2_t squared = __riscv_vfmul_vv_f64m2(err, err, vl);
            vbool32_t bad = __riscv_vmfgt_vf_f64m2_b32(squared, tolerance_sq, vl);
            long first = __riscv_vfirst_m_b32(bad, vl);
            // Commit only through the first failing lane: the scalar function
            // returns before touching any later output or the y axis.
            size_t written = first < 0 ? vl : size_t(first) + 1;
            __riscv_vse64_v_f64m2(out + i, d, written);
            if (first >= 0) return false;
            i += vl;
        }
    }
    return true;
}
