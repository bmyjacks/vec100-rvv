#include "kernel.h"
#include <riscv_vector.h>

static void reduce(u128 &v) {
    u64 t = 0xe100000000000000ULL & (0ULL - (v.lo & 1));
    v.lo = (v.hi << 63) | (v.lo >> 1);
    v.hi = (v.hi >> 1) ^ t;
}

void gcm_init_4bit_isolated_rvv(u128 table[16], const u64 h[2]) {
    table[0] = {0, 0};
    u128 v = {h[0], h[1]};
    for (int i = 8; i; i >>= 1) { table[i] = v; reduce(v); }
    // Combine the table entries bytewise (u64 in this header is not the
    // unsigned long type used by the RVV e64 intrinsic on LP64 targets).
    for (int bit = 2; bit <= 8; bit <<= 1)
        for (int base = 1; base < bit; ++base)
            for (size_t j = 0; j < sizeof(u128); ) {
                size_t vl = __riscv_vsetvl_e8m1(sizeof(u128) - j);
                auto a = __riscv_vle8_v_u8m1(reinterpret_cast<const u8 *>(&table[base]) + j, vl);
                auto b = __riscv_vle8_v_u8m1(reinterpret_cast<const u8 *>(&table[bit]) + j, vl);
                __riscv_vse8_v_u8m1(reinterpret_cast<u8 *>(&table[bit + base]) + j,
                                     __riscv_vxor_vv_u8m1(a, b, vl), vl);
                j += vl;
            }
}

static const u64 rem[16] = {
    0x0000ULL<<48,0x1c20ULL<<48,0x3840ULL<<48,0x2460ULL<<48,
    0x7080ULL<<48,0x6ca0ULL<<48,0x48c0ULL<<48,0x54e0ULL<<48,
    0xe100ULL<<48,0xfd20ULL<<48,0xd940ULL<<48,0xc560ULL<<48,
    0x9180ULL<<48,0x8da0ULL<<48,0xa9c0ULL<<48,0xb5e0ULL<<48};

static void multiply(u64 xi[2], const u128 table[16], const u8 *inp) {
    u8 bytes[16];
    // Block XOR is part of the GHASH main path (inp == nullptr for gmult).
    for (size_t j = 0; j < 16;) {
        size_t vl = __riscv_vsetvl_e8m1(16 - j);
        auto v = __riscv_vle8_v_u8m1(reinterpret_cast<u8 *>(xi) + j, vl);
        if (inp) v = __riscv_vxor_vv_u8m1(v, __riscv_vle8_v_u8m1(inp + j, vl), vl);
        __riscv_vse8_v_u8m1(bytes + j, v, vl);
        j += vl;
    }
    u8 last = bytes[15];
    u128 z = table[last & 15];
    int count = 15;
    size_t nibble = last >> 4;
    while (true) {
        u64 r = z.lo & 15;
        z.lo = (z.hi << 60) | (z.lo >> 4);
        z.hi = (z.hi >> 4) ^ rem[r] ^ table[nibble].hi;
        z.lo ^= table[nibble].lo;
        if (--count < 0) break;
        last = bytes[count];
        nibble = last & 15;
        r = z.lo & 15;
        z.lo = (z.hi << 60) | (z.lo >> 4);
        z.hi = (z.hi >> 4) ^ rem[r] ^ table[nibble].hi;
        z.lo ^= table[nibble].lo;
        nibble = last >> 4;
    }
    for (int i = 0; i < 8; ++i) {
        reinterpret_cast<u8 *>(xi)[i] = (u8)(z.hi >> (56 - i * 8));
        reinterpret_cast<u8 *>(xi)[i + 8] = (u8)(z.lo >> (56 - i * 8));
    }
}

void gcm_gmult_4bit_isolated_rvv(u64 xi[2], const u128 table[16]) {
    multiply(xi, table, nullptr);
}
void gcm_ghash_4bit_isolated_rvv(u64 xi[2], const u128 table[16], const u8 *inp, size_t len) {
    do { multiply(xi, table, inp); inp += 16; len -= 16; } while (len > 0);
}
