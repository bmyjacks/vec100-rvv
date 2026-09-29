/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/encoder/slicetype.cpp
 *    source/common/common.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/encoder/slicetype.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Gopu Govindaswamy <gopu@multicorewareinc.com>
 *          Steve Borho <steve@borho.org>
 *          Ashok Kumar Mishra <ashok@multicorewareinc.com>
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
 * source/common/common.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Deepthi Nandakumar <deepthi@multicorewareinc.com>
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

#include <cstdarg>
#include <cstdio>

/*
 * source/encoder/slicetype.cpp:90-149
 */
bool computeEdge(pixel *edgePic, pixel *refPic, pixel *edgeTheta,
                 intptr_t stride, int height, int width, bool bcalcTheta,
                 pixel whitePixel) {
    intptr_t rowOne = 0, rowTwo = 0, rowThree = 0, colOne = 0, colTwo = 0,
             colThree = 0;
    intptr_t middle = 0, topLeft = 0, topRight = 0, bottomLeft = 0,
             bottomRight = 0;

    const int startIndex = 1;

    if (!edgePic || !refPic || (!edgeTheta && bcalcTheta)) {
        return false;
    } else {
        float gradientH = 0, gradientV = 0, radians = 0, theta = 0;
        float gradientMagnitude = 0;
        pixel blackPixel = 0;

        height = height - startIndex;
        width = width - startIndex;
        for (int rowNum = startIndex; rowNum < height; rowNum++) {
            rowTwo = rowNum * stride;
            rowOne = rowTwo - stride;
            rowThree = rowTwo + stride;

            for (int colNum = startIndex; colNum < width; colNum++) {

                colTwo = colNum;
                colOne = colTwo - startIndex;
                colThree = colTwo + startIndex;
                middle = rowTwo + colTwo;
                topLeft = rowOne + colOne;
                topRight = rowOne + colThree;
                bottomLeft = rowThree + colOne;
                bottomRight = rowThree + colThree;
                gradientH =
                    (float)(-3 * refPic[topLeft] + 3 * refPic[topRight] -
                            10 * refPic[rowTwo + colOne] +
                            10 * refPic[rowTwo + colThree] -
                            3 * refPic[bottomLeft] + 3 * refPic[bottomRight]);
                gradientV =
                    (float)(-3 * refPic[topLeft] -
                            10 * refPic[rowOne + colTwo] -
                            3 * refPic[topRight] + 3 * refPic[bottomLeft] +
                            10 * refPic[rowThree + colTwo] +
                            3 * refPic[bottomRight]);
                gradientMagnitude =
                    sqrtf(gradientH * gradientH + gradientV * gradientV);
                if (bcalcTheta) {
                    edgeTheta[middle] = 0;
                    radians = atan2(gradientV, gradientH);
                    theta = (float)((radians * 180) / PI);
                    if (theta < 0)
                        theta = 180 + theta;
                    edgeTheta[middle] = (pixel)theta;
                }
                edgePic[middle] =
                    (pixel)(gradientMagnitude >= EDGE_THRESHOLD ? whitePixel
                                                                : blackPixel);
            }
        }
        return true;
    }
}

/*
 * source/encoder/slicetype.cpp:151-215
 */
