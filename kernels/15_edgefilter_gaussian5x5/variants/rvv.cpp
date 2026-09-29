#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

static bool overlap(const void *a, size_t an, const void *b, size_t bn) {
    uintptr_t x = reinterpret_cast<uintptr_t>(a);
    uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x < y + bn && y < x + an;
}

void edgeFilter_rvv(Frame *frame, x265_param *param) {
    PicYuv *pic = frame->m_fencPic;
    int height = pic->m_picHeight, width = pic->m_picWidth;
    intptr_t stride = pic->m_stride;
    // An upstream padded image includes the two extra rows/columns read by
    // the last row/column (the original skips only height-2 / width-2).
    if (height < 5 || width < 5 || stride < width + 2 ||
        !param->maxCUSize) {
        edgeFilter_isolated(frame, param);
        return;
    }
    size_t bytes = size_t(stride) *
        (((height + param->maxCUSize - 1) / param->maxCUSize) *
             param->maxCUSize + pic->m_lumaMarginY * 2);
    const pixel *src = pic->m_picOrg[0];
    const size_t srcBytes = size_t(stride) * (height + 2);
    if (overlap(src, srcBytes, frame->m_edgePic, bytes) ||
        overlap(src, srcBytes, frame->m_gaussianPic, bytes) ||
        overlap(src, srcBytes, frame->m_thetaPic, bytes) ||
        overlap(frame->m_edgePic, bytes, frame->m_gaussianPic, bytes) ||
        overlap(frame->m_edgePic, bytes, frame->m_thetaPic, bytes) ||
        overlap(frame->m_gaussianPic, bytes, frame->m_thetaPic, bytes)) {
        edgeFilter_isolated(frame, param);
        return;
    }
    memset(frame->m_edgePic, 0, bytes);
    memset(frame->m_gaussianPic, 0, bytes);
    memset(frame->m_thetaPic, 0, bytes);
    size_t offset = pic->m_lumaMarginY * stride + pic->m_lumaMarginX;
    pixel *edge = frame->m_edgePic + offset;
    pixel *gaussian = frame->m_gaussianPic + offset;
    pixel *theta = frame->m_thetaPic + offset;
    for (int row = 0; row < height; ++row) {
        memcpy(edge + row * stride, src + row * stride, width);
        memcpy(gaussian + row * stride, src + row * stride, width);
    }
    const int weights[5][5] = {
        {2,4,5,4,2}, {4,9,12,9,4}, {5,12,15,12,5},
        {4,9,12,9,4}, {2,4,5,4,2}};
    for (int row = 2; row < height; ++row) {
        if (row == height - 2) continue;
        for (int col = 2; col < width;) {
            if (col == width - 2) { ++col; continue; }
            // Stop before the one skipped column so every vector lane has
            // exactly the same 25 input pixels as the upstream scalar loop.
            int count = col < width - 2 ? width - 2 - col : width - col;
            size_t vl = __riscv_vsetvl_e16m2(count);
            vuint16m2_t sum = __riscv_vmv_v_x_u16m2(0, vl);
            for (int r = 0; r < 5; ++r) {
                for (int c = 0; c < 5; ++c) {
                    vuint8m1_t x = __riscv_vle8_v_u8m1(
                        src + (row + r - 2) * stride + col + c - 2, vl);
                    vuint16m2_t wide = __riscv_vwaddu_vx_u16m2(x, 0, vl);
                    sum = __riscv_vmacc_vx_u16m2(sum, weights[r][c], wide, vl);
                }
            }
            sum = __riscv_vdivu_vx_u16m2(sum, 159, vl);
            __riscv_vse8_v_u8m1(gaussian + row * stride + col,
                                 __riscv_vncvt_x_x_w_u8m1(sum, vl), vl);
            col += vl;
        }
    }
    if (!computeEdge(edge, gaussian, theta, stride, height, width, true))
        x265_log(NULL, X265_LOG_ERROR, "Failed edge computation!");
}
