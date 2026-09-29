#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

void partialButterfly8_rvv(const int16_t *, int16_t *, int, int);

int main() {
    constexpr uint32_t seed = 0x69bc801;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (int line : {0, 1, 2, 3, 4, 7, 8, 9, 15, 16, 17, 31, 33})
        for (int shift : {1, 2, 6, 7, 12, 15, 30, 31})
            for (int delta : {-32, -8, -1, 0, 1, 8, 32, 600})
                for (int trial = 0; trial < 3; ++trial) {
                    std::vector<int16_t> a(2048), b;
                    for (auto &v : a)
                        v = trial == 0 ? (cases % 2 ? INT16_MIN : INT16_MAX)
                                       : static_cast<int16_t>(rng());
                    b = a;
                    partialButterfly8_bench(a.data() + 512, a.data() + 512 + delta,
                                                   shift, line);
                    partialButterfly8_rvv(b.data() + 512, b.data() + 512 + delta,
                                             shift, line);
                    if (a != b) {
                        std::fprintf(stderr, "line=%d shift=%d delta=%d trial=%d seed=%u\n",
                                     line, shift, delta, trial, seed);
                        return 1;
                    }
                    ++cases;
                }
    std::printf("partialButterfly8: %u full-buffer cases (seed %u)\n", cases, seed);
}
