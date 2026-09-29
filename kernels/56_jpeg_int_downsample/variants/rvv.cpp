#include "kernel.h"
#include <cstdint>
#include <riscv_vector.h>

void jpeg_int_downsample_rvv(j_compress_ptr cinfo, jpeg_component_info *compptr,
                             _JSAMPARRAY input_data, _JSAMPARRAY output_data) {
    const int h_expand = cinfo->max_h_samp_factor / compptr->h_samp_factor;
    const int v_expand = cinfo->max_v_samp_factor / compptr->v_samp_factor;
    const int numpix = h_expand * v_expand;
    const JDIMENSION output_cols =
        compptr->width_in_blocks * (cinfo->master->lossless ? 1 : DCTSIZE);
    const JDIMENSION expanded_cols = output_cols * h_expand;

    // Any output/input intersection needs the upstream's sequential read/write order.
    const uintptr_t in_size = expanded_cols > cinfo->image_width
                                  ? expanded_cols : cinfo->image_width;
    for (int r = 0; r < cinfo->max_v_samp_factor; ++r) {
        uintptr_t a = reinterpret_cast<uintptr_t>(input_data[r]);
        for (int o = 0; o < compptr->v_samp_factor; ++o) {
            uintptr_t b = reinterpret_cast<uintptr_t>(output_data[o]);
            if (a < b + output_cols && b < a + in_size) {
                jpeg_int_downsample(cinfo, compptr, input_data, output_data);
                return;
            }
        }
    }

    // Match expand_right_edge(), including its observable input-row writes.
    const int numcols = static_cast<int>(expanded_cols - cinfo->image_width);
    if (numcols > 0) {
        for (int r = 0; r < cinfo->max_v_samp_factor; ++r) {
            _JSAMPROW ptr = input_data[r] + cinfo->image_width;
            _JSAMPLE pixval = ptr[-1];
            for (int n = numcols; n > 0; --n)
                *ptr++ = pixval;
        }
    }

    int inrow = 0;
    for (int outrow = 0; outrow < compptr->v_samp_factor; ++outrow) {
        for (JDIMENSION col = 0; col < output_cols;) {
            size_t vl = __riscv_vsetvl_e16m2(output_cols - col);
            auto sum = __riscv_vmv_v_x_u16m2(0, vl);
            for (int v = 0; v < v_expand; ++v) {
                const _JSAMPROW row = input_data[inrow + v] + col * h_expand;
                for (int h = 0; h < h_expand; ++h) {
                    auto pixels = __riscv_vlse8_v_u8m1(row + h, h_expand, vl);
                    sum = __riscv_vwaddu_wv_u16m2(sum, pixels, vl);
                }
            }
            sum = __riscv_vadd_vx_u16m2(sum, numpix / 2, vl);
            auto avg = __riscv_vdivu_vx_u16m2(sum, numpix, vl);
            auto bytes = __riscv_vnclipu_wx_u8m1(avg, 0, __RISCV_VXRM_RDN, vl);
            __riscv_vse8_v_u8m1(output_data[outrow] + col, bytes, vl);
            col += vl;
        }
        inrow += v_expand;
    }
}
