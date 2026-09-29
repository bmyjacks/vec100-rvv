#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

void interp_hv_pp_8_16x16_rvv(const pixel *, std::intptr_t, pixel *,
                                 std::intptr_t, int, int);

static bool check(int x, int y, intptr_t ss, intptr_t ds, intptr_t shift,
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
    interp_hv_pp_8_16x16(reference.data() + origin, ss,
                                reference.data() + origin + shift, ds, x, y);
    interp_hv_pp_8_16x16_rvv(actual.data() + origin, ss,
                          actual.data() + origin + shift, ds, x, y);
    for (size_t i = 0; i < reference.size(); ++i) {
        if (reference[i] != actual[i]) {
            std::fprintf(stderr, "hv x=%d y=%d ss=%ld ds=%ld shift=%ld pattern=%u seed=%u byte=%zu scalar=%u rvv=%u\n",
                         x, y, static_cast<long>(ss), static_cast<long>(ds),
                         static_cast<long>(shift), pattern, seed, i,
                         reference[i], actual[i]);
            return false;
        }
    }
    return true;
}

int main() {
    constexpr uint32_t seed = 0x53161608;
    unsigned cases = 0;
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
            for (intptr_t ss : {-23L, -16L, -1L, 0L, 1L, 16L, 23L})
                for (intptr_t ds : {-23L, -1L, 0L, 1L, 16L, 23L})
                    for (intptr_t shift : {-800L, -16L, 0L, 1L, 800L})
                        for (unsigned pattern = 0; pattern < 5; ++pattern) {
                            if (!check(x, y, ss, ds, shift, pattern, seed + cases))
                                return 1;
                            ++cases;
                        }
    std::printf("interp_hv_pp_8_16x16: %u byte-exact cases (seed %u)\n", cases, seed);
}