void edgeFilter_isolated(Frame *curFrame, x265_param *param) {
    int height = curFrame->m_fencPic->m_picHeight;
    int width = curFrame->m_fencPic->m_picWidth;
    intptr_t stride = curFrame->m_fencPic->m_stride;
    uint32_t numCuInHeight = (height + param->maxCUSize - 1) / param->maxCUSize;
    int maxHeight = numCuInHeight * param->maxCUSize;

    memset(curFrame->m_edgePic, 0,
           stride * (maxHeight + (curFrame->m_fencPic->m_lumaMarginY * 2)) *
               sizeof(pixel));
    memset(curFrame->m_gaussianPic, 0,
           stride * (maxHeight + (curFrame->m_fencPic->m_lumaMarginY * 2)) *
               sizeof(pixel));
    memset(curFrame->m_thetaPic, 0,
           stride * (maxHeight + (curFrame->m_fencPic->m_lumaMarginY * 2)) *
               sizeof(pixel));

    pixel *src = (pixel *)curFrame->m_fencPic->m_picOrg[0];
    pixel *edgePic = curFrame->m_edgePic +
                     curFrame->m_fencPic->m_lumaMarginY * stride +
                     curFrame->m_fencPic->m_lumaMarginX;
    pixel *refPic = curFrame->m_gaussianPic +
                    curFrame->m_fencPic->m_lumaMarginY * stride +
                    curFrame->m_fencPic->m_lumaMarginX;
    pixel *edgeTheta = curFrame->m_thetaPic +
                       curFrame->m_fencPic->m_lumaMarginY * stride +
                       curFrame->m_fencPic->m_lumaMarginX;

    for (int i = 0; i < height; i++) {
        memcpy(edgePic, src, width * sizeof(pixel));
        memcpy(refPic, src, width * sizeof(pixel));
        src += stride;
        edgePic += stride;
        refPic += stride;
    }

    src = (pixel *)curFrame->m_fencPic->m_picOrg[0];
    refPic = curFrame->m_gaussianPic +
             curFrame->m_fencPic->m_lumaMarginY * stride +
             curFrame->m_fencPic->m_lumaMarginX;
    edgePic = curFrame->m_edgePic +
              curFrame->m_fencPic->m_lumaMarginY * stride +
              curFrame->m_fencPic->m_lumaMarginX;
    pixel pixelValue = 0;

    for (int rowNum = 0; rowNum < height; rowNum++) {
        for (int colNum = 0; colNum < width; colNum++) {
            if ((rowNum >= 2) && (colNum >= 2) && (rowNum != height - 2) &&
                (colNum != width - 2)) {

                const intptr_t rowOne = (rowNum - 2) * stride,
                               colOne = colNum - 2;
                const intptr_t rowTwo = (rowNum - 1) * stride,
                               colTwo = colNum - 1;
                const intptr_t rowThree = rowNum * stride, colThree = colNum;
                const intptr_t rowFour = (rowNum + 1) * stride,
                               colFour = colNum + 1;
                const intptr_t rowFive = (rowNum + 2) * stride,
                               colFive = colNum + 2;
                const intptr_t index = (rowNum * stride) + colNum;

                pixelValue =
                    ((2 * src[rowOne + colOne] + 4 * src[rowOne + colTwo] +
                      5 * src[rowOne + colThree] + 4 * src[rowOne + colFour] +
                      2 * src[rowOne + colFive] + 4 * src[rowTwo + colOne] +
                      9 * src[rowTwo + colTwo] + 12 * src[rowTwo + colThree] +
                      9 * src[rowTwo + colFour] + 4 * src[rowTwo + colFive] +
                      5 * src[rowThree + colOne] + 12 * src[rowThree + colTwo] +
                      15 * src[rowThree + colThree] +
                      12 * src[rowThree + colFour] +
                      5 * src[rowThree + colFive] + 4 * src[rowFour + colOne] +
                      9 * src[rowFour + colTwo] + 12 * src[rowFour + colThree] +
                      9 * src[rowFour + colFour] + 4 * src[rowFour + colFive] +
                      2 * src[rowFive + colOne] + 4 * src[rowFive + colTwo] +
                      5 * src[rowFive + colThree] + 4 * src[rowFive + colFour] +
                      2 * src[rowFive + colFive]) /
                     159);
                refPic[index] = pixelValue;
            }
        }
    }

    if (!computeEdge(edgePic, refPic, edgeTheta, stride, height, width, true))
        x265_log(NULL, X265_LOG_ERROR, "Failed edge computation!");
}

/*
 * source/common/common.cpp:105-142
 */
void general_log(const x265_param *param, const char *caller, int level,
                 const char *fmt, ...) {
    if (param && level > param->logLevel)
        return;
    const int bufferSize = 4096;
    char buffer[bufferSize];
    int p = 0;
    const char *log_level;
    switch (level) {
    case X265_LOG_ERROR:
        log_level = "error";
        break;
    case X265_LOG_WARNING:
        log_level = "warning";
        break;
    case X265_LOG_INFO:
        log_level = "info";
        break;
    case X265_LOG_DEBUG:
        log_level = "debug";
        break;
    case X265_LOG_FULL:
        log_level = "full";
        break;
    default:
        log_level = "unknown";
        break;
    }

    if (caller)
        p += sprintf(buffer, "%-4s [%s]: ", caller, log_level);
    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buffer + p, bufferSize - p, fmt, arg);
    va_end(arg);
    fputs(buffer, stderr);
}
