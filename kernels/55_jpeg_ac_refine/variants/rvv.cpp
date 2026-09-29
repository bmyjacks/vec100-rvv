#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

static bool overlap(const void *a, size_t an, const void *b, size_t bn)
{
  uintptr_t x = reinterpret_cast<uintptr_t>(a);
  uintptr_t y = reinterpret_cast<uintptr_t>(b);
  return x < y + bn && y < x + an;
}

int encode_mcu_AC_refine_prepare_rvv(const JCOEF *block, const int *order, int Sl,
                                 int Al, UJCOEF *absvalues, size_t *bits)
{
  /* Preserve the scalar read/write order when outputs can change inputs or
   * each other.  Upstream passes distinct block, zigzag table and stack
   * workspaces.  The fallback also handles non-upstream parameter values.
   */
  if (Sl < 0 || Sl > 63 || Al < 0 || Al > 13 ||
      overlap(absvalues, size_t(Sl) * sizeof(UJCOEF), block,
              DCTSIZE2 * sizeof(JCOEF)) ||
      overlap(absvalues, size_t(Sl) * sizeof(UJCOEF), order,
              size_t(Sl) * sizeof(int)) ||
      overlap(bits, 2 * sizeof(size_t), absvalues,
              size_t(Sl) * sizeof(UJCOEF)) ||
      overlap(bits, 2 * sizeof(size_t), block,
              DCTSIZE2 * sizeof(JCOEF)) ||
      overlap(bits, 2 * sizeof(size_t), order,
              size_t(Sl) * sizeof(int)))
    return encode_mcu_AC_refine_prepare(block, order, Sl, Al,
                                                absvalues, bits);

  size_t zerobits = 0, signbits = 0;
  int EOB = 0;
  for (int k = 0; k < Sl;) {
    size_t vl = __riscv_vsetvl_e16m1(Sl - k);
    vint32m2_t indices = __riscv_vle32_v_i32m2(order + k, vl);
    vuint32m2_t offsets = __riscv_vsll_vx_u32m2(
        __riscv_vreinterpret_v_i32m2_u32m2(indices), 1, vl);
    vint16m1_t coef = __riscv_vluxei32_v_i16m1(block, offsets, vl);
    vint32m2_t wide = __riscv_vwadd_vx_i32m2(coef, 0, vl);
    vint32m2_t sign = __riscv_vsra_vx_i32m2(wide, 31, vl);
    vint32m2_t mag = __riscv_vsub_vv_i32m2(
        __riscv_vxor_vv_i32m2(wide, sign, vl), sign, vl);
    mag = __riscv_vsra_vx_i32m2(mag, Al, vl);
    vuint16m1_t narrow = __riscv_vncvt_x_x_w_u16m1(
        __riscv_vreinterpret_v_i32m2_u32m2(mag), vl);
    __riscv_vse16_v_u16m1(absvalues + k, narrow, vl);

    /* The EOB is the LAST transformed magnitude of one, not a count of
     * nonzero coefficients; reduce in coefficient order across strips.
     */
    int32_t signs[64];
    __riscv_vse32_v_i32m2(signs, sign, vl);
    for (size_t j = 0; j < vl; ++j) {
      UJCOEF value = absvalues[k + j];
      if (value) {
        zerobits |= size_t(1) << (k + j);
        signbits |= size_t(signs[j] + 1) << (k + j);
      }
      if (value == 1)
        EOB = k + j;
    }
    k += vl;
  }
  bits[0] = zerobits;
  bits[1] = signbits;
  return EOB;
}
