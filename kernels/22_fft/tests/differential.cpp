#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

void fft_rvv(float *, int, float *);
static unsigned long long seed = 0x23bd29ef34cc5239ULL;
static unsigned rnd() {
    seed ^= seed << 13; seed ^= seed >> 7; seed ^= seed << 17;
    return (unsigned)seed;
}
int main() {
    for (int n : {1, 2, 3, 4, 5, 7, 8, 16, 25, 32, 50, 64, 100, 128, 200, 256, 400}) {
        for (int trial = 0; trial < 20; ++trial) {
            // Upstream fft recursively uses in+N and out+2*N as scratch.
            std::vector<float> a(4*n + 16), b(4*n + 16);
            std::vector<float> x(8*n + 16, -101), y(8*n + 16, -101);
            for (int i = 0; i < n; ++i) a[i] = (float)((int)(rnd() % 2001) - 1000) / 64;
            b = a;
            fft_isolated(a.data(), n, x.data());
            fft_rvv(b.data(), n, y.data());
            if (std::memcmp(x.data(), y.data(), x.size()*sizeof(float)) ||
                std::memcmp(a.data(), b.data(), a.size()*sizeof(float))) {
                std::fprintf(stderr, "fft n=%d trial=%d\n", n, trial);
                return 1;
            }
        }
    }
    std::puts("fft: OK");
}
