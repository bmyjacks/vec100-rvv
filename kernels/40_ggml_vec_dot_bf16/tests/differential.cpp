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

static bool check(int n, const std::vector<ggml_bf16_t> &a,
                  const std::vector<ggml_bf16_t> &b) {
    float expected = -7.0f, actual = -7.0f;
    auto x = a, y = b;
    ggml_vec_dot_bf16_isolated(n, &expected, 13, x.data(), 17, y.data(), 19, 1);
    ggml_vec_dot_bf16_rvv(n, &actual, 13, x.data(), 17, y.data(), 19, 1);
    if (bits(expected) != bits(actual) ||
        std::memcmp(x.data(), a.data(), x.size() * sizeof(x[0])) ||
        std::memcmp(y.data(), b.data(), y.size() * sizeof(y[0]))) {
        std::fprintf(stderr, "bf16 n=%d scalar=%08x rvv=%08x\n", n,
                     bits(expected), bits(actual));
        return false;
    }
    return true;
}

int main() {
    constexpr unsigned seed = 0x42b16;
    std::mt19937 rng(seed);
    constexpr std::array<uint16_t, 19> special = {
        0x0000, 0x8000, 0x0001, 0x8001, 0x0080, 0x8080, 0x3f80,
        0xbf80, 0x3f81, 0x7f7f, 0xff7f, 0x7f80, 0xff80, 0x7fc1,
        0xffc1, 0x3eaa, 0xbeaa, 0x4000, 0xc000};
    unsigned cases = 0;
    for (int n : {-3, 0, 1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33,
                  63, 64, 65, 127, 128, 129, 257}) {
        for (unsigned pattern = 0; pattern < 48; ++pattern) {
            std::vector<ggml_bf16_t> x(static_cast<size_t>(n > 0 ? n : 0) + 4);
            auto y = x;
            for (size_t j = 0; j < x.size(); ++j) {
                x[j].bits = pattern % 3 == 0 ? special[(j + pattern) % special.size()]
                                           : static_cast<uint16_t>(rng());
                y[j].bits = pattern % 3 == 0 ? special[(j * 7 + pattern) % special.size()]
                                           : static_cast<uint16_t>(rng());
            }
            if (!check(n, x, y)) return 1;
            ++cases;
        }
    }
    // Small products beside a large one expose f32-lane accumulation instead
    // of the scalar double accumulator, even when every input is finite.
    for (int n : {3, 17, 129}) {
        std::vector<ggml_bf16_t> x(n, {0x3f80}), y(n, {0x3f80});
        x[0].bits = 0x4f00;
        x[1].bits = 0xcf00;
        if (!check(n, x, y)) return 1;
        ++cases;
    }
    std::printf("bf16: %u bit-exact cases (seed %x)\n", cases, seed);
}
