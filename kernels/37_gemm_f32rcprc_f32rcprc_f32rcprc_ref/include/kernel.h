/****************************************************************************
 *
 *
 *  Project: SiFive Kernel Library (SKL) 3.0.0
 *  Source files:
 *    ref/gemm/gemm_f32rcprc_f32rcprc_f32rcprc.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ref/gemm/gemm_f32rcprc_f32rcprc_f32rcprc.h
 *
 * Copyright (c) 2026 SiFive, Inc. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef KERNELS_37_GEMM_F32RCPRC_F32RCPRC_F32RCPRC_REF_INCLUDE_KERNEL_H_
#define KERNELS_37_GEMM_F32RCPRC_F32RCPRC_F32RCPRC_REF_INCLUDE_KERNEL_H_

#include <cstddef>

/*
 * ref/gemm/gemm_f32rcprc_f32rcprc_f32rcprc.h:9-11
 */
extern "C" {

/*
 * ref/gemm/gemm_f32rcprc_f32rcprc_f32rcprc.h:48-53
 */
void skl_gemm_f32rcprc_f32rcprc_f32rcprc_ref(
    size_t m0, size_t n0, size_t k0, size_t m1, size_t n1, size_t k1,
    float alpha, const float *a_pack, size_t rsa0, size_t csa0, size_t rsa1,
    size_t csa1, const float *b_pack, size_t rsb0, size_t csb0, size_t rsb1,
    size_t csb1, float beta, float *c_pack, size_t rsc0, size_t csc0,
    size_t rsc1, size_t csc1);

/*
 * ref/gemm/gemm_f32rcprc_f32rcprc_f32rcprc.h:55-57
 */
} // extern "C"

#endif // KERNELS_37_GEMM_F32RCPRC_F32RCPRC_F32RCPRC_REF_INCLUDE_KERNEL_H_
