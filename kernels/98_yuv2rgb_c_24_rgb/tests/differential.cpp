#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

int yuv2rgb_c_24_rgb_rvv(SwsInternal *, const uint8_t *const [], const int [],
                          int, int, uint8_t *const [], const int []);

int main() {
    std::mt19937 rng(0x98c01234);
    SwsInternal c{};
    std::vector<uint8_t> r(512), g(512), b(512);
    for (unsigned i = 0; i < r.size(); ++i) {
        r[i] = rng(); g[i] = rng(); b[i] = rng();
    }
    for (int i = 0; i < 256; ++i) {
        c.table_rV[i + YUVRGB_TABLE_HEADROOM] = r.data() + (rng() % 100);
        c.table_gU[i + YUVRGB_TABLE_HEADROOM] = g.data() + 60 + (rng() % 40);
        c.table_gV[i + YUVRGB_TABLE_HEADROOM] = static_cast<int>(rng() % 81) - 40;
        c.table_bU[i + YUVRGB_TABLE_HEADROOM] = b.data() + (rng() % 100);
    }
    for (int width : {0, 4, 8, 12, 16, 20, 28, 32, 36, 60, 128, 260}) {
        for (int height : {0, 2, 4, 6}) {
            for (int trial = 0; trial < 12; ++trial) {
                c.opts.dst_w = width;
                const int srcStride[] = {width + 7, width / 2 + 5, width / 2 + 9};
                const int dstStride[] = {3 * width + 11, 0, 0};
                std::vector<uint8_t> yy(srcStride[0] * (height + 2));
                std::vector<uint8_t> uu(srcStride[1] * (height / 2 + 2));
                std::vector<uint8_t> vv(srcStride[2] * (height / 2 + 2));
                for (auto &v : yy) v = rng();
                for (auto &v : uu) v = rng();
                for (auto &v : vv) v = rng();
                const uint8_t *src[] = {yy.data(), uu.data(), vv.data(), nullptr};
                std::vector<uint8_t> dst(dstStride[0] * (height + 6));
                for (auto &v : dst) v = rng();
                auto dst2 = dst;
                uint8_t *d1[] = {dst.data(), nullptr, nullptr};
                uint8_t *d2[] = {dst2.data(), nullptr, nullptr};
                int sliceY = 2 * (trial % 3);
                const auto a = yuv2rgb_c_24_rgb(&c, src, srcStride, sliceY, height, d1, dstStride);
                const auto z = yuv2rgb_c_24_rgb_rvv(&c, src, srcStride, sliceY, height, d2, dstStride);
                if (a != z || dst != dst2) {
                    std::fprintf(stderr, "yuv2rgb mismatch width=%d height=%d trial=%d\n",
                                 width, height, trial);
                    std::abort();
                }
            }
        }
    }
    std::puts("yuv2rgb_c_24_rgb: pass");
}
