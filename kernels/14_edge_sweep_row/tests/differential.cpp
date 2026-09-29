#include "kernel.h"
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

extern "C" void edge_sweep_row_rvv(int32_t *, int16_t *, unsigned, int,
    int32_t, const hb_raster_edge_t &, unsigned &, unsigned &);

int main() {
    constexpr unsigned seed = 0x17ed2026;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (unsigned width : {1u, 2u, 7u, 16u, 17u, 31u, 65u, 257u}) {
        for (int trial = 0; trial < 1700; ++trial) {
            int org = trial % 3 == 0 ? -int(width / 2) : 0;
            int32_t top = trial % 101 == 0 ? INT32_MAX - 256 :
                          trial % 103 == 0 ? INT32_MIN : 0;
            int32_t y0 = top + (trial % 2 ? -16 : 0);
            int32_t y1 = top + (trial % 2 ? 256 : 160);
            // Keep endpoint interpolation inside the int64 upstream domain.
            if (top > INT32_MAX - 512) { y0 = top - 16; y1 = INT32_MAX; }
            if (top == INT32_MIN) { y0 = top; y1 = top + 256; }
            int32_t x = (int32_t)(int(rng() % (width + 20)) - 10 + org) * 256 + int(rng() % 256);
            int32_t delta = trial % 7 == 0 ? 0 : (int(rng() % (2 * width + 60)) - int(width + 30)) * 256;
            hb_raster_edge_t edge{x, y0, x + delta, y1,
                (int64_t(delta) << 16) / (y1 - y0), trial % 2 ? 1 : -1};
            std::vector<int32_t> a(width + 2), b(width + 2);
            std::vector<int16_t> c(width + 2), d(width + 2);
            for (unsigned j = 0; j < width + 2; ++j) {
                a[j] = b[j] = int(rng() % 20000) - 10000;
                c[j] = d[j] = static_cast<int16_t>(rng());
            }
            unsigned amin = width + 1, amax = 0, bmin = amin, bmax = amax;
            edge_sweep_row_isolated(a.data(), c.data(), width, org, top, edge, amin, amax);
            edge_sweep_row_rvv(b.data(), d.data(), width, org, top, edge, bmin, bmax);
            if (a != b || c != d || amin != bmin || amax != bmax) {
                std::fprintf(stderr, "edge seed=%x width=%u trial=%d x=%d delta=%d scalar-bounds=%u,%u rvv-bounds=%u,%u\n",
                             seed, width, trial, x, delta, amin, amax, bmin, bmax);
                return 1;
            }
            ++cases;
        }
    }
    // area and cover deliberately overlap; scalar order must be retained.
    alignas(4) int32_t a[70], b[70];
    for (int i = 0; i < 70; ++i) a[i] = b[i] = i * 157;
    hb_raster_edge_t e{5, 0, 15 * 256 + 7, 256,
        (int64_t(15 * 256 + 2) << 16) / 256, 1};
    unsigned amin = 70, amax = 0, bmin = amin, bmax = 0;
    edge_sweep_row_isolated(a, reinterpret_cast<int16_t *>(a), 32, 0, 0, e, amin, amax);
    edge_sweep_row_rvv(b, reinterpret_cast<int16_t *>(b), 32, 0, 0, e, bmin, bmax);
    if (memcmp(a, b, sizeof(a)) || amin != bmin || amax != bmax) return 2;
    // Both bounds may be the same reference; the scalar updates it per cell.
    unsigned boundA = 15, boundB = 15;
    edge_sweep_row_isolated(a, reinterpret_cast<int16_t *>(a + 40), 20, 0, 0,
                          e, boundA, boundA);
    edge_sweep_row_rvv(b, reinterpret_cast<int16_t *>(b + 40), 20, 0, 0,
                   e, boundB, boundB);
    if (memcmp(a, b, sizeof(a)) || boundA != boundB) return 3;
    std::printf("edge-sweep: %u exact cases + overlap (seed %x)\n", cases, seed);
}
