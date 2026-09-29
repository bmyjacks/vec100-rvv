#include "kernel.h"

#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

extern "C" void dequantize_row_q4_K_rvv(const block_q4_K *, float *, int64_t);

int main() {
    std::mt19937 rng(0x416ec3u);
    for (int blocks : {0, 1, 2, 5, 17}) {
        std::vector<block_q4_K> input(blocks);
        std::vector<float> expected(blocks * QK_K + 8, -777.f);
        std::vector<float> actual = expected;
        for (int trial = 0; trial < 128; ++trial) {
            for (auto &b : input) {
                // Normal/subnormal, positive/negative and signed zeros;
                // exceptional encodings are injected at deterministic trials.
                auto half = [&]() {
                    uint16_t h = uint16_t(rng());
                    return uint16_t((h & 0x8000) | ((h & 0x7c00) == 0x7c00
                                           ? (h & 0x03ff) : (h & 0x7fff)));
                };
                b.d = half(); b.dmin = half();
                if (trial % 16 == 0) b.d = 0x7c00; // +infinity
                if (trial % 16 == 1) b.dmin = 0xfc00; // -infinity
                if (trial % 16 == 2) b.d = 0x7e42; // quiet NaN
                if (trial % 16 == 3) b.dmin = 0x7d01; // signaling NaN
                for (auto &s : b.scales) s = uint8_t(rng());
                for (auto &q : b.qs) q = uint8_t(rng());
            }
            dequantize_row_q4_K(input.data(), expected.data(), blocks * QK_K);
            dequantize_row_q4_K_rvv(input.data(), actual.data(), blocks * QK_K);
            if (std::memcmp(expected.data(), actual.data(), expected.size() * sizeof(float))) {
                for (size_t i = 0; i < expected.size(); ++i)
                    if (std::memcmp(&expected[i], &actual[i], sizeof(float))) {
                        std::fprintf(stderr, "block=%d trial=%d lane=%zu expected=%a actual=%a\n",
                                     blocks, trial, i, expected[i], actual[i]);
                        return 1;
                    }
            }
        }
    }
    std::puts("q4_K bit-exact differential OK");
}
