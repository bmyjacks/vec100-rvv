/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/predict.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/predict.cpp
 *
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
 *
 */

#include "kernel.h"

/*
 * source/common/predict.cpp:717-876
 */
void Predict::fillReferenceSamples(const pixel *adiOrigin, intptr_t picStride,
                                   const IntraNeighbors &intraNeighbors,
                                   pixel dst[258]) {
    const pixel dcValue = (pixel)(1 << (X265_DEPTH - 1));
    int numIntraNeighbor = intraNeighbors.numIntraNeighbor;
    int totalUnits = intraNeighbors.totalUnits;
    uint32_t tuSize = 1 << intraNeighbors.log2TrSize;
    uint32_t refSize = tuSize * 2 + 1;

    if (numIntraNeighbor == 0) {

        for (uint32_t i = 0; i < refSize; i++)
            dst[i] = dcValue;

        for (uint32_t i = 0; i < refSize - 1; i++)
            dst[i + refSize] = dcValue;
    } else if (numIntraNeighbor == totalUnits) {

        const pixel *adiTemp = adiOrigin - picStride - 1;
        memcpy(dst, adiTemp, refSize * sizeof(pixel));

        adiTemp = adiOrigin - 1;
        for (uint32_t i = 0; i < refSize - 1; i++) {
            dst[i + refSize] = adiTemp[0];
            adiTemp += picStride;
        }
    } else {
        const bool *bNeighborFlags = intraNeighbors.bNeighborFlags;
        const bool *pNeighborFlags;
        int aboveUnits = intraNeighbors.aboveUnits;
        int leftUnits = intraNeighbors.leftUnits;
        int unitWidth = intraNeighbors.unitWidth;
        int unitHeight = intraNeighbors.unitHeight;
        int totalSamples =
            (leftUnits * unitHeight) + ((aboveUnits + 1) * unitWidth);
        pixel adiLineBuffer[5 * MAX_CU_SIZE];
        pixel *adi;

        for (int i = 0; i < totalSamples; i++)
            adiLineBuffer[i] = dcValue;

        const pixel *adiTemp = adiOrigin - picStride - 1;
        adi = adiLineBuffer + (leftUnits * unitHeight);
        pNeighborFlags = bNeighborFlags + leftUnits;
        if (*pNeighborFlags) {
            pixel topLeftVal = adiTemp[0];
            for (int i = 0; i < unitWidth; i++)
                adi[i] = topLeftVal;
        }

        adiTemp += picStride;
        adi--;

        for (int j = 0; j < leftUnits * unitHeight; j++) {
            adi[-j] = adiTemp[j * picStride];
        }

        adiTemp = adiOrigin - picStride;
        adi = adiLineBuffer + (leftUnits * unitHeight) + unitWidth;

        memcpy(adi, adiTemp, aboveUnits * unitWidth * sizeof(*adiTemp));

        int curr = 0;
        int next = 1;
        adi = adiLineBuffer;
        int pAdiLineTopRowOffset = leftUnits * (unitHeight - unitWidth);
        if (!bNeighborFlags[0]) {

            while (next < totalUnits && !bNeighborFlags[next])
                next++;

            pixel *pAdiLineNext =
                adiLineBuffer + ((next < leftUnits) ? (next * unitHeight)
                                                    : (pAdiLineTopRowOffset +
                                                       (next * unitWidth)));
            const pixel refSample = *pAdiLineNext;

            int nextOrTop = X265_MIN(next, leftUnits);

            if (curr < nextOrTop) {
                const int fillSize = unitHeight * (nextOrTop - curr);
                memset(adi, refSample, fillSize * sizeof(pixel));
                curr = nextOrTop;
                adi += fillSize;
            }

            if (curr < next) {
                const int fillSize = unitWidth * (next - curr);
                memset(adi, refSample, fillSize * sizeof(pixel));
                curr = next;
                adi += fillSize;
            }
        }

        while (curr < totalUnits) {
            if (!bNeighborFlags[curr]) {
                int numSamplesInCurrUnit =
                    (curr >= leftUnits) ? unitWidth : unitHeight;
                const pixel refSample = *(adi - 1);
                for (int i = 0; i < numSamplesInCurrUnit; i++)
                    adi[i] = refSample;

                adi += numSamplesInCurrUnit;
                curr++;
            } else {
                adi += (curr >= leftUnits) ? unitWidth : unitHeight;
                curr++;
            }
        }

        adi = adiLineBuffer + refSize + unitWidth - 2;
        memcpy(dst, adi, refSize * sizeof(pixel));

        adi = adiLineBuffer + refSize - 1;
        for (int i = 0; i < (int)refSize - 1; i++)
            dst[i + refSize] = adi[-(i + 1)];
    }
}
