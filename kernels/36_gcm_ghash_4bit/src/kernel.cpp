/****************************************************************************
 *
 *
 *  Project: OpenSSL 4.0.2
 *  Source files:
 *    crypto/modes/gcm128.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * crypto/modes/gcm128.c
 *
 * Copyright 2010-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 *
 */

#include "kernel.h"

/* crypto/modes/gcm128.c:36-48 */
#define PACK(s) ((size_t)(s) << (sizeof(size_t) * 8 - 16))
#define REDUCE1BIT(V)                                                          \
    do {                                                                       \
        if (sizeof(size_t) == 8) {                                             \
            u64 T = U64(0xe100000000000000) & (0 - (V.lo & 1));                \
            V.lo = (V.hi << 63) | (V.lo >> 1);                                 \
            V.hi = (V.hi >> 1) ^ T;                                            \
        } else {                                                               \
            u32 T = 0xe1000000U & (0 - (u32)(V.lo & 1));                       \
            V.lo = (V.hi << 63) | (V.lo >> 1);                                 \
            V.hi = (V.hi >> 1) ^ ((u64)T << 32);                               \
        }                                                                      \
    } while (0)

/* crypto/modes/gcm128.c:88-90 */
static void gcm_init_4bit(u128 Htable[16], const u64 H[2]) {
    u128 V;

    /* crypto/modes/gcm128.c:95-99 */
    Htable[0].hi = 0;
    Htable[0].lo = 0;
    V.hi = H[0];
    V.lo = H[1];

    /* crypto/modes/gcm128.c:115-134 */
    Htable[8] = V;
    REDUCE1BIT(V);
    Htable[4] = V;
    REDUCE1BIT(V);
    Htable[2] = V;
    REDUCE1BIT(V);
    Htable[1] = V;
    Htable[3].hi = V.hi ^ Htable[2].hi, Htable[3].lo = V.lo ^ Htable[2].lo;
    V = Htable[4];
    Htable[5].hi = V.hi ^ Htable[1].hi, Htable[5].lo = V.lo ^ Htable[1].lo;
    Htable[6].hi = V.hi ^ Htable[2].hi, Htable[6].lo = V.lo ^ Htable[2].lo;
    Htable[7].hi = V.hi ^ Htable[3].hi, Htable[7].lo = V.lo ^ Htable[3].lo;
    V = Htable[8];
    Htable[9].hi = V.hi ^ Htable[1].hi, Htable[9].lo = V.lo ^ Htable[1].lo;
    Htable[10].hi = V.hi ^ Htable[2].hi, Htable[10].lo = V.lo ^ Htable[2].lo;
    Htable[11].hi = V.hi ^ Htable[3].hi, Htable[11].lo = V.lo ^ Htable[3].lo;
    Htable[12].hi = V.hi ^ Htable[4].hi, Htable[12].lo = V.lo ^ Htable[4].lo;
    Htable[13].hi = V.hi ^ Htable[5].hi, Htable[13].lo = V.lo ^ Htable[5].lo;
    Htable[14].hi = V.hi ^ Htable[6].hi, Htable[14].lo = V.lo ^ Htable[6].lo;
    Htable[15].hi = V.hi ^ Htable[7].hi, Htable[15].lo = V.lo ^ Htable[7].lo;
}

/* crypto/modes/gcm128.c:161-166 */
static const size_t rem_4bit[16] = {
    PACK(0x0000), PACK(0x1C20), PACK(0x3840), PACK(0x2460),
    PACK(0x7080), PACK(0x6CA0), PACK(0x48C0), PACK(0x54E0),
    PACK(0xE100), PACK(0xFD20), PACK(0xD940), PACK(0xC560),
    PACK(0x9180), PACK(0x8DA0), PACK(0xA9C0), PACK(0xB5E0)};

