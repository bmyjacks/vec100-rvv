#include "kernel.h"

#include <riscv_vector.h>

// The upstream Zvfh branch widens and FMA-accumulates in f32. The scalar
// extraction uses the initialized lookup table, rounds each product to f32,
// and accumulates in double in input order. Gather from that same table to
// retain its conversion (including its exceptional-value behavior).
void ggml_vec_dot_f16_rvv(int n, float *GGML_RESTRICT s, size_t bs,
                      ggml_fp16_t *GGML_RESTRICT x, size_t bx,
                      ggml_fp16_t *GGML_RESTRICT y, size_t by, int nrc) {
    assert(nrc == 1);
    GGML_UNUSED(nrc);
    GGML_UNUSED(bs);
    GGML_UNUSED(bx);
    GGML_UNUSED(by);

    ggml_float sumf = 0.0;
    for (int i = 0; i < n;) {
        const size_t vl = __riscv_vsetvl_e16m1(
            static_cast<size_t>(n - i) < 32 ? static_cast<size_t>(n - i) : 32);
        const vuint16m1_t hx = __riscv_vle16_v_u16m1(x + i, vl);
        const vuint16m1_t hy = __riscv_vle16_v_u16m1(y + i, vl);
        const vuint32m2_t ix = __riscv_vsll_vx_u32m2(
            __riscv_vzext_vf2_u32m2(hx, vl), 2, vl);
        const vuint32m2_t iy = __riscv_vsll_vx_u32m2(
            __riscv_vzext_vf2_u32m2(hy, vl), 2, vl);
        const vfloat32m2_t ax = __riscv_vluxei32_v_f32m2(
            ggml_table_f32_f16, ix, vl);
        const vfloat32m2_t ay = __riscv_vluxei32_v_f32m2(
            ggml_table_f32_f16, iy, vl);
        const vfloat32m2_t product = __riscv_vfmul_vv_f32m2(ax, ay, vl);
        float products[32];
        __riscv_vse32_v_f32m2(products, product, vl);
        for (size_t j = 0; j < vl; ++j)
            sumf += static_cast<ggml_float>(products[j]);
        i += static_cast<int>(vl);
    }
    *s = sumf;
}
