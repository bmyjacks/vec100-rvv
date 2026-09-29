#include "kernel.h"
#include <cstdio>
#include <random>
#include <vector>

void edgeFilter_rvv(Frame *, x265_param *);

int main() {
    constexpr unsigned seed = 0x18fa2026;
    std::mt19937 rng(seed);
    unsigned cases = 0;
    for (int width : {5, 6, 7, 8, 9, 15, 16, 17, 31, 65})
        for (int height : {5, 6, 7, 8, 17})
            for (int trial = 0; trial < 8; ++trial) {
                int marginX = 2, marginY = 2, stride = width + 8;
                x265_param param{}; param.maxCUSize = trial % 2 ? 8 : 16;
                int maxHeight = (height + param.maxCUSize - 1) / param.maxCUSize * param.maxCUSize;
                size_t bytes = stride * (maxHeight + 2 * marginY);
                std::vector<pixel> input(stride * (height + 4));
                for (size_t j = 0; j < input.size(); ++j)
                    input[j] = trial == 0 ? 0 : trial == 1 ? 255 :
                        trial == 2 ? (j & 1 ? 0 : 255) : rng();
                std::vector<pixel> ea(bytes, 19), eb(bytes, 19),
                                   ga(bytes, 19), gb(bytes, 19),
                                   ta(bytes, 19), tb(bytes, 19);
                PicYuv pa{}, pb{};
                pa.m_picOrg[0] = pb.m_picOrg[0] = input.data();
                pa.m_picWidth = pb.m_picWidth = width;
                pa.m_picHeight = pb.m_picHeight = height;
                pa.m_stride = pb.m_stride = stride;
                pa.m_lumaMarginX = pb.m_lumaMarginX = marginX;
                pa.m_lumaMarginY = pb.m_lumaMarginY = marginY;
                Frame fa{&pa, ea.data(), ga.data(), ta.data()};
                Frame fb{&pb, eb.data(), gb.data(), tb.data()};
                edgeFilter_isolated(&fa, &param);
                edgeFilter_rvv(&fb, &param);
                if (ea != eb || ga != gb || ta != tb) {
                    std::fprintf(stderr, "gaussian seed=%x width=%d height=%d trial=%d\n", seed, width, height, trial);
                    return 1;
                }
                ++cases;
            }
    // The upstream copy/zero/blur order matters when source is gaussianPic.
    {
        x265_param param{}; param.maxCUSize = 8;
        constexpr int w = 9, h = 7, stride = 16;
        std::vector<pixel> ea(stride * 16), eb = ea, ga = ea, gb = ea, ta = ea, tb = ea;
        for (size_t i = 0; i < ga.size(); ++i) ga[i] = gb[i] = rng();
        PicYuv pa{}, pb{};
        pa.m_picWidth = pb.m_picWidth = w;
        pa.m_picHeight = pb.m_picHeight = h;
        pa.m_stride = pb.m_stride = stride;
        pa.m_lumaMarginX = pb.m_lumaMarginX = 2;
        pa.m_lumaMarginY = pb.m_lumaMarginY = 2;
        pa.m_picOrg[0] = ga.data() + 2 * stride + 2;
        pb.m_picOrg[0] = gb.data() + 2 * stride + 2;
        Frame fa{&pa, ea.data(), ga.data(), ta.data()};
        Frame fb{&pb, eb.data(), gb.data(), tb.data()};
        edgeFilter_isolated(&fa, &param); edgeFilter_rvv(&fb, &param);
        if (ea != eb || ga != gb || ta != tb) return 2;
        ++cases;
    }
    std::printf("gaussian: %u exact frames (seed %x)\n", cases, seed);
}
