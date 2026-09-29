#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

using simdjson::internal::decimal;
using simdjson::internal::number_of_digits_decimal_left_shift_isolated;
namespace simdjson { namespace internal {
uint32_t number_of_digits_decimal_left_shift_rvv(decimal &, uint32_t);
} }

static uint64_t state = 0x64d3c1907ab5e821ULL;
static uint32_t random32() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return (uint32_t)state;
}

static void check(const decimal &input, uint32_t shift) {
    decimal scalar = input;
    decimal vector = input;
    uint32_t a = number_of_digits_decimal_left_shift_isolated(scalar, shift);
    uint32_t b = simdjson::internal::number_of_digits_decimal_left_shift_rvv(vector, shift);
    if (a != b || std::memcmp(&scalar, &vector, sizeof(decimal)) != 0) {
        std::fprintf(stderr, "decimal shift=%u num_digits=%u result=%u/%u seed=0x64d3c1907ab5e821\n",
                     shift, input.num_digits, a, b);
        std::exit(1);
    }
}

int main() {
    decimal input{};
    // Independently generate the threshold 5^shift; its decimal digits are
    // concatenated in the upstream comparison table for shifts 1..60.
    uint8_t little[64] = {1};
    unsigned length = 1;
    for (unsigned shift = 0; shift < 64; ++shift) {
        if (shift != 0 && shift <= 60) {
            unsigned carry = 0;
            for (unsigned j = 0; j < length; ++j) {
                unsigned x = little[j] * 5u + carry;
                little[j] = x % 10;
                carry = x / 10;
            }
            while (carry) {
                little[length++] = carry % 10;
                carry /= 10;
            }
        }
        for (unsigned digits : {0u, 1u, 2u, 7u, 15u, 16u, 17u, 31u,
                                32u, 33u, 63u, 64u, 65u, 127u, 768u}) {
            input.num_digits = digits;
            input.decimal_point = 17;
            input.negative = shift & 1;
            input.truncated = shift & 2;
            for (unsigned i = 0; i < 768; ++i) input.digits[i] = (uint8_t)(random32() % 10);
            for (unsigned i = 0; i < length && i < digits; ++i)
                input.digits[i] = little[length - i - 1];
            check(input, shift);
            check(input, shift + 64 * 7);
            // A difference at every possible comparison position, on both
            // sides of the threshold, including chunk and tail boundaries.
            if (shift == 0 || shift > 60) continue;
            unsigned limit = digits < length ? digits : length;
            for (unsigned pos = 0; pos < limit; ++pos) {
                uint8_t original = input.digits[pos];
                if (original > 0) {
                    input.digits[pos] = original - 1;
                    check(input, shift);
                }
                if (original < 9) {
                    input.digits[pos] = original + 1;
                    check(input, shift);
                }
                input.digits[pos] = original;
            }
            for (unsigned repeat = 0; repeat < 8; ++repeat) {
                for (unsigned i = 0; i < digits; ++i)
                    input.digits[i] = (uint8_t)(random32() % 10);
                check(input, shift);
            }
        }
        // An exact threshold and each possible first mismatch, including
        // the last byte in a full comparison, independent of selected sizes.
        if (shift > 0 && shift <= 60) {
            input.num_digits = length;
            for (unsigned i = 0; i < length; ++i)
                input.digits[i] = little[length - i - 1];
            check(input, shift);
            for (unsigned pos = 0; pos < length; ++pos) {
                uint8_t original = input.digits[pos];
                if (original > 0) {
                    input.digits[pos] = original - 1;
                    check(input, shift);
                }
                if (original < 9) {
                    input.digits[pos] = original + 1;
                    check(input, shift);
                }
                input.digits[pos] = original;
            }
        }
    }
    std::puts("number_of_digits_decimal_left_shift: OK");
}
