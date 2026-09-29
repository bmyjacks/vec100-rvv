#include "kernel.h"

#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

extern "C" bool decompile_deltas_add_to_points_rvv(
    const HBUINT8 *&, hb_array_t<contour_point_t>, float, const HBUINT8 *, unsigned);

static void encode(std::vector<uint8_t> &out, unsigned n, std::mt19937 &rng) {
    unsigned at = 0;
    while (at < n) {
        unsigned count = 1 + (rng() % 64);
        if (count > n - at) count = n - at;
        unsigned kind = (rng() % 4) << 6;
        out.push_back(uint8_t(kind | (count - 1)));
        unsigned width = kind == 0x80 ? 0 : kind == 0x40 ? 2 : kind == 0xc0 ? 4 : 1;
        for (unsigned i = 0; i < count; ++i) {
            uint32_t u = rng();
            for (unsigned j = 0; j < width; ++j)
                out.push_back(uint8_t(u >> (8 * (width - 1 - j))));
        }
        at += count;
    }
}

static bool check(const std::vector<uint8_t> &encoded, unsigned count,
                  unsigned available, unsigned start, float scalar, std::mt19937 &rng) {
    std::vector<contour_point_t> a(count + 2), b(count + 2);
    for (auto &pt : a) pt.init(float(int(rng() % 65536) - 32768) / 32.f,
                                float(int(rng() % 65536) - 32768) / 64.f, rng() & 1);
    b = a;
    const HBUINT8 *p = reinterpret_cast<const HBUINT8 *>(encoded.data());
    const HBUINT8 *q = p;
    const HBUINT8 *end = p + available;
    bool ref = decompile_deltas_add_to_points(p, {a.data(), count}, scalar, end, start);
    bool got = decompile_deltas_add_to_points_rvv(q, {b.data(), count}, scalar, end, start);
    if (ref != got || p != q ||
        std::memcmp(a.data(), b.data(), a.size() * sizeof(contour_point_t))) {
        std::fprintf(stderr, "count=%u available=%u start=%u scalar=%a ref=%d got=%d consumed=%td/%td\n",
                     count, available, start, scalar, ref, got,
                     p - reinterpret_cast<const HBUINT8 *>(encoded.data()),
                     q - reinterpret_cast<const HBUINT8 *>(encoded.data()));
        return false;
    }
    return true;
}

int main() {
    std::mt19937 rng(0x13d3c0de);
    for (unsigned n : {0u, 1u, 3u, 31u, 32u, 33u, 63u, 64u, 65u, 127u, 193u}) {
        for (unsigned trial = 0; trial < 24; ++trial) {
            std::vector<uint8_t> raw;
            encode(raw, n, rng); encode(raw, n, rng);
            if (raw.empty()) raw.push_back(0xff); // valid end pointer for n=0
            for (unsigned start : {0u, n / 2, n, n + 3}) {
                float scalar = (trial & 1) ? -0.625f : 1.125f;
                if (!check(raw, n, raw.size(), start, scalar, rng)) return 1;
                for (unsigned cut : {0u, 1u, unsigned(raw.size() / 2),
                                     unsigned(raw.size() - 1)})
                    if (!check(raw, n, cut, start, scalar, rng)) return 1;
            }
        }
    }
    // Run exceeding the declared point count: control byte consumed, no writes.
    for (unsigned n : {1u, 17u, 63u}) {
        std::vector<uint8_t> invalid{63};
        invalid.resize(128);
        if (!check(invalid, n, invalid.size(), 0, 1.f, rng)) return 1;
    }
    // Serialized input may be a view into the caller's point storage.
    // In particular an x write can modify an as-yet-unread y control byte.
    {
        constexpr unsigned n = 32;
        std::vector<contour_point_t> a(n), b(n);
        for (auto &pt : a) pt.init(1.f, -2.f);
        auto *raw = reinterpret_cast<uint8_t *>(a.data()) + 4;
        raw[0] = raw[33] = 31;
        for (unsigned j = 0; j < n; ++j) raw[1 + j] = raw[34 + j] = uint8_t(j + 3);
        b = a;
        const auto *p = reinterpret_cast<const HBUINT8 *>(raw);
        const auto *q = reinterpret_cast<const HBUINT8 *>(
            reinterpret_cast<const uint8_t *>(b.data()) + 4);
        const auto *p0 = p, *q0 = q;
        bool ref = decompile_deltas_add_to_points(p, {a.data(), n}, 0.5f, p0 + 66, 0);
        bool got = decompile_deltas_add_to_points_rvv(q, {b.data(), n}, 0.5f, q0 + 66, 0);
        if (ref != got || p - p0 != q - q0 ||
            std::memcmp(a.data(), b.data(), n * sizeof(contour_point_t))) {
            std::fputs("aliased delta input mismatch\n", stderr); return 1;
        }
    }
    std::puts("gvar delta run differential OK");
}
