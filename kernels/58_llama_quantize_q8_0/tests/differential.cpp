#include "kernel.h"

#include <array>
#include <cfenv>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <vector>

static bool check(const std::vector<float> &input, unsigned case_no) {
    std::vector<block_q8_0> a(input.size() / QK8_0 + 1);
    std::vector<block_q8_0> b(a.size());
    std::memset(a.data(), 0xa5, a.size() * sizeof(block_q8_0));
    std::memset(b.data(), 0xa5, b.size() * sizeof(block_q8_0));
    quantize_row_q8_0_ref(input.data(), a.data(), input.size());
    quantize_row_q8_0_rvv(input.data(), b.data(), input.size());
    if (std::memcmp(a.data(), b.data(), a.size() * sizeof(block_q8_0))) {
        const auto *pa = reinterpret_cast<const unsigned char *>(a.data());
        const auto *pb = reinterpret_cast<const unsigned char *>(b.data());
        for (size_t j = 0; j < a.size() * sizeof(block_q8_0); ++j) {
            if (pa[j] != pb[j]) {
                std::fprintf(stderr, "q8 case=%u k=%zu byte=%zu %02x != %02x\n",
                             case_no, input.size(), j, pa[j], pb[j]);
                break;
            }
        }
        return false;
    }
    return true;
}

int main() {
    constexpr unsigned seed = 0x1170ca7e;
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> dist(-200.f, 200.f);
    const std::array<float, 14> edges = {
        0.f, -0.f, 127.f, -127.f, 126.5f, -126.5f, 0.5f, -0.5f,
        1.5f, -1.5f, std::numeric_limits<float>::denorm_min(),
        -std::numeric_limits<float>::denorm_min(),
        std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    unsigned cases = 0;
    for (int blocks : {0, 1, 2, 3, 5, 9}) {
        std::vector<float> input(blocks * QK8_0, 0.f);
        if (!check(input, cases++)) return 1;
        for (float &x : input) x = -0.f;
        if (!check(input, cases++)) return 1;
        for (float &x : input) x = dist(rng);
        if (!check(input, cases++)) return 1;
        for (size_t j = 0; j < input.size(); ++j) input[j] = edges[j % edges.size()];
        if (!check(input, cases++)) return 1;
    }
    for (unsigned trial = 0; trial < 700; ++trial) {
        std::vector<float> input((1 + rng() % 9) * QK8_0);
        for (float &x : input) x = trial % 3 ? dist(rng) : edges[rng() % edges.size()];
        if (trial % 17 == 0) {
            for (size_t j = 0; j < input.size(); j += QK8_0) {
                input[j] = -127.f;
                input[j + 1] = 127.f;
            }
        }
        if (!check(input, cases++)) return 1;
    }
    std::vector<float> ties(QK8_0);
    for (size_t j = 0; j < ties.size(); ++j) ties[j] = edges[j % 10];
    for (int mode : {FE_TONEAREST, FE_UPWARD, FE_DOWNWARD, FE_TOWARDZERO}) {
        if (std::fesetround(mode)) return 2;
        if (!check(ties, cases++)) return 1;
    }
    if (std::fesetround(FE_TONEAREST)) return 2;
    std::printf("q8: %u byte-exact cases (seed %x)\n", cases, seed);
}
