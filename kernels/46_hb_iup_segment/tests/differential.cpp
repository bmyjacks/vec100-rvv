#include "kernel.h"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <random>
#include <vector>

static_assert(sizeof(contour_point_t) == 12, "upstream point layout");
constexpr uint32_t seed = 0x11512a5u;
static std::mt19937 rng(seed);
static unsigned cases = 0;

struct Outputs {
    hb_vector_t<double> x, y;
    Outputs(unsigned xlen, unsigned ylen, unsigned cap) {
        // Fully initialized storage makes suffixes (including unwritten lanes)
        // comparable after the upstream first-failure return.
        if (!x.alloc(cap) || !y.alloc(cap)) std::abort();
        x.length = xlen;
        y.length = ylen;
        for (unsigned i = 0; i < unsigned(x.allocated); ++i)
            x.arrayZ[i] = 10000. + i;
        for (unsigned i = 0; i < unsigned(y.allocated); ++i)
            y.arrayZ[i] = -10000. - i;
    }
};

static void same(const hb_vector_t<double> &a, const hb_vector_t<double> &b,
                 unsigned n, const char *which, bool grown = false) {
    const unsigned initialized = grown ? n :
        unsigned(a.allocated < 0 ? -a.allocated - 1 : a.allocated);
    if (a.length != b.length || a.allocated != b.allocated ||
        std::memcmp(a.arrayZ, b.arrayZ,
                    size_t(initialized) * sizeof(double)) != 0) {
        for (unsigned i = 0; i < initialized; ++i) {
            if (std::memcmp(a.arrayZ + i, b.arrayZ + i, sizeof(double))) {
                std::fprintf(stderr,
                    "seed=%08x case=%u n=%u %s[%u] scalar=%a rvv=%a "
                    "len=%u/%u cap=%d/%d\n", seed, cases, n, which, i,
                    a.arrayZ[i], b.arrayZ[i], a.length, b.length,
                    a.allocated, b.allocated);
                std::exit(1);
            }
        }
        std::fprintf(stderr, "case=%u %s metadata mismatch\n", cases, which);
        std::exit(1);
    }
}

static void check(const std::vector<contour_point_t> &points,
                  const std::vector<int> &xd, const std::vector<int> &yd,
                  contour_point_t p1, contour_point_t p2,
                  const std::array<int, 4> &d, double tol,
                  unsigned xlen, unsigned ylen, unsigned cap,
                  bool same_output = false) {
    const unsigned n = points.size();
    Outputs s(xlen, ylen, cap), v(xlen, ylen, cap);
    const unsigned offset = cases % 4;
    std::vector<contour_point_t> ps(offset + n + 2), pv(offset + n + 2);
    std::vector<int> xs(offset + n + 2), xv(offset + n + 2);
    std::vector<int> ys(offset + n + 2), yv(offset + n + 2);
    for (unsigned i = 0; i < n; ++i) {
        ps[offset + i] = points[i];
        xs[offset + i] = xd[i];
        ys[offset + i] = yd[i];
    }
    pv = ps; xv = xs; yv = ys;
    const hb_array_t<const contour_point_t> pa(ps.data() + offset, n);
    const hb_array_t<const contour_point_t> qa(pv.data() + offset, n);
    const hb_array_t<const int> xa(xs.data() + offset, n),
                                ya(ys.data() + offset, n),
                                xb(xv.data() + offset, n),
                                yb(yv.data() + offset, n);
    bool sr, vr;
    if (same_output) {
        sr = hb_012_iup_segment_interpolate_isolated(pa, xa, ya, p1, p2, d[0], d[1],
                                       d[2], d[3], tol, s.x, s.x);
        vr = hb_012_iup_segment_interpolate_rvv(qa, xb, yb, p1, p2, d[0], d[1],
                                            d[2], d[3], tol, v.x, v.x);
    } else {
        sr = hb_012_iup_segment_interpolate_isolated(pa, xa, ya, p1, p2, d[0], d[1],
                                       d[2], d[3], tol, s.x, s.y);
        vr = hb_012_iup_segment_interpolate_rvv(qa, xb, yb, p1, p2, d[0], d[1],
                                            d[2], d[3], tol, v.x, v.y);
    }
    if (sr != vr) {
        std::fprintf(stderr, "seed=%08x case=%u n=%u return %d/%d\n",
                     seed, cases, n, sr, vr);
        std::exit(1);
    }
    same(s.x, v.x, n, "x", cap == 0);
    same(s.y, v.y, n, "y", cap == 0);
    if (std::memcmp(ps.data(), pv.data(), ps.size() * sizeof(ps[0])) ||
        xs != xv || ys != yv) {
        std::fprintf(stderr, "case=%u input or guard modified\n", cases);
        std::exit(1);
    }
    ++cases;
}

static contour_point_t point(float x, float y) {
    contour_point_t p{};
    p.x = x; p.y = y;
    p.flag = uint8_t(rng()); p.is_end_point = bool(rng() & 1);
    return p;
}

