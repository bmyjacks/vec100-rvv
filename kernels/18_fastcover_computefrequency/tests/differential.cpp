#include "kernel.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

void FASTCOVER_computeFrequency_bench_rvv(U32 *, const BYTE *, size_t *,
                                             size_t, size_t, unsigned, unsigned,
                                             unsigned);

static constexpr unsigned seed = 0x21fac017;
static unsigned cases;

static void check(const std::vector<BYTE> &input, const std::vector<size_t> &offsets,
                  unsigned f, unsigned d, unsigned skip, size_t train,
                  unsigned mode, std::mt19937 &rng) {
    const size_t bins = size_t(1) << f;
    std::vector<U32> a(bins), b(bins);
    for (size_t j = 0; j < bins; ++j)
        a[j] = b[j] = rng() % 17 == 0 ? UINT32_MAX - (rng() & 3) : rng() & 7;
    std::vector<BYTE> sa = input, sb = input;
    std::vector<size_t> oa = offsets, ob = offsets;
    FASTCOVER_computeFrequency_bench_isolated(a.data(), sa.data(), oa.data(),
                                             train, offsets.size() - 1, f, d, skip);
    FASTCOVER_computeFrequency_bench_rvv(b.data(), sb.data(), ob.data(), train,
                                      offsets.size() - 1, f, d, skip);
    if (a != b || sa != sb || oa != ob) {
        for (size_t j = 0; j < bins; ++j)
            if (a[j] != b[j]) {
                std::fprintf(stderr, "seed=%x case=%u mode=%u f=%u d=%u skip=%u bin=%zu scalar=%u rvv=%u\n",
                             seed, cases, mode, f, d, skip, j, a[j], b[j]);
                break;
            }
        std::abort();
    }
    ++cases;
}

int main() {
    std::mt19937 rng(seed);
    for (unsigned f : {1u, 2u, 4u, 8u, 12u}) {
        for (unsigned d : {6u, 8u}) {
            for (unsigned skip : {0u, 1u, 3u, 9u}) {
                for (unsigned mode = 0; mode < 5; ++mode) {
                    std::vector<BYTE> input;
                    std::vector<size_t> offsets{0};
                    for (unsigned s = 0; s < 7; ++s) {
                        size_t length = mode == 0 ? s : (rng() % 77 + 7);
                        for (size_t j = 0; j < length; ++j)
                            input.push_back(mode == 1 ? 0 : mode == 2 ?
                                            static_cast<BYTE>(j % 2) :
                                            static_cast<BYTE>(rng()));
                        offsets.push_back(input.size());
                    }
                    check(input, offsets, f, d, skip, 5 + mode % 3, mode, rng);
                }
            }
        }
    }

    // samples alias freqs: updates to an earlier bin can change later hashes.
    for (unsigned d : {6u, 8u}) {
        for (unsigned f : {2u, 8u}) {
            const size_t bins = size_t(1) << f;
            std::vector<U32> a(bins + 80), b(bins + 80);
            for (size_t j = 0; j < a.size(); ++j)
                a[j] = b[j] = rng();
            size_t oa[] = {0, 20, 40, 60, 80, 100};
            size_t ob[] = {0, 20, 40, 60, 80, 100};
            FASTCOVER_computeFrequency_bench_isolated(a.data(),
                reinterpret_cast<BYTE *>(a.data()), oa, 5, 5, f, d, 0);
            FASTCOVER_computeFrequency_bench_rvv(b.data(),
                reinterpret_cast<BYTE *>(b.data()), ob, 5, 5, f, d, 0);
            if (a != b || !std::equal(oa, oa + 6, ob)) {
                std::fprintf(stderr, "seed=%x samples alias f=%u d=%u\n", seed, f, d);
                return 1;
            }
            ++cases;
        }
    }

    // offsets alias a separate region of freqs; bin zero is the only hash.
    // Identical bytes make it easy to keep the offset region outside that bin.
    {
        constexpr unsigned f = 8;
        std::vector<U32> a(256), b(256);
        size_t oa[] = {0, 12, 24, 36, 48, 60};
        const BYTE samples[60] = {};
        const size_t bin = ZSTD_hash8Ptr(samples, f);
        const size_t word = bin < 30 || bin >= 42 ? 30 : 60;
        std::memcpy(a.data() + word, oa, sizeof(oa));
        b = a;
        FASTCOVER_computeFrequency_bench_isolated(a.data(), samples,
            reinterpret_cast<size_t *>(a.data() + word), 5, 5, f, 8, 0);
        FASTCOVER_computeFrequency_bench_rvv(b.data(), samples,
            reinterpret_cast<size_t *>(b.data() + word), 5, 5, f, 8, 0);
        if (a != b) {
            std::fprintf(stderr, "seed=%x offsets alias\n", seed);
            return 1;
        }
        ++cases;
    }
    std::printf("fastcover: %u exact cases (seed %x)\n", cases, seed);
}
