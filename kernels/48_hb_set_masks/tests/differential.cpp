#include "kernel.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

static_assert(sizeof(hb_glyph_info_t) == 20, "compare entire glyph records");

constexpr uint32_t seed = 0x17a5e7u;
static std::mt19937 rng(seed);
static unsigned cases = 0;

static void check(const std::vector<hb_glyph_info_t> &input, unsigned offset,
                  unsigned len, hb_mask_t value, hb_mask_t mask, unsigned start,
                  unsigned end) {
    auto scalar = input;
    auto rvv = input;
    hb_017_buffer_set_masks_isolated(scalar.data() + offset, len, value, mask,
                                   start, end);
    hb_017_buffer_set_masks_rvv(rvv.data() + offset, len, value, mask, start, end);
    for (size_t j = 0; j < scalar.size(); ++j) {
        if (std::memcmp(&scalar[j], &rvv[j], sizeof(scalar[j])) != 0) {
            std::fprintf(stderr,
                         "seed=%08x case=%u len=%u offset=%u index=%zu "
                         "value=%08x mask=%08x range=[%08x,%08x) "
                         "scalar_mask=%08x rvv_mask=%08x\n",
                         seed, cases, len, offset, j, value, mask, start, end,
                         scalar[j].mask, rvv[j].mask);
            std::exit(1);
        }
    }
    ++cases;
}

static hb_glyph_info_t glyph(unsigned i, unsigned mode) {
    constexpr std::array<uint32_t, 10> clusters = {
        0u, 1u,          2u,          7u,          8u,
        9u, 0x7fffffffu, 0x80000000u, 0xfffffffeu, 0xffffffffu};
    hb_glyph_info_t g{};
    g.codepoint = rng();
    g.mask = mode == 0 ? 0u : mode == 1 ? 0xffffffffu : rng();
    g.cluster = mode == 3 ? rng() : clusters[i % clusters.size()];
    g.var1.u32 = rng();
    g.var2.u32 = rng();
    return g;
}

int main() {
    hb_017_buffer_set_masks_isolated(nullptr, 0, 0, 0xffffffffu, 0, ~0u);
    hb_017_buffer_set_masks_rvv(nullptr, 0, 0, 0xffffffffu, 0, ~0u);
    hb_017_buffer_set_masks_isolated(nullptr, 0, 0, 0, 1, 9);
    hb_017_buffer_set_masks_rvv(nullptr, 0, 0, 0, 1, 9);

    constexpr std::array<unsigned, 23> sizes = {
        0,  1,  2,  3,  4,  5,  7,   8,   9,   15,  16, 17,
        31, 32, 33, 63, 64, 65, 127, 128, 129, 255, 257};
    constexpr std::array<hb_mask_t, 8> masks = {
        0u,          1u,          0x80000000u, 0xaaaaaaaau,
        0x55555555u, 0xffff0000u, 0xfffffffeu, 0xffffffffu};
    constexpr std::array<std::array<unsigned, 2>, 12> ranges = {{
        {0, ~0u},
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 2},
        {2, 8},
        {8, 2},
        {8, 9},
        {0x7fffffffu, 0x80000000u},
        {0x80000000u, 0xffffffffu},
        {0xffffffffu, 0xffffffffu},
        {1, ~0u},
    }};

    for (unsigned len : sizes) {
        for (unsigned offset : {0u, 1u, 3u}) {
            for (unsigned mode = 0; mode < 4; ++mode) {
                std::vector<hb_glyph_info_t> input(len + offset + 2);
                for (unsigned j = 0; j < input.size(); ++j)
                    input[j] = glyph(j, mode);
                for (hb_mask_t mask : masks) {
                    hb_mask_t value = rng();
                    if (mode == 0)
                        value = 0;
                    else if (mode == 1)
                        value = ~0u;
                    else if (mode == 2)
                        value = mask;
                    for (const auto &range : ranges)
                        check(input, offset, len, value, mask, range[0],
                              range[1]);
                }
            }
        }
    }

    for (unsigned n = 0; n < 1000; ++n) {
        const unsigned len = rng() % 290;
        const unsigned offset = rng() % 4;
        std::vector<hb_glyph_info_t> input(len + offset + 2);
        for (unsigned j = 0; j < input.size(); ++j)
            input[j] = glyph(j, 3);
        const unsigned start = rng();
        const unsigned end = rng();
        check(input, offset, len, rng(), rng(), start, end);
    }
    std::printf("hb set_masks: %u exact full-record cases (seed %08x)\n", cases,
                seed);
}
