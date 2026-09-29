/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/common.h
 *    source/encoder/slicetype.h
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
 *
 * source/encoder/slicetype.h
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Steve Borho <steve@borho.org>
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

#ifndef KERNELS_17_ESTIMATECUPROPAGATECOST_INCLUDE_KERNEL_H_
#define KERNELS_17_ESTIMATECUPROPAGATECOST_INCLUDE_KERNEL_H_

#include <cstdint>

/*
 * source/common/common.h:181
 */
#define X265_MIN(a, b) ((a) < (b) ? (a) : (b))

/*
 * source/encoder/slicetype.h:41
 */
#define LOWRES_COST_MASK ((1 << 14) - 1)

/*
 * Wrapper for invoking the extracted kernel.
 */
void estimateCUPropagateCost_bench_isolated(int *dst, const uint16_t *propagateIn,
                                   const int32_t *intraCosts,
                                   const uint16_t *interCosts,
                                   const int32_t *invQscales,
                                   const double *fpsFactor, int len);

#endif // KERNELS_17_ESTIMATECUPROPAGATECOST_INCLUDE_KERNEL_H_