int main() {
    const std::array<unsigned, 18> sizes = {
        0, 1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 129, 257};
    const std::array<double, 9> tolerances = {
        -1., 0., 0.25, 1., 2., 1e20,
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::quiet_NaN(), -0.0};
    for (unsigned n : sizes) {
        for (unsigned mode = 0; mode < 10; ++mode) {
            std::vector<contour_point_t> points(n);
            std::vector<int> xd(n), yd(n);
            for (unsigned i = 0; i < n; ++i) {
                float x = float(int(rng() % 33) - 16) * 0.5f;
                float y = float(int(rng() % 37) - 18) * 0.25f;
                if (mode == 9 && i % 11 == 0)
                    x = std::numeric_limits<float>::quiet_NaN();
                points[i] = point(x, y);
                xd[i] = int(rng() % 21) - 10;
                yd[i] = int(rng() % 17) - 8;
            }
            contour_point_t p1 = point(mode % 4 == 0 ? 4.f : -4.f,
                                        mode % 3 == 0 ? 8.f : -8.f);
            contour_point_t p2 = point(mode % 4 == 0 ? 4.f : 6.f,
                                        mode % 3 == 0 ? 8.f : 9.f);
            std::array<int, 4> d = {int(rng() % 11) - 5, int(rng() % 11) - 5,
                                     int(rng() % 11) - 5, int(rng() % 11) - 5};
            if (mode == 0) d[1] = d[0];
            if (mode == 3) d[3] = d[2];
            for (double tol : tolerances) {
                for (unsigned capacity_mode = 0; capacity_mode < 2; ++capacity_mode) {
                    unsigned cap = capacity_mode ? n + 7 : n;
                    unsigned xlen = capacity_mode ? n / 2 : 0;
                    unsigned ylen = capacity_mode ? n / 3 : 0;
                    check(points, xd, yd, p1, p2, d, tol, xlen, ylen, cap);
                }
            }
            check(points, xd, yd, p1, p2, d, 1e30, 0, 0, n + 4, true);
            // Exercise allocation/dirty growth only when both axes write all
            // n elements (so no uninitialized suffix is observed).
            check(points, xd, yd, p1, p2, d,
                  std::numeric_limits<double>::infinity(), 0, 0, 0);
        }
    }
    for (unsigned trial = 0; trial < 1200; ++trial) {
        unsigned n = rng() % 270;
        std::vector<contour_point_t> points(n);
        std::vector<int> xd(n), yd(n);
        for (unsigned i = 0; i < n; ++i) {
            points[i] = point(float(int(rng() % 1000) - 500) / 7.f,
                              float(int(rng() % 1000) - 500) / 13.f);
            xd[i] = int(rng() % 200) - 100;
            yd[i] = int(rng() % 200) - 100;
        }
        auto p1 = point(-25.f, 13.f);
        auto p2 = point(25.f, -13.f);
        std::array<int, 4> d = {-50, 50, 40, -40};
        check(points, xd, yd, p1, p2, d, trial % 3 == 0 ? 1e10 :
              trial % 3 == 1 ? 0. : 100., 0, 0, n + 5);
    }
    {
        std::vector<contour_point_t> points(7, point(1.f, 2.f));
        std::vector<int> xd(7, 0), yd(7, 0);
        hb_array_t<const contour_point_t> pa(points.data(), points.size());
        hb_array_t<const int> xa(xd.data(), xd.size()), ya(yd.data(), yd.size());
        auto p1 = point(0.f, 0.f), p2 = point(3.f, 4.f);
        for (unsigned failed_axis = 0; failed_axis < 2; ++failed_axis) {
            Outputs s(2, 3, 10), v(2, 3, 10);
            (failed_axis ? s.y : s.x).set_error();
            (failed_axis ? v.y : v.x).set_error();
            bool sr = hb_012_iup_segment_interpolate_isolated(pa, xa, ya, p1, p2,
                       1, 2, 3, 4, 0., s.x, s.y);
            bool vr = hb_012_iup_segment_interpolate_rvv(pa, xa, ya, p1, p2,
                       1, 2, 3, 4, 0., v.x, v.y);
            if (sr || vr) std::abort();
            same(s.x, v.x, 7, "failed resize x");
            same(s.y, v.y, 7, "failed resize y");
            ++cases;
        }
        std::array<double, 16> old_s{}, old_v{};
        for (unsigned i = 0; i < old_s.size(); ++i)
            old_s[i] = old_v[i] = 42. + i;
        hb_vector_t<double> sx, sy, vx, vy;
        sx.arrayZ = old_s.data(); sx.length = 3;
        vx.arrayZ = old_v.data(); vx.length = 3;
        bool sr = hb_012_iup_segment_interpolate_isolated(pa, xa, ya, p1, p2,
                    1, 2, 3, 4, 1e30, sx, sy);
        bool vr = hb_012_iup_segment_interpolate_rvv(pa, xa, ya, p1, p2,
                    1, 2, 3, 4, 1e30, vx, vy);
        if (sr != vr || !sr || old_s != old_v ||
            sx.arrayZ == old_s.data() || vx.arrayZ == old_v.data())
            std::abort();
        same(sx, vx, 7, "foreign x", true);
        same(sy, vy, 7, "foreign y", true);
        ++cases;
    }
    std::printf("hb iup_segment: %u exact full-storage cases (seed %08x)\n",
                cases, seed);
}
