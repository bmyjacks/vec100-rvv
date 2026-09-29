#include "../include/kernel.h"
#include <riscv_vector.h>

static void feistel_rvv(u32 &s0, u32 &s1, u32 &s2, u32 &s3, const u32 *key);

// Include the exact local S-box tables and encrypt-block source. The variant
// macro changes only the Feistel operation, using the same round schedule.
#define CAMELLIA_RVV_VARIANT
#include "../src/kernel.cpp"
#undef CAMELLIA_RVV_VARIANT

static void feistel_rvv(u32 &s0, u32 &s1, u32 &s2, u32 &s3, const u32 *key) {
    u32 t0 = s0 ^ key[0];
    u32 t1 = s1 ^ key[1];
    static_assert(sizeof(u32) == 4, "Camellia source words must be 32 bits");
    constexpr u32 word_bytes = sizeof(u32);
    // Each of the eight source lookups names one of the four literal 256-word
    // Camellia tables. RVV indexed offsets are byte offsets, not word indices.
    u32 offsets[8] = {
        ((1 * 256) + (t0 & 0xff)) * word_bytes,
        ((3 * 256) + ((t0 >> 8) & 0xff)) * word_bytes,
        ((2 * 256) + ((t0 >> 16) & 0xff)) * word_bytes,
        ((0 * 256) + (t0 >> 24)) * word_bytes,
        ((0 * 256) + (t1 & 0xff)) * word_bytes,
        ((1 * 256) + ((t1 >> 8) & 0xff)) * word_bytes,
        ((3 * 256) + ((t1 >> 16) & 0xff)) * word_bytes,
        ((2 * 256) + (t1 >> 24)) * word_bytes,
    };
    u32 box[8];
    for (size_t i = 0; i < 8;) {
        size_t vl = __riscv_vsetvl_e32m1(8 - i);
        vuint32m1_t idx = __riscv_vle32_v_u32m1(offsets + i, vl);
        vuint32m1_t values =
            __riscv_vloxei32_v_u32m1(&Camellia_SBOX[0][0], idx, vl);
        __riscv_vse32_v_u32m1(box + i, values, vl);
        i += vl;
    }
    u32 t3 = box[0];
    t3 ^= box[1];
    u32 t2 = box[4];
    t3 ^= box[2];
    t2 ^= box[5];
    t3 ^= box[3];
    t2 ^= t3;
    t3 = RightRotate(t3, 8);
    t2 ^= box[6];
    s3 ^= t3;
    t2 ^= box[7];
    s2 ^= t2;
    s3 ^= t2;
}
