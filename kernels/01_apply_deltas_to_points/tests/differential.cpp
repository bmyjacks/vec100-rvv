#include "kernel.h"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>

void gvar_iup_interpolate_rvv(hb_array_t<contour_point_t>, hb_array_t<contour_point_t>,
                              hb_array_t<contour_point_t>, bool, bool);
#ifndef RVV_STANDALONE
void gvar_iup_interpolate(hb_array_t<contour_point_t>, hb_array_t<contour_point_t>,
                                 hb_array_t<contour_point_t>, bool, bool);
#endif

static uint32_t seed = 0x02de17a5;
static uint32_t random_word() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

// All three array views point into a single allocation; the displacement of
// each view is preserved in the independently copied scalar/RVV allocations.
static void check(std::vector<contour_point_t> storage, unsigned count,
                  unsigned p, unsigned d, unsigned o, bool all, bool phantom) {
    auto expected = storage, actual = storage;
    const auto invoke = [&](auto &v, bool vector) {
        hb_array_t<contour_point_t> points(v.data() + p, count);
        hb_array_t<contour_point_t> deltas(v.data() + d, count);
        hb_array_t<contour_point_t> orig(v.data() + o, count);
        if (vector) gvar_iup_interpolate_rvv(points, deltas, orig, all, phantom);
#ifndef RVV_STANDALONE
        else gvar_iup_interpolate(points, deltas, orig, all, phantom);
#endif
    };
#ifndef RVV_STANDALONE
    invoke(expected, false);
#endif
    invoke(actual, true);
#ifndef RVV_STANDALONE
    for (size_t i = 0; i < storage.size(); ++i) {
        if (std::memcmp(&actual[i], &expected[i], sizeof(contour_point_t))) {
            std::fprintf(stderr, "seed=0x02de17a5 count=%u views=%u,%u,%u flags=%d,%d index=%zu\n",
                         count, p, d, o, all, phantom, i);
            std::exit(1);
        }
    }
#endif
}

int main() {
    for (unsigned n : {0u, 1u, 2u, 3u, 4u, 7u, 8u, 9u, 16u, 17u, 31u, 65u, 137u}) {
        for (int trial = 0; trial < 170; ++trial) {
            std::vector<contour_point_t> storage(3 * n + 8);
            const bool same = trial % 13 == 0;
            const unsigned p = 0, d = same ? 0 : n + 2;
            const unsigned o = trial % 19 == 0 ? d : (trial % 17 == 0 ? d + 1 : 2 * n + 4);
            for (auto &v : storage) {
                v.x = static_cast<float>(int(random_word() % 201) - 100) / 7;
                v.y = static_cast<float>(int(random_word() % 301) - 150) / 11;
                v.flag = 0;
                v.is_end_point = false;
            }
            if (n) {
                // A complete contour; plus a second one for longer buffers.
                storage[p + n - 1].is_end_point = true;
                if (n > 8) storage[p + n / 2 - 1].is_end_point = true;
                for (unsigned i = 0; i < n; ++i)
                    storage[d + i].flag = (trial % 5 == 0) ? 1 : (trial % 7 == 0 ? 0 : (random_word() % 7 == 0));
                if (trial % 5 != 0 && trial % 7 != 0) {
                    storage[d].flag = 1;
                    storage[d + n - 1].flag = 1;
                }
                if (trial % 23 == 0) {
                    storage[o].x = std::numeric_limits<float>::quiet_NaN();
                    storage[o].y = std::numeric_limits<float>::infinity();
                }
            }
            check(storage, n, p, d, o, false, false);
            if (trial % 10 == 0) {
                check(storage, n, p, d, o, true, false);
                check(storage, n, p, d, o, false, true);
            }
        }
        // Long single gap: force strip-mined interpolation across VLENs.
        if (n >= 3) {
            std::vector<contour_point_t> storage(3 * n + 8);
            for (unsigned i = 0; i < n; ++i) {
                storage[i] = {float(i), float(i * 2), 0, i == n - 1};
                storage[n + 2 + i] = {i == 0 ? 0.f : 1.f, 2.f, uint8_t(i == 0 || i == n - 1), false};
                storage[2 * n + 4 + i] = {float(i), float(i * 2), 0, false};
            }
            check(storage, n, 0, n + 2, 2 * n + 4, false, false);
            // Signed zero, subnormals, and equal coordinates exercise the
            // scalar branches and floating-point exceptional-case fallback.
            storage[2 * n + 4].x = -0.f;
            storage[2 * n + 4 + n - 1].x = +0.f;
            storage[2 * n + 4].y = std::numeric_limits<float>::denorm_min();
            storage[2 * n + 4 + n - 1].y = -std::numeric_limits<float>::denorm_min();
            check(storage, n, 0, n + 2, 2 * n + 4, false, false);
        }
    }
    std::puts("IUP PASS (seed=0x02de17a5)");
}
