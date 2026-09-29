#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

int satd_4x4_bench_rvv(const pixel *, intptr_t, const pixel *, intptr_t);

static bool check(intptr_t stride_a, intptr_t stride_b, intptr_t shift,
                  unsigned pattern, uint32_t seed) {
    constexpr intptr_t origin = 1024;
    std::mt19937 rng(seed);
    std::vector<pixel> data(2048);
    for (size_t i = 0; i < data.size(); ++i) {
        switch (pattern) {
        case 0: data[i] = 0; break;
        case 1: data[i] = 255; break;
        case 2: data[i] = (i & 1) ? 255 : 0; break;
        case 3: data[i] = static_cast<pixel>(i); break;
        default: data[i] = static_cast<pixel>(rng()); break;
        }
    }
    const pixel *a = data.data() + origin;
    const pixel *b = a + shift;
    const int expected = satd_4x4_bench(a, stride_a, b, stride_b);
    const int got = satd_4x4_bench_rvv(a, stride_a, b, stride_b);
    if (got != expected) {
        std::fprintf(stderr,
            "satd sa=%ld sb=%ld shift=%ld pattern=%u seed=%u scalar=%d rvv=%d\n",
            static_cast<long>(stride_a), static_cast<long>(stride_b),
            static_cast<long>(shift), pattern, seed, expected, got);
        return false;
    }
    return true;
}

int main() {
    constexpr uint32_t seed = 0x78040478;
    pixel zero[16] = {};
    pixel full[16];
    for (pixel &p : full) p = 255;
    if (satd_4x4_bench_rvv(zero, 4, zero, 4) != 0 ||
        satd_4x4_bench_rvv(full, 4, zero, 4) != 2040)
        return 1;
    unsigned cases = 0;
    for (intptr_t sa : {-19L, -4L, -1L, 0L, 1L, 4L, 19L})
        for (intptr_t sb : {-19L, -4L, -1L, 0L, 1L, 4L, 19L})
            for (intptr_t shift : {-256L, -4L, -1L, 0L, 1L, 4L, 256L})
                for (unsigned pattern = 0; pattern < 5; ++pattern) {
                    if (!check(sa, sb, shift, pattern, seed + cases)) return 1;
                    ++cases;
                }
    std::printf("satd_4x4: %u differential cases (seed %u)\n", cases, seed);
}
