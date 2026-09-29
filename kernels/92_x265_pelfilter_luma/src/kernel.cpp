/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/deblock.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 * source/common/deblock.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Author: Gopu Govindaswamy <gopu@multicorewareinc.com>
 *         Min Chen <chenm003@163.com>
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
#include <cstdlib>

/* source/common/deblock.cpp:278-315 */
static inline void pelFilterLuma(pixel* src, intptr_t srcStep, intptr_t offset, int32_t tc, int32_t maskP, int32_t maskQ,
                                 int32_t maskP1, int32_t maskQ1)
{
    int32_t thrCut = tc * 10;
    int32_t tc2 = tc >> 1;
    maskP1 &= maskP;
    maskQ1 &= maskQ;

    for (int32_t i = 0; i < UNIT_SIZE; i++, src += srcStep)
    {
        int16_t m4  = (int16_t)src[0];
        int16_t m3  = (int16_t)src[-offset];
        int16_t m5  = (int16_t)src[offset];
        int16_t m2  = (int16_t)src[-offset * 2];

        int32_t delta = (9 * (m4 - m3) - 3 * (m5 - m2) + 8) >> 4;

        if (abs(delta) < thrCut)
        {
            delta = x265_clip3(-tc, tc, delta);

            src[-offset] = x265_clip(m3 + (delta & maskP));
            src[0] = x265_clip(m4 - (delta & maskQ));
            if (maskP1)
            {
                int16_t m1  = (int16_t)src[-offset * 3];
                int32_t delta1 = x265_clip3(-tc2, tc2, ((((m1 + m3 + 1) >> 1) - m2 + delta) >> 1));
                src[-offset * 2] = x265_clip(m2 + delta1);
            }
            if (maskQ1)
            {
                int16_t m6  = (int16_t)src[offset * 2];
                int32_t delta2 = x265_clip3(-tc2, tc2, ((((m6 + m4 + 1) >> 1) - m5 - delta) >> 1));
                src[offset] = x265_clip(m5 + delta2);
            }
        }
    }
}

/* Wrapper for invoking the extracted kernel. */
void pelFilterLuma_bench(pixel* src, intptr_t srcStep, intptr_t offset, int32_t tc,
                         int32_t maskP, int32_t maskQ, int32_t maskP1, int32_t maskQ1)
{
    pelFilterLuma(src, srcStep, offset, tc, maskP, maskQ, maskP1, maskQ1);
}
