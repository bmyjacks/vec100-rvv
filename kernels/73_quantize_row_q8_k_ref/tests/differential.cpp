#include "kernel.h"

#include <array>
#include <cfenv>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <vector>

void quantize_row_q8_K_rvv(const float *, block_q8_K *, int64_t);

static bool check(const std::vector<float> &input, unsigned case_no) {
    const int64_t k = input.size();
    const size_t bytes = (k / QK_K) * sizeof(block_q8_K);
    std::vector<float> a = input, b = input;
    std::vector<uint8_t> scalar(bytes + 32, 0xa5), rvv(bytes + 32, 0xa5);
    quantize_row_q8_K_bench(a.data(), reinterpret_cast<block_q8_K *>(scalar.data()), k);
    quantize_row_q8_K_rvv(b.data(), reinterpret_cast<block_q8_K *>(rvv.data()), k);
    if (std::memcmp(a.data(), input.data(), k * sizeof(float)) ||
        std::memcmp(b.data(), input.data(), k * sizeof(float)) || scalar != rvv) {
        size_t at = 0;
        while (at < scalar.size() && scalar[at] == rvv[at]) ++at;
        std::fprintf(stderr, "q8 case=%u k=%lld byte=%zu scalar=%02x rvv=%02x\n",
                     case_no, static_cast<long long>(k), at,
                     at < scalar.size() ? scalar[at] : 0,
                     at < rvv.size() ? rvv[at] : 0);
        return false;
    }
    return true;
}

int main() {
    // Complete 256-float blocks only. Nonzero blocks containing NaN/Inf, or
    // values whose scaled form violates nearest_int's bound, are not valid.
    constexpr unsigned seed = 0x7500cafe;
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> value(-200.0f, 200.0f);
    unsigned cases = 0;
    for (size_t nb : {0u, 1u, 2u, 3u, 5u, 7u}) {
        std::vector<float> input(nb * QK_K, 0.f);
        if (!check(input, cases++)) return 1; // bsums must remain 0xa5
        for (float &f : input) f = -0.f;
        if (!check(input, cases++)) return 1;
        for (float &f : input) f = value(rng);
        if (!check(input, cases++)) return 1;
        for (size_t j = 0; j < input.size(); ++j)
            input[j] = j % 2 ? 127.f : -127.f;
        if (!check(input, cases++)) return 1;
        // All-NaN blocks take the reference's zero-block branch.
        for (float &f : input) f = std::numeric_limits<float>::quiet_NaN();
        if (!check(input, cases++)) return 1;
    }
    {
        std::vector<float> mixed(3 * QK_K, 0.f);
        for (int j = QK_K; j < 2 * QK_K; ++j)
            mixed[j] = j % 2 ? 127.f : -127.f;
        for (int j = 2 * QK_K; j < 3 * QK_K; ++j)
            mixed[j] = j % 2 ? std::numeric_limits<float>::quiet_NaN() : -0.f;
        if (!check(mixed, cases++)) return 1;
        mixed[QK_K + 3] = std::numeric_limits<float>::denorm_min();
        if (!check(mixed, cases++)) return 1;
    }
    const std::array<float, 12> edges = {
        0.f, -0.f, -127.f, 127.f, -126.5f, 126.5f,
        0.5f, -0.5f, 1.5f, -1.5f,
        std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    for (unsigned trial = 0; trial < 350; ++trial) {
        const size_t nb = 1 + rng() % 6;
        std::vector<float> input(nb * QK_K);
        for (float &f : input) f = trial % 4 == 0 ? edges[rng() % edges.size()]
                                                   : value(rng);
        if (trial % 7 == 0) {
            for (size_t b = 0; b < nb; ++b) {
                input[b * QK_K] = -127.f;
                input[b * QK_K + 1] = 127.f;
                input[b * QK_K + 2] = 0.5f;
            }
        }
        if (!check(input, cases++)) return 1;
    }
    {
        std::vector<float> ties(QK_K);
        for (size_t j = 0; j < ties.size(); ++j)
            ties[j] = edges[j % 10];
        for (int mode : {FE_TONEAREST, FE_DOWNWARD, FE_UPWARD, FE_TOWARDZERO}) {
            if (std::fesetround(mode) != 0) return 2;
            if (!check(ties, cases++)) return 1;
        }
        if (std::fesetround(FE_TONEAREST) != 0) return 2;
    }
    std::printf("q8: %u exact cases (seed %x)\n", cases, seed);
}
