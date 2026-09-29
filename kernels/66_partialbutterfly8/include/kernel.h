/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/constants.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/constants.h
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

#ifndef KERNELS_66_PARTIALBUTTERFLY8_INCLUDE_KERNEL_H_
#define KERNELS_66_PARTIALBUTTERFLY8_INCLUDE_KERNEL_H_

#include <stdint.h>

/*
 * source/common/constants.h:60
 */
extern const int16_t g_t8[8][8];

/*
 * Wrapper for invoking the extracted kernel.
 */
void partialButterfly8_bench(const int16_t *src, int16_t *dst, int shift,
                             int line);

#endif // KERNELS_66_PARTIALBUTTERFLY8_INCLUDE_KERNEL_H_
