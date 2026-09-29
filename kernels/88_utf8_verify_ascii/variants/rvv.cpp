#include "kernel.h"

#include <riscv_vector.h>
#include <string.h>

void utf8_verify_ascii_rvv(const char **strp, gsize *lenp) {
    const char *str = *strp;
    // The NULL-length interface is a C string, whereas an explicit length
    // may include embedded NULs and does not require a terminator.
    gsize len = lenp ? *lenp : strlen(str);
    while (len) {
        const gsize vl = __riscv_vsetvl_e8m1(len);
        const auto bytes = __riscv_vle8_v_u8m1(
            reinterpret_cast<const guint8 *>(str), vl);
        const auto nonzero = __riscv_vmsne_vx_u8m1_b8(bytes, 0, vl);
        const auto ascii = __riscv_vmsltu_vx_u8m1_b8(bytes, 128, vl);
        const auto valid = __riscv_vmand_mm_b8(nonzero, ascii, vl);
        const long first = __riscv_vfirst_m_b8(__riscv_vmnot_m_b8(valid, vl), vl);
        const gsize advance = first < 0 ? vl : static_cast<gsize>(first);
        str += advance;
        len -= advance;
        if (first >= 0)
            break;
    }
    *strp = str;
    if (lenp)
        *lenp = len;
}
