#include "kernel.h"
#include <riscv_vector.h>

void h2v2_fancy_upsample_rvv(j_decompress_ptr cinfo, jpeg_component_info *comp,
                              _JSAMPARRAY input, _JSAMPARRAY *output_ptr) {
    _JSAMPARRAY output = *output_ptr;
    JDIMENSION width = comp->downsampled_width;
    for (int row = 0; 2 * row < cinfo->max_v_samp_factor; ++row)
        for (int v = 0; v < 2; ++v) {
            const JSAMPLE *p = input[row], *q = input[row + (v ? 1 : -1)];
            JSAMPLE *out = output[2 * row + v];
            int first = 3 * p[0] + q[0];
            int second = 3 * p[1] + q[1];
            out[0] = (first * 4 + 8) >> 4;
            out[1] = (first * 3 + second + 7) >> 4;
            for (JDIMENSION j = 1; j + 1 < width;) {
                size_t vl = __riscv_vsetvl_e8m1(width - 1 - j);
                auto prev = __riscv_vle8_v_u8m1(p + j - 1, vl);
                auto cur = __riscv_vle8_v_u8m1(p + j, vl);
                auto next = __riscv_vle8_v_u8m1(p + j + 1, vl);
                auto qp = __riscv_vle8_v_u8m1(q + j - 1, vl);
                auto qc = __riscv_vle8_v_u8m1(q + j, vl);
                auto qn = __riscv_vle8_v_u8m1(q + j + 1, vl);
                auto even = __riscv_vwmulu_vx_u16m2(cur, 9, vl);
                even = __riscv_vwmaccu_vx_u16m2(even, 3, qc, vl);
                even = __riscv_vwmaccu_vx_u16m2(even, 3, prev, vl);
                even = __riscv_vwmaccu_vx_u16m2(even, 1, qp, vl);
                even = __riscv_vadd_vx_u16m2(even, 8, vl);
                auto odd = __riscv_vwmulu_vx_u16m2(cur, 9, vl);
                odd = __riscv_vwmaccu_vx_u16m2(odd, 3, qc, vl);
                odd = __riscv_vwmaccu_vx_u16m2(odd, 3, next, vl);
                odd = __riscv_vwmaccu_vx_u16m2(odd, 1, qn, vl);
                odd = __riscv_vadd_vx_u16m2(odd, 7, vl);
                auto e = __riscv_vnclipu_wx_u8m1(even, 4, __RISCV_VXRM_RDN, vl);
                auto o = __riscv_vnclipu_wx_u8m1(odd, 4, __RISCV_VXRM_RDN, vl);
                __riscv_vsseg2e8_v_u8m1x2(out + 2 * j, __riscv_vcreate_v_u8m1x2(e, o), vl);
                j += vl;
            }
            int last = 3 * p[width - 2] + q[width - 2];
            int current = 3 * p[width - 1] + q[width - 1];
            out[2 * width - 2] = (current * 3 + last + 8) >> 4;
            out[2 * width - 1] = (current * 4 + 7) >> 4;
        }
}
