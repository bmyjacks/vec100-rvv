#include "kernel.h"

#include <riscv_vector.h>

// Polynomial product in the alternating 26/25-bit radix.  Capture both
// operands before writing h: fe_mul and fe_sq both permit in-place operation.
void fe_mul_rvv(fe h, const fe f, const fe g) {
    int32_t a[10], b[10];
    for (int i = 0; i < 10; ++i) { a[i] = f[i]; b[i] = g[i]; }
    int64_t t[10];
    const size_t vl = __riscv_vsetvl_e32m4(10);
    vint32m4_t va = __riscv_vle32_v_i32m4(a, vl);
    for (int k = 0; k < 10; ++k) {
        int32_t ordered[10];
        int64_t factors[10];
        for (int i = 0; i < 10; ++i) {
            int j = (k - i + 10) % 10;
            ordered[i] = b[j];
            factors[i] = (i + j >= 10 ? 19 : 1) *
                         ((i & 1) && (j & 1) ? 2 : 1);
        }
        vint32m4_t vb = __riscv_vle32_v_i32m4(ordered, vl);
        vint64m8_t product = __riscv_vwmul_vv_i64m8(va, vb, vl);
        product = __riscv_vmul_vv_i64m8(product,
            __riscv_vle64_v_i64m8(factors, vl), vl);
        vint64m1_t zero = __riscv_vmv_v_x_i64m1(0, 1);
        vint64m1_t total = __riscv_vredsum_vs_i64m8_i64m1(product, zero, vl);
        t[k] = __riscv_vmv_x_s_i64m1_i64(total);
    }
    const int64_t mask26 = kTop38Bits, mask25 = kTop39Bits;
    int64_t carry;
#define CARRY(i, next, bias, shift, mask) \
    carry = t[i] + (int64_t(1) << bias); \
    t[next] += carry >> shift; \
    t[i] -= carry & mask
    CARRY(0, 1, 25, 26, mask26);
    CARRY(4, 5, 25, 26, mask26);
    CARRY(1, 2, 24, 25, mask25);
    CARRY(5, 6, 24, 25, mask25);
    CARRY(2, 3, 25, 26, mask26);
    CARRY(6, 7, 25, 26, mask26);
    CARRY(3, 4, 24, 25, mask25);
    CARRY(7, 8, 24, 25, mask25);
    CARRY(4, 5, 25, 26, mask26);
    CARRY(8, 9, 25, 26, mask26);
    carry = t[9] + (int64_t(1) << 24);
    t[0] += (carry >> 25) * 19;
    t[9] -= carry & mask25;
    CARRY(0, 1, 25, 26, mask26);
#undef CARRY
    for (int k = 0; k < 10; ++k) h[k] = static_cast<int32_t>(t[k]);
}

void fe_sq_rvv(fe h, const fe f) { fe_mul_rvv(h, f, f); }
