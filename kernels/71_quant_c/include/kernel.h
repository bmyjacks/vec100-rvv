/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/common.h
 *
 *
 *  The original file copyright and license notices follow.
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

#ifndef KERNELS_71_QUANT_C_INCLUDE_KERNEL_H_
#define KERNELS_71_QUANT_C_INCLUDE_KERNEL_H_

#include <stdint.h>

/*
 * source/common/common.h:122-124
 */
#define X265_CHECK(expr, ...)

/*
 * source/common/common.h:167-171
 */
template <typename T> inline T x265_min(T a, T b) { return a < b ? a : b; }

template <typename T> inline T x265_max(T a, T b) { return a > b ? a : b; }

/*
 * source/common/common.h:173-174
 */
template <typename T> inline T x265_clip3(T minVal, T maxVal, T a) {
    return x265_min(x265_max(minVal, a), maxVal);
}

/*
 * Wrapper for invoking the extracted kernel.
 */
uint32_t quant_c_bench(const int16_t *coef, const int32_t *quantCoeff,
                       int32_t *deltaU, int16_t *qCoef, int qBits, int add,
                       int numCoeff);

#endif // KERNELS_71_QUANT_C_INCLUDE_KERNEL_H_
