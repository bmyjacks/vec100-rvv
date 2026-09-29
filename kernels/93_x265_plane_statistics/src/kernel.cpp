/****************************************************************************
 * Project: x265 3.4
 * Source files: source/common/picyuv.cpp, source/common/common.h
 *
 * source/common/picyuv.cpp
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
 * source/common/common.h
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Deepthi Nandakumar <deepthi@multicorewareinc.com>
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
 */
#include "kernel.h"

/* source/common/common.h:181-182 */
#define X265_MIN(a, b) ((a) < (b) ? (a) : (b))
#define X265_MAX(a, b) ((a) > (b) ? (a) : (b))

/* Wrapper for the statistics region of PicYuv::copyFromPicture.
 * The original csvLogLevel/maxCLL/maxFALL guard selects this region;
 * chroma selects its original internalCsp != X265_CSP_I400 branch. */
void plane_statistics(const pixel *y, const pixel *u, const pixel *v,
                      std::intptr_t strideY, std::intptr_t strideC, int width,
                      int height, uint32_t picWidth, uint32_t picHeight,
                      uint32_t hShift, uint32_t vShift, bool chroma,
                      PlaneStatistics *stats) {
    /* source/common/picyuv.cpp:239-242 */
    uint64_t lumaSum;
    uint64_t cbSum;
    uint64_t crSum;
    lumaSum = cbSum = crSum = 0;
    /* source/common/picyuv.cpp:353-359 */
    const pixel *yPic = y;
    const pixel *uPic = u;
    const pixel *vPic = v;

    /* source/common/picyuv.cpp:374-387 */
    {
        for (int r = 0; r < height; r++) {
            for (int c = 0; c < width; c++) {
                stats->maxY = X265_MAX(yPic[c], stats->maxY);
                stats->minY = X265_MIN(yPic[c], stats->minY);
                lumaSum += yPic[c];
            }
            yPic += strideY;
        }
        stats->avgY = (double)lumaSum / (picHeight * picWidth);
    }
    /* source/common/picyuv.cpp:388-411 */
    if (chroma) {
        for (int r = 0; r < height >> vShift; r++) {
            for (int c = 0; c < width >> hShift; c++) {
                stats->maxU = X265_MAX(uPic[c], stats->maxU);
                stats->minU = X265_MIN(uPic[c], stats->minU);
                cbSum += uPic[c];

                stats->maxV = X265_MAX(vPic[c], stats->maxV);
                stats->minV = X265_MIN(vPic[c], stats->minV);
                crSum += vPic[c];
            }
            uPic += strideC;
            vPic += strideC;
        }
        stats->avgU = (double)cbSum / ((height >> vShift) * (width >> hShift));
        stats->avgV = (double)crSum / ((height >> vShift) * (width >> hShift));
    }
}
