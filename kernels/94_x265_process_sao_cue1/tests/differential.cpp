#include "kernel.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

static std::mt19937 rng(0x13526586);

// Independent ordered model: read each value when visited, not from a
// snapshot, so aliases and overlapping rows affect subsequent iterations.
static void expected(std::vector<uint8_t> &a, size_t recAt, size_t signAt,
                     size_t offsetAt, intptr_t stride, int width) {
    for (int y = 0; y != 2; ++y) {
        for (int x = 0; x < width; ++x) {
            const size_t at = recAt + y * stride + x;
            const int cur = a[at];
            const int down = a[at + stride];
            const int s = (cur > down) - (cur < down);
            const int up = static_cast<int8_t>(a[signAt + x]);
            a[signAt + x] = static_cast<uint8_t>(-s);
            const int delta = static_cast<int8_t>(a[offsetAt + s + up + 2]);
            a[at] = static_cast<uint8_t>(
                std::clamp(int(a[at]) + delta, 0, 255));
        }
    }
}

static void check(const std::vector<uint8_t> &original, size_t recAt,
                  size_t signAt, size_t offsetAt, intptr_t stride, int width,
                  const char *label) {
    auto oracle = original;
    auto scalar = original;
    auto vector = original;
    expected(oracle, recAt, signAt, offsetAt, stride, width);
    process_sao_cue1_2rows(scalar.data() + recAt,
        reinterpret_cast<int8_t *>(scalar.data() + signAt),
        reinterpret_cast<int8_t *>(scalar.data() + offsetAt), stride, width);
    process_sao_cue1_2rows_rvv(vector.data() + recAt,
        reinterpret_cast<int8_t *>(vector.data() + signAt),
        reinterpret_cast<int8_t *>(vector.data() + offsetAt), stride, width);
    if (scalar != oracle || vector != oracle) {
        std::cerr << label << " width=" << width << " stride=" << stride
                  << " rec=" << recAt << " sign=" << signAt
                  << " offset=" << offsetAt << '\n';
        std::exit(1);
    }
}

int main() {
    const int widths[] = {0, 1, 2, 3, 4, 5, 15, 16, 17, 31, 32, 33,
                          63, 64, 65, 127, 129};
    for (int width : widths) {
        for (int gap : {0, 1, 7}) {
            const int stride = width + gap;
            for (int trial = 0; trial < 30; ++trial) {
                std::vector<uint8_t> a(1024);
                for (auto &b : a) b = static_cast<uint8_t>(rng());
                for (int i = 0; i < width; ++i)
                    a[650 + i] = static_cast<uint8_t>(int(rng() % 3) - 1);
                for (int i = 0; i < 5; ++i)
                    a[900 + i] = static_cast<uint8_t>(int(rng() % 49) - 24);
                if (trial % 5 == 0)
                    std::fill(a.begin() + 32, a.begin() + 32 + 2 * stride + width,
                              trial % 2 ? 255 : 0);
                check(a, 32, 650, 900, stride, width, "disjoint");
            }
        }
    }

    // Overlapping pixel rows: the previous write can affect the next read,
    // even within a row when the stride is smaller than width.
    for (int stride : {-19, -1, 0, 1, 4, 16}) {
        for (int width : {1, 3, 17, 37, 65}) {
            for (int trial = 0; trial < 20; ++trial) {
                std::vector<uint8_t> a(1024);
                for (auto &b : a) b = static_cast<uint8_t>(rng());
                for (int i = 0; i < width; ++i)
                    a[650 + i] = static_cast<uint8_t>(int(rng() % 3) - 1);
                for (int i = 0; i < 5; ++i)
                    a[900 + i] = static_cast<uint8_t>(int(rng() % 41) - 20);
                check(a, 128, 650, 900, stride, width, "row overlap");
            }
        }
    }

    for (int trial = 0; trial < 200; ++trial) {
        const int width = 1 + int(rng() % 97);
        const int stride = width + 3;
        std::vector<uint8_t> a(1024);
        for (auto &b : a) b = static_cast<uint8_t>(rng());
        for (int i = 0; i < width; ++i)
            a[650 + i] = static_cast<uint8_t>(int(rng() % 3) - 1);
        for (int i = 0; i < 5; ++i)
            a[900 + i] = static_cast<uint8_t>(int(rng() % 41) - 20);
        // Lookup table aliases pixel row 0 or 1, or the sign array.
        check(a, 32, 650, 32 + trial % width, stride, width, "offset/row0");
        check(a, 32, 650, 32 + stride, stride, width, "offset/row1");
        check(a, 32, 650, 32 + 2 * stride, stride, width, "offset/row2");
        check(a, 32, 650, 650, stride, width, "offset/sign");
        // The sign array is also a pixel row. Zero pixels and zero offsets
        // ensure all dynamically used edge indices remain in [0, 4].
        std::fill(a.begin() + 32, a.begin() + 32 + 2 * stride + width, 0);
        std::fill(a.begin() + 900, a.begin() + 905, 0);
        a[32] = 1; // sign store aliases current pixel before the clip read
        check(a, 32, 32, 900, stride, width, "sign/row0");
        a[32] = 0;
        check(a, 32, 32 + stride, 900, stride, width, "sign/row1");
        check(a, 32, 32 + 2 * stride, 900, stride, width, "sign/row2");
        check(a, 32, 33, 900, stride, width, "partial sign/row0");
        // Partial sign/offset alias with signs still valid for the first
        // access; later table writes are observable in scalar order.
        std::fill(a.begin() + 650, a.begin() + 650 + width + 5, 0);
        check(a, 32, 650, 651, stride, width, "partial sign/table");
    }
    std::cout << "seeded full-buffer scalar/RVV differential OK\n";
}
