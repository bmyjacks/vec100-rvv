#include "kernel.h"

#include <riscv_vector.h>

// PCRE2's 32-byte start bitmap is indexed by the unsigned subject byte.
// Scan only up to end_subject; mb_end_subject affects the nomatch flag only.
static PCRE2_SPTR scan(PCRE2_SPTR p, PCRE2_SPTR end, const uint8_t *bits) {
    while (p < end) {
        const size_t vl = __riscv_vsetvl_e8m1(static_cast<size_t>(end - p));
        vuint8m1_t c = __riscv_vle8_v_u8m1(p, vl);
        vuint8m1_t index = __riscv_vsrl_vx_u8m1(c, 3, vl);
        vuint8m1_t bitmap = __riscv_vluxei8_v_u8m1(bits, index, vl);
        vuint8m1_t shift = __riscv_vand_vx_u8m1(c, 7, vl);
        vuint8m1_t mask = __riscv_vsll_vv_u8m1(
            __riscv_vmv_v_x_u8m1(1, vl), shift, vl);
        vuint8m1_t hit = __riscv_vand_vv_u8m1(bitmap, mask, vl);
        vbool8_t found = __riscv_vmsne_vx_u8m1_b8(hit, 0, vl);
        const long first = __riscv_vfirst_m_b8(found, vl);
        if (first >= 0) return p + first;
        p += vl;
    }
    return p;
}

PCRE2_SPTR pcre2_match_start_bits_scan_rvv(PCRE2_SPTR p, PCRE2_SPTR end,
                                        PCRE2_SPTR mb_end, const uint8_t *bits,
                                        uint16_t partial, BOOL *nomatch) {
    *nomatch = 0;
    if (bits) {
        p = scan(p, end, bits);
        if (partial == 0 && p >= mb_end) *nomatch = 1;
    }
    return p;
}

PCRE2_SPTR pcre2_dfa_match_start_bits_scan_rvv(PCRE2_SPTR p, PCRE2_SPTR end,
                                            PCRE2_SPTR mb_end, const uint8_t *bits,
                                            uint32_t options, BOOL *nomatch) {
    *nomatch = 0;
    if (bits) {
        p = scan(p, end, bits);
        if (!(options & (PCRE2_PARTIAL_HARD | PCRE2_PARTIAL_SOFT)) && p >= mb_end)
            *nomatch = 1;
    }
    return p;
}
