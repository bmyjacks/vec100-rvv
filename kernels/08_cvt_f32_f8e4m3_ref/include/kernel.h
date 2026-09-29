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

#ifndef KERNELS_08_CVT_F32_F8E4M3_REF_INCLUDE_KERNEL_H_
#define KERNELS_08_CVT_F32_F8E4M3_REF_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

/*
 * ref/cvt/cvt_ofp8.h:12-14
 */
extern "C" {

/*
 * ref/cvt/cvt_ofp8.h:38
 */
uint8_t skl_cvt_f32_f8e4m3(float in, bool is_sat);

/*
 * ref/cvt/cvt_ofp8.h:59-60
 */
void skl_cvt_f32_f8e4m3_ref(uint8_t *pDst, const float *pSrc,
                            float scaling_factor, size_t n);

/*
 * ref/cvt/cvt_ofp8.h:160-162
 */
} // extern "C"

#endif // KERNELS_08_CVT_F32_F8E4M3_REF_INCLUDE_KERNEL_H_
