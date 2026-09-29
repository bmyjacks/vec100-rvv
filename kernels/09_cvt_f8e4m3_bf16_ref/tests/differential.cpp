#include "kernel.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

void skl_cvt_f8e4m3_bf16_rvv(__bf16 *, const uint8_t *, size_t);

static uint16_t bits(__bf16 value) {
    uint16_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static bool check(const std::vector<uint8_t> &input) {
    const size_t n = input.size();
    std::vector<__bf16> scalar(n + 3), rvv(n + 3);
    std::memset(scalar.data(), 0xa5, scalar.size() * sizeof(__bf16));
    std::memset(rvv.data(), 0xa5, rvv.size() * sizeof(__bf16));
    skl_cvt_f8e4m3_bf16_ref(scalar.data(), input.data(), n);
    skl_cvt_f8e4m3_bf16_rvv(rvv.data(), input.data(), n);
    for (size_t i = 0; i < n + 3; ++i) {
        if (bits(scalar[i]) != bits(rvv[i])) {
            std::fprintf(stderr, "n=%zu i=%zu input=%02x scalar=%04x rvv=%04x\n",
                         n, i, i < n ? input[i] : 0, bits(scalar[i]),
                         bits(rvv[i]));
            return false;
        }
    }
    return true;
}

static bool overlap(size_t n, size_t src_offset, size_t dst_offset) {
    alignas(16) std::array<uint8_t, 1024> scalar{}, rvv{};
    for (size_t i = 0; i < scalar.size(); ++i)
        scalar[i] = static_cast<uint8_t>(i * 73 + 127);
    rvv = scalar;
    auto *const a = reinterpret_cast<__bf16 *>(scalar.data() + dst_offset);
    auto *const b = reinterpret_cast<__bf16 *>(rvv.data() + dst_offset);
    skl_cvt_f8e4m3_bf16_ref(a, scalar.data() + src_offset, n);
    skl_cvt_f8e4m3_bf16_rvv(b, rvv.data() + src_offset, n);
    if (scalar != rvv) {
        std::fprintf(stderr, "overlap n=%zu src=%zu dst=%zu\n", n,
                     src_offset, dst_offset);
        return false;
    }
    return true;
}

int main() {
    std::vector<uint8_t> all(256);
    for (size_t i = 0; i < 256; ++i)
        all[i] = static_cast<uint8_t>(i);
    if (!check(all))
        return 1;
    std::mt19937 rng(0x11f8bf16);
    for (size_t n = 0; n <= 80; ++n) {
        std::vector<uint8_t> input(n);
        for (auto &byte : input)
            byte = static_cast<uint8_t>(rng());
        if (!check(input))
            return 1;
    }
    for (size_t n : {size_t(127), size_t(129), size_t(511), size_t(1023)}) {
        std::vector<uint8_t> input(n);
        for (size_t i = 0; i < n; ++i)
            input[i] = all[(i * 149 + static_cast<size_t>(rng())) & 255];
        if (!check(input))
            return 1;
    }
    for (size_t n : {size_t(0), size_t(1), size_t(3), size_t(17), size_t(91)})
        for (size_t src : {size_t(0), size_t(2), size_t(24), size_t(128)})
            for (size_t dst : {size_t(0), size_t(2), size_t(24), size_t(128)})
                if (!overlap(n, src, dst))
                    return 1;
    std::puts("FP8->BF16: all 256 encodings, random tails, overlaps OK");
}
