/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/intrapred.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/intrapred.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Min Chen <chenm003@163.com>
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

namespace {
/*
 * source/common/intrapred.cpp:102-204
 */
template <int width>
void intra_pred_ang_c(pixel *dst, intptr_t dstStride, const pixel *srcPix0,
                      int dirMode, int bFilter) {
    int width2 = width << 1;

    int horMode = dirMode < 18;
    pixel neighbourBuf[129];
    const pixel *srcPix = srcPix0;

    if (horMode) {
        neighbourBuf[0] = srcPix[0];
        for (int i = 0; i < width << 1; i++) {
            neighbourBuf[1 + i] = srcPix[width2 + 1 + i];
            neighbourBuf[width2 + 1 + i] = srcPix[1 + i];
        }
        srcPix = neighbourBuf;
    }

    const int8_t angleTable[17] = {-32, -26, -21, -17, -13, -9, -5, -2, 0,
                                   2,   5,   9,   13,  17,  21, 26, 32};
    const int16_t invAngleTable[8] = {4096, 1638, 910, 630, 482, 390, 315, 256};

    int angleOffset = horMode ? 10 - dirMode : dirMode - 26;
    int angle = angleTable[8 + angleOffset];

    if (!angle) {
        for (int y = 0; y < width; y++)
            for (int x = 0; x < width; x++)
                dst[y * dstStride + x] = srcPix[1 + x];

        if (bFilter) {
            int topLeft = srcPix[0], top = srcPix[1];
            for (int y = 0; y < width; y++)
                dst[y * dstStride] = x265_clip(
                    (int16_t)(top + ((srcPix[width2 + 1 + y] - topLeft) >> 1)));
        }
    } else {

        pixel refBuf[64];
        const pixel *ref;

        if (angle < 0) {

            int nbProjected = -((width * angle) >> 5) - 1;
            pixel *ref_pix = refBuf + nbProjected + 1;

            int invAngle = invAngleTable[-angleOffset - 1];
            int invAngleSum = 128;
            for (int i = 0; i < nbProjected; i++) {
                invAngleSum += invAngle;
                ref_pix[-2 - i] = srcPix[width2 + (invAngleSum >> 8)];
            }

            for (int i = 0; i < width + 1; i++)
                ref_pix[-1 + i] = srcPix[i];
            ref = ref_pix;
        } else
            ref = srcPix + 1;

        int angleSum = 0;
        for (int y = 0; y < width; y++) {
            angleSum += angle;
            int offset = angleSum >> 5;
            int fraction = angleSum & 31;

            if (fraction)
                for (int x = 0; x < width; x++)
                    dst[y * dstStride + x] =
                        (pixel)(((32 - fraction) * ref[offset + x] +
                                 fraction * ref[offset + x + 1] + 16) >>
                                5);
            else
                for (int x = 0; x < width; x++)
                    dst[y * dstStride + x] = ref[offset + x];
        }
    }

    if (horMode) {
        for (int y = 0; y < width - 1; y++) {
            for (int x = y + 1; x < width; x++) {
                pixel tmp = dst[y * dstStride + x];
                dst[y * dstStride + x] = dst[x * dstStride + y];
                dst[x * dstStride + y] = tmp;
            }
        }
    }
}
} // namespace

/*
 * Wrapper for invoking the extracted kernel.
 */
void intra_ang_horizontal_transpose(pixel *dst, std::intptr_t dstStride,
                                    const pixel *srcPix) {
    intra_pred_ang_c<16>(dst, dstStride, srcPix, 10, 0);
}
