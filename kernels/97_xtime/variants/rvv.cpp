#include "kernel.h"

#include <riscv_vector.h>

static inline vuint8m1_t xtime_rvv(vuint8m1_t a, size_t vl) {
    const vuint8m1_t shift = __riscv_vsll_vx_u8m1(a, 1, vl);
    const vuint8m1_t high = __riscv_vsrl_vx_u8m1(a, 7, vl);
    const vuint8m1_t reduction = __riscv_vmul_vx_u8m1(high, 0x1b, vl);
    return __riscv_vxor_vv_u8m1(shift, reduction, vl);
}

static inline vuint8m1_t gf09(vuint8m1_t a, size_t vl) {
    return __riscv_vxor_vv_u8m1(xtime_rvv(xtime_rvv(xtime_rvv(a, vl), vl), vl),
                                a, vl);
}

static inline vuint8m1_t gf0b(vuint8m1_t a, size_t vl) {
    const vuint8m1_t t2 = xtime_rvv(xtime_rvv(a, vl), vl);
    return __riscv_vxor_vv_u8m1(xtime_rvv(__riscv_vxor_vv_u8m1(t2, a, vl), vl),
                                a, vl);
}

static inline vuint8m1_t gf0d(vuint8m1_t a, size_t vl) {
    return __riscv_vxor_vv_u8m1(
        xtime_rvv(xtime_rvv(__riscv_vxor_vv_u8m1(xtime_rvv(a, vl), a, vl), vl),
                  vl),
        a, vl);
}

static inline vuint8m1_t gf0e(vuint8m1_t a, size_t vl) {
    const vuint8m1_t t2 =
        xtime_rvv(__riscv_vxor_vv_u8m1(xtime_rvv(a, vl), a, vl), vl);
    return xtime_rvv(__riscv_vxor_vv_u8m1(t2, a, vl), vl);
}

static inline vuint8m1_t xor4(vuint8m1_t a, vuint8m1_t b, vuint8m1_t c,
                              vuint8m1_t d, size_t vl) {
    return __riscv_vxor_vv_u8m1(__riscv_vxor_vv_u8m1(a, b, vl),
                                __riscv_vxor_vv_u8m1(c, d, vl), vl);
}

SoftAesBlock softaes_inv_mix_columns_rvv(const SoftAesBlock block) {
    SoftAesBlock out;
    const auto *in = reinterpret_cast<const uint8_t *>(&block);
    auto *dest = reinterpret_cast<uint8_t *>(&out);
    const size_t vl = __riscv_vsetvl_e8m1(4);
    const vuint8m1_t a = __riscv_vlse8_v_u8m1(in, 4, vl);
    const vuint8m1_t b = __riscv_vlse8_v_u8m1(in + 1, 4, vl);
    const vuint8m1_t c = __riscv_vlse8_v_u8m1(in + 2, 4, vl);
    const vuint8m1_t d = __riscv_vlse8_v_u8m1(in + 3, 4, vl);

    __riscv_vsse8_v_u8m1(
        dest, 4, xor4(gf0e(a, vl), gf0b(b, vl), gf0d(c, vl), gf09(d, vl), vl),
        vl);
    __riscv_vsse8_v_u8m1(
        dest + 1, 4,
        xor4(gf09(a, vl), gf0e(b, vl), gf0b(c, vl), gf0d(d, vl), vl), vl);
    __riscv_vsse8_v_u8m1(
        dest + 2, 4,
        xor4(gf0d(a, vl), gf09(b, vl), gf0e(c, vl), gf0b(d, vl), vl), vl);
    __riscv_vsse8_v_u8m1(
        dest + 3, 4,
        xor4(gf0b(a, vl), gf0d(b, vl), gf09(c, vl), gf0e(d, vl), vl), vl);
    return out;
}

void softaes_invert_key_schedule128_rvv(SoftAesBlock rkeys[11]) {
    for (size_t i = 1; i < 10; ++i)
        rkeys[i] = softaes_inv_mix_columns_rvv(rkeys[i]);
}

void softaes_invert_key_schedule256_rvv(SoftAesBlock rkeys[15]) {
    for (size_t i = 1; i < 14; ++i)
        rkeys[i] = softaes_inv_mix_columns_rvv(rkeys[i]);
}
