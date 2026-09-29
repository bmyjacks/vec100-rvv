#include "kernel.h"

#include <cstdio>
#include <random>
#include <vector>

void pelFilterLuma_bench_rvv(pixel*, intptr_t, intptr_t, int32_t, int32_t, int32_t, int32_t, int32_t);

int main()
{
    constexpr unsigned seed = 0x12426581;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (int orientation = 0; orientation < 2; ++orientation)
    {
        const int step = orientation ? 1 : 32;
        const int off = orientation ? 32 : 1;
        std::vector<pixel> a(1024, 55);
        pixel* edge = a.data() + 400;
        for (int i = 0; i < UNIT_SIZE; ++i)
        {
            pixel* p = edge + i * step;
            p[-3 * off] = 70;
            p[-2 * off] = 80;
            p[-off] = 100;
            p[0] = 120;
            p[off] = 130;
            p[2 * off] = 140;
        }
        auto b = a;
        pelFilterLuma_bench(a.data() + 400, step, off, 2, -1, -1, -1, -1);
        pelFilterLuma_bench_rvv(b.data() + 400, step, off, 2, -1, -1, -1, -1);
        for (int i = 0; i < UNIT_SIZE; ++i)
        {
            pixel* p = a.data() + 400 + i * step;
            if (p[-2 * off] != 81 || p[-off] != 102 || p[0] != 118 || p[off] != 129)
            {
                std::fprintf(stderr, "unexpected upstream p1/p0/q0/q1 calculation\n");
                return 1;
            }
        }
        if (a != b) return 1;
        ++cases;

        // All four lanes fail the per-pixel threshold, so no store may occur.
        b = a;
        pelFilterLuma_bench_rvv(b.data() + 400, step, off, 0, -1, -1, -1, -1);
        if (a != b) return 1;
        ++cases;
    }
    // Positive, reverse, overlapping and degenerate steps, including both
    // native image orientations (step=1,offset=stride and vice versa).
    for (int step : {-9, -1, 0, 1, 2, 3, 4, 6, 8, 16, 32, 64})
    for (int off : {-5, -1, 0, 1, 2, 3, 4, 5, 8, 16, 32})
    for (int tc : {0, 1, 2, 4, 8, 16, 255, 32000, 32767, 40000})
    for (int masks = 0; masks < 20; ++masks)
    for (int trial = 0; trial < 3; ++trial)
    {
        int p = (masks & 1) ? -1 : 0;
        int q = (masks & 2) ? -1 : 0;
        int p1 = (masks & 4) ? -1 : 0;
        int q1 = (masks & 8) ? -1 : 0;
        if (masks == 16) p = 3;
        if (masks == 17) q = 7;
        if (masks == 18) p1 = 5, p = -1;
        if (masks == 19) q1 = 6, q = -1;
        std::vector<pixel> a(1024);
        for (auto& v : a)
            v = trial == 0 ? 0 : trial == 1 ? 255 : static_cast<pixel>(rng());
        auto b = a;
        pelFilterLuma_bench(a.data() + 400, step, off, tc, p, q, p1, q1);
        pelFilterLuma_bench_rvv(b.data() + 400, step, off, tc, p, q, p1, q1);
        if (a != b)
        {
            std::fprintf(stderr, "seed=%u step=%d off=%d tc=%d masks=%d,%d,%d,%d trial=%d\n",
                         seed, step, off, tc, p, q, p1, q1, trial);
            return 1;
        }
        ++cases;
    }
    std::printf("luma: %u exact full-buffer cases (seed %u)\n", cases, seed);
}
