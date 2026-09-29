/****************************************************************************
 * Project: x265 3.4, source/common/quant.cpp
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 * Authors: Steve Borho <steve@borho.org>, Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 */
#ifndef KERNELS_95_X265_RDO_RECOUNT_SIGN_INCLUDE_KERNEL_H_
#define KERNELS_95_X265_RDO_RECOUNT_SIGN_INCLUDE_KERNEL_H_

#include <cstdint>

// Exact standalone source/common/quant.cpp:1253-1260, after bestLastIdx is
// chosen. coeffCount is the transform's coefficient count (16/64/256/1024).
// Preconditions: 0 <= bestLastIdx <= coeffCount; scan has bestLastIdx entries;
// each scan[pos] at the instant it is read is < coeffCount; dstCoeff and
// resiDctCoeff each have coeffCount accessible int16_t elements. All pointers
// are valid even if bestLastIdx is zero. Buffers may overlap (including scan
// with dstCoeff); reads and writes then occur in original scan order. No
// requirement that scan is a permutation or that levels are nonnegative.
// Returns the number of nonzero levels READ, not the final nonzero count.
uint32_t rdo_recount_sign(const uint16_t *scan, int16_t *dstCoeff,
                          const int16_t *resiDctCoeff, int bestLastIdx,
                          int coeffCount);
uint32_t rdo_recount_sign_rvv(const uint16_t *scan, int16_t *dstCoeff,
                              const int16_t *resiDctCoeff, int bestLastIdx,
                              int coeffCount);

#endif
