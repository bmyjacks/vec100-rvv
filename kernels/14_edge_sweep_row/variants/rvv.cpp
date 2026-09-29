#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

static bool overlap(const void *a, size_t an, const void *b, size_t bn) {
    uintptr_t x = reinterpret_cast<uintptr_t>(a);
    uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x < y + bn && y < x + an;
}

extern "C" void edge_sweep_row_rvv(int32_t *area, int16_t *cover, unsigned width,
                                 int x_org, int32_t y_top,
                                 const hb_raster_edge_t &edge,
                                 unsigned &x_min, unsigned &x_max) {
    // Off-screen edges feed cover[0] repeatedly, so retain upstream order.
    int32_t y_bot = (int32_t)hb_min((int64_t)y_top + 256, (int64_t)INT32_MAX);
    int32_t ey0 = hb_max(edge.yL, y_top), ey1 = hb_min(edge.yH, y_bot);
    if (ey0 >= ey1) return;
    int64_t x0_64 = (int64_t)edge.xL +
        ((((int64_t)ey0 - edge.yL) * edge.slope) >> 16);
    int64_t x1_64 = (int64_t)edge.xL +
        ((((int64_t)ey1 - edge.yL) * edge.slope) >> 16);
    int32_t x0 = (int32_t)hb_clamp(x0_64, (int64_t)INT32_MIN, (int64_t)INT32_MAX);
    int32_t x1 = (int32_t)hb_clamp(x1_64, (int64_t)INT32_MIN, (int64_t)INT32_MAX);
    int32_t cx0 = x0 >> 8, cx1 = x1 >> 8;
    int64_t first = (int64_t)cx0 - x_org, last = (int64_t)cx1 - x_org;
    int64_t cells = cx0 > cx1 ? (int64_t)cx0 - cx1 : (int64_t)cx1 - cx0;
    if (cells < 3 || first < 0 || last < 0 || first >= width || last >= width ||
        width > INT32_MAX ||
        &x_min == &x_max ||
        overlap(area, size_t(width) * 4, cover, size_t(width) * 2) ||
        overlap(&edge, sizeof(edge), area, size_t(width) * 4) ||
        overlap(&edge, sizeof(edge), cover, size_t(width) * 2) ||
        overlap(&edge, sizeof(edge), &x_min, sizeof(x_min)) ||
        overlap(&edge, sizeof(edge), &x_max, sizeof(x_max)) ||
        overlap(&x_min, sizeof(x_min), area, size_t(width) * 4) ||
        overlap(&x_max, sizeof(x_max), area, size_t(width) * 4) ||
        overlap(&x_min, sizeof(x_min), cover, size_t(width) * 2) ||
        overlap(&x_max, sizeof(x_max), cover, size_t(width) * 2)) {
        edge_sweep_row_isolated(area, cover, width, x_org, y_top, edge, x_min, x_max);
        return;
    }
    int64_t dx = (int64_t)x1 - x0;
    int64_t dy = (int64_t)ey1 - ey0;
    int32_t delta = (int32_t)(256 * dy / dx);
    bool forward = dx > 0;
    int64_t boundary = forward ? ((int64_t)cx0 + 1) * 256 : (int64_t)cx0 * 256;
    int32_t xb = (int32_t)hb_clamp(boundary, (int64_t)INT32_MIN, (int64_t)INT32_MAX);
    int32_t fy0 = ey0 - y_top, fy1 = ey1 - y_top;
    int32_t fyb = fy0 + (int32_t)(((int64_t)xb - x0) * dy / dx);
    // The 32-bit scalar arithmetic is defined only if its intermediates fit.
    // Wide windings and extreme slopes continue through the original path.
    int64_t step = forward ? delta : -(int64_t)delta;
    int64_t prev = (int64_t)fyb + (cells - 1) * step;
    int64_t firstArea = (x0 & 255) + (forward ? 256 : 0);
    int64_t lastArea = (forward ? 0 : 256) + (x1 & 255);
    int64_t v0 = firstArea * (fyb - fy0) * edge.wind;
    int64_t vm = 256 * step * edge.wind;
    int64_t v1 = lastArea * (fy1 - prev) * edge.wind;
    if (prev < INT32_MIN || prev > INT32_MAX ||
        v0 < INT32_MIN || v0 > INT32_MAX ||
        vm < INT32_MIN || vm > INT32_MAX ||
        v1 < INT32_MIN || v1 > INT32_MAX) {
        edge_sweep_row_isolated(area, cover, width, x_org, y_top, edge, x_min, x_max);
        return;
    }
    int firstCol = (int)first, lastCol = (int)last;
    area[firstCol] += (int32_t)v0;
    cover[firstCol] += (int16_t)((fyb - fy0) * edge.wind);
    int start = forward ? firstCol + 1 : lastCol + 1;
    int n = (int)cells - 1;
    for (int i = 0; i < n;) {
        size_t vl = __riscv_vsetvl_e32m1(n - i);
        vint32m1_t ar = __riscv_vle32_v_i32m1(area + start + i, vl);
        ar = __riscv_vadd_vx_i32m1(ar, (int32_t)vm, vl);
        __riscv_vse32_v_i32m1(area + start + i, ar, vl);
        vint16mf2_t cv = __riscv_vle16_v_i16mf2(cover + start + i, vl);
        cv = __riscv_vadd_vx_i16mf2(cv, (int16_t)(step * edge.wind), vl);
        __riscv_vse16_v_i16mf2(cover + start + i, cv, vl);
        i += vl;
    }
    area[lastCol] += (int32_t)v1;
    cover[lastCol] += (int16_t)((fy1 - prev) * edge.wind);
    x_min = hb_min(x_min, (unsigned)hb_min(firstCol, lastCol));
    x_max = hb_max(x_max, (unsigned)hb_max(firstCol, lastCol));
}
