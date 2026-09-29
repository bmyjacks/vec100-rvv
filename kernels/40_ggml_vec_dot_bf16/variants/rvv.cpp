#include "kernel.h"

#include <riscv_vector.h>

// The upstream Zvfbfwma branch uses f32 FMA lanes and a vector reduction.
// The extracted scalar branch instead rounds each product to f32, then adds
// it to a double accumulator in input order. Keep those semantics, including
// for cancellation and lengths smaller than one vector register.
void ggml_vec_dot_bf16_rvv(int n, float *GGML_RESTRICT s, size_t bs,
                       ggml_bf16_t *GGML_RESTRICT x, size_t bx,
                       ggml_bf16_t *GGML_RESTRICT y, size_t by, int nrc) {
    assert(nrc == 1);
    GGML_UNUSED(nrc);
    GGML_UNUSED(bs);
    GGML_UNUSED(bx);
    GGML_UNUSED(by);

    ggml_float sumf = 0;
    for (int i = 0; i < n;) {
        // A fixed upper bound keeps the temporary independent of VLEN.
        const size_t vl = __riscv_vsetvl_e16m1(
            static_cast<size_t>(n - i) < 32 ? static_cast<size_t>(n - i) : 32);
        const vuint16m1_t ax = __riscv_vle16_v_u16m1(&x[i].bits, vl);
        const vuint16m1_t ay = __riscv_vle16_v_u16m1(&y[i].bits, vl);
        const vuint32m2_t wx = __riscv_vsll_vx_u32m2(
            __riscv_vzext_vf2_u32m2(ax, vl), 16, vl);
        const vuint32m2_t wy = __riscv_vsll_vx_u32m2(
            __riscv_vzext_vf2_u32m2(ay, vl), 16, vl);
        const vfloat32m2_t product = __riscv_vfmul_vv_f32m2(
            __riscv_vreinterpret_v_u32m2_f32m2(wx),
            __riscv_vreinterpret_v_u32m2_f32m2(wy), vl);
        float products[32];
        __riscv_vse32_v_f32m2(products, product, vl);
        for (size_t j = 0; j < vl; ++j)
            sumf += static_cast<ggml_float>(products[j]);
        i += static_cast<int>(vl);
    }
    *s = sumf;
}
