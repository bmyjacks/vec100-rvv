#include "kernel.h"

#include <riscv_vector.h>

bool validate_utf8_rvv(const char *buf, size_t len) {
    const auto *data = reinterpret_cast<const uint8_t *>(buf);
    size_t pos = 0;
    while (pos < len) {
        const size_t vl = __riscv_vsetvl_e8m1(len - pos);
        const auto bytes = __riscv_vle8_v_u8m1(data + pos, vl);
        const auto high = __riscv_vmsgeu_vx_u8m1_b8(bytes, 0x80, vl);
        const long first = __riscv_vfirst_m_b8(high, vl);
        pos += first < 0 ? vl : static_cast<size_t>(first);
        if (first < 0) continue;

        const unsigned c = data[pos];
        const size_t remaining = len - pos;
        if (c >= 0xc2 && c <= 0xdf && remaining >= 2 &&
            data[pos + 1] >= 0x80 && data[pos + 1] <= 0xbf) {
            pos += 2;
        } else if (c >= 0xe0 && c <= 0xef && remaining >= 3 &&
                   data[pos + 1] >= 0x80 && data[pos + 1] <= 0xbf &&
                   data[pos + 2] >= 0x80 && data[pos + 2] <= 0xbf &&
                   (c != 0xe0 || data[pos + 1] >= 0xa0) &&
                   (c != 0xed || data[pos + 1] <= 0x9f)) {
            pos += 3;
        } else if (c >= 0xf0 && c <= 0xf4 && remaining >= 4 &&
                   data[pos + 1] >= 0x80 && data[pos + 1] <= 0xbf &&
                   data[pos + 2] >= 0x80 && data[pos + 2] <= 0xbf &&
                   data[pos + 3] >= 0x80 && data[pos + 3] <= 0xbf &&
                   (c != 0xf0 || data[pos + 1] >= 0x90) &&
                   (c != 0xf4 || data[pos + 1] <= 0x8f)) {
            pos += 4;
        } else {
            return false;
        }
    }
    return true;
}
