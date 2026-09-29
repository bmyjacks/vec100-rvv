#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

// Return the start of the first malformed code point. ASCII runs are skipped
// with RVV; non-ASCII code points are checked in source order so the prefix
// always ends at a code point boundary, including on a truncated tail.
extern "C" size_t utf8_range_ValidPrefix_rvv(const char *data, size_t len) {
    size_t pos = 0;
    while (pos < len) {
        const size_t vl = __riscv_vsetvl_e8m1(len - pos);
        const auto bytes = __riscv_vle8_v_u8m1(
            reinterpret_cast<const uint8_t *>(data + pos), vl);
        const auto high = __riscv_vmsgeu_vx_u8m1_b8(bytes, 0x80, vl);
        const long first = __riscv_vfirst_m_b8(high, vl);
        pos += first < 0 ? vl : static_cast<size_t>(first);
        if (first < 0) continue;

        const auto *s = reinterpret_cast<const uint8_t *>(data + pos);
        const size_t remaining = len - pos;
        const unsigned c = s[0];
        if (c >= 0xc2 && c <= 0xdf && remaining >= 2 &&
            s[1] >= 0x80 && s[1] <= 0xbf) {
            pos += 2;
        } else if (c >= 0xe0 && c <= 0xef && remaining >= 3 &&
                   s[1] >= 0x80 && s[1] <= 0xbf &&
                   s[2] >= 0x80 && s[2] <= 0xbf &&
                   (c != 0xe0 || s[1] >= 0xa0) &&
                   (c != 0xed || s[1] <= 0x9f)) {
            pos += 3;
        } else if (c >= 0xf0 && c <= 0xf4 && remaining >= 4 &&
                   s[1] >= 0x80 && s[1] <= 0xbf &&
                   s[2] >= 0x80 && s[2] <= 0xbf &&
                   s[3] >= 0x80 && s[3] <= 0xbf &&
                   (c != 0xf0 || s[1] >= 0x90) &&
                   (c != 0xf4 || s[1] <= 0x8f)) {
            pos += 4;
        } else {
            return pos;
        }
    }
    return pos;
}

extern "C" bool utf8_range_IsValid_rvv(const char *data, size_t len) {
    return utf8_range_ValidPrefix_rvv(data, len) == len;
}
