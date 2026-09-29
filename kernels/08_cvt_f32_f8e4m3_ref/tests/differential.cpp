#include "kernel.h"

#include <array>
#include <cfenv>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <vector>

void skl_cvt_f32_f8e4m3_rvv(uint8_t *, const float *, float, size_t);

static float from_bits(uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static bool check(const std::vector<float> &input, float scale) {
    std::vector<uint8_t> scalar(input.size() + 7, 0xa5);
    std::vector<uint8_t> rvv = scalar;
    skl_cvt_f32_f8e4m3_ref(scalar.data(), input.data(), scale, input.size());
    skl_cvt_f32_f8e4m3_rvv(rvv.data(), input.data(), scale, input.size());
    for (size_t i = 0; i < scalar.size(); ++i) {
        if (scalar[i] != rvv[i]) {
            uint32_t in = 0, factor = 0;
            if (i < input.size())
                std::memcpy(&in, &input[i], sizeof(in));
            std::memcpy(&factor, &scale, sizeof(factor));
            std::fprintf(stderr,
                         "n=%zu i=%zu in=%08x factor=%08x scalar=%02x rvv=%02x\n",
                         input.size(), i, in, factor, scalar[i], rvv[i]);
            return false;
        }
    }
    return true;
}

static bool overlap(size_t n, size_t src_offset, size_t dst_offset,
                    float scale) {
    alignas(16) std::array<uint32_t, 256> scalar{}, rvv{};
    for (size_t i = 0; i < scalar.size(); ++i)
        scalar[i] = (static_cast<uint32_t>(i * 37) << 16) | 0x3f000001u;
    rvv = scalar;
    auto *const a = reinterpret_cast<uint8_t *>(scalar.data());
    auto *const b = reinterpret_cast<uint8_t *>(rvv.data());
    skl_cvt_f32_f8e4m3_ref(a + dst_offset,
                            reinterpret_cast<const float *>(a + src_offset),
                            scale, n);
    skl_cvt_f32_f8e4m3_rvv(b + dst_offset,
                            reinterpret_cast<const float *>(b + src_offset),
                            scale, n);
    if (scalar != rvv) {
        std::fprintf(stderr, "overlap n=%zu src=%zu dst=%zu\n", n,
                     src_offset, dst_offset);
        return false;
    }
    return true;
}

int main() {
    std::vector<float> edges;
    for (uint32_t b : {0u, 0x80000000u, 1u, 0x80000001u, 0x7f7fffffu,
                       0xff7fffffu, 0x7f800000u, 0xff800000u, 0x7fc00001u,
                       0xffc00001u, 0x7f800001u, 0xff800001u})
        edges.push_back(from_bits(b));
    // Every FP8 magnitude, its immediate FP32 neighbors, and all adjacent
    // midpoints, including the subnormal/normal and max/NaN transitions.
    std::vector<float> levels;
    for (int k = 0; k <= 126; ++k) {
        const int e = k >> 3, m = k & 7;
        const float value = e == 0 ? std::ldexp(float(m), -9)
                                    : std::ldexp(1.0f + float(m) / 8, e - 7);
        levels.push_back(value);
        edges.push_back(value);
        edges.push_back(-value);
        edges.push_back(std::nextafter(value, -INFINITY));
        edges.push_back(std::nextafter(value, INFINITY));
    }
    for (size_t i = 1; i < levels.size(); ++i) {
        const float middle = (levels[i - 1] + levels[i]) / 2;
        for (float x : {middle, std::nextafter(middle, -INFINITY),
                        std::nextafter(middle, INFINITY), -middle})
            edges.push_back(x);
    }
    for (float x : {448.0f, 464.0f, 480.0f, 512.0f}) {
        edges.push_back(x);
        edges.push_back(std::nextafter(x, INFINITY));
        edges.push_back(-x);
    }

    std::mt19937 rng(0x10f32f8u);
    std::vector<float> random;
    for (int i = 0; i < 12000; ++i)
        random.push_back(from_bits(rng()));
    for (float scale : {1.0f, -1.0f, 0.5f, 2.0f, 1.7f, -3.0f,
                        0.0f, -0.0f, 1.0e-20f, 1.0e20f,
                        INFINITY, -INFINITY,
                        std::numeric_limits<float>::quiet_NaN()}) {
        if (!check(edges, scale) || !check(random, scale))
            return 1;
        for (size_t n = 0; n <= 35; ++n) {
            std::vector<float> small(random.begin(), random.begin() + n);
            if (!check(small, scale))
                return 1;
        }
    }
    // nearbyintf in the source is sensitive to the active rounding mode;
    // exercise vector conversion and scaling under all four modes.
    for (int mode : {FE_TONEAREST, FE_TOWARDZERO, FE_UPWARD, FE_DOWNWARD}) {
        if (std::fesetround(mode) != 0) {
            std::fputs("fesetround failed\n", stderr);
            return 1;
        }
        for (float scale : {1.0f, 1.7f, -3.0f}) {
            if (!check(edges, scale) ||
                !check(std::vector<float>(random.begin(), random.begin() + 512),
                       scale))
                return 1;
        }
    }
    std::fesetround(FE_TONEAREST);
    for (size_t n : {size_t(0), size_t(1), size_t(4), size_t(17), size_t(61)})
        for (size_t src : {size_t(0), size_t(4), size_t(32), size_t(256)})
            for (size_t dst : {size_t(0), size_t(2), size_t(32), size_t(256)})
                if (!overlap(n, src, dst, 1.7f))
                    return 1;
    std::puts("FP32->FP8: boundaries, random bits, scales, rounding, tails, overlaps OK");
}
