#include "kernel.h"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using softmax = void (*)(float *, const float *, float, size_t);

// Local, independent ordered oracle for the RVV-only link. In particular,
// read the stored value back before adding it, even when dst overlaps src.
static void oracle(float *dst, const float *src, float beta, size_t n) {
    if (n == 0) return;
    float maximum = src[0];
    for (size_t i = 1; i < n; ++i) maximum = fmaxf(src[i], maximum);
    float total = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        dst[i] = expf(beta * (src[i] - maximum));
        total += dst[i];
    }
    const float inv = 1.0f / total;
    for (size_t i = 0; i < n; ++i) dst[i] *= inv;
}

static float from_bits(uint32_t b) {
    float x;
    std::memcpy(&x, &b, sizeof(x));
    return x;
}
static uint32_t bits(float x) {
    uint32_t b;
    std::memcpy(&b, &x, sizeof(b));
    return b;
}

static uint32_t rng = 0x73138e45u;
static uint32_t next() {
    rng ^= rng << 13;
    rng ^= rng >> 17;
    rng ^= rng << 5;
    return rng;
}

static void check(const std::vector<float> &initial, size_t n, int offset,
                  float beta, unsigned scenario) {
    const size_t src = n + 32;
    const size_t dst = static_cast<size_t>(static_cast<ptrdiff_t>(src) + offset);
    std::vector<float> expected = initial;
    oracle(expected.data() + dst, expected.data() + src, beta, n);
#ifndef STANDALONE
    std::vector<float> baseline = initial;
    sifive_skl::skl_softmax_f32_ref(baseline.data() + dst,
                                   baseline.data() + src, beta, n);
#endif
#ifndef SCALAR_ONLY
    std::vector<float> actual = initial;
    sifive_skl::skl_softmax_f32_rvv(actual.data() + dst,
                                   actual.data() + src, beta, n);
#endif
    for (size_t i = 0; i < initial.size(); ++i) {
#ifndef STANDALONE
        if (bits(expected[i]) != bits(baseline[i])) {
            std::fprintf(stderr, "reference mismatch n=%zu offset=%d beta=%08x case=%u at %zu: %08x vs %08x\n",
                         n, offset, bits(beta), scenario, i,
                         bits(expected[i]), bits(baseline[i]));
            std::exit(1);
        }
#endif
#ifndef SCALAR_ONLY
        if (bits(expected[i]) != bits(actual[i])) {
            std::fprintf(stderr, "RVV mismatch n=%zu offset=%d beta=%08x case=%u at %zu: %08x vs %08x\n",
                         n, offset, bits(beta), scenario, i,
                         bits(expected[i]), bits(actual[i]));
            std::exit(1);
        }
#endif
    }
}

int main() {
    // Test the no-access guard with genuinely invalid pointers as well.
#ifndef STANDALONE
    sifive_skl::skl_softmax_f32_ref(nullptr, nullptr, 1.0f, 0);
#endif
#ifndef SCALAR_ONLY
    sifive_skl::skl_softmax_f32_rvv(nullptr, nullptr, 1.0f, 0);
#endif
    const size_t lengths[] = {1, 2, 3, 4, 7, 8, 15, 16, 17, 31, 32,
                              33, 63, 64, 65, 127, 128, 129, 257, 513};
    const uint32_t special[] = {
        0u, 0x80000000u, 0x7f800000u, 0xff800000u,
        0x7fc00001u, 0xffc01234u, 0x7f800001u, 0x00000001u,
        0x80000001u, 0x3f800000u, 0xbf800000u, 0x7f7fffffu};
    const float betas[] = {1.0f, -0.0f, -1.0f, 0.125f, 200.0f,
                           from_bits(0x7f800000u)};
    unsigned cases = 0;
    for (size_t n : lengths) {
        const int offsets[] = {0, 1, 3, -1, -3, static_cast<int>(n) + 1,
                               -static_cast<int>(n) - 1};
        for (unsigned scenario = 0; scenario < 18; ++scenario) {
            std::vector<float> initial(3 * n + 128, from_bits(0x42123456u));
            for (size_t i = 0; i < n; ++i) {
                // Normal finite data; force zero, infinity and NaN cases,
                // including mixed signs/payloads and the first element.
                uint32_t b = (next() & 0x007fffffu) | 0x3e800000u;
                if (next() & 1) b |= 0x80000000u;
                if (scenario != 0 && ((i + scenario) % 7 == 0 || i == 0))
                    b = special[(i + scenario) % (sizeof(special) / sizeof(*special))];
                initial[n + 32 + i] = from_bits(b);
            }
            for (int offset : offsets) {
                check(initial, n, offset, betas[scenario % 6], scenario);
                ++cases;
            }
        }
    }
    std::printf("%u exact full-buffer cases OK\n", cases);
}
