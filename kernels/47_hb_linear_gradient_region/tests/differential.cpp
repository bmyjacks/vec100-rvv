#include "kernel.h"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>

static uint32_t state = 0x13905abcu;
static uint32_t next() {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

// Separate scalar specification: byte-wise BGRA composition, with the pinned
// lookup's nonfinite, extend, nearest-entry and early-alpha rules.
static uint32_t expected_pixel(uint32_t dst, const uint32_t *lut, float proj,
                               hb_paint_extend_t extend) {
    float u = proj;
    if (!std::isfinite(u)) u = 0.f;
    else if (extend == HB_PAINT_EXTEND_PAD) {
        u = u >= 0.f ? u : 0.f;
        u = u <= 1.f ? u : 1.f;
    } else if (extend == HB_PAINT_EXTEND_REPEAT) {
        u -= std::floor(u);
        if (u < 0.f) u += 1.f;
    } else {
        u = std::fmod(std::fabs(u), 2.f);
        if (u > 1.f) u = 2.f - u;
    }
    if (!std::isfinite(u)) u = 0.f;
    else {
        u = u >= 0.f ? u : 0.f;
        u = u <= 1.f ? u : 1.f;
    }
    const uint32_t src = lut[static_cast<unsigned>(u * 255.f + 0.5f)];
    unsigned alpha = src >> 24;
    if (alpha == 0) return dst;
    if (alpha == 255) return src;
    uint32_t out = 0;
    for (unsigned b = 0; b < 4; ++b) {
        unsigned shift = b * 8;
        unsigned s = (src >> shift) & 255;
        unsigned d = (dst >> shift) & 255;
        unsigned scaled = (d * (255 - alpha) + 255) / 256;
        out |= uint32_t(uint8_t(s + scaled)) << shift;
    }
    return out;
}

static void check(unsigned width, unsigned min_x, unsigned pad,
                  hb_paint_extend_t extend, float gx, float gy,
                  float gx0, float gy0, float dx, float dy,
                  float inv_denom, float inv_xx, float inv_yx, unsigned case_no) {
    const unsigned max_x = min_x + width;
    // Deliberately unaligned packed pixels; exercise guards and row padding.
    const size_t bytes = 1 + size_t(max_x + pad) * 4 + 13;
    std::vector<uint8_t> input(bytes), scalar(bytes), vector(bytes), oracle(bytes);
    for (auto &v : input) v = uint8_t(next());
    scalar = vector = oracle = input;
    uint32_t lut[256];
    for (unsigned i = 0; i < 256; ++i) {
        unsigned a = i % 17 == 0 ? 0 : i % 19 == 0 ? 255 : next() & 255;
        unsigned b = ((next() & 255) * a) / 255;
        unsigned g = ((next() & 255) * a) / 255;
        unsigned r = ((next() & 255) * a) / 255;
        lut[i] = b | (g << 8) | (r << 16) | (a << 24);
    }
    auto row = [](std::vector<uint8_t> &v) {
        return reinterpret_cast<hb_packed_t<uint32_t> *>(v.data() + 1);
    };
    float seed_x = gx, seed_y = gy;
    for (unsigned px = min_x; px < max_x; ++px) {
        uint32_t old;
        std::memcpy(&old, oracle.data() + 1 + size_t(px) * 4, 4);
        float proj = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
        uint32_t want = expected_pixel(old, lut, proj, extend);
        std::memcpy(oracle.data() + 1 + size_t(px) * 4, &want, 4);
        gx += inv_xx;
        gy += inv_yx;
    }
    hb_139_linear_gradient_region(row(scalar), min_x, max_x, seed_x, seed_y,
                                  gx0, gy0, dx, dy, inv_denom, inv_xx, inv_yx,
                                  lut, extend);
    hb_139_linear_gradient_region_rvv(row(vector), min_x, max_x, seed_x, seed_y,
                                      gx0, gy0, dx, dy, inv_denom, inv_xx, inv_yx,
                                      lut, extend);
    if (scalar != oracle || vector != oracle) {
        for (size_t i = 0; i < bytes; ++i)
            if (scalar[i] != oracle[i] || vector[i] != oracle[i]) {
                std::fprintf(stderr, "case=%u width=%u min=%u mode=%d byte=%zu expected=%u scalar=%u rvv=%u\n",
                             case_no, width, min_x, int(extend), i,
                             oracle[i], scalar[i], vector[i]);
                std::exit(1);
            }
    }
}

int main() {
    const unsigned widths[] = {0, 1, 2, 3, 4, 5, 7, 15, 16, 17, 31, 32,
                               33, 63, 64, 65, 127, 129, 257};
    unsigned case_no = 0;
    for (int m = 0; m < 3; ++m) {
        auto extend = static_cast<hb_paint_extend_t>(m);
        for (unsigned w : widths) {
            // Non-unit delta yields values across and beyond [0,1].
            check(w, 0, 7, extend, -0.45f, 0.25f, 0.1f, 0.2f,
                  0.8f, -0.3f, 0.65f, 0.0078125f, 0.002f, case_no++);
            check(w, 3, 1, extend, 2.1f, -0.5f, -0.25f, 0.125f,
                  -1.75f, 0.75f, 1.f, -0.018f, 0.003f, case_no++);
            // Nonfinite projection, including infinite component cancellation.
            check(w, 2, 4, extend, std::numeric_limits<float>::infinity(), 1.f,
                  0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, case_no++);
            check(w, 1, 2, extend, std::numeric_limits<float>::quiet_NaN(), 0.f,
                  0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, case_no++);
        }
        // Repeatedly cross nearest-LUT entry boundaries, including chunk
        // boundaries after accumulated (non-reconstructible) float rounding.
        check(1025, 5, 3, extend, 0.5f / 255.f, 0.25f, 0.f, 0.f,
              1.f, 0.f, 1.f, 1.f / 255.f, 0.f, case_no++);
        check(513, 0, 0, extend, 1.e35f, -1.e35f, 0.f, 0.f,
              1.e10f, 1.e10f, 1.f, 0.f, 0.f, case_no++);
        for (unsigned j = 0; j < 300; ++j) {
            int x = int(next() % 8000) - 4000;
            int y = int(next() % 8000) - 4000;
            unsigned width = next() % 280;
            unsigned min_x = next() % 13;
            unsigned pad = next() % 17;
            float gx0 = (int(next() % 3000) - 1500) / 1024.f;
            float gy0 = (int(next() % 3000) - 1500) / 1024.f;
            float dx = (int(next() % 3000) - 1500) / 1024.f;
            float dy = (int(next() % 3000) - 1500) / 1024.f;
            float inv_denom = (next() % 1024 + 1) / 1024.f;
            float inv_xx = (int(next() % 1000) - 500) / 65536.f;
            float inv_yx = (int(next() % 1000) - 500) / 65536.f;
            check(width, min_x, pad, extend, x / 1024.f, y / 1024.f,
                  gx0, gy0, dx, dy, inv_denom, inv_xx, inv_yx, case_no++);
        }
    }
    std::printf("linear gradient region OK (%u cases, seed 0x13905abc)\n", case_no);
}
