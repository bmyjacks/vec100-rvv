#include "kernel.h"

#include <cstdio>
#include <random>
#include <vector>

int main() {
    constexpr unsigned seed = 0x30a11u;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (size_t length :
         {0UL, 1UL, 2UL, 3UL, 7UL, 8UL, 9UL, 15UL, 16UL, 17UL, 31UL, 32UL, 33UL,
          63UL, 64UL, 65UL, 127UL, 128UL, 129UL}) {
        for (unsigned trial = 0; trial < 100; ++trial) {
            std::vector<gchar> a(length + 17, static_cast<gchar>(0x55));
            for (size_t i = 0; i < length; ++i)
                a[i + 8] = static_cast<gchar>(1 + rng() % 255);
            a[length + 8] = 0;
            std::vector<gchar> b = a;
            if (g_strreverse_isolated(a.data() + 8) != a.data() + 8 ||
                g_strreverse_rvv(b.data() + 8) != b.data() + 8 || a != b) {
                std::fprintf(stderr, "length=%zu trial=%u seed=%x\n", length,
                             trial, seed);
                return 1;
            }
            ++cases;
        }
    }
    std::printf("g_strreverse: %u exact randomized cases (seed %x)\n", cases,
                seed);
}
