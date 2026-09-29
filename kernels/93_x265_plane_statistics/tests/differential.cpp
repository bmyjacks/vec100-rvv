#include "kernel.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <random>
#include <vector>

namespace {
constexpr uint32_t seed = 0x12526579;

struct Plane {
    std::vector<pixel> bytes;
    size_t offset;

    const pixel *data() const { return bytes.data() + offset; }
};

Plane make_plane(std::mt19937 &rng, int width, int height, int stride,
                 int pattern) {
    const size_t rows = static_cast<size_t>(height - 1);
    const size_t pitch = stride < 0 ? -stride : stride;
    const size_t span = rows * pitch + width;
    Plane p{{}, 37 + (stride < 0 ? span - width : 0)};
    p.bytes.resize(span + 74);
    for (auto &v : p.bytes)
        v = pattern == 0 ? 0 : pattern == 1 ? 255 : static_cast<pixel>(rng());
    return p;
}

bool check(std::mt19937 &rng, int width, int height, int hShift, int vShift,
           int sign, bool chroma, int pattern, bool wrappedDenominator) {
    const int strideY = sign * (width + 13);
    const int strideC = sign * ((width >> hShift) + 19);
    Plane y = make_plane(rng, width, height, strideY, pattern);
    Plane u =
        make_plane(rng, width >> hShift, height >> vShift, strideC, pattern);
    Plane v =
        make_plane(rng, width >> hShift, height >> vShift, strideC, pattern);
    const auto yBefore = y.bytes, uBefore = u.bytes, vBefore = v.bytes;

    PlaneStatistics scalar, vector;
    std::memset(&scalar, 0xa5, sizeof(scalar));
    scalar.maxY = scalar.maxU = scalar.maxV = pattern == 1 ? 210 : 13;
    scalar.minY = scalar.minU = scalar.minV = pattern == 0 ? 45 : 240;
    std::memcpy(&vector, &scalar, sizeof(scalar));

    // The upstream luma denominator is a uint32_t multiplication. Here it
    // wraps even though the actual sampled plane is deliberately small.
    const uint32_t picWidth = wrappedDenominator ? 65537 : width;
    const uint32_t picHeight = wrappedDenominator ? 65537 : height;
    plane_statistics(y.data(), chroma ? u.data() : nullptr,
                     chroma ? v.data() : nullptr, strideY, strideC, width,
                     height, picWidth, picHeight, hShift, vShift, chroma,
                     &scalar);
    plane_statistics_rvv(y.data(), chroma ? u.data() : nullptr,
                         chroma ? v.data() : nullptr, strideY, strideC, width,
                         height, picWidth, picHeight, hShift, vShift, chroma,
                         &vector);
    if (std::memcmp(&scalar, &vector, sizeof(scalar)) != 0 ||
        y.bytes != yBefore || u.bytes != uBefore || v.bytes != vBefore) {
        std::fprintf(stderr,
                     "mismatch w=%d h=%d hs=%d vs=%d sign=%d chroma=%d "
                     "pattern=%d wrap=%d seed=%u\n",
                     width, height, hShift, vShift, sign, chroma, pattern,
                     wrappedDenominator, seed);
        return false;
    }
    if (wrappedDenominator && pattern == 1 &&
        scalar.avgY != static_cast<double>(width * height * 255ULL) / 131073) {
        std::fprintf(stderr, "uint32 luma denominator did not wrap\n");
        return false;
    }
    return true;
}
} // namespace

int main() {
    std::mt19937 rng(seed);
    int cases = 0;
    constexpr int widths[] = {1, 2, 3, 7, 15, 16, 17, 31, 32, 33,
                              127, 128, 129, 254, 255, 256, 257, 511, 1025};
    for (int width : widths)
        for (int height : {1, 2, 3, 9})
            for (int shifts = 0; shifts < 4; ++shifts)
                for (bool chroma : {false, true}) {
                    const int hs = shifts & 1, vs = shifts >> 1;
                    if ((width >> hs) == 0 || (height >> vs) == 0)
                        continue;
                    for (int sign : {-1, 0, 1})
                        for (int pattern : {0, 1, 2}) {
                            if (!check(rng, width, height, hs, vs, sign, chroma,
                                       pattern, (cases % 7) == 0))
                                return 1;
                            ++cases;
                        }
                }
    std::printf("plane_statistics: %d full-buffer exact cases (seed 0x%x)\n",
                cases, seed);
}
