#include "kernel.h"
#include <cstdio>
#include <cstring>
#include <random>

void fe_mul_rvv(fe, const fe, const fe);
void fe_sq_rvv(fe, const fe);
int main() {
    constexpr unsigned seed = 0x22fe2026;
    std::mt19937 rng(seed);
    for (int trial = 0; trial < 5000; ++trial) {
        int32_t a[10], b[10], expected[10], actual[10];
        for (int i = 0; i < 10; ++i) {
            int bound = i & 1 ? 1 << 24 : 1 << 25;
            a[i] = trial == 0 ? 0 : trial == 1 ? bound : trial == 2 ? -bound :
                int(rng() % (2 * bound + 1)) - bound;
            b[i] = trial == 0 ? 0 : trial == 1 ? -bound : trial == 2 ? bound :
                int(rng() % (2 * bound + 1)) - bound;
        }
        for (int mode = 0; mode < 4; ++mode) {
            int32_t x[10], y[10];
            memcpy(x, a, sizeof(x)); memcpy(y, b, sizeof(y));
            if (mode == 0) { fe_mul_isolated(expected, x, y); fe_mul_rvv(actual, x, y); }
            if (mode == 1) { fe_sq_isolated(expected, x); fe_sq_rvv(actual, x); }
            if (mode == 2) { fe_mul_isolated(x, x, y); fe_mul_rvv(y, a, y); memcpy(expected, x, sizeof(x)); memcpy(actual, y, sizeof(y)); }
            if (mode == 3) { memcpy(y, x, sizeof(x)); fe_sq_isolated(x, x); fe_sq_rvv(y, y); memcpy(expected, x, sizeof(x)); memcpy(actual, y, sizeof(y)); }
            if (memcmp(expected, actual, sizeof(expected))) {
                for (int k = 0; k < 10; ++k) if (expected[k] != actual[k]) {
                    std::fprintf(stderr, "fe seed=%x trial=%d mode=%d limb=%d scalar=%d rvv=%d\n",
                                 seed, trial, mode, k, expected[k], actual[k]); break;
                }
                return 1;
            }
        }
    }
    std::printf("fe-mul/sq: 20000 exact cases (seed %x)\n", seed);
}
