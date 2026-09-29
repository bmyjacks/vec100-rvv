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
 * source/common/pixel.cpp:188
 */
#define BITS_PER_SUM (8 * sizeof(sum_t))

/*
 * source/common/pixel.cpp:190-199
 */
#define HADAMARD4(d0, d1, d2, d3, s0, s1, s2, s3)                              \
    {                                                                          \
        sum2_t t0 = s0 + s1;                                                   \
        sum2_t t1 = s0 - s1;                                                   \
        sum2_t t2 = s2 + s3;                                                   \
        sum2_t t3 = s2 - s3;                                                   \
        d0 = t0 + t2;                                                          \
        d2 = t0 - t2;                                                          \
        d1 = t1 + t3;                                                          \
        d3 = t1 - t3;                                                          \
    }

/*
 * source/common/pixel.cpp:203-208
 */
inline sum2_t abs2(sum2_t a) {
    sum2_t s = ((a >> (BITS_PER_SUM - 1)) & (((sum2_t)1 << BITS_PER_SUM) + 1)) *
               ((sum_t)-1);

    return (a + s) ^ s;
}

/*
 * source/common/pixel.cpp:210-236
 */
static int satd_4x4(const pixel *pix1, intptr_t stride_pix1, const pixel *pix2,
                    intptr_t stride_pix2) {
    sum2_t tmp[4][2];
    sum2_t a0, a1, a2, a3, b0, b1;
    sum2_t sum = 0;

    for (int i = 0; i < 4; i++, pix1 += stride_pix1, pix2 += stride_pix2) {
        a0 = pix1[0] - pix2[0];
        a1 = pix1[1] - pix2[1];
        b0 = (a0 + a1) + ((a0 - a1) << BITS_PER_SUM);
        a2 = pix1[2] - pix2[2];
        a3 = pix1[3] - pix2[3];
        b1 = (a2 + a3) + ((a2 - a3) << BITS_PER_SUM);
        tmp[i][0] = b0 + b1;
        tmp[i][1] = b0 - b1;
    }

    for (int i = 0; i < 2; i++) {
        HADAMARD4(a0, a1, a2, a3, tmp[0][i], tmp[1][i], tmp[2][i], tmp[3][i]);
        a0 = abs2(a0) + abs2(a1) + abs2(a2) + abs2(a3);
        sum += ((sum_t)a0) + (a0 >> BITS_PER_SUM);
    }

    return (int)(sum >> 1);
}
/*
 * Wrapper for invoking the extracted kernel.
 */
int satd_4x4_bench(const pixel *pix1, intptr_t stride_pix1, const pixel *pix2,
                   intptr_t stride_pix2) {
    return satd_4x4(pix1, stride_pix1, pix2, stride_pix2);
}
