#include "kernel.h"
#include <array>
#include <cstdio>
#include <random>
#include <vector>

int main() {
    constexpr uint32_t seed = 0x11326530;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (int size : {4, 8, 16, 32})
        for (uint32_t fs : {0u, 1u, 4u, 35u, 37u, 63u})
            for (std::intptr_t rs : {-39L, 0L, 1L, 37L})
                for (int overlap = 0; overlap < 3; ++overlap)
                    for (int kind = 0; kind < 5; ++kind) {
                        std::vector<pixel> a(4096), b;
                        for (auto &p : a)
                            p = kind == 0 ? 0 : kind == 1 ? 255
                                : kind == 2 ? static_cast<pixel>(rng() & 1 ? 0 : 255)
                                : static_cast<pixel>(rng());
                        b = a;
                        const size_t f = 2048, r = overlap ? 2048 + overlap - 1 : 3000;
                        uint64_t sa = 0xaabbccddu, da = 0x87654321u;
                        uint64_t sb = sa, db = da;
                        uint64_t *oa = &sa, *ob = &sb;
                        if (overlap == 2) { // outputs alias; later dc store wins
                            oa = &da;
                            ob = &db;
                        }
                        ssim_distortion_dc(a.data() + f, fs, a.data() + r, rs,
                                           size, oa, &da);
                        ssim_distortion_dc_rvv(b.data() + f, fs, b.data() + r, rs,
                                               size, ob, &db);
                        if (a != b || sa != sb || da != db) {
                            std::fprintf(stderr, "113 mismatch size=%d fs=%u rs=%ld overlap=%d kind=%d seed=%u\n",
                                         size, fs, long(rs), overlap, kind, seed);
                            return 1;
                        }
                        // An independent known-answer check for 4x4: only (0,0) is sampled.
                        if (size == 4 && fs == 37 && rs == 37 && overlap == 0) {
                            const int d = int(a[f]) - int(a[r]);
                            if (sa != uint64_t(d * d) || da != uint64_t(a[f]) * a[f])
                                return 2;
                        }
                        ++cases;
                    }
    std::printf("113: %u exact cases (seed 0x%x)\n", cases, seed);
}
