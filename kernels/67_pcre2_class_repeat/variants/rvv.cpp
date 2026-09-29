#include "kernel.h"

#include <riscv_vector.h>

// Find the first class failure in each VLA chunk; the byte-indexed table is
// intentionally loaded via RVV indexed byte loads (the original ctypes map).
ClassRunResult pcre2_class_repeat_rvv(const uint8_t *subject, size_t length,
    size_t start, uint32_t min, uint32_t max, ClassType type,
    const uint8_t *ctypes, int partial, size_t start_used,
    int allowemptypartial, int hitend) {
    if (type < OP_NOT_HSPACE || type > OP_WORDCHAR)
        return {start, hitend, -1};
    size_t pos = start;
    uint32_t i = min;
    while (i < max) {
        if (pos == length) {
            if (partial != 0 && (pos > start_used || allowemptypartial)) {
                hitend = 1;
                if (partial > 1) return {pos, hitend, -2};
            }
            break;
        }
        const size_t remaining = length - pos;
        const uint32_t count = max - i;
        const size_t bound = remaining < count ? remaining : count;
        const size_t vl = __riscv_vsetvl_e8m1(bound);
        const vuint8m1_t ch = __riscv_vle8_v_u8m1(subject + pos, vl);
        vbool8_t good;
        if (type <= OP_VSPACE) {
            if (type <= OP_HSPACE) {
                const vbool8_t tab = __riscv_vmseq_vx_u8m1_b8(ch, 9, vl);
                const vbool8_t sp = __riscv_vmseq_vx_u8m1_b8(ch, 32, vl);
                const vbool8_t nbsp = __riscv_vmseq_vx_u8m1_b8(ch, 0xa0, vl);
                const vbool8_t both = __riscv_vmor_mm_b8(tab, sp, vl);
                good = __riscv_vmor_mm_b8(both, nbsp, vl);
            } else {
                const vbool8_t lo = __riscv_vmsgeu_vx_u8m1_b8(ch, 10, vl);
                const vbool8_t hi = __riscv_vmsleu_vx_u8m1_b8(ch, 13, vl);
                const vbool8_t range = __riscv_vmand_mm_b8(lo, hi, vl);
                const vbool8_t nel = __riscv_vmseq_vx_u8m1_b8(ch, 0x85, vl);
                good = __riscv_vmor_mm_b8(range, nel, vl);
            }
        } else {
            const uint8_t bit = type <= OP_DIGIT ? ctype_digit :
                                type <= OP_WHITESPACE ? ctype_space : ctype_word;
            const vuint8m1_t cls = __riscv_vloxei8_v_u8m1(ctypes, ch, vl);
            const vuint8m1_t selected = __riscv_vand_vx_u8m1(cls, bit, vl);
            good = __riscv_vmsne_vx_u8m1_b8(selected, 0, vl);
        }
        if ((static_cast<int>(type) & 1) == 0)
            good = __riscv_vmnot_m_b8(good, vl);
        const vbool8_t bad = __riscv_vmnot_m_b8(good, vl);
        const long first = __riscv_vfirst_m_b8(bad, vl);
        if (first >= 0) return {pos + static_cast<size_t>(first), hitend, 0};
        pos += vl;
        i += static_cast<uint32_t>(vl);
    }
    return {pos, hitend, 0};
}
