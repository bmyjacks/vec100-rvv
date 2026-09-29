#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

size_t ia64_code_rvv(void *, uint32_t, bool, uint8_t *, size_t);

static void bit(std::vector<uint8_t> &data, size_t pos, bool value) {
    const uint8_t mask = uint8_t(1U << (pos % 8));
    if (value) data[pos / 8] |= mask;
    else data[pos / 8] &= uint8_t(~mask);
}

static bool check(size_t bundles, size_t extra, size_t offset, unsigned template_id,
                  unsigned pattern, uint32_t now, bool encode, uint32_t seed) {
    std::mt19937 rng(seed);
    const size_t length = bundles * 16 + extra;
    std::vector<uint8_t> reference(offset + length + 32);
    for (uint8_t &byte : reference) byte = uint8_t(rng());
    // Install branch candidates in every slot, including adjacent qualifying
    // slots with shared bytes; pattern 1/2 invalidates predicates and 3 keeps
    // completely random instructions to exercise sparse/no-op behavior.
    for (size_t b = 0; b < bundles; ++b) {
        const size_t start = (offset + b * 16) * 8;
        for (unsigned slot = 0; slot < 3; ++slot) {
            if (pattern == 3) continue;
            const size_t base = start + 5 + slot * 41;
            for (unsigned j = 0; j < 4; ++j)
                bit(reference, base + 37 + j, ((pattern == 1 ? 4U : 5U) >> j) & 1);
            for (unsigned j = 0; j < 3; ++j)
                bit(reference, base + 9 + j, pattern == 2 && j == 1);
            for (unsigned j = 0; j < 20; ++j)
                bit(reference, base + 13 + j, (rng() & 1) != 0);
            bit(reference, base + 36, (rng() & 1) != 0);
        }
        reference[offset + b * 16] = uint8_t(
            (reference[offset + b * 16] & ~uint8_t(31)) | template_id);
    }
    const std::vector<uint8_t> original = reference;
    std::vector<uint8_t> actual = reference;
    const size_t expected = ia64_code_isolated(nullptr, now, encode,
                                                       reference.data() + offset, length);
    const size_t got = ia64_code_rvv(nullptr, now, encode,
                                          actual.data() + offset, length);
    if (bundles && pattern == 0 && (template_id == 22 || template_id == 23) &&
        original == reference) {
        std::fprintf(stderr, "expected active branches seed=%u\n", seed);
        return false;
    }
    if (got != expected || reference != actual) {
        size_t bad = 0;
        while (bad < actual.size() && reference[bad] == actual[bad]) ++bad;
        std::fprintf(stderr, "seed=%u bundles=%zu extra=%zu offset=%zu template=%u "
                     "pattern=%u now=%08x encoder=%d return=%zu/%zu byte=%zu %02x/%02x\n",
                     seed, bundles, extra, offset, template_id, pattern, now,
                     encode, got, expected, bad,
                     bad < actual.size() ? actual[bad] : 0,
                     bad < reference.size() ? reference[bad] : 0);
        return false;
    }
    if (encode && !(now & 15) && (template_id == 22 || template_id == 23) &&
        pattern == 0) {
        const size_t decoded = ia64_code_rvv(nullptr, now, false,
                                                   actual.data() + offset, length);
        if (decoded != expected || actual != original) {
            std::fprintf(stderr, "roundtrip failed seed=%u now=%08x\n", seed, now);
            return false;
        }
    }
    return true;
}

int main() {
    constexpr uint32_t seed = 0x50a16450;
    unsigned cases = 0;
    for (unsigned template_id = 0; template_id < 32; ++template_id)
        for (unsigned pattern = 0; pattern < 4; ++pattern)
            for (bool encode : {false, true})
                for (size_t bundles : {size_t(0), size_t(1), size_t(2), size_t(3),
                                       size_t(4), size_t(5), size_t(9), size_t(33)}) {
                    const uint32_t now = (pattern == 0 || pattern == 3) ?
                                         0xfffffff0U : 0xabcdeff7U;
                    if (!check(bundles, (cases * 7) & 15, cases % 4, template_id,
                               pattern, now, encode, seed + cases)) return 1;
                    ++cases;
                }
    std::mt19937 rng(seed ^ 0x9876abcd);
    for (unsigned n = 0; n < 400; ++n) {
        if (!check(rng() % 129, rng() & 15, rng() & 7, rng() & 31,
                   rng() & 3, rng(), (rng() & 1) != 0, seed + cases)) return 1;
        ++cases;
    }
    std::printf("ia64_code: %u strict seeded differential cases (seed %u)\n", cases, seed);
}
