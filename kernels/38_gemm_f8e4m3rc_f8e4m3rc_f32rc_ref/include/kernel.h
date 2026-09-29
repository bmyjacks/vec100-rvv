/****************************************************************************
 *
 *
 *  Project: SiFive Kernel Library (SKL) 3.0.0
 *  Source files:
 *    ref/gemm/gemm_f8e4m3rc_f8e4m3rc_f32rc.h
 *    ref/cvt/cvt_ofp8.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ref/gemm/gemm_f8e4m3rc_f8e4m3rc_f32rc.h
 *
 * Copyright (c) 2026 SiFive, Inc. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * SPDX-License-Identifier: MIT
 *
 *
 * ref/cvt/cvt_ofp8.h
 *
 * Copyright (c) 2026 SiFive, Inc. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef KERNELS_38_GEMM_F8E4M3RC_F8E4M3RC_F32RC_REF_INCLUDE_KERNEL_H_
#define KERNELS_38_GEMM_F8E4M3RC_F8E4M3RC_F32RC_REF_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

/*
 * ref/gemm/gemm_f8e4m3rc_f8e4m3rc_f32rc.h:11-13
 */
extern "C" {

/*
 * ref/gemm/gemm_f8e4m3rc_f8e4m3rc_f32rc.h:49-54
 */
void skl_gemm_f8e4m3rc_f8e4m3rc_f32rc_ref(size_t m, size_t n, size_t k,
                                          float alpha, const uint8_t *a,
                                          size_t rsa, size_t csa,
                                          const uint8_t *b, size_t rsb,
                                          size_t csb, float beta, float *c,
                                          size_t rsc, size_t csc);

/*
 * ref/gemm/gemm_f8e4m3rc_f8e4m3rc_f32rc.h:56-58
 */
} // extern "C"

/*
 * ref/cvt/cvt_ofp8.h:12-14
 */
extern "C" {

/*
 * ref/cvt/cvt_ofp8.h:21
 */
float skl_cvt_f8e4m3_f32(uint8_t in);

/*
 * ref/cvt/cvt_ofp8.h:160-162
 */
} // extern "C"

#endif // KERNELS_38_GEMM_F8E4M3RC_F8E4M3RC_F32RC_REF_INCLUDE_KERNEL_H_
