/****************************************************************************
 *
 *
 *  Project: OpenSSL 4.0.2
 *  Source files:
 *    crypto/camellia/cmll_local.h
 *    include/openssl/camellia.h
 *
 *  The original file copyright and license notices follow.
 *
 */

/* crypto/camellia/cmll_local.h:1-23 */
/*
 * Copyright 2006-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/* ====================================================================
 * Copyright 2006 NTT (Nippon Telegraph and Telephone Corporation) .
 * ALL RIGHTS RESERVED.
 *
 * Intellectual Property information for Camellia:
 *     http://info.isl.ntt.co.jp/crypt/eng/info/chiteki.html
 *
 * News Release for Announcement of Camellia open source:
 *     http://www.ntt.co.jp/news/news06e/0604/060413a.html
 *
 * The Camellia Code included herein is developed by
 * NTT (Nippon Telegraph and Telephone Corporation), and is contributed
 * to the OpenSSL project.
 */

/* include/openssl/camellia.h:1-8 */
/*
 * Copyright 2006-2026 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef KERNELS_03_CAMELLIA_ROUNDS_INCLUDE_KERNEL_H_
#define KERNELS_03_CAMELLIA_ROUNDS_INCLUDE_KERNEL_H_

/* crypto/camellia/cmll_local.h:28-29 */
typedef unsigned int u32;
typedef unsigned char u8;

/* include/openssl/camellia.h:41-45 */
#define CAMELLIA_TABLE_BYTE_LEN 272
#define CAMELLIA_TABLE_WORD_LEN (CAMELLIA_TABLE_BYTE_LEN / 4)
typedef unsigned int KEY_TABLE_TYPE[CAMELLIA_TABLE_WORD_LEN];

/* crypto/camellia/cmll_local.h:33-35; original function signature. */
#ifdef __cplusplus
extern "C" {
#endif
void Camellia_EncryptBlock_Rounds(int grandRounds, const u8 plaintext[],
                                  const KEY_TABLE_TYPE keyTable, u8 ciphertext[]);

/* Wrapper for the RVV implementation of the same function signature. */
void Camellia_EncryptBlock_Rounds_rvv(int grandRounds, const u8 plaintext[],
                                      const KEY_TABLE_TYPE keyTable,
                                      u8 ciphertext[]);
#ifdef __cplusplus
}
#endif

#endif // KERNELS_03_CAMELLIA_ROUNDS_INCLUDE_KERNEL_H_
