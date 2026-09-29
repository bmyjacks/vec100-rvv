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

/*
 * source/common/pixel.cpp:393-399
 */
template <int size>
static void blockfill_s_c(int16_t *dst, intptr_t dstride, int16_t val) {
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            dst[y * dstride + x] = val;
}

/*
 * Wrapper for invoking the extracted kernel; pixel.cpp:1055-1063,1159
 * selects blockfill_s_c<16> for both alignment types.
 */
void blockfill_s_16(int16_t *dst, intptr_t dstride, int16_t val) {
    blockfill_s_c<16>(dst, dstride, val);
}
