#include "kernel.h"

#include <climits>
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

extern "C" void normalize_glyphs_cluster_rvv(hb_glyph_info_t *,
                                                   hb_glyph_position_t *,
                                                   unsigned, unsigned, bool);

static bool check(unsigned n, unsigned start, unsigned end, bool backward,
                  std::mt19937 &rng, unsigned mode) {
    std::vector<hb_glyph_info_t> ri(n), vi;
    std::vector<hb_glyph_position_t> rp(n), vp;
    for (unsigned i = 0; i < n; ++i) {
        ri[i].mask = rng();
        ri[i].cluster = rng();
        ri[i].var1.u32 = rng();
        ri[i].var2.u32 = rng();
        rp[i].x_advance = static_cast<int32_t>(rng());
        rp[i].y_advance = static_cast<int32_t>(rng());
        rp[i].x_offset = static_cast<int32_t>(rng());
        rp[i].y_offset = static_cast<int32_t>(rng());
        rp[i].var.u32 = rng();
        // Equal keys exercise stability. Keep comparator differences small:
        // upstream subtracts signed codepoints without overflow checks.
        ri[i].codepoint = (rng() % 5) + (mode == 2 ? 0x80000000u : 0);
        if (mode == 0) {
            int vals[] = {INT_MAX, INT_MIN, 1, -1, 0};
            rp[i].x_advance = vals[i % 5];
            rp[i].y_advance = vals[(i + 2) % 5];
            rp[i].x_offset = vals[(i + 3) % 5];
            rp[i].y_offset = vals[(i + 1) % 5];
        } else if (mode == 1) {
            rp[i].x_advance = int(rng() % 1001) - 500;
            rp[i].y_advance = int(rng() % 1001) - 500;
        }
    }
    vi = ri;
    vp = rp;
    normalize_glyphs_cluster_isolated(ri.data(), rp.data(), start, end, backward);
    normalize_glyphs_cluster_rvv(vi.data(), vp.data(), start, end, backward);
    if (std::memcmp(ri.data(), vi.data(), n * sizeof(ri[0])) ||
        std::memcmp(rp.data(), vp.data(), n * sizeof(rp[0]))) {
        std::fprintf(stderr, "glyph mismatch n=%u start=%u end=%u backward=%u mode=%u\n",
                     n, start, end, backward, mode);
        return false;
    }
    return true;
}

int main() {
    std::mt19937 rng(0x63bada55u);
    for (unsigned n : {1u, 2u, 3u, 4u, 8u, 15u, 16u, 17u, 32u, 33u,
                       64u, 65u, 127u, 128u, 129u, 256u}) {
        for (unsigned t = 0; t < 18; ++t) {
            const unsigned start = t % 3 == 0 ? 0 : rng() % n;
            const unsigned end = t % 3 == 0 ? n : start + 1 + rng() % (n - start);
            for (unsigned mode = 0; mode < 3; ++mode)
                for (bool backward : {false, true})
                    if (!check(n, start, end, backward, rng, mode)) return 1;
        }
    }
    std::puts("normalize_glyphs_cluster: seeded full-record differential OK");
}
