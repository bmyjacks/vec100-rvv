#include "kernel.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

namespace simdjson {
error_code minify_rvv(const uint8_t *, size_t, uint8_t *, size_t &) noexcept;
}

static bool check(const std::vector<uint8_t> &input, unsigned placement) {
    const size_t n = input.size();
    // placement: separate, in-place, destination before source, or inside it.
    const size_t src_at = placement == 2 ? 16 : 0;
    const size_t dst_at = placement == 3 ? 3 : 0;
    const size_t cap = n + 64;
    std::vector<uint8_t> ref(cap + 32, 0xa5), vec = ref;
    std::vector<uint8_t> ref_dst(cap, 0xa5), vec_dst = ref_dst;
    if (n) std::memcpy(ref.data() + src_at, input.data(), n);
    vec = ref;
    uint8_t *r = placement == 0 ? ref_dst.data() : ref.data() + dst_at;
    uint8_t *v = placement == 0 ? vec_dst.data() : vec.data() + dst_at;
    size_t nr = 12345, nv = 12345;
    auto er = simdjson::minify_isolated(ref.data() + src_at, n, r, nr);
    auto ev = simdjson::minify_rvv(vec.data() + src_at, n, v, nv);
    // Compare all bytes, including the reference's write beyond dst_len.
    if (nr != nv || er != ev || ref != vec || ref_dst != vec_dst) {
        std::fprintf(stderr, "minify mismatch len=%zu placement=%u ref=%zu/%d rvv=%zu/%d\n",
                     n, placement, nr, er, nv, ev);
        return false;
    }
    return true;
}

int main() {
    std::mt19937 rng(0x61b1a5u);
    const std::array<uint8_t, 14> alphabet = {
        ' ', '\t', '\n', '\r', '"', '\\', 'a', '0', '[', ']',
        0, 0xff, 11, 'Z'};
    for (size_t n : {size_t(0), size_t(1), size_t(2), size_t(15), size_t(16),
                     size_t(17), size_t(31), size_t(32), size_t(33),
                     size_t(63), size_t(64), size_t(65), size_t(127),
                     size_t(128), size_t(129), size_t(1024)}) {
        for (unsigned t = 0; t < 100; ++t) {
            std::vector<uint8_t> input(n);
            for (auto &b : input) b = alphabet[rng() % alphabet.size()];
            for (unsigned placement = 0; placement < 4; ++placement)
                if (!check(input, placement)) return 1;
        }
        for (uint8_t ch : alphabet) {
            std::vector<uint8_t> input(n, ch);
            for (unsigned placement = 0; placement < 4; ++placement)
                if (!check(input, placement)) return 1;
        }
    }
    std::vector<uint8_t> all(256);
    for (unsigned c = 0; c < 256; ++c) all[c] = static_cast<uint8_t>(c);
    for (unsigned placement = 0; placement < 4; ++placement) {
        if (!check(all, placement)) return 1;
        for (uint8_t c : all)
            if (!check(std::vector<uint8_t>(257, c), placement)) return 1;
    }
    std::puts("minify: seeded full-buffer differential OK");
}
