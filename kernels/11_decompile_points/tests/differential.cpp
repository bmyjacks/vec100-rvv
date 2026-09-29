#include "kernel.h"

#include <cstdio>
#include <random>
#include <vector>

extern "C" bool decompile_points_rvv(const HBUINT8 *&,
                                      hb_vector_t<unsigned int> &, const HBUINT8 *);

static std::vector<uint8_t> encode(unsigned count, std::mt19937 &rng) {
    std::vector<uint8_t> data;
    if (count < 128) data.push_back(uint8_t(count));
    else { data.push_back(uint8_t(0x80 | (count >> 8))); data.push_back(uint8_t(count)); }
    for (unsigned i = 0; i < count;) {
        const unsigned run = std::min(1u + unsigned(rng() % 128), count - i);
        const bool words = rng() & 1;
        data.push_back(uint8_t((run - 1) | (words ? 0x80 : 0)));
        for (unsigned j = 0; j < run; ++j) {
            unsigned delta = rng() & (words ? 0xffff : 0xff);
            if (words) data.push_back(uint8_t(delta >> 8));
            data.push_back(uint8_t(delta));
        }
        i += run;
    }
    return data;
}

static bool check(const std::vector<uint8_t> &data, size_t available,
                  unsigned previous, std::mt19937 &rng, bool error = false) {
    hb_vector_t<unsigned int> a, b;
    // Give both dirty-resize implementations identical backing memory, even
    // where an invalid stream leaves output elements unwritten.
    a.resize(400); b.resize(400);
    for (unsigned j = 0; j < 400; ++j) a.arrayZ[j] = b.arrayZ[j] = rng();
    a.resize(previous); b.resize(previous);
    if (error) { a.set_error(); b.set_error(); }
    const HBUINT8 *p = reinterpret_cast<const HBUINT8 *>(data.data());
    const HBUINT8 *q = p;
    bool ref = decompile_points(p, a, p + available);
    bool got = decompile_points_rvv(q, b, q + available);
    bool equal = ref == got && p == q && a.length == b.length && a.in_error() == b.in_error();
    if (equal) for (unsigned j = 0; j < a.length; ++j)
        if (a.arrayZ[j] != b.arrayZ[j]) { equal = false; break; }
    if (!equal) {
        std::fprintf(stderr, "bytes=%zu/%zu old=%u error=%d ref=%d got=%d pos=%td/%td len=%u/%u\n",
                     available, data.size(), previous, error, ref, got,
                     p - reinterpret_cast<const HBUINT8 *>(data.data()),
                     q - reinterpret_cast<const HBUINT8 *>(data.data()), a.length, b.length);
    }
    return equal;
}

int main() {
    std::mt19937 rng(0x14c0de);
    for (unsigned count : {0u, 1u, 2u, 15u, 16u, 17u, 31u, 32u, 33u,
                           63u, 64u, 127u, 128u, 255u, 256u, 305u}) {
        for (int trial = 0; trial < 16; ++trial) {
            auto data = encode(count, rng);
            for (unsigned prev : {0u, count / 2, count + 3}) {
                for (size_t available : {size_t(0), size_t(1), data.size() / 2,
                                         data.size() - 1, data.size()})
                    if (!check(data, available, prev, rng)) return 1;
            }
        }
    }
    std::vector<uint8_t> invalid{3, 4, 1, 1, 1, 1, 1};
    if (!check(invalid, invalid.size(), 5, rng)) return 1;
    if (!check(invalid, invalid.size(), 5, rng, true)) return 1;
    {
        // A buffer-backed serialized stream, whose unread bytes may be
        // overwritten by preceding cumulative output elements.
        hb_vector_t<unsigned> a, b;
        a.resize(32); b.resize(32);
        auto *raw = reinterpret_cast<uint8_t *>(a.arrayZ) + 4;
        raw[0] = 32; raw[1] = 31;
        for (unsigned j = 0; j < 32; ++j) raw[2 + j] = uint8_t(j + 1);
        for (unsigned j = 0; j < 32; ++j) b.arrayZ[j] = a.arrayZ[j];
        const auto *p = reinterpret_cast<const HBUINT8 *>(raw);
        const auto *q = reinterpret_cast<const HBUINT8 *>(
            reinterpret_cast<const uint8_t *>(b.arrayZ) + 4);
        const auto *p0 = p, *q0 = q;
        bool ref = decompile_points(p, a, p0 + 34);
        bool got = decompile_points_rvv(q, b, q0 + 34);
        if (ref != got || p - p0 != q - q0 || a.length != b.length) {
            std::fputs("aliased point input state mismatch\n", stderr); return 1;
        }
        for (unsigned j = 0; j < a.length; ++j)
            if (a.arrayZ[j] != b.arrayZ[j]) {
                std::fputs("aliased point input result mismatch\n", stderr); return 1;
            }
    }
    std::puts("point-prefix differential OK");
}
