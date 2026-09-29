// Copyright (c) 2026 SiFive, Inc. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.
// SPDX-License-Identifier: MIT
// SiFive SKL 3.0.0 ref/softmax/softmax_f32.c:10-31.
// Only the SKL_FUNC declaration macro is removed; the body and f32 arithmetic
// (including the order of the max and sum) are preserved.

#include "kernel.h"

#include <math.h>

namespace sifive_skl {
void skl_softmax_f32_ref(float *pDst, const float *pSrc, float beta,
                         size_t n) {
  if (n < 1) {
    return;
  }

  float max = pSrc[0];
  for (size_t i = 1; i < n; i++) {
    max = fmaxf(pSrc[i], max);
  }

  float sum = 0;
  for (size_t i = 0; i < n; i++) {
    pDst[i] = expf(beta * (pSrc[i] - max));
    sum += pDst[i];
  }

  float recip_sum = 1.0f / sum;
  for (size_t i = 0; i < n; i++) {
    pDst[i] *= recip_sum;
  }
}
} // namespace sifive_skl
