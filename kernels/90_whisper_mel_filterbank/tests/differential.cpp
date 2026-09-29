#include "kernel.h"

#include <array>
#include <cfenv>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <vector>

#ifdef CHECK_ORACLE
void whisper_mel_filterbank_projection_oracle(const float *, const float *,
                                              float *, int, int, int, int);
#endif

static bool check(int n_fft, int n_mel, int len, int i,
                  const std::vector<float> &fft, const std::vector<float> &filters,
                  unsigned case_no) {
    std::vector<float> a(n_mel * len + 8, -42.f);
    std::vector<float> b = a;
    whisper_mel_filterbank_projection(fft.data(), filters.data(), a.data(),
                                      n_fft, n_mel, len, i);
    whisper_mel_filterbank_projection_rvv(fft.data(), filters.data(), b.data(),
                                          n_fft, n_mel, len, i);
#ifdef CHECK_ORACLE
    std::vector<float> oracle(n_mel * len + 8, -42.f);
    whisper_mel_filterbank_projection_oracle(fft.data(), filters.data(),
                                              oracle.data(), n_fft, n_mel, len, i);
#endif
    const char *mismatch = nullptr;
    const float *expected = a.data();
    const float *actual = b.data();
    if (std::memcmp(a.data(), b.data(), a.size() * sizeof(float)))
        mismatch = "benchmark vs RVV";
#ifdef CHECK_ORACLE
    else if (std::memcmp(oracle.data(), a.data(), a.size() * sizeof(float))) {
        mismatch = "strict oracle vs benchmark";
        expected = oracle.data();
        actual = a.data();
    }
#endif
    if (mismatch) {
        for (size_t at = 0; at < a.size(); ++at) {
            if (std::memcmp(&expected[at], &actual[at], sizeof(float))) {
                uint32_t sa, va;
                std::memcpy(&sa, &expected[at], 4);
                std::memcpy(&va, &actual[at], 4);
                std::fprintf(stderr, "mel %s case=%u fft=%d mel=%d len=%d i=%d at=%zu %08x != %08x\n",
                             mismatch, case_no, n_fft, n_mel, len, i, at, sa, va);
                break;
            }
        }
        return false;
    }
    return true;
}

int main() {
    constexpr unsigned seed = 0x1160ca7e;
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> dist(0.f, 4.f);
    unsigned cases = 0;
    const std::array<int, 21> sizes = {0, 1, 2, 3, 4, 5, 7, 8, 9, 15, 16,
                                        17, 31, 32, 33, 63, 64, 65, 127, 128, 201};
    for (int n : sizes) {
        for (int m : {0, 1, 3, 7}) {
            std::vector<float> fft(n), filters(n * m);
            for (float &x : fft) x = dist(rng);
            for (float &x : filters) x = dist(rng);
            if (!check(n, m, 9, 5, fft, filters, cases++)) return 1;
            for (float &x : fft) x = 0.f;
            if (!check(n, m, 9, 0, fft, filters, cases++)) return 1;
            for (int k = 0; k < n; ++k) fft[k] = k % 2 ? -1.125f : 1.125f;
            for (float &x : filters) x = 1.f;
            if (!check(n, m, 9, 8, fft, filters, cases++)) return 1;
        }
    }
    for (int trial = 0; trial < 450; ++trial) {
        int n = rng() % 210, m = rng() % 9, len = 1 + rng() % 13;
        std::vector<float> fft(n), filters(n * m);
        for (float &x : fft) x = dist(rng);
        for (float &x : filters) x = dist(rng);
        if (trial % 7 == 0) {
            for (float &x : fft) x = (rng() % 2) ? -x : x;
        }
        if (!check(n, m, len, rng() % len, fft, filters, cases++)) return 1;
    }
    for (float special : {std::numeric_limits<float>::quiet_NaN(),
                          std::numeric_limits<float>::infinity(),
                          -std::numeric_limits<float>::infinity(), -0.f,
                          std::numeric_limits<float>::denorm_min()}) {
        std::vector<float> fft(33, 1.f), filters(99, 1.f);
        for (int pos : {0, 1, 3, 4, 31, 32}) {
            fft[pos] = special;
            if (!check(33, 3, 5, 2, fft, filters, cases++)) return 1;
            fft[pos] = 1.f;
            filters[pos] = special;
            if (!check(33, 3, 5, 2, fft, filters, cases++)) return 1;
            filters[pos] = 1.f;
        }
    }
    // Distinct quiet-NaN payloads and signs in the same four-term group.
    for (int n : {4, 17, 65, 201}) {
        std::vector<float> fft(n, 1.f), filters(n * 3, 1.f);
        const uint32_t nan_bits[] = {0x7fc00001u, 0xffc00012u, 0x7fc54321u};
        for (int placement = 0; placement < 4; ++placement) {
            for (int k = 0; k < n; ++k) fft[k] = 1.f;
            std::memcpy(&fft[placement], &nan_bits[placement % 3], 4);
            std::memcpy(&filters[1], &nan_bits[(placement + 1) % 3], 4);
            if (!check(n, 3, 7, 2, fft, filters, cases++)) return 1;
        }
    }
    // Cancellation across group boundaries can expose any change to the
    // widening FP64 reduction order, even when FP32 contraction is disabled.
    for (int n : {16, 33, 65, 128, 201}) {
        std::vector<float> fft(n, 0.f), filters(n * 3, 1.f);
        for (int offset : {0, 1, 2, 3}) {
            fft.assign(n, 0.f);
            fft[offset] = 0x1p100f;
            fft[offset + 4] = 1.f;
            fft[offset + 8] = -0x1p100f;
            fft[offset + 12] = 2.f;
            if (!check(n, 3, 5, 1, fft, filters, cases++)) return 1;
        }
    }
    for (int trial = 0; trial < 600; ++trial) {
        int n = 1 + rng() % 220, m = 1 + rng() % 5;
        std::vector<float> fft(n), filters(n * m);
        for (float &x : fft) {
            x = std::ldexp(1.f + static_cast<float>(rng() % 512) / 512.f,
                           static_cast<int>(rng() % 250) - 125);
            if (rng() % 2) x = -x;
        }
        for (float &x : filters) x = static_cast<float>(rng() % 512) / 512.f;
        if (!check(n, m, 11, 6, fft, filters, cases++)) return 1;
    }
    {
        std::vector<float> fft(65), filters(65 * 3);
        for (float &x : fft) x = dist(rng);
        for (float &x : filters) x = dist(rng);
        for (int mode : {FE_TONEAREST, FE_UPWARD, FE_DOWNWARD, FE_TOWARDZERO}) {
            if (std::fesetround(mode)) return 2;
            if (!check(65, 3, 7, 4, fft, filters, cases++)) return 1;
        }
        if (std::fesetround(FE_TONEAREST)) return 2;
    }
    std::printf("mel: %u bit-exact cases (seed %x)\n", cases, seed);
}
