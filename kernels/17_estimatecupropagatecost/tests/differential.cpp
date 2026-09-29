#include "kernel.h"
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

void estimateCUPropagateCost_bench_rvv(int *, const uint16_t *, const int32_t *,
                                           const uint16_t *, const int32_t *, const double *, int);
int main() {
    constexpr unsigned seed = 0x20c05726;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (int len : {0, 1, 2, 3, 4, 7, 8, 15, 16, 17, 31, 33, 65, 257})
        for (int trial = 0; trial < 150; ++trial) {
            std::vector<int> a(len + 3, 0x55555555), b = a;
            std::vector<int32_t> intra(len), q(len);
            std::vector<uint16_t> prop(len), inter(len);
            for (int i = 0; i < len; ++i) {
                intra[i] = trial % 7 == 0 ? 1 : 1 + rng() % 16383;
                q[i] = trial % 9 == 0 ? 0 : int(rng() % 512);
                prop[i] = trial % 11 == 0 ? 65535 : rng();
                inter[i] = trial % 5 == 0 ? intra[i] : rng();
            }
            double fps = trial % 5 == 0 ? 0 : trial % 5 == 1 ? 2.56 :
                         trial % 5 == 2 ? 256 : (rng() % 255 + 1) * 0.5;
            estimateCUPropagateCost_bench_isolated(a.data(), prop.data(), intra.data(), inter.data(), q.data(), &fps, len);
            estimateCUPropagateCost_bench_rvv(b.data(), prop.data(), intra.data(), inter.data(), q.data(), &fps, len);
            if (a != b) {
                for (int i = 0; i < len; ++i) if (a[i] != b[i]) {
                    std::fprintf(stderr, "propagate seed=%x len=%d trial=%d lane=%d scalar=%d rvv=%d\n",
                                 seed, len, trial, i, a[i], b[i]); break;
                }
                return 1;
            }
            ++cases;
        }
    // dst aliases intraCosts, including both exact and shifted overlap.
    for (int shift : {0, 1}) {
        int32_t a[36], b[36], q[31];
        uint16_t prop[31], inter[31];
        for (int i = 0; i < 36; ++i) a[i] = b[i] = 20 + i;
        for (int i = 0; i < 31; ++i) { q[i] = 128; prop[i] = 77; inter[i] = 11; }
        double fps = 128;
        estimateCUPropagateCost_bench_isolated(a + shift, prop, a, inter, q, &fps, 31);
        estimateCUPropagateCost_bench_rvv(b + shift, prop, b, inter, q, &fps, 31);
        if (memcmp(a, b, sizeof(a))) return 2;
        ++cases;
    }
    std::printf("propagate: %u exact cases (seed %x)\n", cases, seed);
}
