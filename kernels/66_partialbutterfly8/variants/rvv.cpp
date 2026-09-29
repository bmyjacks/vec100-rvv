#include "kernel.h"

#include <cstdint>
#include <initializer_list>
#include <riscv_vector.h>

namespace {
// The first four coefficients of each pinned x265 g_t8 row are all this
// butterfly reads. Keep the RVV translation unit independently linkable.
constexpr int16_t t8[8][4] = {
    {64, 64, 64, 64}, {89, 75, 50, 18}, {83, 36, -36, -83},
    {75, -18, -89, -50}, {64, -64, -64, 64}, {50, -89, 18, 75},
    {36, -83, 83, -36}, {18, -50, 75, -89}};

void scalar_order(const int16_t *src, int16_t *dst, int shift, int line) {
    const int add = 1 << (shift - 1);
    for (int j = 0; j < line; ++j) {
        int E[4], O[4];
        for (int k = 0; k < 4; ++k) {
            E[k] = src[k] + src[7 - k];
            O[k] = src[k] - src[7 - k];
        }
        const int EE0 = E[0] + E[3], EE1 = E[1] + E[2];
        const int EO0 = E[0] - E[3], EO1 = E[1] - E[2];
        dst[0] = static_cast<int16_t>((t8[0][0] * EE0 + t8[0][1] * EE1 + add) >> shift);
        dst[4 * line] = static_cast<int16_t>((t8[4][0] * EE0 + t8[4][1] * EE1 + add) >> shift);
        dst[2 * line] = static_cast<int16_t>((t8[2][0] * EO0 + t8[2][1] * EO1 + add) >> shift);
        dst[6 * line] = static_cast<int16_t>((t8[6][0] * EO0 + t8[6][1] * EO1 + add) >> shift);
        for (int k : {1, 3, 5, 7})
            dst[k * line] = static_cast<int16_t>((t8[k][0] * O[0] +
                t8[k][1] * O[1] + t8[k][2] * O[2] +
                t8[k][3] * O[3] + add) >> shift);
        src += 8;
        ++dst;
    }
}
} // namespace

void partialButterfly8_rvv(const int16_t *src, int16_t *dst, int shift,
                             int line) {
    if (line <= 0)
        return;
    const uintptr_t a = reinterpret_cast<uintptr_t>(src);
    const uintptr_t b = reinterpret_cast<uintptr_t>(dst);
    const size_t bytes = static_cast<size_t>(line) * 8 * sizeof(int16_t);
    if (a < b + bytes && b < a + bytes) {
        scalar_order(src, dst, shift, line);
        return;
    }

    const int add = 1 << (shift - 1);
    const ptrdiff_t step = 8 * sizeof(int16_t);
    for (int j = 0; j < line;) {
        const size_t vl = __riscv_vsetvl_e16m1(line - j);
        // RVV sizeless types cannot be stored in C++ arrays.
#define LOAD_PAIR(k) \
        vint16m1_t lhs##k = __riscv_vlse16_v_i16m1(src + 8 * j + k, step, vl); \
        vint16m1_t rhs##k = __riscv_vlse16_v_i16m1(src + 8 * j + 7 - k, step, vl); \
        vint32m2_t l##k = __riscv_vwadd_vx_i32m2(lhs##k, 0, vl); \
        vint32m2_t r##k = __riscv_vwadd_vx_i32m2(rhs##k, 0, vl); \
        vint32m2_t E##k = __riscv_vadd_vv_i32m2(l##k, r##k, vl); \
        vint32m2_t O##k = __riscv_vsub_vv_i32m2(l##k, r##k, vl)
        LOAD_PAIR(0);
        LOAD_PAIR(1);
        LOAD_PAIR(2);
        LOAD_PAIR(3);
#undef LOAD_PAIR
        vint32m2_t EE0 = __riscv_vadd_vv_i32m2(E0, E3, vl);
        vint32m2_t EE1 = __riscv_vadd_vv_i32m2(E1, E2, vl);
        vint32m2_t EO0 = __riscv_vsub_vv_i32m2(E0, E3, vl);
        vint32m2_t EO1 = __riscv_vsub_vv_i32m2(E1, E2, vl);
        for (int k : {0, 4, 2, 6, 1, 3, 5, 7}) {
            vint32m2_t acc;
            if (k == 0 || k == 4) {
                acc = __riscv_vmul_vx_i32m2(EE0, t8[k][0], vl);
                acc = __riscv_vmacc_vx_i32m2(acc, t8[k][1], EE1, vl);
            } else if (k == 2 || k == 6) {
                acc = __riscv_vmul_vx_i32m2(EO0, t8[k][0], vl);
                acc = __riscv_vmacc_vx_i32m2(acc, t8[k][1], EO1, vl);
            } else {
                acc = __riscv_vmul_vx_i32m2(O0, t8[k][0], vl);
                acc = __riscv_vmacc_vx_i32m2(acc, t8[k][1], O1, vl);
                acc = __riscv_vmacc_vx_i32m2(acc, t8[k][2], O2, vl);
                acc = __riscv_vmacc_vx_i32m2(acc, t8[k][3], O3, vl);
            }
            acc = __riscv_vadd_vx_i32m2(acc, add, vl);
            acc = __riscv_vsra_vx_i32m2(acc, shift, vl);
            vint16m1_t out = __riscv_vncvt_x_x_w_i16m1(acc, vl);
            __riscv_vse16_v_i16m1(dst + k * line + j, out, vl);
        }
        j += static_cast<int>(vl);
    }
}
