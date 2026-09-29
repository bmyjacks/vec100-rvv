#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>

void encode_mcu_AC_first_prepare_rvv(const JCOEF *, const int *, int, int, UJCOEF *, size_t *);
int main() {
    constexpr unsigned seed = 0x19ac2026;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    const int lengths[] = {0, 1, 2, 7, 15, 16, 17, 31, 32, 33, 62, 63};
    for (int Al : {0, 1, 7, 14, 15}) for (int Sl : lengths)
        for (int trial = 0; trial < 120; ++trial) {
            JCOEF block[64];
            int order[64];
            for (int j = 0; j < 64; ++j) {
                order[j] = (trial % 3 == 0) ? j : rng() % 64;
                int v = trial % 7 == 0 ? (j % 2 ? -32768 : 32767) :
                    trial % 7 == 1 ? (j % 3 - 1) : int(rng() % 65536) - 32768;
                block[j] = static_cast<JCOEF>(v);
            }
            UJCOEF a[132], b[132];
            for (int j = 0; j < 132; ++j) a[j] = b[j] = rng();
            size_t ma[2] = {~size_t(0), 0x1234}, mb[2] = {ma[0], ma[1]};
            encode_mcu_AC_first_prepare_isolated(block, order, Sl, Al, a, ma);
            encode_mcu_AC_first_prepare_rvv(block, order, Sl, Al, b, mb);
            if (memcmp(a, b, sizeof(a)) || memcmp(ma, mb, sizeof(ma))) {
                std::fprintf(stderr, "jpeg seed=%x case=%u Sl=%d Al=%d trial=%d\n", seed, cases, Sl, Al, trial);
                return 1;
            }
            ++cases;
        }
    // Overlap of the block and output changes subsequent input coefficients.
    alignas(8) UJCOEF a[192], b[192];
    int order[64];
    for (int j = 0; j < 64; ++j) order[j] = j;
    for (int j = 0; j < 192; ++j) a[j] = b[j] = static_cast<UJCOEF>(rng());
    size_t ma, mb;
    encode_mcu_AC_first_prepare_isolated(reinterpret_cast<JCOEF *>(a), order, 63, 2, a, &ma);
    encode_mcu_AC_first_prepare_rvv(reinterpret_cast<JCOEF *>(b), order, 63, 2, b, &mb);
    if (memcmp(a, b, sizeof(a)) || ma != mb) return 2;
    std::printf("jpeg-ac: %u exact cases + overlap (seed %x)\n", cases, seed);
}
