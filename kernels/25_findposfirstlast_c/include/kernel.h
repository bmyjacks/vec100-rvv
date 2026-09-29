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

#ifndef KERNELS_25_FINDPOSFIRSTLAST_C_INCLUDE_KERNEL_H_
#define KERNELS_25_FINDPOSFIRSTLAST_C_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

/*
 * source/common/common.h:290
 */
#define MLS_CG_SIZE 4

/*
 * source/common/common.h:304
 */
#define SCAN_SET_SIZE 16

/*
 * source/common/common.h:123
 */
#define X265_CHECK(expr, ...)

/*
 * Wrapper for invoking the extracted kernel.
 */
uint32_t findPosFirstLast_c(const int16_t *c, std::intptr_t s,
                            const uint16_t *t);

#endif // KERNELS_25_FINDPOSFIRSTLAST_C_INCLUDE_KERNEL_H_
