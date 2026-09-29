#include "kernel.h"

#include <cstring>
#include <riscv_vector.h>

gchar *g_strreverse_rvv(gchar *string) {
    g_return_val_if_fail(string != NULL, NULL);
    if (!*string)
        return string;

    auto *left = reinterpret_cast<unsigned char *>(string);
    auto *right = left + std::strlen(string) - 1;
    while (left < right) {
        const size_t half = static_cast<size_t>(right - left + 1) / 2;
        const size_t vl = __riscv_vsetvl_e8m1(half);
        // Gather both sides before either store, keeping the source's
        // in-place behavior for the middle and for every vector boundary.
        const vuint8m1_t lo = __riscv_vle8_v_u8m1(left, vl);
        const vuint8m1_t hi = __riscv_vlse8_v_u8m1(right, -1, vl);
        __riscv_vse8_v_u8m1(left, hi, vl);
        __riscv_vsse8_v_u8m1(right, -1, lo, vl);
        left += vl;
        right -= vl;
    }
    return string;
}
