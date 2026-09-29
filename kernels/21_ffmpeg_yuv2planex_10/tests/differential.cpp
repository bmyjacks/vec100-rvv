#include "kernel.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

void yuv2planeX_10_c_template_rvv(const int16_t *, int, const int16_t **,
                                  uint16_t *, int, int, int);

static uint32_t state = 0x11110a55u;
static uint32_t next_random() {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

static void check(int width, int taps, int bits, int be, int mode) {
    constexpr int guard = 8;
    constexpr int extra = 11;
    constexpr std::array<int16_t, 10> boundaries = {
        0, 1, 16383, 16384, 32703, 32704, 32735, 32736, 32767, -32768};
    std::vector<std::vector<int16_t>> planes(taps, std::vector<int16_t>(width + 1));
    std::vector<const int16_t *> src(taps);
    std::vector<int16_t> filter(taps);
    for (int j = 0; j < taps; ++j) {
        filter[j] = mode == 0 ? 0 : mode == 1 ? 32767 :
                    mode == 2 ? -32768 : mode == 4 ? (bits == 14 ? 8192 : 4096) :
                    static_cast<int>(next_random() % 257) - 128;
        for (int i = 0; i <= width; ++i) {
            planes[j][i] = mode == 0 ? 0 : mode == 1 ? 32767 :
                           mode == 2 ? -32768 : mode == 4 ? boundaries[i % boundaries.size()] :
                           static_cast<int>(next_random() % 2049) - 1024;
        }
        src[j] = planes[j].data();
    }
    // Distinct destination storage, including untouched prefix, suffix and odd widths.
    std::vector<uint16_t> initial(guard + width + extra);
    for (auto &x : initial)
        x = static_cast<uint16_t>(next_random());
    auto expected = initial;
#ifndef RVV_ONLY
    auto scalar = initial;
#endif
    auto vector = initial;
    for (int i = 0; i < width; ++i) {
        const int shift = 27 - bits;
        int64_t sum = int64_t{1} << (shift - 1);
        for (int j = 0; j < taps; ++j)
            sum += static_cast<int64_t>(planes[j][i]) * filter[j];
        if (sum < INT32_MIN || sum > INT32_MAX) {
            std::fputs("invalid test: signed accumulation overflow\n", stderr);
            std::exit(1);
        }
        int64_t pixel = sum >> shift;
        if (pixel < 0) pixel = 0;
        if (pixel > (int64_t{1} << bits) - 1) pixel = (int64_t{1} << bits) - 1;
        uint16_t value = static_cast<uint16_t>(pixel);
        expected[guard + i] = be ? static_cast<uint16_t>((value << 8) | (value >> 8)) : value;
    }
    const int16_t **s = src.data();
#ifndef RVV_ONLY
    yuv2planeX_10_c_template(filter.data(), taps, s, scalar.data() + guard,
                             width, be, bits);
#endif
    yuv2planeX_10_c_template_rvv(filter.data(), taps, s, vector.data() + guard,
                                 width, be, bits);
#ifndef RVV_ONLY
    if (std::memcmp(scalar.data(), expected.data(), expected.size() * sizeof(uint16_t))) {
        std::fprintf(stderr, "scalar mismatch: w=%d taps=%d bits=%d be=%d mode=%d\n",
                     width, taps, bits, be, mode);
        std::exit(1);
    }
#endif
    if (std::memcmp(vector.data(), expected.data(), expected.size() * sizeof(uint16_t))) {
        std::fprintf(stderr, "RVV mismatch: w=%d taps=%d bits=%d be=%d mode=%d\n",
                     width, taps, bits, be, mode);
        std::exit(1);
    }
}

static void check_alias(int width, int bits, int be, int offset, bool filter_alias) {
    constexpr int guard = 8;
    std::vector<uint16_t> initial(guard + width + 12);
    for (auto &v : initial)
        v = static_cast<uint16_t>(next_random());
    // Keep the filter-alias input range small enough for int32 accumulation.
    std::vector<int16_t> independent(width + 12);
    for (auto &v : independent)
        v = static_cast<int>(next_random() % 61) - 30;
    const int16_t fixed_filter[2] = {200, -7};
    auto expected = initial;
#ifndef RVV_ONLY
    auto scalar = initial;
#endif
    auto vector = initial;
    auto run = [&](std::vector<uint16_t> &data, int kind) {
        const int16_t *f = filter_alias
            ? reinterpret_cast<const int16_t *>(data.data() + guard + 1)
            : fixed_filter;
        const int16_t *src[2] = {
            filter_alias ? independent.data()
                : reinterpret_cast<const int16_t *>(data.data() + guard + offset),
            independent.data() + 2};
        if (kind == 0) {
            for (int i = 0; i < width; ++i) {
                int64_t sum = int64_t{1} << (26 - bits);
                for (int j = 0; j < 2; ++j)
                    sum += static_cast<int64_t>(src[j][i]) * f[j];
                if (sum < INT32_MIN || sum > INT32_MAX) std::abort();
                int64_t pixel = sum >> (27 - bits);
                if (pixel < 0) pixel = 0;
                if (pixel > (int64_t{1} << bits) - 1)
                    pixel = (int64_t{1} << bits) - 1;
                uint16_t value = static_cast<uint16_t>(pixel);
                data[guard + i] = be
                    ? static_cast<uint16_t>((value << 8) | (value >> 8)) : value;
            }
        }
#ifndef RVV_ONLY
        else if (kind == 1)
            yuv2planeX_10_c_template(f, 2, src, data.data() + guard,
                                     width, be, bits);
#endif
        else
            yuv2planeX_10_c_template_rvv(f, 2, src, data.data() + guard,
                                         width, be, bits);
    };
    run(expected, 0);
#ifndef RVV_ONLY
    run(scalar, 1);
    if (scalar != expected) {
        std::fprintf(stderr, "scalar alias mismatch: width=%d bits=%d be=%d offset=%d filter=%d\n",
                     width, bits, be, offset, filter_alias);
        std::exit(1);
    }
#endif
    run(vector, 2);
    if (vector != expected) {
        std::fprintf(stderr, "RVV alias mismatch: width=%d bits=%d be=%d offset=%d filter=%d\n",
                     width, bits, be, offset, filter_alias);
        std::exit(1);
    }
}

int main() {
    constexpr std::array<int, 4> bits = {9, 10, 12, 14};
    constexpr std::array<int, 19> widths = {0, 1, 2, 3, 4, 7, 8, 9, 15, 16,
                                            17, 31, 32, 33, 63, 64, 65, 129, 257};
    int count = 0;
    for (int b : bits)
        for (int be = 0; be <= 1; ++be)
            for (int w : widths) {
                for (int taps : {0, 1, 2, 7, 12}) {
                    check(w, taps, b, be, 0); ++count;
                    check(w, taps, b, be, 3); ++count;
                }
                for (int mode : {1, 2}) {
                    check(w, 1, b, be, mode); ++count;
                }
                check(w, 1, b, be, 4); ++count;
            }
    for (int b : bits)
        for (int be = 0; be <= 1; ++be)
            for (int w : {1, 2, 7, 17, 33, 129}) {
                for (int offset : {-3, 0, 1, 4}) {
                    check_alias(w, b, be, offset, false); ++count;
                }
                check_alias(w, b, be, 0, true); ++count;
            }
    for (int n = 0; n < 600; ++n) {
        check(next_random() % 310, next_random() % 13, bits[next_random() % 4],
              next_random() & 1, 3);
        ++count;
    }
    std::printf("%d full-buffer cases passed (seed 0x11110a55)\n", count);
}
