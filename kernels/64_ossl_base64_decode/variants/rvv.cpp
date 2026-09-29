/* RVV counterpart of OpenSSL 4.0.2 crypto/evp/encode.c:396-474.
 * The source-faithful scalar implementation and tables live in src/kernel.cpp.
 */
#include "kernel.h"
#include <cstdint>
#include <riscv_vector.h>

namespace {
unsigned char lookup(unsigned char c, const unsigned char *table) {
    return c & 0x80 ? B64_ERROR : table[c];
}

// One lane is one 4-character quantum. Strided loads/gathers and strided
// stores operate on the actual 4-to-3 loop, rather than vectorizing a copy.
// Every table index is bounded by 127; the high bit is checked separately.
bool chunk(const unsigned char *f, unsigned char *t, size_t count,
           const unsigned char *table, bool write) {
    const size_t table_vl = __riscv_vsetvl_e8m8(128);
    vuint8m8_t lut = __riscv_vle8_v_u8m8(table, table_vl);
    const size_t vl = __riscv_vsetvl_e8m8(count);
    vuint8m8_t x = __riscv_vlse8_v_u8m8(f, 4, vl);
    vuint8m8_t y = __riscv_vlse8_v_u8m8(f + 1, 4, vl);
    vuint8m8_t z = __riscv_vlse8_v_u8m8(f + 2, 4, vl);
    vuint8m8_t w = __riscv_vlse8_v_u8m8(f + 3, 4, vl);
    vuint8m8_t high = __riscv_vor_vv_u8m8(__riscv_vor_vv_u8m8(x, y, vl),
                                            __riscv_vor_vv_u8m8(z, w, vl), vl);
    x = __riscv_vrgather_vv_u8m8(lut, x, vl);
    y = __riscv_vrgather_vv_u8m8(lut, y, vl);
    z = __riscv_vrgather_vv_u8m8(lut, z, vl);
    w = __riscv_vrgather_vv_u8m8(lut, w, vl);
    high = __riscv_vor_vv_u8m8(high, x, vl);
    high = __riscv_vor_vv_u8m8(high, y, vl);
    high = __riscv_vor_vv_u8m8(high, z, vl);
    high = __riscv_vor_vv_u8m8(high, w, vl);
    vuint8m1_t any = __riscv_vredor_vs_u8m8_u8m1(
        high, __riscv_vmv_v_x_u8m1(0, 1), vl);
    if (__riscv_vmv_x_s_u8m1_u8(any) & 0x80)
        return false;
    if (write) {
        vuint8m8_t p = __riscv_vor_vv_u8m8(
            __riscv_vsll_vx_u8m8(x, 2, vl), __riscv_vsrl_vx_u8m8(y, 4, vl), vl);
        vuint8m8_t q = __riscv_vor_vv_u8m8(
            __riscv_vsll_vx_u8m8(y, 4, vl), __riscv_vsrl_vx_u8m8(z, 2, vl), vl);
        vuint8m8_t r = __riscv_vor_vv_u8m8(
            __riscv_vsll_vx_u8m8(z, 6, vl), w, vl);
        __riscv_vsse8_v_u8m8(t, 3, p, vl);
        __riscv_vsse8_v_u8m8(t + 1, 3, q, vl);
        __riscv_vsse8_v_u8m8(t + 2, 3, r, vl);
    }
    return true;
}
} // namespace

int evp_decodeblock_int_rvv(EVP_ENCODE_CTX *ctx, unsigned char *t,
                            const unsigned char *f, int n, int eof) {
    // The scalar path owns all trimming, nonmultiples, and overlapping ranges.
    // In particular, in-place and shifted decoding must read/write one quantum
    // at a time; prefetching a later quantum would change error-side effects.
    if (eof < -1 || eof > 2 || n <= 4)
        return evp_decodeblock_int(ctx, t, f, n, eof);
    const int srp = ctx && (ctx->flags & EVP_ENCODE_CTX_USE_SRP_ALPHABET);
    const unsigned char *table = ossl_base64_decode_table(srp);
    if (lookup(f[0], table) == B64_WS ||
        B64_NOT_BASE64(lookup(f[n - 1], table)) || n % 4 != 0)
        return evp_decodeblock_int(ctx, t, f, n, eof);

    const uintptr_t in = reinterpret_cast<uintptr_t>(f);
    const uintptr_t out = reinterpret_cast<uintptr_t>(t);
    const uintptr_t out_size = static_cast<uintptr_t>(n / 4) * 3;
    if ((out <= in && in - out < out_size) ||
        (out > in && out - in < static_cast<uintptr_t>(n)))
        return evp_decodeblock_int(ctx, t, f, n, eof);

    const size_t blocks = static_cast<size_t>(n / 4 - 1);
    // Prevalidate BEFORE any stores. On invalid input delegate to the source
    // implementation, which commits only the earlier quanta and returns -1.
    // This also preserves source semantics when a context aliases the output.
    for (size_t pos = 0; pos < blocks;) {
        const size_t count = __riscv_vsetvl_e8m8(blocks - pos);
        if (!chunk(f + 4 * pos, nullptr, count, table, false))
            return evp_decodeblock_int(ctx, t, f, n, eof);
        pos += count;
    }
    for (size_t pos = 0; pos < blocks;) {
        const size_t count = __riscv_vsetvl_e8m8(blocks - pos);
        chunk(f + 4 * pos, t + 3 * pos, count, table, true);
        pos += count;
    }

    // Last group uses upstream's eof switch, including explicit eof overrides
    // and the fact that '=' maps to zero (even in a non-final position).
    f += 4 * blocks;
    t += 3 * blocks;
    int a = lookup(f[0], table), b = lookup(f[1], table);
    int c = lookup(f[2], table), d = lookup(f[3], table);
    if ((a | b | c | d) & 0x80)
        return -1;
    unsigned long l = ((((unsigned long)a) << 18L) | (((unsigned long)b) << 12L) | (((unsigned long)c) << 6L) | (((unsigned long)d)));
    if (eof == -1)
        eof = (c == '=') + (d == '=');
    switch (eof) {
    case 2:
        *t = (unsigned char)(l >> 16L) & 0xff;
        break;
    case 1:
        *(t++) = (unsigned char)(l >> 16L) & 0xff;
        *t = (unsigned char)(l >> 8L) & 0xff;
        break;
    case 0:
        *(t++) = (unsigned char)(l >> 16L) & 0xff;
        *(t++) = (unsigned char)(l >> 8L) & 0xff;
        *t = (unsigned char)(l) & 0xff;
        break;
    }
    return static_cast<int>(3 * blocks + 3 - eof);
}
