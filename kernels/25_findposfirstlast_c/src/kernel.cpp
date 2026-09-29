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

/*
 * source/common/dct.cpp:791-830
 */
static uint32_t findPosFirstLast_c_impl(const int16_t *dstCoeff,
                                        const intptr_t trSize,
                                        const uint16_t scanTbl[16]) {
    int n;

    for (n = SCAN_SET_SIZE - 1; n >= 0; n--) {
        const uint32_t idx = scanTbl[n];
        const uint32_t idxY = idx / MLS_CG_SIZE;
        const uint32_t idxX = idx % MLS_CG_SIZE;
        if (dstCoeff[idxY * trSize + idxX])
            break;
    }

    X265_CHECK(n >= -1, "non-zero coeff scan failuare!\n");

    uint32_t lastNZPosInCG = (uint32_t)n;

    for (n = 0; n < SCAN_SET_SIZE; n++) {
        const uint32_t idx = scanTbl[n];
        const uint32_t idxY = idx / MLS_CG_SIZE;
        const uint32_t idxX = idx % MLS_CG_SIZE;
        if (dstCoeff[idxY * trSize + idxX])
            break;
    }

    uint32_t firstNZPosInCG = (uint32_t)n;

    uint32_t absSumSign = 0;
    for (n = firstNZPosInCG; n <= (int)lastNZPosInCG; n++) {
        const uint32_t idx = scanTbl[n];
        const uint32_t idxY = idx / MLS_CG_SIZE;
        const uint32_t idxX = idx % MLS_CG_SIZE;
        absSumSign += dstCoeff[idxY * trSize + idxX];
    }

    return ((absSumSign << 31) | (lastNZPosInCG << 8) | firstNZPosInCG);
}

/*
 * Wrapper for invoking the extracted kernel.
 */
uint32_t findPosFirstLast_c(const int16_t *c, std::intptr_t s,
                            const uint16_t *t) {
    return findPosFirstLast_c_impl(c, s, t);
}
