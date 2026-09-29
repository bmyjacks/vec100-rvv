#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

void intra_ang_horizontal_transpose_rvv(pixel *, std::intptr_t, const pixel *);

int main() {
    constexpr uint32_t seed = 0x55a1061;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (std::intptr_t stride : {-32L, -17L, -16L, -15L, -2L, -1L,
                                 0L, 1L, 2L, 15L, 16L, 17L, 32L})
        for (int delta : {-64, -32, -1, 0, 1, 16, 33, 48, 64, 512})
            for (int trial = 0; trial < 12; ++trial) {
                std::vector<pixel> a(4096), b;
                for (pixel &p : a)
                    p = trial == 0 ? 0 : trial == 1 ? 255 : static_cast<pixel>(rng());
                b = a;
                intra_ang_horizontal_transpose(a.data() + 1600 + delta,
                                                      stride, a.data() + 1600);
                intra_ang_horizontal_transpose_rvv(b.data() + 1600 + delta,
                                               stride, b.data() + 1600);
                if (a != b) {
                    std::fprintf(stderr, "stride=%ld delta=%d trial=%d seed=%u\n",
                                 static_cast<long>(stride), delta, trial, seed);
                    return 1;
                }
                ++cases;
            }
    std::printf("intra_ang_horizontal_transpose: %u full-buffer cases (seed %u)\n",
                cases, seed);
}
