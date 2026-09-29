#include "kernel.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

void emulated_edge_mc_16_rvv(uint8_t *, const uint8_t *, ptrdiff_t,
                                ptrdiff_t, int, int, int, int, int, int);

namespace {
constexpr unsigned seed = 0x120ee16;
constexpr int origin = 4096;
constexpr int width = 32;
constexpr int height = 19;

void randomize(std::vector<uint8_t> &a, std::mt19937 &rng) {
    for (auto &v : a)
        v = static_cast<uint8_t>(rng());
}

// Independent reference for disjoint src/dst: clamp each requested pixel
// directly to the image rectangle; preserve every byte outside the block.
void oracle(std::vector<uint8_t> &dst, const std::vector<uint8_t> &src,
            int stride, int bw, int bh, int sx, int sy) {
    for (int y = 0; y < bh; ++y)
        for (int x = 0; x < bw; ++x) {
            const int xx = std::clamp(sx + x, 0, width - 1);
            const int yy = std::clamp(sy + y, 0, height - 1);
            std::memcpy(dst.data() + origin + y * stride + 2 * x,
                        src.data() + origin + yy * stride + 2 * xx, 2);
        }
}

bool check_disjoint(std::mt19937 &rng, int stride, int bw, int bh,
                    int sx, int sy) {
    std::vector<uint8_t> src(8192), scalar(8192), rvv, expected;
    randomize(src, rng);
    randomize(scalar, rng);
    rvv = expected = scalar;
    const uint8_t *requested = src.data() + origin + sy * stride + 2 * sx;
    emulated_edge_mc_16_isolated(scalar.data() + origin, requested,
                               stride, stride, bw, bh, sx, sy, width, height);
    emulated_edge_mc_16_rvv(rvv.data() + origin, requested,
                        stride, stride, bw, bh, sx, sy, width, height);
    oracle(expected, src, stride, bw, bh, sx, sy);
    if (scalar != expected || rvv != expected) {
        std::fprintf(stderr, "disjoint stride=%d bw=%d bh=%d x=%d y=%d seed=%u scalar=%d rvv=%d\n",
                     stride, bw, bh, sx, sy, seed,
                     scalar == expected, rvv == expected);
        return false;
    }
    return true;
}

// src and dst share one allocation. The destination is one entire row away
// (or 64 bytes away within a row) from the source of EACH individual memcpy.
// A destination row can nonetheless overwrite a *later* source row.
bool check_alias(std::mt19937 &rng, int stride, int delta, int sx, int sy) {
    constexpr int bw = 16, bh = 9;
    std::vector<uint8_t> scalar(8192), rvv;
    randomize(scalar, rng);
    rvv = scalar;
    const int source = origin + sy * stride + 2 * sx;
    emulated_edge_mc_16_isolated(scalar.data() + origin + delta,
                               scalar.data() + source, stride, stride,
                               bw, bh, sx, sy, width, height);
    emulated_edge_mc_16_rvv(rvv.data() + origin + delta,
                        rvv.data() + source, stride, stride,
                        bw, bh, sx, sy, width, height);
    if (scalar != rvv) {
        std::fprintf(stderr, "alias stride=%d delta=%d x=%d y=%d seed=%u\n",
                     stride, delta, sx, sy, seed);
        return false;
    }
    return true;
}
} // namespace

int main() {
    std::mt19937 rng(seed);
    unsigned cases = 0;
    // Empty image: no pointer dereference, even with null pointers.
    for (int w : {0, 7})
        for (int h : {0, 9}) {
            if (w && h)
                continue;
            emulated_edge_mc_16_isolated(nullptr, nullptr, 0, 0, 0, 0, 0, 0, w, h);
            emulated_edge_mc_16_rvv(nullptr, nullptr, 0, 0, 0, 0, 0, 0, w, h);
            ++cases;
        }
    for (int stride : {-96, 96})
        for (int bw : {1, 2, 7, 16, 32})
            for (int bh : {1, 3, 9, 16}) {
                const int xs[] = {-bw - 3, -bw, -1, 0, 1,
                                  width - 1, width, width + 3};
                const int ys[] = {-bh - 3, -bh, -1, 0, 1,
                                  height - 1, height, height + 3};
                for (int sx : xs)
                    for (int sy : ys) {
                        if (!check_disjoint(rng, stride, bw, bh, sx, sy))
                            return 1;
                        ++cases;
                    }
            }
    for (int stride : {-96, 96})
        for (int delta : {-stride, stride, 64})
            for (int sx : {-3, 0, 14, 32})
                for (int sy : {0, 3}) {
                    // Far-right source adjustment with in-row destination at
                    // x=32 would make some per-row memcpy ranges overlap.
                    if (delta == 64 && sx >= 14)
                        continue;
                    if (!check_alias(rng, stride, delta, sx, sy))
                        return 1;
                    ++cases;
                }
    std::printf("emulated_edge_mc_16: %u full-buffer/oracle cases (seed %u)\n",
                cases, seed);
}
