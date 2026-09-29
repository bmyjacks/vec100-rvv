#include "kernel.h"

#include <riscv_vector.h>
#include <stdint.h>

// The unsigned arithmetic of the original search bounds and length check is
// intentional: offset is converted to unsigned before the first iteration.
int xdg_mime_magic_matchlet_compare_to_data_rvv(
    XdgMimeMagicMatchlet *matchlet, const void *data, size_t len) {
    const unsigned int end = matchlet->offset + matchlet->range_length;
    const auto *bytes = static_cast<const unsigned char *>(data);
    for (unsigned int i = matchlet->offset; i < end; ++i) {
        if (i + matchlet->value_length > len)
            return 0;

        unsigned int j = 0;
        for (; j < matchlet->value_length;) {
            const size_t vl = __riscv_vsetvl_e8m1(matchlet->value_length - j);
            const auto value = __riscv_vle8_v_u8m1(matchlet->value + j, vl);
            const auto input = __riscv_vle8_v_u8m1(bytes + i + j, vl);
            vbool8_t mismatch;
            if (matchlet->mask) {
                const auto mask = __riscv_vle8_v_u8m1(matchlet->mask + j, vl);
                const auto a = __riscv_vand_vv_u8m1(value, mask, vl);
                const auto b = __riscv_vand_vv_u8m1(input, mask, vl);
                mismatch = __riscv_vmsne_vv_u8m1_b8(a, b, vl);
            } else {
                mismatch = __riscv_vmsne_vv_u8m1_b8(value, input, vl);
            }
            if (__riscv_vfirst_m_b8(mismatch, vl) >= 0)
                break;
            j += static_cast<unsigned int>(vl);
        }
        if (j == matchlet->value_length)
            return 1;
    }
    return 0;
}
