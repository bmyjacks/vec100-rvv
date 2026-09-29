/****************************************************************************
 * Project: x265 3.4
 * Source files: source/common/common.h, source/common/picyuv.h
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
 *
 * source/common/picyuv.h
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Steve Borho <steve@borho.org>
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
#ifndef KERNELS_93_X265_PLANE_STATISTICS_INCLUDE_KERNEL_H_
#define KERNELS_93_X265_PLANE_STATISTICS_INCLUDE_KERNEL_H_

#include <cstdint>

/* source/common/common.h:135 (X265_DEPTH=8) */
typedef uint8_t pixel;

/* Wrapper for the selected fields of source/common/picyuv.h:64-74. */
struct PlaneStatistics {
    pixel maxY, minY;
    double avgY;
    pixel maxU, minU;
    double avgU;
    pixel maxV, minV;
    double avgV;
};

/* Wrapper for source/common/picyuv.cpp:374-411. Strides count pixels.
 * When chroma is false, U/V and their statistics are not touched. */
void plane_statistics(const pixel *y, const pixel *u, const pixel *v,
                      std::intptr_t strideY, std::intptr_t strideC, int width,
                      int height, uint32_t picWidth, uint32_t picHeight,
                      uint32_t hShift, uint32_t vShift, bool chroma,
                      PlaneStatistics *stats);
void plane_statistics_rvv(const pixel *y, const pixel *u, const pixel *v,
                          std::intptr_t strideY, std::intptr_t strideC,
                          int width, int height, uint32_t picWidth,
                          uint32_t picHeight, uint32_t hShift, uint32_t vShift,
                          bool chroma, PlaneStatistics *stats);

#endif // KERNELS_93_X265_PLANE_STATISTICS_INCLUDE_KERNEL_H_
