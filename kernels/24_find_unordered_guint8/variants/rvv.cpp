#include "kernel.h"

#include <riscv_vector.h>
#include <string.h>

// The extracted GLib header uses identity FROM_LE casts on its RV64
// little-endian target. RVV element loads therefore have the same value as
// the scalar memcpy loads. Avoid typed vector loads on unaligned byte tables:
// RVV implementations may trap on misaligned element accesses.
#define FIND_UNORDERED(type, bits, maskbits)                                    \
    gsize find_unordered_##type##_rvv(const guint8 *data, gsize start,         \
                                      gsize len) {                              \
        if (start + 1 >= len)                                                   \
            return start;                                                       \
        constexpr gsize width = sizeof(type);                                  \
        if (((uintptr_t)(data + start * width) & (width - 1)) != 0) {           \
            type previous, current;                                            \
            memcpy(&previous, data + start * width, width);                    \
            for (gsize i = start + 1; i < len; ++i) {                          \
                memcpy(&current, data + i * width, width);                     \
                if (current < previous)                                        \
                    return i - 1;                                              \
                previous = current;                                            \
            }                                                                  \
            return len - 1;                                                     \
        }                                                                      \
        gsize i = start + 1;                                                    \
        while (i < len) {                                                       \
            const gsize vl = __riscv_vsetvl_e##bits##m1(len - i);               \
            const auto previous = __riscv_vle##bits##_v_u##bits##m1(           \
                reinterpret_cast<const type *>(data + (i - 1) * width), vl);   \
            const auto current = __riscv_vle##bits##_v_u##bits##m1(            \
                reinterpret_cast<const type *>(data + i * width), vl);         \
            const auto decreases = __riscv_vmsltu_vv_u##bits##m1_b##maskbits(  \
                current, previous, vl);                                        \
            const long first = __riscv_vfirst_m_b##maskbits(decreases, vl);     \
            if (first >= 0)                                                     \
                return i + static_cast<gsize>(first) - 1;                      \
            i += vl;                                                           \
        }                                                                      \
        return len - 1;                                                         \
    }

FIND_UNORDERED(guint8, 8, 8)
FIND_UNORDERED(guint16, 16, 16)
FIND_UNORDERED(guint32, 32, 32)
FIND_UNORDERED(guint64, 64, 64)

#undef FIND_UNORDERED
