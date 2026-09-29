#include "kernel.h"

#include <riscv_vector.h>

// Points whose SAME flag is set and SHORT flag is clear consume no bytes.
// Their coordinate equals the running value. Vectorize maximal runs of these
// points; decode the variable-length deltas and bounds checks in source order.
extern "C" bool read_points_rvv(const HBUINT8 *&p,
                                 hb_array_t<contour_point_t> points,
                                 const HBUINT8 *end, float contour_point_t::*m,
                                 SimpleGlyph::simple_glyph_flag_t short_flag,
                                 SimpleGlyph::simple_glyph_flag_t same_flag) {
    int value = 0;
    for (unsigned i = 0; i < points.length;) {
        const size_t vl = __riscv_vsetvl_e32m1(points.length - i);
        const auto flags = __riscv_vlse8_v_u8mf4(
            &points.arrayZ[i].flag, sizeof(contour_point_t), vl);
        const auto short_bits = __riscv_vand_vx_u8mf4(flags, short_flag, vl);
        const auto same_bits = __riscv_vand_vx_u8mf4(flags, same_flag, vl);
        const auto bad = __riscv_vmor_mm_b32(
            __riscv_vmsne_vx_u8mf4_b32(short_bits, 0, vl),
            __riscv_vmseq_vx_u8mf4_b32(same_bits, 0, vl), vl);
        const long first = __riscv_vfirst_m_b32(bad, vl);
        const size_t run = first < 0 ? vl : static_cast<size_t>(first);
        if (run) {
            __riscv_vsse32_v_f32m1(&(points.arrayZ[i].*m),
                                    sizeof(contour_point_t),
                                    __riscv_vfmv_v_f_f32m1(static_cast<float>(value), run), run);
            i += static_cast<unsigned>(run);
        }
        if (i == points.length) break;
        if (run == vl) continue;
        const unsigned flag = points.arrayZ[i].flag;
        if (flag & short_flag) {
            if (p + 1 > end) return false;
            value += (bool(flag & same_flag) * 2 - 1) * *p++;
        } else if (!(flag & same_flag)) {
            if (p + HBINT16::static_size > end) return false;
            value += *(const HBINT16 *)p;
            p += HBINT16::static_size;
        }
        points.arrayZ[i].*m = value;
        ++i;
    }
    return true;
}
