/****************************************************************************
 *
 *
 *  Project: SiFive Kernel Library (SKL) 3.0.0
 *  Source files:
 *    ref/cvt/cvt_ofp8.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ref/cvt/cvt_ofp8.h
 *
 * Copyright (c) 2026 SiFive, Inc. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * SPDX-License-Identifier: MIT
 *
 *
 */

#ifndef KERNELS_09_CVT_F8E4M3_BF16_REF_INCLUDE_KERNEL_H_
#define KERNELS_09_CVT_F8E4M3_BF16_REF_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

/*
 * ref/cvt/cvt_ofp8.h:12-14
 */
extern "C" {

/*
 * ref/cvt/cvt_ofp8.h:21
 */
float skl_cvt_f8e4m3_f32(uint8_t in);

/*
 * ref/cvt/cvt_ofp8.h:150
 */
void skl_cvt_f8e4m3_bf16_ref(__bf16 *pDst, const uint8_t *pSrc, size_t n);

/*
 * ref/cvt/cvt_ofp8.h:160-162
 */
} // extern "C"

#endif // KERNELS_09_CVT_F8E4M3_BF16_REF_INCLUDE_KERNEL_H_
