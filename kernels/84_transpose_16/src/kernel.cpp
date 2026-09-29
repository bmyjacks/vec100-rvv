/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/pixel.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/pixel.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Steve Borho <steve@borho.org>
 *          Mandar Gurav <mandar@multicorewareinc.com>
 *          Mahesh Pittala <mahesh@multicorewareinc.com>
 *          Min Chen <min.chen@multicorewareinc.com>
 *          Hongbin Liu<liuhongbin1@huawei.com>
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

namespace {
/*
 * source/common/pixel.cpp:485-491
 */
template <int blockSize>
void transpose(pixel *dst, const pixel *src, intptr_t stride) {
    for (int k = 0; k < blockSize; k++)
        for (int l = 0; l < blockSize; l++)
            dst[k * blockSize + l] = src[l * stride + k];
}
} // namespace

/*
 * Wrapper for invoking the extracted kernel.
 */
void transpose_16(pixel *dst, const pixel *src, intptr_t stride) {
    transpose<16>(dst, src, stride);
}
