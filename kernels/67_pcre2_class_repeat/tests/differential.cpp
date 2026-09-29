#include "kernel.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>

static constexpr uint32_t seed = 0x110c1a55;
static std::mt19937 rng(seed);
static unsigned cases = 0;

// Independent specification: first non-member of the selected class stays
// unread; a partial event only occurs when another iteration reaches end.
static ClassRunResult oracle(const uint8_t *data, size_t length, size_t start,
    uint32_t min, uint32_t max, ClassType type, const uint8_t *ctypes,
    int partial, size_t start_used, int allow_empty, int hitend) {
    size_t pos = start;
    for (uint32_t i = min; i < max; ++i) {
        if (pos == length) {
            if (partial && (pos > start_used || allow_empty)) {
                hitend = 1;
                if (partial > 1) return {pos, hitend, -2};
            }
            return {pos, hitend, 0};
        }
        const unsigned char c = data[pos];
        bool member;
        switch (type) {
        case OP_HSPACE: case OP_NOT_HSPACE:
            member = c == 9 || c == 32 || c == 160; break;
        case OP_VSPACE: case OP_NOT_VSPACE:
            member = c == 10 || c == 11 || c == 12 || c == 13 || c == 133;
            break;
        default:
            member = (ctypes[c] & (type <= OP_DIGIT ? 8 :
                type <= OP_WHITESPACE ? 1 : 16)) != 0;
        }
        const bool positive = (static_cast<int>(type) & 1) != 0;
        if (member != positive) return {pos, hitend, 0};
        ++pos;
    }
    return {pos, hitend, 0};
}

static void check(const std::array<uint8_t, 640> &input,
    const std::array<uint8_t, 256> &table, size_t length, size_t start,
    uint32_t min, uint32_t max, ClassType type, int partial,
    size_t start_used, int allow_empty, int hitend) {
    auto scalar_input = input, vector_input = input;
    auto scalar_table = table, vector_table = table;
    const auto want = oracle(input.data(), length, start, min, max, type,
        table.data(), partial, start_used, allow_empty, hitend);
    const auto scalar = pcre2_class_repeat(scalar_input.data(), length, start,
        min, max, type, scalar_table.data(), partial, start_used, allow_empty, hitend);
    const auto vector = pcre2_class_repeat_rvv(vector_input.data(), length, start,
        min, max, type, vector_table.data(), partial, start_used, allow_empty, hitend);
    if (scalar.offset != want.offset || scalar.hitend != want.hitend ||
        scalar.status != want.status || vector.offset != want.offset ||
        vector.hitend != want.hitend || vector.status != want.status ||
        scalar_input != input || vector_input != input ||
        scalar_table != table || vector_table != table) {
        std::fprintf(stderr, "seed=%08x case=%u type=%d len=%zu start=%zu "
            "min=%u max=%u partial=%d used=%zu empty=%d hit=%d "
            "want=(%zu,%d,%d) scalar=(%zu,%d,%d) rvv=(%zu,%d,%d)\n",
            seed, cases, type, length, start, min, max, partial,
            start_used, allow_empty, hitend,
            want.offset, want.hitend, want.status,
            scalar.offset, scalar.hitend, scalar.status,
            vector.offset, vector.hitend, vector.status);
        std::exit(1);
    }
    ++cases;
}

int main() {
    std::array<uint8_t, 640> data{};
    std::array<uint8_t, 256> table{};
    // Forced all-pass and first/last/mid-chunk failure for every class, both
    // polarities, partial modes and vector-length boundaries.
    for (int t = 0; t < 10; ++t) {
        const auto type = static_cast<ClassType>(t);
        data.fill(0);
        table.fill(0);
        for (int partial = 0; partial <= 2; ++partial)
            for (int empty = 0; empty <= 1; ++empty)
                for (int hit = 0; hit <= 1; ++hit)
                    check(data, table, 0, 0, 0, UINT32_MAX, type,
                        partial, 0, empty, hit);
        for (int v = 0; v < 256; ++v) {
            data.fill(static_cast<uint8_t>(v));
            table.fill(0);
            table[v] = 0x19;
            for (size_t length : {0u, 1u, 15u, 16u, 17u, 31u, 32u,
                                  33u, 63u, 64u, 65u, 129u}) {
                for (uint32_t max : {0u, 1u, 16u, 32u, 33u, 130u}) {
                    const uint32_t min = max ? 1 : 0;
                    check(data, table, length, 0, min, max, type, 2, 0, 0, 0);
                }
            }
        }
        for (unsigned n = 0; n < 2400; ++n) {
            for (auto &byte : data) byte = static_cast<uint8_t>(rng());
            for (auto &byte : table) byte = static_cast<uint8_t>(rng());
            const size_t length = rng() % 512;
            const size_t start = length ? rng() % (length + 1) : 0;
            const uint32_t min = rng() % 12;
            const uint32_t max = min + rng() % 280;
            const size_t used = rng() % (length + 2);
            check(data, table, length, start, min, max, type,
                rng() % 3, used, rng() & 1, rng() & 1);
        }
        // Construct long runs with failure at each VLEN boundary and with
        // non-zero start, independent of how the random table is populated.
        for (unsigned n = 0; n < 140; ++n) {
            data.fill(0x41);
            table.fill(0);
            table[0x41] = 0x19;
            const bool positive = t & 1;
            const uint8_t pass = t < 2 ? (positive ? 9 : 0x41) :
                t < 4 ? (positive ? 10 : 0x41) : (positive ? 0x41 : 0x42);
            const uint8_t fail = t < 2 ? (positive ? 0x41 : 9) :
                t < 4 ? (positive ? 0x41 : 10) : (positive ? 0x42 : 0x41);
            data.fill(pass);
            data[3 + n] = fail;
            check(data, table, 400, 3, 2, 300, type, 1, 2, 0, 0);
            if (n == 0) {
                data.fill(pass);
                check(data, table, 400, 3, 0, UINT32_MAX, type, 2, 0, 0, 0);
            }
        }
    }
    std::printf("PCRE2 class repeat: %u exact cases (seed %08x)\n", cases, seed);
}
