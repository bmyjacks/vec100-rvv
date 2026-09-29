/****************************************************************************
 *
 *
 *  Project: OpenSSL 4.0.2
 *  Source files:
 *    include/openssl/e_os2.h
 *    include/crypto/sm4.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * include/openssl/e_os2.h
 *
 * Copyright 1995-2026 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 *
 *
 * include/crypto/sm4.h
 *
 * Copyright 2017-2021 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright 2017 Ribose Inc. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 *
 */

#ifndef KERNELS_65_OSSL_SM4_ENCRYPT_INCLUDE_KERNEL_H_
#define KERNELS_65_OSSL_SM4_ENCRYPT_INCLUDE_KERNEL_H_

#include <stdint.h>

/*
 * include/openssl/e_os2.h:272
 */
#define ossl_inline inline

/*
 * include/crypto/sm4.h:26-30
 */
#define SM4_KEY_SCHEDULE 32

typedef struct SM4_KEY_st {
    uint32_t rk[SM4_KEY_SCHEDULE];
} SM4_KEY;

/*
 * include/crypto/sm4.h:32-36
 */
int ossl_sm4_set_key(const uint8_t *key, SM4_KEY *ks);

void ossl_sm4_encrypt(const uint8_t *in, uint8_t *out, const SM4_KEY *ks);

void ossl_sm4_decrypt(const uint8_t *in, uint8_t *out, const SM4_KEY *ks);

#endif // KERNELS_65_OSSL_SM4_ENCRYPT_INCLUDE_KERNEL_H_
