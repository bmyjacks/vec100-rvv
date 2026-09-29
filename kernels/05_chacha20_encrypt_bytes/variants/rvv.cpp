#include "kernel.h"

#include <cstdint>
#include <riscv_vector.h>

static uint32_t rotate(uint32_t x, int bits) {
    return (x << bits) | (x >> (32 - bits));
}

#define SCALAR_QR(a, b, c, d)                                                   \
    a += b; d = rotate(d ^ a, 16); c += d; b = rotate(b ^ c, 12);              \
    a += b; d = rotate(d ^ a, 8); c += d; b = rotate(b ^ c, 7)

static void scalar_blocks(chacha_ctx *ctx, const uint8_t *m, uint8_t *c,
                          unsigned long long bytes) {
    while (bytes) {
        uint32_t x[16];
        for (int w = 0; w < 16; ++w)
            x[w] = ctx->input[w];
        for (int round = 0; round < 10; ++round) {
            SCALAR_QR(x[0], x[4], x[8], x[12]);
            SCALAR_QR(x[1], x[5], x[9], x[13]);
            SCALAR_QR(x[2], x[6], x[10], x[14]);
            SCALAR_QR(x[3], x[7], x[11], x[15]);
            SCALAR_QR(x[0], x[5], x[10], x[15]);
            SCALAR_QR(x[1], x[6], x[11], x[12]);
            SCALAR_QR(x[2], x[7], x[8], x[13]);
            SCALAR_QR(x[3], x[4], x[9], x[14]);
        }
        uint8_t stream[64];
        for (int w = 0; w < 16; ++w) {
            const uint32_t v = x[w] + ctx->input[w];
            for (int b = 0; b < 4; ++b)
                stream[4 * w + b] = static_cast<uint8_t>(v >> (8 * b));
        }
        const unsigned int n = bytes < 64 ? bytes : 64;
        uint8_t input[64];
        for (unsigned int j = 0; j < n; ++j)
            input[j] = m[j];
        for (unsigned int j = 0; j < n; ++j)
            c[j] = input[j] ^ stream[j];
        if (++ctx->input[12] == 0)
            ++ctx->input[13];
        m += n;
        c += n;
        bytes -= n;
    }
}

using V = vuint32m1_t;
static V add(V a, V b, size_t vl) { return __riscv_vadd_vv_u32m1(a, b, vl); }
static V xor32(V a, V b, size_t vl) { return __riscv_vxor_vv_u32m1(a, b, vl); }
static V rol(V a, unsigned bits, size_t vl) {
    return __riscv_vor_vv_u32m1(__riscv_vsll_vx_u32m1(a, bits, vl),
                                 __riscv_vsrl_vx_u32m1(a, 32 - bits, vl), vl);
}

#define QR(a, b, c, d)                                                          \
    a = add(a, b, vl); d = rol(xor32(d, a, vl), 16, vl);                         \
    c = add(c, d, vl); b = rol(xor32(b, c, vl), 12, vl);                         \
    a = add(a, b, vl); d = rol(xor32(d, a, vl), 8, vl);                          \
    c = add(c, d, vl); b = rol(xor32(b, c, vl), 7, vl)

void chacha20_encrypt_bytes_rvv(chacha_ctx *ctx, const uint8_t *m,
                                uint8_t *c, unsigned long long bytes) {
    if (!bytes)
        return;
    // The reference snapshots all 16 state words before processing any
    // blocks. Ciphertext may itself overlap the caller's context.
    chacha_ctx snapshot = *ctx;
    chacha_ctx *const original = ctx;
    ctx = &snapshot;
    // The reference processes shifted overlapping buffers block by block.
    // Vector batches are safe for disjoint buffers and exact in-place use.
    const uintptr_t mi = reinterpret_cast<uintptr_t>(m);
    const uintptr_t ci = reinterpret_cast<uintptr_t>(c);
    if ((mi | ci) & 3 || (m != c &&
        ((ci >= mi && ci - mi < bytes) || (mi > ci && mi - ci < bytes)))) {
        scalar_blocks(ctx, m, c, bytes);
        original->input[12] = ctx->input[12];
        original->input[13] = ctx->input[13];
        return;
    }
    unsigned long long blocks = bytes / 64;
    while (blocks) {
        const size_t vl = __riscv_vsetvl_e32m1(blocks);
        const V lane = __riscv_vid_v_u32m1(vl);
        const uint64_t counter = (uint64_t(ctx->input[13]) << 32) | ctx->input[12];
        const V ctr = __riscv_vadd_vx_u32m1(lane, ctx->input[12], vl);
        const auto wrapped = __riscv_vmsltu_vx_u32m1_b32(ctr, ctx->input[12], vl);
        const V high = __riscv_vadd_vx_u32m1(
            __riscv_vmerge_vxm_u32m1(
                __riscv_vmv_v_x_u32m1(0, vl), 1, wrapped, vl),
            ctx->input[13], vl);
#define INIT(w) V j##w = __riscv_vmv_v_x_u32m1(ctx->input[w], vl); V x##w = j##w
        INIT(0); INIT(1); INIT(2); INIT(3);
        INIT(4); INIT(5); INIT(6); INIT(7);
        INIT(8); INIT(9); INIT(10); INIT(11);
        V j12 = ctr, x12 = ctr, j13 = high, x13 = high;
        INIT(14); INIT(15);
#undef INIT
        for (int round = 0; round < 10; ++round) {
            QR(x0, x4, x8, x12);
            QR(x1, x5, x9, x13);
            QR(x2, x6, x10, x14);
            QR(x3, x7, x11, x15);
            QR(x0, x5, x10, x15);
            QR(x1, x6, x11, x12);
            QR(x2, x7, x8, x13);
            QR(x3, x4, x9, x14);
        }
        // Finish all input loads before any output stores (also supports m==c).
#define LOAD(w) V in##w = __riscv_vlse32_v_u32m1(                               \
            reinterpret_cast<const uint32_t *>(m + 4 * w), 64, vl);           \
            x##w = xor32(add(x##w, j##w, vl), in##w, vl)
        LOAD(0); LOAD(1); LOAD(2); LOAD(3);
        LOAD(4); LOAD(5); LOAD(6); LOAD(7);
        LOAD(8); LOAD(9); LOAD(10); LOAD(11);
        LOAD(12); LOAD(13); LOAD(14); LOAD(15);
#undef LOAD
#define STORE(w) __riscv_vsse32_v_u32m1(                                        \
            reinterpret_cast<uint32_t *>(c + 4 * w), 64, x##w, vl)
        STORE(0); STORE(1); STORE(2); STORE(3);
        STORE(4); STORE(5); STORE(6); STORE(7);
        STORE(8); STORE(9); STORE(10); STORE(11);
        STORE(12); STORE(13); STORE(14); STORE(15);
#undef STORE
        const uint64_t next = counter + vl;
        ctx->input[12] = static_cast<uint32_t>(next);
        ctx->input[13] = static_cast<uint32_t>(next >> 32);
        blocks -= vl;
        m += 64 * vl;
        c += 64 * vl;
    }
    scalar_blocks(ctx, m, c, bytes % 64);
    original->input[12] = ctx->input[12];
    original->input[13] = ctx->input[13];
}

#undef QR
#undef SCALAR_QR
