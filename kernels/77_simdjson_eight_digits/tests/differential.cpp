#include "kernel.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sys/mman.h>
#include <unistd.h>

namespace {
using simdjson::fallback::numberparsing::is_made_of_eight_digits_fast;
using simdjson::fallback::numberparsing::is_made_of_eight_digits_fast_rvv;

constexpr uint64_t seed = 0x121d16175eedULL;
size_t cases = 0;

void check(const uint8_t *p, bool expected, const char *label, size_t offset) {
    const bool scalar = is_made_of_eight_digits_fast(p);
    const bool vector = is_made_of_eight_digits_fast_rvv(p);
    ++cases;
    if (scalar != expected || vector != expected) {
        std::cerr << "eight-digits mismatch seed=" << seed << " case=" << cases
                  << " label=" << label << " offset=" << offset
                  << " expected=" << expected << " scalar=" << scalar
                  << " rvv=" << vector << '\n';
        std::exit(1);
    }
}

bool simple_oracle(const uint8_t *p) {
    for (size_t i = 0; i < 8; ++i)
        if (p[i] < '0' || p[i] > '9')
            return false;
    return true;
}

void check_bytes(const std::array<uint8_t, 8> &bytes, const char *label) {
    // All byte alignments modulo 8, with sentinels outside the 8-byte window.
    std::array<uint8_t, 32> storage;
    storage.fill(0xff);
    for (size_t offset = 0; offset < 16; ++offset) {
        for (size_t i = 0; i < 8; ++i)
            storage[offset + i] = bytes[i];
        check(storage.data() + offset, simple_oracle(bytes.data()), label,
              offset);
        storage[offset + 0] = 0xff;
    }
}

void guard_page_test() {
    const long page = sysconf(_SC_PAGESIZE);
    if (page < 8)
        std::exit(2);
    void *mem =
        mmap(nullptr, static_cast<size_t>(page) * 2, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED || mprotect(static_cast<uint8_t *>(mem) + page,
                                      static_cast<size_t>(page), PROT_NONE)) {
        std::cerr << "guard page setup failed\n";
        std::exit(2);
    }
    uint8_t *p = static_cast<uint8_t *>(mem) + page - 8;
    for (size_t i = 0; i < 8; ++i)
        p[i] = static_cast<uint8_t>('0' + i);
    check(p, true, "exactly-eight-readable", 0);
    p[7] = '/';
    check(p, false, "last-byte-invalid", 0);
    if (munmap(mem, static_cast<size_t>(page) * 2))
        std::exit(2);
}
} // namespace

int main() {
    std::array<uint8_t, 8> bytes{};
    bytes.fill('0');
    check_bytes(bytes, "all-zero-digits");
    bytes.fill('9');
    check_bytes(bytes, "all-nine-digits");
    for (size_t position = 0; position < 8; ++position) {
        bytes.fill('9');
        for (int value = 0; value < 256; ++value) {
            bytes[position] = static_cast<uint8_t>(value);
            check_bytes(bytes, "single-byte-exhaustive");
        }
    }
    // Logical input lengths 0..7, all with at least eight readable bytes:
    // the predicate still considers padding, including embedded NULs.
    for (size_t logical = 0; logical < 8; ++logical) {
        bytes.fill(0);
        for (size_t i = 0; i < logical; ++i)
            bytes[i] = '5';
        check_bytes(bytes, "partial-logical-input-with-NUL-padding");
        bytes.fill('0');
        check_bytes(bytes, "partial-logical-input-with-digit-padding");
    }
    std::mt19937_64 rng(seed);
    for (int trial = 0; trial < 4000; ++trial) {
        for (auto &byte : bytes)
            byte = static_cast<uint8_t>(rng());
        check_bytes(bytes, "random-all-bytes");
        for (auto &byte : bytes)
            byte = static_cast<uint8_t>('0' + rng() % 10);
        check_bytes(bytes, "random-digits");
    }
    guard_page_test();
    std::cout << "eight-digits OK seed=" << seed << " cases=" << cases << '\n';
}
