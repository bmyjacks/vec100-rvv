#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

extern "C" FT_Error FT_Outline_Get_BBox_rvv(FT_Outline *, FT_BBox *);
static unsigned long long seed = 0x285d478369b3cf11ULL;
static unsigned rnd() {
    seed ^= seed << 13; seed ^= seed >> 7; seed ^= seed << 17;
    return (unsigned)seed;
}

static void check(std::vector<FT_Vector> points, std::vector<FT_Byte> tags,
                  std::vector<FT_UShort> contours) {
    FT_Outline outline = {(FT_UShort)contours.size(), (FT_UShort)points.size(),
                          points.data(), tags.data(), contours.data(), 0};
    FT_BBox a = {7, 8, 9, 10}, b = a;
    FT_Error ea = FT_Outline_Get_BBox(&outline, &a);
    FT_Error eb = FT_Outline_Get_BBox_rvv(&outline, &b);
    if (ea != eb || a.xMin != b.xMin || a.yMin != b.yMin ||
        a.xMax != b.xMax || a.yMax != b.yMax) {
        std::fprintf(stderr, "bbox points=%zu contours=%zu errors=%d/%d bbox=(%ld,%ld,%ld,%ld)/(%ld,%ld,%ld,%ld)\n",
                     points.size(), contours.size(), ea, eb,
                     a.xMin, a.yMin, a.xMax, a.yMax,
                     b.xMin, b.yMin, b.xMax, b.yMax);
        std::exit(1);
    }
}

int main() {
    check({}, {}, {});
    for (unsigned n : {1u, 2u, 3u, 4u, 15u, 16u, 17u, 32u, 65u, 256u}) {
        for (unsigned trial = 0; trial < 40; ++trial) {
            std::vector<FT_Vector> p(n);
            std::vector<FT_Byte> t(n, FT_CURVE_TAG_ON);
            for (auto &v : p) {
                v.x = (long)(rnd() % 10001) - 5000;
                v.y = (long)(rnd() % 10001) - 5000;
            }
            // Outlying conic and cubic controls exercise scalar extrema.
            if (n >= 3 && trial % 3 == 0) {
                t[1] = FT_CURVE_TAG_CONIC;
                p[1] = {20000, -20000};
            }
            if (n >= 4 && trial % 3 == 1) {
                t[1] = t[2] = FT_CURVE_TAG_CUBIC;
                p[1] = {20000, -20000};
                p[2] = {-20000, 20000};
            }
            if (n >= 3 && trial % 3 == 2) {
                t[0] = FT_CURVE_TAG_CONIC;
                p[0] = {20000, -20000};
            }
            check(p, t, {(FT_UShort)(n - 1)});
            if (n >= 4) check(p, t, {3, (FT_UShort)(n - 1)});
        }
    }
    std::puts("ft_outline_get_bbox: OK");
}
