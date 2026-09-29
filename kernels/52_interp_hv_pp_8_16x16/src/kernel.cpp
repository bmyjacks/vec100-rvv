/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/ipfilter.cpp
 *    source/common/constants.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/ipfilter.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Deepthi Devaki <deepthidevaki@multicorewareinc.com>,
 *          Rajesh Paulraj <rajesh@multicorewareinc.com>
 *          Praveen Kumar Tiwari <praveen@multicorewareinc.com>
 *          Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *
 * source/common/constants.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Steve Borho <steve@borho.org>
 *          Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *
 */

#include "kernel.h"

namespace X265_NS {
/*
 * source/common/constants.cpp:250-268
 */
const int16_t g_lumaFilter[4][NTAPS_LUMA] = {{0, 0, 0, 64, 0, 0, 0, 0},
                                             {-1, 4, -10, 58, 17, -5, 1, 0},
                                             {-1, 4, -11, 40, 40, -11, 4, -1},
                                             {0, 1, -5, 17, 58, -10, 4, -1}};

const int16_t g_chromaFilter[8][NTAPS_CHROMA] = {
    {0, 64, 0, 0},    {-2, 58, 10, -2}, {-4, 54, 16, -2}, {-6, 46, 28, -4},
    {-4, 36, 36, -4}, {-4, 28, 46, -6}, {-2, 16, 54, -4}, {-2, 10, 58, -2}};
} // namespace X265_NS

using namespace X265_NS;

namespace {
/*
 * source/common/ipfilter.cpp:120-162
 */
template <int N, int width, int height>
void interp_horiz_ps_c(const pixel *src, intptr_t srcStride, int16_t *dst,
                       intptr_t dstStride, int coeffIdx, int isRowExt) {
    const int16_t *coeff =
        (N == 4) ? g_chromaFilter[coeffIdx] : g_lumaFilter[coeffIdx];
    int headRoom = IF_INTERNAL_PREC - X265_DEPTH;
    int shift = IF_FILTER_PREC - headRoom;
    int offset = (unsigned)-IF_INTERNAL_OFFS << shift;
    int blkheight = height;
    src -= N / 2 - 1;

    if (isRowExt) {
        src -= (N / 2 - 1) * srcStride;
        blkheight += N - 1;
    }

    int row, col;
    for (row = 0; row < blkheight; row++) {
        for (col = 0; col < width; col++) {
            int sum;

            sum = src[col + 0] * coeff[0];
            sum += src[col + 1] * coeff[1];
            sum += src[col + 2] * coeff[2];
            sum += src[col + 3] * coeff[3];
            if (N == 8) {
                sum += src[col + 4] * coeff[4];
                sum += src[col + 5] * coeff[5];
                sum += src[col + 6] * coeff[6];
                sum += src[col + 7] * coeff[7];
            }

            int16_t val = (int16_t)((sum + offset) >> shift);
            dst[col] = val;
        }

        src += srcStride;
        dst += dstStride;
    }
}

/*
 * source/common/ipfilter.cpp:319-360
 */
template <int N>
void filterVertical_sp_c(const int16_t *src, intptr_t srcStride, pixel *dst,
                         intptr_t dstStride, int width, int height,
                         int coeffIdx) {
    int headRoom = IF_INTERNAL_PREC - X265_DEPTH;
    int shift = IF_FILTER_PREC + headRoom;
    int offset = (1 << (shift - 1)) + (IF_INTERNAL_OFFS << IF_FILTER_PREC);
    uint16_t maxVal = (1 << X265_DEPTH) - 1;
    const int16_t *coeff =
        (N == 8 ? g_lumaFilter[coeffIdx] : g_chromaFilter[coeffIdx]);

    src -= (N / 2 - 1) * srcStride;

    int row, col;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            int sum;

            sum = src[col + 0 * srcStride] * coeff[0];
            sum += src[col + 1 * srcStride] * coeff[1];
            sum += src[col + 2 * srcStride] * coeff[2];
            sum += src[col + 3 * srcStride] * coeff[3];
            if (N == 8) {
                sum += src[col + 4 * srcStride] * coeff[4];
                sum += src[col + 5 * srcStride] * coeff[5];
                sum += src[col + 6 * srcStride] * coeff[6];
                sum += src[col + 7 * srcStride] * coeff[7];
            }

            int16_t val = (int16_t)((sum + offset) >> shift);

            val = (val < 0) ? 0 : val;
            val = (val > maxVal) ? maxVal : val;

            dst[col] = (pixel)val;
        }

        src += srcStride;
        dst += dstStride;
    }
}

/*
 * source/common/ipfilter.cpp:362-369
 */
template <int N, int width, int height>
void interp_hv_pp_c(const pixel *src, intptr_t srcStride, pixel *dst,
                    intptr_t dstStride, int idxX, int idxY) {
    ALIGN_VAR_32(int16_t, immed[width * (height + N - 1)]);

    interp_horiz_ps_c<N, width, height>(src, srcStride, immed, width, idxX, 1);
    filterVertical_sp_c<N>(immed + (N / 2 - 1) * width, width, dst, dstStride,
                           width, height, idxY);
}
} // namespace

/*
 * Wrapper for invoking the extracted kernel.
 */
void interp_hv_pp_8_16x16(const pixel *src, std::intptr_t srcStride, pixel *dst,
                          std::intptr_t dstStride, int idxX, int idxY) {
    interp_hv_pp_c<8, 16, 16>(src, srcStride, dst, dstStride, idxX, idxY);
}
