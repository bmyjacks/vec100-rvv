#include "kernel.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

void transpose_16_rvv(pixel *, const pixel *, intptr_t);

static bool compare(intptr_t stride, intptr_t shift, uint32_t seed) {
    constexpr intptr_t source_origin = 512;
    std::mt19937 rng(seed);
    std::vector<pixel> a(2048), b;
    for (pixel &p : a)
        p = static_cast<pixel>(rng());
    b = a;

    // An offset outside the source footprint gives an independent output;
    // offsets near the source exercise partial and complete overlap.
    pixel *const out_a = a.data() + source_origin + shift;
    pixel *const out_b = b.data() + source_origin + shift;
    const pixel *const in_a = a.data() + source_origin;
    const pixel *const in_b = b.data() + source_origin;
    transpose_16(out_a, in_a, stride);
    transpose_16_rvv(out_b, in_b, stride);
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            std::fprintf(
                stderr,
                "stride=%ld shift=%ld seed=%u at=%zu scalar=%u rvv=%u\n",
                static_cast<long>(stride), static_cast<long>(shift), seed, i,
                a[i], b[i]);
            return false;
        }
    }
    return true;
}

int main() {
    constexpr uint32_t seed = 0x8716001;
    unsigned cases = 0;
    for (intptr_t stride : {-31L, -17L, -1L, 0L, 1L, 2L, 16L, 17L, 31L}) {
        for (intptr_t shift :
             {-256L, -32L, -1L, 0L, 1L, 16L, 32L, 256L, 600L}) {
            for (unsigned trial = 0; trial < 6; ++trial) {
                if (!compare(stride, shift, seed + trial + cases))
                    return 1;
                ++cases;
            }
        }
    }
    std::printf("transpose_16: %u byte-exact cases (seed %u)\n", cases, seed);
}
