#include "kernel.h"

#include <riscv_vector.h>

static_assert(sizeof(HBUINT8) == 1 && sizeof(HBUINT16) == 2 &&
                  sizeof(unsigned int) == 4,
              "HarfBuzz packed big-endian points require 8/16/32-bit types");

static bool overlap(const void *a, size_t n, const void *b, size_t m) {
    if (!n || !m) return false;
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x <= y ? y - x < n : x - y < m;
}

extern "C" bool decompile_points_rvv(const HBUINT8 *&p,
                                      hb_vector_t<unsigned int> &points,
                                      const HBUINT8 *end) {
    constexpr unsigned word_flag = 0x80, count_mask = 0x7f;
    if (unlikely(p + 1 > end)) return false;
    unsigned count = *p++;
    if (count & word_flag) {
        if (unlikely(p + 1 > end)) return false;
        count = ((count & count_mask) << 8) | *p++;
    }
    if (unlikely(!points.resize_dirty(count))) return false;

    unsigned n = 0;
    unsigned i = 0;
    while (i < count) {
        if (unlikely(p + 1 > end)) return false;
        unsigned control = *p++;
        unsigned run_count = (control & count_mask) + 1;
        unsigned stop = i + run_count;
        if (unlikely(stop > count)) return false;
        const unsigned width = control & word_flag ? 2 : 1;
        if (unlikely(p + run_count * width > end)) return false;

        // The original writes each value immediately. If the encoded data
        // aliases the vector's buffer, decoding the whole run ahead would
        // change subsequent reads; retain ordered accesses for that case.
        if (overlap(p, size_t(run_count) * width, points.arrayZ,
                    size_t(count) * sizeof(unsigned))) {
            for (; i < stop; ++i) {
                if (width == 2) {
                    n += *reinterpret_cast<const HBUINT16 *>(p);
                    p += 2;
                } else {
                    n += *p++;
                }
                points.arrayZ[i] = n;
            }
            continue;
        }

        while (i < stop) {
            const size_t vl = __riscv_vsetvl_e8m1(stop - i);
            vuint8m1_t lo = width == 2
                ? __riscv_vlse8_v_u8m1(reinterpret_cast<const uint8_t *>(p) + 1, 2, vl)
                : __riscv_vle8_v_u8m1(reinterpret_cast<const uint8_t *>(p), vl);
            vuint32m4_t sums = __riscv_vzext_vf4_u32m4(lo, vl);
            if (width == 2) {
                vuint8m1_t hi = __riscv_vlse8_v_u8m1(
                    reinterpret_cast<const uint8_t *>(p), 2, vl);
                sums = __riscv_vor_vv_u32m4(
                    sums, __riscv_vsll_vx_u32m4(__riscv_vzext_vf4_u32m4(hi, vl), 8, vl), vl);
            }
            // Inclusive scan within this strip. slideup's low lanes come
            // from the zero destination; n carries the preceding strip.
            for (size_t offset = 1; offset < vl; offset <<= 1) {
                vuint32m4_t shifted = __riscv_vslideup_vx_u32m4(
                    __riscv_vmv_v_x_u32m4(0, vl), sums, offset, vl);
                sums = __riscv_vadd_vv_u32m4(sums, shifted, vl);
            }
            sums = __riscv_vadd_vx_u32m4(sums, n, vl);
            __riscv_vse32_v_u32m4(points.arrayZ + i, sums, vl);
            n = points.arrayZ[i + vl - 1];
            i += vl;
            p += width * vl;
        }
    }
    return true;
}
