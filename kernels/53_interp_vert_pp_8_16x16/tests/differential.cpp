#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

void interp_vert_pp_8_16x16_rvv(const pixel *, std::intptr_t, pixel *,
                                   std::intptr_t, int);

static bool check(int idx, intptr_t ss, intptr_t ds, intptr_t shift,
                  unsigned pattern, uint32_t seed) {
    constexpr intptr_t origin = 4096;
    std::mt19937 rng(seed);
    std::vector<pixel> reference(8192), actual;
    for (size_t i = 0; i < reference.size(); ++i) {
        switch (pattern) {
        case 0: reference[i] = 0; break;
        case 1: reference[i] = 255; break;
        case 2: reference[i] = (i & 1) ? 255 : 0; break;
        case 3: reference[i] = static_cast<pixel>(rng()); break;
        default: reference[i] = static_cast<pixel>(rng() % 7); break;
        }
    }
    actual = reference;
    interp_vert_pp_8_16x16(reference.data() + origin, ss,
                                   reference.data() + origin + shift, ds, idx);
    interp_vert_pp_8_16x16_rvv(actual.data() + origin, ss,
                            actual.data() + origin + shift, ds, idx);
    for (size_t i = 0; i < reference.size(); ++i) {
        if (reference[i] != actual[i]) {
            std::fprintf(stderr,
                "interp idx=%d ss=%ld ds=%ld shift=%ld pattern=%u seed=%u "
                "byte=%zu scalar=%u rvv=%u\n", idx, static_cast<long>(ss),
                static_cast<long>(ds), static_cast<long>(shift), pattern, seed,
                i, reference[i], actual[i]);
            return false;
        }
    }
    return true;
}

int main() {
    constexpr uint32_t seed = 0x54161608;
    unsigned cases = 0;
    for (int idx = 0; idx < 4; ++idx)
        for (intptr_t ss : {-23L, -16L, -1L, 0L, 1L, 16L, 23L})
            for (intptr_t ds : {-23L, -1L, 0L, 1L, 16L, 23L})
                for (intptr_t shift : {-800L, -64L, -16L, -1L, 0L,
                                        1L, 16L, 64L, 800L})
                    for (unsigned pattern = 0; pattern < 5; ++pattern) {
                        if (!check(idx, ss, ds, shift, pattern, seed + cases))
                            return 1;
                        ++cases;
                    }
    std::printf("interp_vert_pp_8_16x16: %u byte-exact cases (seed %u)\n",
                cases, seed);
}
