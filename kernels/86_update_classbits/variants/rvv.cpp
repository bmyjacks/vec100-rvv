#include "kernel.h"

#include <riscv_vector.h>

// Preserve the upstream property-table lookups, but pack the 256 independent
// per-codepoint decisions with RVV mask stores. Each mask store covers complete
// bytes; the existing bitmap is ORed in, rather than replaced.
void update_classbits_rvv(uint32_t ptype, uint32_t pdata, BOOL negated,
                          uint8_t *classbits) {
    if (ptype == PT_ANY) {
        if (!negated) memset(classbits, 0xff, 32);
        return;
    }
    uint8_t match[256];
    for (unsigned c = 0; c < 256; ++c) {
        const ucd_record *prop = GET_UCD(c);
        uint32_t gentype;
        int chartype;
        BOOL set_bit = FALSE;
        switch (ptype) {
        case PT_LAMP:
            chartype = prop->chartype;
            set_bit = chartype == ucp_Lu || chartype == ucp_Ll || chartype == ucp_Lt;
            break;
        case PT_GC: set_bit = PRIV(ucp_gentype)[prop->chartype] == pdata; break;
        case PT_PC: set_bit = prop->chartype == pdata; break;
        case PT_SC: set_bit = prop->script == pdata; break;
        case PT_SCX:
            set_bit = prop->script == pdata ||
                MAPBIT(PRIV(ucd_script_sets) + UCD_SCRIPTX_PROP(prop), pdata) != 0;
            break;
        case PT_ALNUM:
            gentype = PRIV(ucp_gentype)[prop->chartype];
            set_bit = gentype == ucp_L || gentype == ucp_N;
            break;
        case PT_SPACE:
        case PT_PXSPACE:
            switch (c) {
            HSPACE_BYTE_CASES:
            VSPACE_BYTE_CASES:
                set_bit = TRUE;
                break;
            default:
                set_bit = PRIV(ucp_gentype)[prop->chartype] == ucp_Z;
                break;
            }
            break;
        case PT_WORD:
            chartype = prop->chartype;
            gentype = PRIV(ucp_gentype)[chartype];
            set_bit = gentype == ucp_L || gentype == ucp_N ||
                chartype == ucp_Mn || chartype == ucp_Pc;
            break;
        case PT_UCNC:
            set_bit = c == CHAR_DOLLAR_SIGN || c == CHAR_COMMERCIAL_AT ||
                c == CHAR_GRAVE_ACCENT || c >= 0xa0;
            break;
        case PT_BIDICL: set_bit = UCD_BIDICLASS_PROP(prop) == pdata; break;
        case PT_BOOL:
            set_bit = MAPBIT(PRIV(ucd_boolprop_sets) + UCD_BPROPS_PROP(prop), pdata) != 0;
            break;
        case PT_PXGRAPH:
            chartype = prop->chartype;
            gentype = PRIV(ucp_gentype)[chartype];
            set_bit = gentype != ucp_Z && (gentype != ucp_C || chartype == ucp_Cf);
            break;
        case PT_PXPRINT:
            chartype = prop->chartype;
            set_bit = chartype != ucp_Zl && chartype != ucp_Zp &&
                (PRIV(ucp_gentype)[chartype] != ucp_C || chartype == ucp_Cf);
            break;
        case PT_PXPUNCT:
            gentype = PRIV(ucp_gentype)[prop->chartype];
            set_bit = gentype == ucp_P || (c < 128 && gentype == ucp_S);
            break;
        default:
            set_bit = (c >= CHAR_0 && c <= CHAR_9) ||
                (c >= CHAR_A && c <= CHAR_F) || (c >= CHAR_a && c <= CHAR_f);
            break;
        }
        match[c] = (set_bit != 0) != (negated != 0);
    }
    for (size_t i = 0; i < 256;) {
        size_t vl = __riscv_vsetvl_e8m1(256 - i);
        vl &= ~size_t(7); // mask stores must not overwrite the next byte
        const auto values = __riscv_vle8_v_u8m1(match + i, vl);
        const auto mask = __riscv_vmsne_vx_u8m1_b8(values, 0, vl);
        uint8_t packed[32];
        __riscv_vsm_v_b8(packed, mask, vl);
        const size_t bytes = vl / 8;
        const auto old = __riscv_vle8_v_u8m1(classbits + i / 8, bytes);
        const auto bits = __riscv_vle8_v_u8m1(packed, bytes);
        __riscv_vse8_v_u8m1(classbits + i / 8, __riscv_vor_vv_u8m1(old, bits, bytes), bytes);
        i += vl;
    }
}