/* crypto/modes/gcm128.c:168-233 */
static void gcm_gmult_4bit(u64 Xi[2], const u128 Htable[16]) {
    u128 Z;
    int cnt = 15;
    size_t rem, nlo, nhi;
    DECLARE_IS_ENDIAN;

    nlo = ((const u8 *)Xi)[15];
    nhi = nlo >> 4;
    nlo &= 0xf;

    Z.hi = Htable[nlo].hi;
    Z.lo = Htable[nlo].lo;

    while (1) {
        rem = (size_t)Z.lo & 0xf;
        Z.lo = (Z.hi << 60) | (Z.lo >> 4);
        Z.hi = (Z.hi >> 4);
        if (sizeof(size_t) == 8)
            Z.hi ^= rem_4bit[rem];
        else
            Z.hi ^= (u64)rem_4bit[rem] << 32;

        Z.hi ^= Htable[nhi].hi;
        Z.lo ^= Htable[nhi].lo;

        if (--cnt < 0)
            break;

        nlo = ((const u8 *)Xi)[cnt];
        nhi = nlo >> 4;
        nlo &= 0xf;

        rem = (size_t)Z.lo & 0xf;
        Z.lo = (Z.hi << 60) | (Z.lo >> 4);
        Z.hi = (Z.hi >> 4);
        if (sizeof(size_t) == 8)
            Z.hi ^= rem_4bit[rem];
        else
            Z.hi ^= (u64)rem_4bit[rem] << 32;

        Z.hi ^= Htable[nlo].hi;
        Z.lo ^= Htable[nlo].lo;
    }

    if (IS_LITTLE_ENDIAN) {
        u8 *p = (u8 *)Xi;
        u32 v;
        v = (u32)(Z.hi >> 32);
        PUTU32(p, v);
        v = (u32)(Z.hi);
        PUTU32(p + 4, v);
        v = (u32)(Z.lo >> 32);
        PUTU32(p + 8, v);
        v = (u32)(Z.lo);
        PUTU32(p + 12, v);
    } else {
        Xi[0] = Z.hi;
        Xi[1] = Z.lo;
    }
}

/* crypto/modes/gcm128.c:246-321 */
static void gcm_ghash_4bit(u64 Xi[2], const u128 Htable[16], const u8 *inp,
                           size_t len) {
    u128 Z;
    int cnt;
    size_t rem, nlo, nhi;
    DECLARE_IS_ENDIAN;

    do {
        cnt = 15;
        nlo = ((const u8 *)Xi)[15];
        nlo ^= inp[15];
        nhi = nlo >> 4;
        nlo &= 0xf;

        Z.hi = Htable[nlo].hi;
        Z.lo = Htable[nlo].lo;

        while (1) {
            rem = (size_t)Z.lo & 0xf;
            Z.lo = (Z.hi << 60) | (Z.lo >> 4);
            Z.hi = (Z.hi >> 4);
            if (sizeof(size_t) == 8)
                Z.hi ^= rem_4bit[rem];
            else
                Z.hi ^= (u64)rem_4bit[rem] << 32;

            Z.hi ^= Htable[nhi].hi;
            Z.lo ^= Htable[nhi].lo;

            if (--cnt < 0)
                break;

            nlo = ((const u8 *)Xi)[cnt];
            nlo ^= inp[cnt];
            nhi = nlo >> 4;
            nlo &= 0xf;

            rem = (size_t)Z.lo & 0xf;
            Z.lo = (Z.hi << 60) | (Z.lo >> 4);
            Z.hi = (Z.hi >> 4);
            if (sizeof(size_t) == 8)
                Z.hi ^= rem_4bit[rem];
            else
                Z.hi ^= (u64)rem_4bit[rem] << 32;

            Z.hi ^= Htable[nlo].hi;
            Z.lo ^= Htable[nlo].lo;
        }

        if (IS_LITTLE_ENDIAN) {
            u8 *p = (u8 *)Xi;
            u32 v;
            v = (u32)(Z.hi >> 32);
            PUTU32(p, v);
            v = (u32)(Z.hi);
            PUTU32(p + 4, v);
            v = (u32)(Z.lo >> 32);
            PUTU32(p + 8, v);
            v = (u32)(Z.lo);
            PUTU32(p + 12, v);
        } else {
            Xi[0] = Z.hi;
            Xi[1] = Z.lo;
        }

        inp += 16;

        len -= 16;
    } while (len > 0);
}

/* Wrapper for invoking the extracted kernel. */
void gcm_init_4bit_isolated(u128 Htable[16], const u64 H[2]) {
    gcm_init_4bit(Htable, H);
}

/* Wrapper for invoking the extracted kernel. */
void gcm_gmult_4bit_isolated(u64 Xi[2], const u128 Htable[16]) {
    gcm_gmult_4bit(Xi, Htable);
}

/* Wrapper for invoking the extracted kernel. */
void gcm_ghash_4bit_isolated(u64 Xi[2], const u128 Htable[16], const u8 *inp,
                             size_t len) {
    gcm_ghash_4bit(Xi, Htable, inp, len);
}
