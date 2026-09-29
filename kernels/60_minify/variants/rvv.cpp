#include "kernel.h"

#include <riscv_vector.h>

namespace simdjson {
namespace fallback {

// The fallback jump table removes precisely JSON's four whitespace bytes.
// A quote or backslash is handled in order so that the escape state across
// vector boundaries is identical to the table-driven state machine.
error_code implementation::minify(const uint8_t *buf, size_t len, uint8_t *dst,
                                   size_t &dst_len) const noexcept {
    size_t i = 0, pos = 0;
    uint8_t quote = 0, nonescape = 1;
    // A destination starting inside the unread source has scalar read/write
    // dependencies; preserve even that overlap's table-driven behavior.
    if (reinterpret_cast<uintptr_t>(dst) > reinterpret_cast<uintptr_t>(buf) &&
        reinterpret_cast<uintptr_t>(dst) - reinterpret_cast<uintptr_t>(buf) < len) {
        while (i < len) {
            const uint8_t c = buf[i++];
            quote ^= uint8_t(c == '"') & nonescape;
            dst[pos] = c;
            pos += uint8_t(c != ' ' && c != '\t' && c != '\n' && c != '\r') | quote;
            nonescape = uint8_t(~nonescape) | uint8_t(c != '\\');
        }
        dst_len = pos;
        return quote ? UNCLOSED_STRING : SUCCESS;
    }
    const uint8_t last = len ? buf[len - 1] : 0;
    while (i < len) {
        size_t vl = __riscv_vsetvl_e8m1(len - i);
        vuint8m1_t bytes = __riscv_vle8_v_u8m1(buf + i, vl);
        vbool8_t special = __riscv_vmseq_vx_u8m1_b8(bytes, '"', vl);
        special = __riscv_vmor_mm_b8(special,
                        __riscv_vmseq_vx_u8m1_b8(bytes, '\\', vl), vl);
        const long first = __riscv_vfirst_m_b8(special, vl);
        if (first == 0) {
            const uint8_t c = buf[i++];
            if (c == '"' && (nonescape & 1)) quote ^= 1;
            dst[pos] = c;
            pos += 1; // Both quote and backslash are retained.
            nonescape = uint8_t(~nonescape) | uint8_t(c != '\\');
            continue;
        }
        if (first > 0) {
            vl = static_cast<size_t>(first);
            bytes = __riscv_vle8_v_u8m1(buf + i, vl);
        }
        if (quote) {
            // In-place copies are safe: the whole vector is loaded before
            // writing, and output never runs ahead of unread input.
            __riscv_vse8_v_u8m1(dst + pos, bytes, vl);
            pos += vl;
        } else {
            vbool8_t space = __riscv_vmseq_vx_u8m1_b8(bytes, ' ', vl);
            space = __riscv_vmor_mm_b8(space,
                    __riscv_vmseq_vx_u8m1_b8(bytes, '\t', vl), vl);
            space = __riscv_vmor_mm_b8(space,
                    __riscv_vmseq_vx_u8m1_b8(bytes, '\n', vl), vl);
            space = __riscv_vmor_mm_b8(space,
                    __riscv_vmseq_vx_u8m1_b8(bytes, '\r', vl), vl);
            const vbool8_t keep = __riscv_vmnot_m_b8(space, vl);
            const size_t kept = __riscv_vcpop_m_b8(keep, vl);
            const vuint8m1_t packed = __riscv_vcompress_vm_u8m1(bytes, keep, vl);
            __riscv_vse8_v_u8m1(dst + pos, packed, kept);
            pos += kept;
        }
        i += vl;
        // All bytes above are neither quote nor backslash: the fallback
        // state becomes nonescape=255, including after a lone backslash.
        nonescape = 255;
    }
    // The fallback also writes each discarded character to dst[pos], even
    // though it does not count it. Preserve the final such byte exactly.
    if (len && !quote && (last == ' ' || last == '\t' ||
                          last == '\n' || last == '\r'))
        dst[pos] = last;
    dst_len = pos;
    return quote ? UNCLOSED_STRING : SUCCESS;
}

} // namespace fallback

error_code minify_rvv(const uint8_t *buf, size_t len, uint8_t *dst,
                           size_t &dst_len) noexcept {
    return fallback::implementation{}.minify(buf, len, dst, dst_len);
}
} // namespace simdjson
