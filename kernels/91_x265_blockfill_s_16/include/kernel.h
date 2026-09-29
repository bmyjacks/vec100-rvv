/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/primitives.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/primitives.h
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Steve Borho <steve@borho.org>
 *          Mandar Gurav <mandar@multicorewareinc.com>
 *          Deepthi Devaki Akkoorath <deepthidevaki@multicorewareinc.com>
 *          Mahesh Pittala <mahesh@multicorewareinc.com>
 *          Rajesh Paulraj <rajesh@multicorewareinc.com>
 *          Praveen Kumar Tiwari <praveen@multicorewareinc.com>
 *          Min Chen <chenm003@163.com>
 *          Hongbin Liu<liuhongbin1@huawei.com>
 *          Yimeng Su <yimeng.su@huawei.com>
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

#ifndef KERNELS_91_X265_BLOCKFILL_S_16_INCLUDE_KERNEL_H_
#define KERNELS_91_X265_BLOCKFILL_S_16_INCLUDE_KERNEL_H_

#include <cstdint>

/*
 * source/common/primitives.h:141
 */
typedef void (*blockfill_s_t)(int16_t *dst, intptr_t dstride, int16_t val);

/*
 * Wrapper for invoking the extracted 16x16 scalar primitive.
 */
void blockfill_s_16(int16_t *dst, intptr_t dstride, int16_t val);

/*
 * Wrapper for invoking the separate RVV implementation.
 */
void blockfill_s_16_rvv(int16_t *dst, intptr_t dstride, int16_t val);

#endif // KERNELS_91_X265_BLOCKFILL_S_16_INCLUDE_KERNEL_H_
