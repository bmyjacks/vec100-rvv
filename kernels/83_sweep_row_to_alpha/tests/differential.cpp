#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

extern "C" int32_t sweep_row_to_alpha_rvv(uint8_t *, int32_t *, int16_t *, unsigned, unsigned);

int main() {
    std::mt19937 rng(0x86a1f00d);
    for (unsigned n : {1u, 2u, 3u, 7u, 8u, 15u, 16u, 17u, 31u,
                       32u, 33u, 64u, 65u, 129u, 511u}) {
        for (unsigned trial = 0; trial < 140; ++trial) {
            std::vector<uint8_t> row(n + 12), row2;
            std::vector<int32_t> area(n + 12), area2;
            std::vector<int16_t> cover(n + 12), cover2;
            for (unsigned i = 0; i < row.size(); ++i) {
                row[i] = rng();
                area[i] = static_cast<int>(rng() % 260001) - 130000;
                cover[i] = static_cast<int>(rng() % 601) - 300;
            }
            row2 = row; area2 = area; cover2 = cover;
            unsigned begin = trial % 9;
            unsigned end = begin + n - 1;
            const auto a = sweep_row_to_alpha(row.data(), area.data(), cover.data(), begin, end);
            const auto b = sweep_row_to_alpha_rvv(row2.data(), area2.data(), cover2.data(), begin, end);
            if (a != b || row != row2 || area != area2 || cover != cover2) {
                std::fprintf(stderr, "sweep mismatch n=%u trial=%u begin=%u\n", n, trial, begin);
                std::abort();
            }
        }
    }
    std::puts("sweep_row_to_alpha: pass");
}
