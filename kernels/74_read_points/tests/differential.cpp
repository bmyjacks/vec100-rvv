#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

extern "C" bool read_points_rvv(const HBUINT8 *&, hb_array_t<contour_point_t>,
                                 const HBUINT8 *, float contour_point_t::*,
                                 SimpleGlyph::simple_glyph_flag_t,
                                 SimpleGlyph::simple_glyph_flag_t);
int main() {
    std::mt19937 rng(0x77f00d);
    for (unsigned n : {0u, 1u, 2u, 3u, 7u, 8u, 15u, 16u, 17u,
                       32u, 33u, 64u, 65u, 129u, 257u}) {
        for (unsigned trial = 0; trial < 150; ++trial) {
            std::vector<contour_point_t> a(n + 5), b(n + 5);
            std::vector<HBUINT8> input(2 * n + 16);
            for (auto &v : input) v = uint8_t(rng());
            for (auto &v : a) {
                std::memset(&v, 0, sizeof v);
                v.x = float(int(rng() % 1000) - 500);
                v.y = float(int(rng() % 1000) - 500);
                v.flag = (rng() % 3 == 0) ?
                    (SimpleGlyph::FLAG_X_SAME | SimpleGlyph::FLAG_Y_SAME) :
                    uint8_t(rng());
                v.is_end_point = (rng() & 1) != 0;
            }
            std::memcpy(b.data(), a.data(), a.size() * sizeof a[0]);
            // Truncated data, including the exact bounds and each possible
            // failure position; a 16-bit delta has big-endian sign extension.
            const unsigned available = trial % input.size();
            const HBUINT8 *p1 = input.data(), *p2 = input.data();
            float contour_point_t::*field = trial & 1 ? &contour_point_t::x : &contour_point_t::y;
            auto sh = trial & 1 ? SimpleGlyph::FLAG_X_SHORT : SimpleGlyph::FLAG_Y_SHORT;
            auto same = trial & 1 ? SimpleGlyph::FLAG_X_SAME : SimpleGlyph::FLAG_Y_SAME;
            bool ok1 = read_points(p1, {a.data(), n}, input.data() + available, field, sh, same);
            bool ok2 = read_points_rvv(p2, {b.data(), n}, input.data() + available, field, sh, same);
            if (ok1 != ok2 || p1 != p2 || std::memcmp(a.data(), b.data(), a.size() * sizeof a[0])) {
                std::fprintf(stderr, "read_points mismatch n=%u trial=%u available=%u\n", n, trial, available);
                std::abort();
            }
        }
    }
    // Long no-input runs cross several VLEN-sized batches. Check both axes
    // and the exact transition back to variable-length signed deltas.
    for (auto axis : {0, 1}) {
        std::vector<contour_point_t> a(263), b(263);
        for (auto &v : a) {
            std::memset(&v, 0, sizeof v);
            v.flag = SimpleGlyph::FLAG_X_SAME | SimpleGlyph::FLAG_Y_SAME;
        }
        a[130].flag = 0;
        a[260].flag = SimpleGlyph::FLAG_X_SHORT | SimpleGlyph::FLAG_Y_SHORT;
        std::memcpy(b.data(), a.data(), a.size() * sizeof a[0]);
        HBUINT8 data[] = {HBUINT8(0xff), HBUINT8(0xfe), HBUINT8(17)};
        const HBUINT8 *p = data, *q = data;
        auto field = axis ? &contour_point_t::x : &contour_point_t::y;
        auto sh = axis ? SimpleGlyph::FLAG_X_SHORT : SimpleGlyph::FLAG_Y_SHORT;
        auto same = axis ? SimpleGlyph::FLAG_X_SAME : SimpleGlyph::FLAG_Y_SAME;
        bool ok1 = read_points(p, {a.data(), unsigned(a.size())}, data + 3, field, sh, same);
        bool ok2 = read_points_rvv(q, {b.data(), unsigned(b.size())}, data + 3, field, sh, same);
        if (ok1 != ok2 || p != q || std::memcmp(a.data(), b.data(), a.size() * sizeof a[0]))
            std::abort();
    }
    std::puts("read_points: pass");
}
