#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

void put_pixels8_8_c_rvv(uint8_t *, const uint8_t *, ptrdiff_t, int);

int main() {
    constexpr uint32_t seed = 0x7288001;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (int h : {-2, 0, 1, 2, 3, 7, 8, 9, 16, 17, 33})
        for (ptrdiff_t stride : {-17L, -9L, -8L, -4L, -1L, 0L, 1L,
                                 4L, 7L, 8L, 9L, 17L})
            for (int delta : {-256, -8, -4, 0, 4, 8, 12, 256})
                for (int trial = 0; trial < 3; ++trial) {
                    std::vector<uint8_t> a(4096), b;
                    for (auto &v : a)
                        v = static_cast<uint8_t>(rng());
                    b = a;
                    // Aligned destination for the scalar's pixel4 stores;
                    // the source is deliberately often not 4-byte aligned.
                    int in = 1024 + trial;
                    int out = 1024 + delta;
                    put_pixels8_8_c(a.data() + out, a.data() + in, stride, h);
                    put_pixels8_8_c_rvv(b.data() + out, b.data() + in, stride, h);
                    if (a != b) {
                        std::fprintf(stderr, "h=%d stride=%ld delta=%d trial=%d seed=%u\n",
                                     h, static_cast<long>(stride), delta, trial, seed);
                        return 1;
                    }
                    ++cases;
                }
    std::printf("put_pixels8_8_c: %u full-buffer cases (seed %u)\n", cases, seed);
}
