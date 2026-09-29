#include "kernel.h"

#include <array>
#include <cfenv>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <vector>

void quantize_row_q4_0_rvv(const float *, block_q4_0 *, int64_t);

static bool check(const std::vector<float> &input, unsigned case_no) {
    const int64_t k = input.size();
    const size_t bytes = (k / QK4_0) * sizeof(block_q4_0);
    std::vector<float> a = input, b = input;
    std::vector<uint8_t> scalar(bytes + 32, 0xa5), rvv(bytes + 32, 0xa5);
    quantize_row_q4_0_bench(a.data(), scalar.data(), k);
    quantize_row_q4_0_rvv(b.data(), reinterpret_cast<block_q4_0 *>(rvv.data()), k);
    if (std::memcmp(a.data(), input.data(), k * sizeof(float)) ||
        std::memcmp(b.data(), input.data(), k * sizeof(float)) || scalar != rvv) {
        size_t at = 0;
        while (at < scalar.size() && scalar[at] == rvv[at]) ++at;
        std::fprintf(stderr, "q4 case=%u k=%lld byte=%zu scalar=%02x rvv=%02x\n",
                     case_no, static_cast<long long>(k), at,
                     at < scalar.size() ? scalar[at] : 0,
                     at < rvv.size() ? rvv[at] : 0);
        return false;
    }
    return true;
}

int main() {
    // The reference requires complete 32-float blocks. NaNs/infinities that
    // reach its float-to-int8 cast have undefined behavior and are not inputs.
    constexpr unsigned seed = 0x7400cafe;
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> value(-120.0f, 120.0f);
    unsigned cases = 0;
    for (size_t nb : {0u, 1u, 2u, 3u, 5u, 7u, 9u}) {
        std::vector<float> input(nb * QK4_0, 0.0f);
        if (!check(input, cases++)) return 1;
        for (float &f : input) f = -0.0f;
        if (!check(input, cases++)) return 1;
        for (float &f : input) f = value(rng);
        if (!check(input, cases++)) return 1;
        for (size_t j = 0; j < input.size(); ++j)
            input[j] = j % 2 ? 8.0f : -8.0f; // first signed max wins
        if (!check(input, cases++)) return 1;
    }
    {
        // d rounds to zero in float32, so the reciprocal is zero (valid conversion).
        std::vector<float> tiny(QK4_0, std::numeric_limits<float>::denorm_min());
        if (!check(tiny, cases++)) return 1;
        for (float &f : tiny) f = -std::numeric_limits<float>::denorm_min();
        if (!check(tiny, cases++)) return 1;
    }
    const std::array<float, 16> edges = {
        0.f, -0.f, -8.f, 8.f, -7.5f, 7.5f, -7.f, 7.f,
        6.5f, -6.5f, 7.499999f, -7.499999f,
        std::numeric_limits<float>::min(), -std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    for (unsigned trial = 0; trial < 500; ++trial) {
        const size_t nb = 1 + rng() % 10;
        std::vector<float> input(nb * QK4_0);
        for (float &f : input) f = trial % 4 == 0 ? edges[rng() % edges.size()]
                                                   : value(rng);
        // Explicit half-integer, nibble saturation, and zero-scale cases.
        if (trial % 7 == 0) {
            for (size_t b = 0; b < nb; ++b) {
                input[b * QK4_0] = -8.f;
                input[b * QK4_0 + 16] = 8.f;
            }
        }
        if (!check(input, cases++)) return 1;
    }
    {
        std::vector<float> ties(QK4_0);
        for (size_t j = 0; j < ties.size(); ++j)
            ties[j] = edges[j % 12];
        for (int mode : {FE_TONEAREST, FE_DOWNWARD, FE_UPWARD, FE_TOWARDZERO}) {
            if (std::fesetround(mode) != 0) return 2;
            if (!check(ties, cases++)) return 1;
        }
        if (std::fesetround(FE_TONEAREST) != 0) return 2;
    }
    std::printf("q4: %u exact cases (seed %x)\n", cases, seed);
}
