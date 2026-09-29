/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/constants.cpp
 *    source/common/dct.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/constants.cpp
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
 * source/common/dct.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Mandar Gurav <mandar@multicorewareinc.com>
 *          Deepthi Devaki Akkoorath <deepthidevaki@multicorewareinc.com>
 *          Mahesh Pittala <mahesh@multicorewareinc.com>
 *          Rajesh Paulraj <rajesh@multicorewareinc.com>
 *          Min Chen <min.chen@multicorewareinc.com>
 *          Praveen Kumar Tiwari <praveen@multicorewareinc.com>
 *          Nabajit Deka <nabajit@multicorewareinc.com>
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
 * source/common/constants.cpp:278-288
 */
const int16_t g_t8[8][8] = {
    {64, 64, 64, 64, 64, 64, 64, 64},     {89, 75, 50, 18, -18, -50, -75, -89},
    {83, 36, -36, -83, -83, -36, 36, 83}, {75, -18, -89, -50, 50, 89, 18, -75},
    {64, -64, -64, 64, 64, -64, -64, 64}, {50, -89, 18, 75, -75, -18, 89, -50},
    {36, -83, 83, -36, -36, 83, -83, 36}, {18, -50, 75, -89, 89, -75, 50, -18}};

/*
 * source/common/dct.cpp:205-240
 */
static void partialButterfly8(const int16_t *src, int16_t *dst, int shift,
                              int line) {
    int j, k;
    int E[4], O[4];
    int EE[2], EO[2];
    int add = 1 << (shift - 1);

    for (j = 0; j < line; j++) {
        for (k = 0; k < 4; k++) {
            E[k] = src[k] + src[7 - k];
            O[k] = src[k] - src[7 - k];
        }

        EE[0] = E[0] + E[3];
        EO[0] = E[0] - E[3];
        EE[1] = E[1] + E[2];
        EO[1] = E[1] - E[2];

        dst[0] =
            (int16_t)((g_t8[0][0] * EE[0] + g_t8[0][1] * EE[1] + add) >> shift);
        dst[4 * line] =
            (int16_t)((g_t8[4][0] * EE[0] + g_t8[4][1] * EE[1] + add) >> shift);
        dst[2 * line] =
            (int16_t)((g_t8[2][0] * EO[0] + g_t8[2][1] * EO[1] + add) >> shift);
        dst[6 * line] =
            (int16_t)((g_t8[6][0] * EO[0] + g_t8[6][1] * EO[1] + add) >> shift);

        dst[line] = (int16_t)((g_t8[1][0] * O[0] + g_t8[1][1] * O[1] +
                               g_t8[1][2] * O[2] + g_t8[1][3] * O[3] + add) >>
                              shift);
        dst[3 * line] =
            (int16_t)((g_t8[3][0] * O[0] + g_t8[3][1] * O[1] +
                       g_t8[3][2] * O[2] + g_t8[3][3] * O[3] + add) >>
                      shift);
        dst[5 * line] =
            (int16_t)((g_t8[5][0] * O[0] + g_t8[5][1] * O[1] +
                       g_t8[5][2] * O[2] + g_t8[5][3] * O[3] + add) >>
                      shift);
        dst[7 * line] =
            (int16_t)((g_t8[7][0] * O[0] + g_t8[7][1] * O[1] +
                       g_t8[7][2] * O[2] + g_t8[7][3] * O[3] + add) >>
                      shift);

        src += 8;
        dst++;
    }
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void partialButterfly8_bench(const int16_t *src, int16_t *dst, int shift,
                             int line) {
    partialButterfly8(src, dst, shift, line);
}
