#include "kernel.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>

BOOL pcre2_009_class_intersect_rvv(const uint8_t *, const uint8_t *, BOOL);

constexpr uint32_t seed = 0x09c1a55u;
static std::mt19937 rng(seed);
static unsigned cases = 0;

static BOOL expected(const uint8_t *a, const uint8_t *b, BOOL invert) {
    for (unsigned i = 0; i < 32; ++i)
        if (a[i] & (invert ? ~b[i] : b[i]))
            return FALSE;
    return TRUE;
}

static void check(const std::array<uint8_t, 128> &bytes, size_t a, size_t b,
                  BOOL invert) {
    auto scalar_input = bytes;
    auto rvv_input = bytes;
    const BOOL want = expected(bytes.data() + a, bytes.data() + b, invert);
    const BOOL scalar = pcre2_009_class_intersect(
        scalar_input.data() + a, scalar_input.data() + b, invert);
    const BOOL rvv = pcre2_009_class_intersect_rvv(
        rvv_input.data() + a, rvv_input.data() + b, invert);
    if (scalar != want || rvv != want || scalar_input != bytes ||
        rvv_input != bytes) {
        std::fprintf(stderr,
                     "seed=%08x case=%u a=%zu b=%zu invert=%d "
                     "expected=%d scalar=%d rvv=%d input_modified=%d\n",
                     seed, cases, a, b, invert, want, scalar, rvv,
                     scalar_input != bytes || rvv_input != bytes);
        std::exit(1);
    }
    ++cases;
}

int main() {
    constexpr std::array<unsigned, 7> positions = {0, 1, 7, 15, 16, 30, 31};
    constexpr std::array<uint8_t, 8> bits = {1, 2, 4, 8, 16, 32, 64, 128};

    for (BOOL invert : {FALSE, TRUE, 2, -1}) {
        for (unsigned a : {0u, 1u, 3u}) {
            for (unsigned b : {64u, 65u, 67u, a, a + 1, a + 31}) {
                std::array<uint8_t, 128> bytes{};
                bytes.fill(0xa5);
                for (unsigned i = 0; i < 32; ++i) {
                    bytes[a + i] = 0;
                    bytes[b + i] = invert ? 0xff : 0;
                }
                // The oracle checks the resulting bytes for overlapping inputs.
                check(bytes, a, b, invert);

                for (unsigned pos : positions) {
                    for (uint8_t bit : bits) {
                        auto one = bytes;
                        one[a + pos] = bit;
                        one[b + pos] = invert ? 0 : bit;
                        check(one, a, b, invert);
                    }
                }

                bytes.fill(0xff);
                check(bytes, a, b, invert);
                bytes.fill(0);
                check(bytes, a, b, invert);
            }
        }
    }

    // All-zero, all-one, disjoint bitmaps, first/last hit, and high-bit
    // patterns; additional pseudorandom pairs and overlapping/identical views.
    for (unsigned n = 0; n < 12000; ++n) {
        std::array<uint8_t, 128> bytes;
        for (auto &c : bytes)
            c = static_cast<uint8_t>(rng());
        const size_t a = rng() % 8;
        const size_t b = n % 5 == 0   ? a
                         : n % 5 == 1 ? a + 1
                         : n % 5 == 2 ? a + 31
                                      : 64 + rng() % 8;
        const BOOL invert = n % 3 == 0 ? FALSE : TRUE;
        if (n % 7 == 0 || n % 7 == 1) {
            for (unsigned i = 0; i < 32; ++i) {
                bytes[a + i] = static_cast<uint8_t>(rng());
                const uint8_t other = static_cast<uint8_t>(~bytes[a + i]);
                bytes[b + i] = invert ? bytes[a + i] : other;
            }
        }
        check(bytes, a, b, invert);
    }

    std::printf(
        "pcre2 compare_opcodes: %u exact BOOL and unchanged-input cases "
        "(seed %08x)\n",
        cases, seed);
}
