#include "kernel.h"

#include <cstdio>
#include <random>
#include <vector>

PCRE2_SPTR pcre2_match_start_bits_scan_rvv(PCRE2_SPTR, PCRE2_SPTR,
    PCRE2_SPTR, const uint8_t *, uint16_t, BOOL *);
PCRE2_SPTR pcre2_dfa_match_start_bits_scan_rvv(PCRE2_SPTR, PCRE2_SPTR,
    PCRE2_SPTR, const uint8_t *, uint32_t, BOOL *);

int main() {
    constexpr unsigned seed = 0x70c2e2;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (int n : {0, 1, 2, 3, 7, 15, 16, 17, 31, 32, 33, 127, 257})
    for (int trial = 0; trial < 80; ++trial) {
        std::vector<uint8_t> subject(n + 5), bits(32);
        for (auto &v : subject) v = static_cast<uint8_t>(rng());
        for (auto &v : bits) v = trial % 4 == 0 ? 0 :
                                 trial % 4 == 1 ? 255 : static_cast<uint8_t>(rng());
        for (bool present : {false, true})
        for (int mb : {0, n / 2, n, n + 1})
        for (uint32_t opt : {0u, PCRE2_PARTIAL_SOFT, PCRE2_PARTIAL_HARD, 0x100u}) {
            BOOL a = -1, b = -1;
            auto s = subject.data();
            const uint8_t *map = present ? bits.data() : nullptr;
            auto x = pcre2_match_start_bits_scan(s, s + n, s + mb, map,
                                                          static_cast<uint16_t>(opt), &a);
            auto y = pcre2_match_start_bits_scan_rvv(s, s + n, s + mb, map,
                                                   static_cast<uint16_t>(opt), &b);
            if (x != y || a != b) return std::fprintf(stderr, "match n=%d trial=%d\n", n, trial), 1;
            x = pcre2_dfa_match_start_bits_scan(s, s + n, s + mb, map, opt, &a);
            y = pcre2_dfa_match_start_bits_scan_rvv(s, s + n, s + mb, map, opt, &b);
            if (x != y || a != b) return std::fprintf(stderr, "dfa n=%d trial=%d\n", n, trial), 1;
            ++cases;
        }
    }
    std::printf("pcre2 scans: %u cases (seed %u)\n", cases, seed);
}
