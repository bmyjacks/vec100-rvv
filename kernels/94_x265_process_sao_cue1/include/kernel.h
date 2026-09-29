/****************************************************************************
 * x265 3.4, source/common/loopfilter.cpp
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 * Authors: Praveen Kumar Tiwari <praveen@multicorewareinc.com>
 *          Dnyaneshwar Gorade <dnyaneshwar@multicorewareinc.com>
 *          Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111, USA.
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 ****************************************************************************/
#ifndef KERNELS_94_X265_PROCESS_SAO_CUE1_INCLUDE_KERNEL_H_
#define KERNELS_94_X265_PROCESS_SAO_CUE1_INCLUDE_KERNEL_H_

#include <cstdint>

// The pinned main extraction is the 8-bit x265 build (pixel == uint8_t).
using pixel = uint8_t;

// The caller supplies three accessible rows at rec + {0, stride, 2*stride},
// each with width pixels, width sign bytes, and five edge offsets. The initial
// upBuff1 entries are signs (-1, 0, 1); buffers may overlap, including offset
// aliases. The scalar order is y=0,1 then x=0..width-1, with each read at the
// time of its iteration. width <= 0 performs no element accesses.
void process_sao_cue1_2rows(pixel *rec, int8_t *upBuff1, int8_t *offsetEo,
                             intptr_t stride, int width);
void process_sao_cue1_2rows_rvv(pixel *rec, int8_t *upBuff1, int8_t *offsetEo,
                                 intptr_t stride, int width);

#endif
