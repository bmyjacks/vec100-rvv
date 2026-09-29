#include "kernel.h"

#include <riscv_vector.h>

extern "C" int32_t sweep_row_to_alpha_rvv(uint8_t *__restrict row_buf,
                                            int32_t *__restrict area,
                                            int16_t *__restrict cover,
                                            unsigned x_min, unsigned x_max) {
    int32_t accum = 0;
    for (unsigned x = x_min; x <= x_max;) {
        const size_t vl = __riscv_vsetvl_e32m2(static_cast<size_t>(x_max) - x + 1);
        const auto c16 = __riscv_vle16_v_i16m1(cover + x, vl);
        auto prefix = __riscv_vwadd_vx_i32m2(c16, 0, vl);
        // Inclusive scan: each slide reads the previous stage, not elements
        // already updated in this stage. The carry is added after the scan.
        for (size_t offset = 1; offset < vl; offset *= 2) {
            const auto shifted = __riscv_vslideup_vx_i32m2(
                __riscv_vmv_v_x_i32m2(0, vl), prefix, offset, vl);
            prefix = __riscv_vadd_vv_i32m2(prefix, shifted, vl);
        }
        const auto last = __riscv_vslidedown_vx_i32m2(prefix, vl - 1, vl);
        const int32_t sum = __riscv_vmv_x_s_i32m2_i32(last);
        prefix = __riscv_vadd_vx_i32m2(prefix, accum, vl);
        accum += sum;

        const auto a = __riscv_vle32_v_i32m2(area + x, vl);
        const auto val = __riscv_vsub_vv_i32m2(
            __riscv_vsll_vx_i32m2(prefix, 9, vl), a, vl);
        const auto mag = __riscv_vmax_vv_i32m2(
            val, __riscv_vneg_v_i32m2(val, vl), vl);
        const auto clamped = __riscv_vmin_vx_i32m2(mag, HB_RASTER_FULL_COVERAGE, vl);
        const auto scaled = __riscv_vsrl_vx_u32m2(
            __riscv_vadd_vx_u32m2(
                __riscv_vmul_vx_u32m2(
                    __riscv_vreinterpret_v_i32m2_u32m2(clamped), 255, vl),
                HB_RASTER_FULL_COVERAGE / 2, vl), 17, vl);
        const auto half = __riscv_vnsrl_wx_u16m1(scaled, 0, vl);
        __riscv_vse8_v_u8mf2(row_buf + x, __riscv_vnsrl_wx_u8mf2(half, 0, vl), vl);
        __riscv_vse32_v_i32m2(area + x, __riscv_vmv_v_x_i32m2(0, vl), vl);
        __riscv_vse16_v_i16m1(cover + x, __riscv_vmv_v_x_i16m1(0, vl), vl);
        x += static_cast<unsigned>(vl);
    }
    return accum;
}
