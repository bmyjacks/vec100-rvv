/****************************************************************************
 * Project: x265 3.4
 * Source files: source/common/common.h
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
 */
#ifndef KERNELS_96_X265_SSIM_DISTORTION_INCLUDE_KERNEL_H_
#define KERNELS_96_X265_SSIM_DISTORTION_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

/* source/common/common.h:135 (8-bit build) */
typedef uint8_t pixel;

/* The two DC accumulations of source/common/quant.cpp:493-500,513-521.
 * trSize is a transform size 4,8,16,32; strides count pixels. */
void ssim_distortion_dc(const pixel *fenc, uint32_t fStride,
                        const pixel *recon, std::intptr_t rstride, int trSize,
                        uint64_t *ssDcOut, uint64_t *dcOut);
void ssim_distortion_dc_rvv(const pixel *fenc, uint32_t fStride,
                            const pixel *recon, std::intptr_t rstride, int trSize,
                            uint64_t *ssDcOut, uint64_t *dcOut);

#endif // KERNELS_96_X265_SSIM_DISTORTION_INCLUDE_KERNEL_H_
