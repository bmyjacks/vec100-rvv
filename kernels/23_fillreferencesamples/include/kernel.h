/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/predict.h
 *    source/common/common.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/predict.h
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
 *
 * source/common/common.h
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

#ifndef KERNELS_23_FILLREFERENCESAMPLES_INCLUDE_KERNEL_H_
#define KERNELS_23_FILLREFERENCESAMPLES_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>
#include <cstring>

/*
 * source/common/common.h:135
 */
typedef uint8_t pixel;

/*
 * source/common/common.h:181
 */
#define X265_MIN(a, b) ((a) < (b) ? (a) : (b))

/*
 * source/common/common.h:255
 */
#define MAX_LOG2_CU_SIZE 6

/*
 * source/common/common.h:257
 */
#define MAX_CU_SIZE (1 << MAX_LOG2_CU_SIZE)

/*
 * source/common/common.h:266
 */
#define MIN_PU_SIZE 4

/*
 * source/common/common.h:268
 */
#define MAX_NUM_SPU_W (MAX_CU_SIZE / MIN_PU_SIZE)

/*
 * source/common/predict.h:51-110 (class Predict reduced to the IntraNeighbors
 * struct and the fillReferenceSamples declaration; the unused members and
 * methods are omitted)
 */
class Predict {
  public:
    struct IntraNeighbors {
        int numIntraNeighbor;
        int totalUnits;
        int aboveUnits;
        int leftUnits;
        int unitWidth;
        int unitHeight;
        int log2TrSize;
        bool bNeighborFlags[4 * MAX_NUM_SPU_W + 1];
    };

    static void fillReferenceSamples(const pixel *adiOrigin, intptr_t picStride,
                                     const IntraNeighbors &intraNeighbors,
                                     pixel dst[258]);
};

#endif // KERNELS_23_FILLREFERENCESAMPLES_INCLUDE_KERNEL_H_
