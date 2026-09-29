#include "kernel.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

static uint32_t bits(float f) {
    uint32_t u;
    std::memcpy(&u, &f, sizeof u);
    return u;
}

static bool check(int n, const std::vector<ggml_fp16_t> &a,
                  const std::vector<ggml_fp16_t> &b) {
    float expected = -7.0f, actual = -7.0f;
    auto x = a, y = b;
    ggml_vec_dot_f16_isolated(n, &expected, 13, x.data(), 17, y.data(), 19, 1);
    ggml_vec_dot_f16_rvv(n, &actual, 13, x.data(), 17, y.data(), 19, 1);
    if (bits(expected) != bits(actual) || x != a || y != b) {
        std::fprintf(stderr, "f16 n=%d scalar=%08x rvv=%08x\n", n,
                     bits(expected), bits(actual));
        return false;
    }
    return true;
}

int main() {
    // The scalar isolated wrapper fills all 65,536 entries once, as in ggml_cpu_init.
    ggml_fp16_t zero = 0;
    float dummy;
    ggml_vec_dot_f16_isolated(0, &dummy, 0, &zero, 0, &zero, 0, 1);
    if (bits(ggml_table_f32_f16[0x3c00]) != bits(1.0f) ||
        bits(ggml_table_f32_f16[0x0001]) != bits(0x1p-24f) ||
        bits(ggml_table_f32_f16[0x8000]) != bits(-0.0f)) {
        std::fputs("f16 table initialization failed\n", stderr);
        return 1;
    }
    constexpr unsigned seed = 0x43f16;
    std::mt19937 rng(seed);
    constexpr std::array<uint16_t, 19> special = {
        0x0000, 0x8000, 0x0001, 0x8001, 0x0400, 0x8400, 0x3c00,
        0xbc00, 0x3c01, 0x7bff, 0xfbff, 0x7c00, 0xfc00, 0x7e01,
        0xfe01, 0x3555, 0xb555, 0x4000, 0xc000};
    unsigned cases = 0;
    for (int n : {-3, 0, 1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33,
                  63, 64, 65, 127, 128, 129, 257}) {
        for (unsigned pattern = 0; pattern < 48; ++pattern) {
            std::vector<ggml_fp16_t> x(static_cast<size_t>(n > 0 ? n : 0) + 4);
            auto y = x;
            for (size_t j = 0; j < x.size(); ++j) {
                x[j] = pattern % 3 == 0 ? special[(j + pattern) % special.size()]
                                        : static_cast<uint16_t>(rng());
                y[j] = pattern % 3 == 0 ? special[(j * 7 + pattern) % special.size()]
                                        : static_cast<uint16_t>(rng());
            }
            if (!check(n, x, y)) return 1;
            ++cases;
        }
    }
    for (int n : {3, 17, 129}) {
        std::vector<ggml_fp16_t> x(n, 0x3c00), y(n, 0x3c00);
        x[0] = 0x7000;
        x[1] = 0xf000;
        if (!check(n, x, y)) return 1;
        ++cases;
    }
    std::printf("f16: %u bit-exact cases (seed %x)\n", cases, seed);
}
