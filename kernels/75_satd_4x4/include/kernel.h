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

#ifndef KERNELS_75_SATD_4X4_INCLUDE_KERNEL_H_
#define KERNELS_75_SATD_4X4_INCLUDE_KERNEL_H_

#include <stdint.h>

/*
 * source/common/common.h:136-142 (8-bit configuration)
 */
typedef uint8_t pixel;
typedef uint16_t sum_t;
typedef uint32_t sum2_t;

/*
 * Wrapper for invoking the extracted kernel.
 */
int satd_4x4_bench(const pixel *pix1, intptr_t stride_pix1, const pixel *pix2,
                   intptr_t stride_pix2);

#endif // KERNELS_75_SATD_4X4_INCLUDE_KERNEL_H_
