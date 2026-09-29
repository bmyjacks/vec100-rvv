#include "kernel.h"

#include <cstdio>
#include <random>
#include <vector>

uint32_t quant_c_rvv(const int16_t *, const int32_t *, int32_t *, int16_t *, int, int, int);

int main() {
    constexpr unsigned seed = 0x73a11;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (int n : {0, 1, 7, 16, 17, 32, 48, 128, 256})
    for (int qbits : {8, 9, 13, 16, 20, 24, 29})
    for (int trial = 0; trial < 120; ++trial) {
        std::vector<int16_t> coef(n + 8), qa(n + 8, -123), qb = qa;
        std::vector<int32_t> coeff(n + 8), da(n + 8, -98765), db = da;
        const int add = trial % 3 == 0 ? 0 : trial % 3 == 1 ? (1 << (qbits - 1)) :
                        static_cast<int>(rng() % (1u << qbits));
        for (int i = 0; i < n; ++i) {
            coef[i] = trial % 4 == 0 ? INT16_MIN : trial % 4 == 1 ? INT16_MAX :
                      static_cast<int16_t>(rng());
            // Keep all signed intermediate arithmetic and left shifts defined.
            coeff[i] = static_cast<int32_t>(rng() % (1u << (qbits < 12 ? qbits : 12)));
            if (qbits < 16) coeff[i] %= 512;
        }
        const uint32_t a = quant_c_bench(coef.data(), coeff.data(), da.data(),
                                                 qa.data(), qbits, add, n);
        const uint32_t b = quant_c_rvv(coef.data(), coeff.data(), db.data(),
                                          qb.data(), qbits, add, n);
        if (a != b || da != db || qa != qb) {
            std::fprintf(stderr, "quant n=%d qbits=%d trial=%d seed=%u\n", n, qbits, trial, seed);
            return 1;
        }
        ++cases;
    }
    std::printf("quant: %u full-buffer cases (seed %u)\n", cases, seed);
}
