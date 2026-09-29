#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

uint32_t findPosFirstLast_c(const int16_t *, std::intptr_t, const uint16_t *);
uint32_t findPosFirstLast_c_rvv(const int16_t *, std::intptr_t, const uint16_t *);

static uint64_t seed = 0xa51c4d321736987bull;
static uint32_t random_word() {
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return static_cast<uint32_t>(seed);
}

static void check(const int16_t *coeff, int stride, const uint16_t *scan,
                  int mode) {
    const uint32_t expected = findPosFirstLast_c(coeff, stride, scan);
    const uint32_t actual = findPosFirstLast_c_rvv(coeff, stride, scan);
    bool any = false;
    for (int i = 0; i < 16; ++i)
        any |= coeff[(scan[i] / 4) * stride + scan[i] % 4] != 0;
    if ((any && actual != expected) || (!any && (actual & 255) != 16)) {
        std::fprintf(stderr, "findpos stride=%d mode=%d: %08x != %08x\n",
                     stride, mode, actual, expected);
        std::exit(1);
    }
}

int main() {
    uint16_t scan[16];
    for (int stride : {4, 8, 16, 32}) {
        std::vector<int16_t> coeff(stride * 4);
        for (int permutation = 0; permutation < 64; ++permutation) {
            for (int i = 0; i < 16; ++i) scan[i] = i;
            for (int i = 15; i > 0; --i) {
                const int j = random_word() % (i + 1);
                const uint16_t t = scan[i]; scan[i] = scan[j]; scan[j] = t;
            }
            for (int mode = 0; mode < 6; ++mode) {
                for (int pos = 0; pos < 16; ++pos) {
                    for (int16_t &v : coeff) v = 0;
                    if (mode == 0) { // all zero: only low byte has a contract
                    } else if (mode == 1) {
                        coeff[(scan[pos] / 4) * stride + scan[pos] % 4] = -32768;
                    } else if (mode == 2) {
                        coeff[(scan[0] / 4) * stride + scan[0] % 4] = -1;
                        coeff[(scan[15] / 4) * stride + scan[15] % 4] = 32767;
                    } else {
                        for (int j = 0; j < 16; ++j)
                            coeff[(scan[j] / 4) * stride + scan[j] % 4] =
                                mode == 3 ? static_cast<int16_t>(random_word()) :
                                mode == 4 ? (j & 1 ? -1 : 0) :
                                ((random_word() & 3) ? 0 :
                                 static_cast<int16_t>(random_word()));
                    }
                    check(coeff.data(), stride, scan, mode);
                }
            }
        }
    }
    // Both parameters are read-only, and signed/unsigned versions of the
    // same 16-bit element type may alias. No restrict assumption is needed.
    alignas(16) uint16_t shared[16];
    for (int i = 0; i < 16; ++i) shared[i] = i;
    check(reinterpret_cast<const int16_t *>(shared), 4, shared, 6);
    std::puts("findpos: OK");
}
