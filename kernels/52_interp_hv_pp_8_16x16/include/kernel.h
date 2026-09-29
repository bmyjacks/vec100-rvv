/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/common.h
 *    source/common/constants.h
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

#ifndef KERNELS_52_INTERP_HV_PP_8_16X16_INCLUDE_KERNEL_H_
#define KERNELS_52_INTERP_HV_PP_8_16X16_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

/*
 * source/common/common.h:77
 */
#define ALIGN_VAR_32(T, var) T var __attribute__((aligned(32)))

/*
 * source/common/common.h:135
 */
typedef uint8_t pixel;

/*
 * source/common/constants.h:66-70
 */
#define NTAPS_LUMA 8
#define NTAPS_CHROMA 4
#define IF_INTERNAL_PREC 14
#define IF_FILTER_PREC 6
#define IF_INTERNAL_OFFS (1 << (IF_INTERNAL_PREC - 1))

/*
 * source/common/constants.h:73-74
 */
namespace X265_NS {
extern const int16_t g_lumaFilter[4][NTAPS_LUMA];
extern const int16_t g_chromaFilter[8][NTAPS_CHROMA];
} // namespace X265_NS

/*
 * Wrapper for invoking the extracted kernel.
 */
void interp_hv_pp_8_16x16(const pixel *src, std::intptr_t srcStride, pixel *dst,
                          std::intptr_t dstStride, int idxX, int idxY);

#endif // KERNELS_52_INTERP_HV_PP_8_16X16_INCLUDE_KERNEL_H_
