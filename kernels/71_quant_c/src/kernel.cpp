/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/dct.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/dct.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Mandar Gurav <mandar@multicorewareinc.com>
 *          Deepthi Devaki Akkoorath <deepthidevaki@multicorewareinc.com>
 *          Mahesh Pittala <mahesh@multicorewareinc.com>
 *          Rajesh Paulraj <rajesh@multicorewareinc.com>
 *          Min Chen <min.chen@multicorewareinc.com>
 *          Praveen Kumar Tiwari <praveen@multicorewareinc.com>
 *          Nabajit Deka <nabajit@multicorewareinc.com>
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

#include <cstdlib>

/*
 * source/common/dct.cpp:664-686
 */
static uint32_t quant_c(const int16_t *coef, const int32_t *quantCoeff,
                        int32_t *deltaU, int16_t *qCoef, int qBits, int add,
                        int numCoeff) {
    X265_CHECK(qBits >= 8, "qBits less than 8\n");
    X265_CHECK((numCoeff % 16) == 0, "numCoeff must be multiple of 16\n");
    int qBits8 = qBits - 8;
    uint32_t numSig = 0;

    for (int blockpos = 0; blockpos < numCoeff; blockpos++) {
        int level = coef[blockpos];
        int sign = (level < 0 ? -1 : 1);

        int tmplevel = abs(level) * quantCoeff[blockpos];
        level = ((tmplevel + add) >> qBits);
        deltaU[blockpos] = ((tmplevel - (level << qBits)) >> qBits8);
        if (level)
            ++numSig;
        level *= sign;
        qCoef[blockpos] = (int16_t)x265_clip3(-32768, 32767, level);
    }

    return numSig;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
uint32_t quant_c_bench(const int16_t *coef, const int32_t *quantCoeff,
                       int32_t *deltaU, int16_t *qCoef, int qBits, int add,
                       int numCoeff) {
    return quant_c(coef, quantCoeff, deltaU, qCoef, qBits, add, numCoeff);
}
