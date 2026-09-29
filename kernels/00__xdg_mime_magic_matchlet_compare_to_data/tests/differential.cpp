#include "kernel.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

int xdg_mime_magic_matchlet_compare_to_data_rvv(XdgMimeMagicMatchlet *,
                                                const void *, size_t);
#ifndef RVV_STANDALONE
int xdg_mime_magic_matchlet_compare_to_data_isolated(XdgMimeMagicMatchlet *,
                                                     const void *, size_t);
#endif

static uint32_t seed = 0x00c0ffee;
static uint32_t random_word() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

static void check(unsigned int offset, unsigned int range, unsigned int length,
                  size_t data_len, bool masked, int match_at, int alignment) {
    std::vector<unsigned char> data(data_len + alignment + 1);
    std::vector<unsigned char> value(length + 1), mask(length + 1);
    for (auto &b : data) b = static_cast<unsigned char>(random_word());
    for (auto &b : value) b = static_cast<unsigned char>(random_word());
    for (auto &b : mask) b = static_cast<unsigned char>(random_word());
    auto *bytes = data.data() + alignment;
    if (match_at >= 0 && static_cast<size_t>(match_at) + length <= data_len) {
        for (unsigned int j = 0; j < length; ++j)
            bytes[match_at + j] = value[j];
    }
    XdgMimeMagicMatchlet m{};
    m.offset = static_cast<int>(offset);
    m.range_length = range;
    m.value_length = length;
    m.value = value.data();
    m.mask = masked ? mask.data() : nullptr;
#ifndef RVV_STANDALONE
    const int expected =
        xdg_mime_magic_matchlet_compare_to_data_isolated(&m, bytes, data_len);
#endif
    const int actual = xdg_mime_magic_matchlet_compare_to_data_rvv(&m, bytes, data_len);
#ifndef RVV_STANDALONE
    if (actual != expected) {
        std::fprintf(stderr, "seed=0x00c0ffee offset=%u range=%u length=%u len=%zu mask=%d match=%d align=%d: %d != %d\n",
                     offset, range, length, data_len, masked, match_at, alignment, actual, expected);
        std::exit(1);
    }
#else
    if (actual != 0 && actual != 1) std::abort();
#endif
}

int main() {
    for (bool masked : {false, true}) {
        for (unsigned int length : {0u, 1u, 2u, 15u, 16u, 17u, 31u, 32u, 33u, 127u, 257u})
            for (unsigned int range : {0u, 1u, 3u, 19u})
                for (int alignment : {0, 1, 3}) {
                    const size_t n = 300;
                    check(2, range, length, n, masked, -1, alignment);
                    check(2, range, length, n, masked, 2, alignment);
                    check(2, range, length, n, masked, 2 + static_cast<int>(range / 2), alignment);
                    check(2, range, length, n, masked, 2 + static_cast<int>(range), alignment);
                    check(2, range, length, length ? length - 1 : 0, masked, -1, alignment);
                }
        // Empty windows and unsigned start/end conversion, with no data reads.
        check(0xffffffffu, 0, 0, 0, masked, -1, 0);
        check(0xffffffffu, 1, 0, 0, masked, -1, 0);
        for (int trial = 0; trial < 2000; ++trial) {
            const unsigned int length = random_word() % 300;
            const unsigned int range = random_word() % 40;
            const unsigned int offset = random_word() % 20;
            const size_t n = random_word() % 340;
            const int at = random_word() & 1 ? static_cast<int>(offset + random_word() % (range + 1)) : -1;
            check(offset, range, length, n, masked, at, random_word() % 4);
        }
        // The interface has no non-alias promise: value and mask may each be
        // views of the input, including overlapping and unaligned views.
        for (unsigned length : {1u, 15u, 16u, 17u, 65u}) {
            std::vector<unsigned char> backing(length + 80);
            for (auto &b : backing) b = static_cast<unsigned char>(random_word());
            XdgMimeMagicMatchlet m{};
            m.offset = 1;
            m.range_length = 17;
            m.value_length = length;
            m.value = backing.data() + 3;
            m.mask = masked ? backing.data() + 5 : nullptr;
#ifndef RVV_STANDALONE
            const int expected =
                xdg_mime_magic_matchlet_compare_to_data_isolated(
                    &m, backing.data(), backing.size());
#endif
            const int actual = xdg_mime_magic_matchlet_compare_to_data_rvv(&m, backing.data(), backing.size());
#ifndef RVV_STANDALONE
            if (actual != expected) std::abort();
#else
            if (actual != 0 && actual != 1) std::abort();
#endif
        }
        if (masked) {
            unsigned char data[96] = {}, value[65], mask[65] = {};
            for (auto &v : value) v = 255;
            XdgMimeMagicMatchlet m{};
            m.value = value;
            m.mask = mask;
            m.value_length = 65;
            m.range_length = 5;
#ifndef RVV_STANDALONE
            const int expected =
                xdg_mime_magic_matchlet_compare_to_data_isolated(&m, data,
                                                                 sizeof data);
#endif
            const int actual = xdg_mime_magic_matchlet_compare_to_data_rvv(&m, data, sizeof data);
            if (actual != 1) std::abort();
#ifndef RVV_STANDALONE
            if (actual != expected) std::abort();
#endif
        }
    }
    std::puts("matcher PASS (seed=0x00c0ffee)");
}
