#include "kernel.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {
constexpr uint64_t seed = 0x5a850065eedULL;
size_t cases = 0;

void check(std::vector<unsigned char> input, const std::string *separator,
           size_t in_pos = 16, size_t out_pos = 2048,
           size_t sep_pos = 0) {
    std::vector<unsigned char> scalar(8192, 0xa5);
    std::copy(input.begin(), input.end(), scalar.begin() + in_pos);
    if (separator) {
        if (!sep_pos)
            sep_pos = 4096;
        std::copy(separator->begin(), separator->end(), scalar.begin() + sep_pos);
        scalar[sep_pos + separator->size()] = 0;
    }
    std::vector<unsigned char> rvv = scalar;
    auto *a_sep = separator ? reinterpret_cast<char *>(scalar.data() + sep_pos)
                            : nullptr;
    auto *b_sep = separator ? reinterpret_cast<char *>(rvv.data() + sep_pos)
                            : nullptr;
    char *a_out = reinterpret_cast<char *>(scalar.data() + out_pos);
    char *b_out = reinterpret_cast<char *>(rvv.data() + out_pos);
    const ptrdiff_t a_len =
        sqlite_to_base85(scalar.data() + in_pos, static_cast<int>(input.size()),
                         a_out, a_sep) -
        a_out;
    const ptrdiff_t b_len =
        sqlite_to_base85_rvv(rvv.data() + in_pos,
                             static_cast<int>(input.size()), b_out, b_sep) -
        b_out;
    ++cases;
    if (a_len != b_len || scalar != rvv) {
        std::cerr << "base85 mismatch seed=" << seed << " case=" << cases
                  << " n=" << input.size() << " sep="
                  << (separator ? *separator : "<null>") << " in=" << in_pos
                  << " out=" << out_pos << " sep_pos=" << sep_pos << '\n';
        std::exit(1);
    }
}
} // namespace

int main() {
    const std::string newline = "\n", empty, multi = "::\r\n", long_sep(17, 'x');
    const std::string *seps[] = {nullptr, &empty, &newline, &multi, &long_sep};
    for (size_t n : {0u, 1u, 2u, 3u, 4u, 5u, 7u, 15u, 16u, 31u, 32u,
                     60u, 61u, 62u, 63u, 64u, 65u, 66u, 67u, 68u, 69u,
                     124u, 127u, 128u, 129u, 255u, 256u, 257u, 511u}) {
        for (unsigned char value : {0u, 0x55u, 0x80u, 0xffu}) {
            for (const auto *sep : seps)
                check(std::vector<unsigned char>(n, value), sep);
        }
    }
    std::mt19937_64 rng(seed);
    for (int trial = 0; trial < 350; ++trial) {
        const size_t n = rng() % 1100;
        std::vector<unsigned char> input(n);
        for (auto &c : input)
            c = static_cast<unsigned char>(rng());
        std::string dynamic;
        const size_t sep_len = rng() % 25;
        for (size_t i = 0; i < sep_len; ++i)
            dynamic += static_cast<char>('!' + rng() % 90);
        check(input, &dynamic);
        if (trial % 11 == 0) {
            check(input, nullptr, 100, 100); // in-place and overlapping writes
            check(input, &newline, 103, 100);
        }
    }
    // A separator inside the output is harmless when the input is empty.
    check({}, &multi, 16, 2048, 2048);
    std::cout << "base85 differential OK seed=" << seed << " cases=" << cases
              << '\n';
}
