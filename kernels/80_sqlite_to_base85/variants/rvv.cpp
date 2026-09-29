#include "kernel.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <riscv_vector.h>

// Test intervals without forming an out-of-range pointer (including adjacent
// buffers and in-place encoding). Such cases retain the upstream serial order.
static bool overlaps(const void *a, size_t na, const void *b, size_t nb) {
    const auto x = reinterpret_cast<uintptr_t>(a);
    const auto y = reinterpret_cast<uintptr_t>(b);
    return na && nb && (x <= y ? y - x < na : x - y < nb);
}

char *sqlite_to_base85_rvv(unsigned char *pIn, int nbIn, char *pOut,
                           char *pSep) {
    const size_t groups = static_cast<size_t>(nbIn) / 4;
    const size_t rest = static_cast<size_t>(nbIn) % 4;
    const size_t digits = 5 * groups + (rest ? rest + 1 : 0);
    const size_t sep_len = pSep ? std::strlen(pSep) : 0;
    const size_t sep_count = pSep ? groups / 16 + ((groups % 16 || rest) ? 1 : 0)
                                  : 0;
    const size_t output_len = digits + sep_count * sep_len + 1;
    if (overlaps(pOut, output_len, pIn, static_cast<size_t>(nbIn)) ||
        (pSep && overlaps(pOut, output_len, pSep, sep_len + 1))) {
        return sqlite_to_base85(pIn, nbIn, pOut, pSep);
    }

    size_t remaining = groups;
    int nCol = 0;
    while (remaining) {
        const size_t until_sep = pSep ? 16 - nCol / 5 : remaining;
        const size_t vl = __riscv_vsetvl_e32m4(std::min(remaining, until_sep));
        const vuint32m4_t b0 = __riscv_vzext_vf2_u32m4(
            __riscv_vzext_vf2_u16m2(__riscv_vlse8_v_u8m1(pIn, 4, vl), vl),
            vl);
        const vuint32m4_t b1 = __riscv_vzext_vf2_u32m4(
            __riscv_vzext_vf2_u16m2(__riscv_vlse8_v_u8m1(pIn + 1, 4, vl), vl),
            vl);
        const vuint32m4_t b2 = __riscv_vzext_vf2_u32m4(
            __riscv_vzext_vf2_u16m2(__riscv_vlse8_v_u8m1(pIn + 2, 4, vl), vl),
            vl);
        const vuint32m4_t b3 = __riscv_vzext_vf2_u32m4(
            __riscv_vzext_vf2_u16m2(__riscv_vlse8_v_u8m1(pIn + 3, 4, vl),
                                    vl),
            vl);
        vuint32m4_t value = __riscv_vor_vv_u32m4(
            __riscv_vor_vv_u32m4(__riscv_vsll_vx_u32m4(b0, 24, vl),
                                  __riscv_vsll_vx_u32m4(b1, 16, vl), vl),
            __riscv_vor_vv_u32m4(__riscv_vsll_vx_u32m4(b2, 8, vl), b3, vl),
            vl);

        for (int pos = 4; pos >= 0; --pos) {
            const vuint32m4_t quotient = __riscv_vdivu_vx_u32m4(value, 85, vl);
            const vuint32m4_t digit = __riscv_vsub_vv_u32m4(
                value, __riscv_vmul_vx_u32m4(quotient, 85, vl), vl);
            const vbool8_t small = __riscv_vmsltu_vx_u32m4_b8(digit, 4, vl);
            const vuint32m4_t ascii = __riscv_vadd_vx_u32m4(digit, 38, vl);
            // For digits 0..3 SQLite uses '#'..'&' instead of '*'..'%'.
            const vuint32m4_t encoded = __riscv_vsub_vv_u32m4(
                ascii, __riscv_vmerge_vxm_u32m4(
                           __riscv_vmv_v_x_u32m4(0, vl), 3, small, vl),
                vl);
            const vuint8m1_t bytes = __riscv_vncvt_x_x_w_u8m1(
                __riscv_vncvt_x_x_w_u16m2(encoded, vl), vl);
            __riscv_vsse8_v_u8m1(
                reinterpret_cast<uint8_t *>(pOut + pos), 5, bytes, vl);
            value = quotient;
        }
        pIn += 4 * vl;
        pOut += 5 * vl;
        remaining -= vl;
        if (pSep && (nCol += static_cast<int>(5 * vl)) >= 80) {
            for (const char *s = pSep; *s; ++s)
                *pOut++ = *s;
            nCol = 0;
        }
    }

    if (rest) {
        int nco = static_cast<int>(rest) + 1;
        unsigned long qv = *pIn++;
        int nbe = 1;
        while (nbe++ < static_cast<int>(rest)) {
            qv = (qv << 8) | *pIn++;
        }
        nCol += nco;
        while (nco > 0) {
            unsigned char dv = static_cast<unsigned char>(qv % 85);
            qv /= 85;
            pOut[--nco] = static_cast<char>(dv < 4 ? dv + '#' : dv - 4 + '*');
        }
        pOut += rest + 1;
    }
    if (pSep && nCol > 0) {
        for (const char *s = pSep; *s; ++s)
            *pOut++ = *s;
    }
    *pOut = 0;
    return pOut;
}
