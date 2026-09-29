// Copyright (c) 2026 SiFive, Inc. All rights reserved.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.
// SPDX-License-Identifier: MIT
// Independent base-RVV variant of SKL 3.0.0 ref/softmax/softmax_f32.c.

#include "kernel.h"

#include <math.h>
#include <riscv_vector.h>

namespace sifive_skl {
void skl_softmax_f32_rvv(float *pDst, const float *pSrc, float beta,
                         size_t n) {
  if (n < 1) {
    return;
  }

  // For nonzero, non-NaN operands, fmaxf and vfmax have the same maximum
  // regardless of reduction order. Zeros have sign-sensitive tie behavior;
  // NaNs (including signaling NaNs) may have different payload/exception
  // behavior. In those cases replay the upstream max in its original order.
  float max = pSrc[0];
  bool ordered_max = false;
  for (size_t i = 0; i < n;) {
    const size_t vl = __riscv_vsetvl_e32m1(n - i);
    const vfloat32m1_t x = __riscv_vle32_v_f32m1(pSrc + i, vl);
    const vuint32m1_t bits = __riscv_vreinterpret_v_f32m1_u32m1(x);
    const vuint32m1_t magnitude =
        __riscv_vand_vx_u32m1(bits, 0x7fffffffu, vl);
    const vbool32_t zero = __riscv_vmseq_vx_u32m1_b32(magnitude, 0, vl);
    const vbool32_t nan =
        __riscv_vmsgtu_vx_u32m1_b32(magnitude, 0x7f800000u, vl);
    if (__riscv_vfirst_m_b32(zero, vl) >= 0 ||
        __riscv_vfirst_m_b32(nan, vl) >= 0) {
      ordered_max = true;
      break;
    }
    const vfloat32m1_t seed = __riscv_vfmv_s_f_f32m1(max, 1);
    max = __riscv_vfmv_f_s_f32m1_f32(
        __riscv_vfredmax_vs_f32m1_f32m1(x, seed, vl));
    i += vl;
  }
  if (ordered_max) {
    max = pSrc[0];
    for (size_t i = 1; i < n; ++i) {
      max = fmaxf(pSrc[i], max);
    }
  }

  // The stored expf result is exactly what is added to the f32 accumulator.
  // Neither an approximate vector exp nor a tree reduction can replace this.
  // Forward writes also reproduce the reference for partially overlapping
  // source and destination (including in-place operation).
  float sum = 0;
  for (size_t i = 0; i < n; ++i) {
    pDst[i] = expf(beta * (pSrc[i] - max));
    sum += pDst[i];
  }

  const float recip_sum = 1.0f / sum;
  for (size_t i = 0; i < n;) {
    const size_t vl = __riscv_vsetvl_e32m1(n - i);
    const vfloat32m1_t x = __riscv_vle32_v_f32m1(pDst + i, vl);
    __riscv_vse32_v_f32m1(
        pDst + i, __riscv_vfmul_vf_f32m1(x, recip_sum, vl), vl);
    i += vl;
  }
}
} // namespace sifive_skl
