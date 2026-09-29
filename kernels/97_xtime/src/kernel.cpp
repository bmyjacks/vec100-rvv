/****************************************************************************
 *
 *
 *  Project: libsodium 1.0.22
 *  Source files:
 *    src/libsodium/crypto_core/softaes/softaes.c
 *  License file: LICENSE
 *
 *
 *  The upstream source file has no per-file license header. The applicable
 *  project copyright and license notice follows, copied from LICENSE.
 *
 *
 * LICENSE
 *
 * ISC License
 *
 * Copyright (c) 2013-2026
 * Frank Denis <j at pureftpd dot org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <stddef.h>
#include <stdint.h>

#include "kernel.h"

/*
 * src/libsodium/crypto_core/softaes/softaes.c:121-125
 */
static inline uint8_t xtime(uint8_t a) {
    return (uint8_t)((a << 1) ^ (((a >> 7) & 1) * 0x1b));
}

/*
 * src/libsodium/crypto_core/softaes/softaes.c:127-131
 */
static inline uint8_t gf_mul_09(uint8_t a) {
    return xtime(xtime(xtime(a))) ^ a;
}

/*
 * src/libsodium/crypto_core/softaes/softaes.c:133-137
 */
static inline uint8_t gf_mul_0b(uint8_t a) {
    return xtime(xtime(xtime(a)) ^ a) ^ a;
}

/*
 * src/libsodium/crypto_core/softaes/softaes.c:139-143
 */
static inline uint8_t gf_mul_0d(uint8_t a) {
    return xtime(xtime(xtime(a) ^ a)) ^ a;
}

/*
 * src/libsodium/crypto_core/softaes/softaes.c:145-149
 */
static inline uint8_t gf_mul_0e(uint8_t a) {
    return xtime(xtime(xtime(a) ^ a) ^ a);
}

/*
 * src/libsodium/crypto_core/softaes/softaes.c:151-165
 */
static uint32_t inv_mix_column(uint32_t col) {
    uint8_t b0 = (uint8_t)col;
    uint8_t b1 = (uint8_t)(col >> 8);
    uint8_t b2 = (uint8_t)(col >> 16);
    uint8_t b3 = (uint8_t)(col >> 24);

    uint8_t r0 = gf_mul_0e(b0) ^ gf_mul_0b(b1) ^ gf_mul_0d(b2) ^ gf_mul_09(b3);
    uint8_t r1 = gf_mul_09(b0) ^ gf_mul_0e(b1) ^ gf_mul_0b(b2) ^ gf_mul_0d(b3);
    uint8_t r2 = gf_mul_0d(b0) ^ gf_mul_09(b1) ^ gf_mul_0e(b2) ^ gf_mul_0b(b3);
    uint8_t r3 = gf_mul_0b(b0) ^ gf_mul_0d(b1) ^ gf_mul_09(b2) ^ gf_mul_0e(b3);

    return (uint32_t)r0 | ((uint32_t)r1 << 8) | ((uint32_t)r2 << 16) |
           ((uint32_t)r3 << 24);
}

/*
 * src/libsodium/crypto_core/softaes/softaes.c:167-176
 */
SoftAesBlock softaes_inv_mix_columns(const SoftAesBlock block) {
    SoftAesBlock out;
    out.w0 = inv_mix_column(block.w0);
    out.w1 = inv_mix_column(block.w1);
    out.w2 = inv_mix_column(block.w2);
    out.w3 = inv_mix_column(block.w3);
    return out;
}

/*
 * src/libsodium/crypto_core/softaes/softaes.c:178-186
 */
void softaes_invert_key_schedule128(SoftAesBlock rkeys[11]) {
    size_t i;

    for (i = 1; i < 10; i++) {
        rkeys[i] = softaes_inv_mix_columns(rkeys[i]);
    }
}

/*
 * src/libsodium/crypto_core/softaes/softaes.c:188-196
 */
void softaes_invert_key_schedule256(SoftAesBlock rkeys[15]) {
    size_t i;

    for (i = 1; i < 14; i++) {
        rkeys[i] = softaes_inv_mix_columns(rkeys[i]);
    }
}
