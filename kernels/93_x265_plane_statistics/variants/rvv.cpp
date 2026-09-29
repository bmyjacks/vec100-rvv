#include "kernel.h"

#include <riscv_vector.h>

namespace {
void reduce_row(const pixel *row, int width, pixel &maxval, pixel &minval,
                uint64_t &sum) {
    for (int c = 0; c < width;) {
        // A u8 -> u16 widening reduction is exact for at most 255 pixels.
        const size_t remaining = static_cast<size_t>(width - c);
        const size_t vl =
            __riscv_vsetvl_e8m1(remaining < 255 ? remaining : 255);
        const vuint8m1_t values = __riscv_vle8_v_u8m1(row + c, vl);
        const vuint8m1_t maxseed = __riscv_vmv_v_x_u8m1(maxval, 1);
        const vuint8m1_t minseed = __riscv_vmv_v_x_u8m1(minval, 1);
        maxval = __riscv_vmv_x_s_u8m1_u8(
            __riscv_vredmaxu_vs_u8m1_u8m1(values, maxseed, vl));
        minval = __riscv_vmv_x_s_u8m1_u8(
            __riscv_vredminu_vs_u8m1_u8m1(values, minseed, vl));
        const vuint16m1_t zero = __riscv_vmv_v_x_u16m1(0, 1);
        sum += __riscv_vmv_x_s_u16m1_u16(
            __riscv_vwredsumu_vs_u8m1_u16m1(values, zero, vl));
        c += static_cast<int>(vl);
    }
}
} // namespace

void plane_statistics_rvv(const pixel *y, const pixel *u, const pixel *v,
                          std::intptr_t strideY, std::intptr_t strideC,
                          int width, int height, uint32_t picWidth,
                          uint32_t picHeight, uint32_t hShift, uint32_t vShift,
                          bool chroma, PlaneStatistics *stats) {
    uint64_t lumaSum = 0, cbSum = 0, crSum = 0;
    pixel maxY = stats->maxY, minY = stats->minY;
    for (int r = 0; r < height; ++r) {
        reduce_row(y, width, maxY, minY, lumaSum);
        y += strideY;
    }
    stats->maxY = maxY;
    stats->minY = minY;
    stats->avgY = (double)lumaSum / (picHeight * picWidth);

    if (chroma) {
        pixel maxU = stats->maxU, minU = stats->minU;
        pixel maxV = stats->maxV, minV = stats->minV;
        for (int r = 0; r < height >> vShift; ++r) {
            reduce_row(u, width >> hShift, maxU, minU, cbSum);
            reduce_row(v, width >> hShift, maxV, minV, crSum);
            u += strideC;
            v += strideC;
        }
        stats->maxU = maxU;
        stats->minU = minU;
        stats->maxV = maxV;
        stats->minV = minV;
        stats->avgU = (double)cbSum / ((height >> vShift) * (width >> hShift));
        stats->avgV = (double)crSum / ((height >> vShift) * (width >> hShift));
    }
}
