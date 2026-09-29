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
#ifndef KERNELS_92_X265_PELFILTER_LUMA_INCLUDE_KERNEL_H_
#define KERNELS_92_X265_PELFILTER_LUMA_INCLUDE_KERNEL_H_

#include <cstdint>

/* source/common/common.h:135 (8-bit build) */
typedef uint8_t pixel;

/* source/common/common.h:167-177 */
template<typename T>
inline T x265_min(T a, T b) { return a < b ? a : b; }
template<typename T>
inline T x265_max(T a, T b) { return a > b ? a : b; }
template<typename T>
inline T x265_clip3(T minVal, T maxVal, T a) { return x265_min(x265_max(minVal, a), maxVal); }
template<typename T>
inline pixel x265_clip(T x) { return (pixel)x265_min<T>(T((1 << X265_DEPTH) - 1), x265_max<T>(T(0), x)); }

/* source/common/common.h:259-260 */
#define LOG2_UNIT_SIZE 2
#define UNIT_SIZE (1 << LOG2_UNIT_SIZE)

/* Wrapper for invoking the extracted kernel. */
void pelFilterLuma_bench(pixel* src, intptr_t srcStep, intptr_t offset, int32_t tc,
                         int32_t maskP, int32_t maskQ, int32_t maskP1, int32_t maskQ1);

#endif // KERNELS_92_X265_PELFILTER_LUMA_INCLUDE_KERNEL_H_
