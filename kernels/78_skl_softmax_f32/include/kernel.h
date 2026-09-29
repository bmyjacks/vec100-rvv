// Copyright (c) 2026 SiFive, Inc. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.
// SPDX-License-Identifier: MIT
// Extracted from SiFive SKL 3.0.0 ref/softmax/softmax_f32.h.

#ifndef KERNELS_78_SKL_SOFTMAX_F32_INCLUDE_KERNEL_H_
#define KERNELS_78_SKL_SOFTMAX_F32_INCLUDE_KERNEL_H_

#include <cstddef>

namespace sifive_skl {
// n is size_t: the upstream n < 1 guard means n == 0 is a no-op.
void skl_softmax_f32_ref(float *pDst, const float *pSrc, float beta, size_t n);
void skl_softmax_f32_rvv(float *pDst, const float *pSrc, float beta, size_t n);
} // namespace sifive_skl

#endif
