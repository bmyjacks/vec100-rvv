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
 * source/common/pixel.cpp:914-940
 */
static void estimateCUPropagateCost(int *dst, const uint16_t *propagateIn,
                                    const int32_t *intraCosts,
                                    const uint16_t *interCosts,
                                    const int32_t *invQscales,
                                    const double *fpsFactor, int len) {
    double fps = *fpsFactor / 256;
    for (int i = 0; i < len; i++) {
        int intraCost = intraCosts[i];
        int interCost =
            X265_MIN(intraCosts[i], interCosts[i] & LOWRES_COST_MASK);
        double propagateIntra = intraCost * invQscales[i];
        double propagateAmount = (double)propagateIn[i] + propagateIntra * fps;
        double propagateNum = (double)(intraCost - interCost);

        double propagateDenom = (double)intraCost;
        dst[i] = (int)(propagateAmount * propagateNum / propagateDenom + 0.5);
    }
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void estimateCUPropagateCost_bench_isolated(int *dst, const uint16_t *propagateIn,
                                   const int32_t *intraCosts,
                                   const uint16_t *interCosts,
                                   const int32_t *invQscales,
                                   const double *fpsFactor, int len) {
    estimateCUPropagateCost(dst, propagateIn, intraCosts, interCosts,
                            invQscales, fpsFactor, len);
}
