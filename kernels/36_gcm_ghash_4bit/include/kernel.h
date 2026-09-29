/****************************************************************************
 *
 *
 *  Project: OpenSSL 4.0.2
 *  Source files:
 *    include/crypto/modes.h
 *    include/internal/endian.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * include/crypto/modes.h
 *
 * Copyright 2010-2022 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 *
 *
 * include/internal/endian.h
 *
 * Copyright 2019-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 *
 */

#ifndef KERNELS_36_GCM_GHASH_4BIT_INCLUDE_KERNEL_H_
#define KERNELS_36_GCM_GHASH_4BIT_INCLUDE_KERNEL_H_

#include <stddef.h>

/* include/crypto/modes.h:22-28 (GCC/Clang LP64 configuration) */
typedef unsigned long long u64;
#define U64(C) C##ULL
typedef unsigned int u32;
typedef unsigned char u8;

/* include/crypto/modes.h:98-99 (unaligned-safe scalar configuration) */
#define PUTU32(p, v)                                                           \
    ((p)[0] = (u8)((v) >> 24), (p)[1] = (u8)((v) >> 16),                       \
     (p)[2] = (u8)((v) >> 8), (p)[3] = (u8)(v))

/* include/crypto/modes.h:101-103 */
typedef struct {
    u64 hi, lo;
} u128;

/* include/internal/endian.h:24-26 (GCC/Clang configuration) */
#define DECLARE_IS_ENDIAN                                                      \
    const int ossl_is_little_endian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define IS_LITTLE_ENDIAN (ossl_is_little_endian)

/* Wrapper for invoking the extracted kernel. */
void gcm_init_4bit_isolated(u128 Htable[16], const u64 H[2]);

/* Wrapper for invoking the extracted kernel. */
void gcm_gmult_4bit_isolated(u64 Xi[2], const u128 Htable[16]);

/* Wrapper for invoking the extracted kernel. */
void gcm_ghash_4bit_isolated(u64 Xi[2], const u128 Htable[16], const u8 *inp,
                             size_t len);

#endif // KERNELS_36_GCM_GHASH_4BIT_INCLUDE_KERNEL_H_
