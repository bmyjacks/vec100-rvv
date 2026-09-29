#include "kernel.h"

#include <cstdlib>
#include <cstring>
#include <riscv_vector.h>

// The extracted g_strdup/g_strndup retain GLib's allocation and zero-padding
// contract.  Convert only bytes before the first NUL (including for len >= 0).
gchar *g_ascii_strdown_rvv(const gchar *str, gssize len) {
    if (!str)
        return nullptr;
    gchar *result = len < 0 ? g_strdup(str) : g_strndup(str, (gsize)len);
    const size_t length = std::strlen(result);
    for (size_t i = 0; i < length;) {
        const size_t vl = __riscv_vsetvl_e8m1(length - i);
        auto v = __riscv_vle8_v_u8m1((const unsigned char *)result + i, vl);
        auto upper = __riscv_vmsgeu_vx_u8m1_b8(v, 'A', vl);
        upper = __riscv_vmand_mm_b8(upper,
                   __riscv_vmsleu_vx_u8m1_b8(v, 'Z', vl), vl);
        v = __riscv_vadd_vx_u8m1_m(upper, v, 32, vl);
        __riscv_vse8_v_u8m1((unsigned char *)result + i, v, vl);
        i += vl;
    }
    return result;
}
