/* OpenSSL 4.0.2, crypto/evp/encode.c and crypto/evp/evp_local.h.
 * Copyright 1995-2026 The OpenSSL Project Authors. All Rights Reserved.
 * crypto/evp/evp_local.h: Copyright 2000-2026 The OpenSSL Project Authors.
 * include/openssl/types.h: Copyright 2001-2026 The OpenSSL Project Authors.
 * include/crypto/evp.h: Copyright 2015-2026 The OpenSSL Project Authors.
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * of the license at https://www.openssl.org/source/license.html
 * Revision a586d91e9f9203b942ab2b10b5ba805e97f20fc1.
 */
#ifndef KERNELS_129_OSSL_BASE64_DECODE_KERNEL_H_
#define KERNELS_129_OSSL_BASE64_DECODE_KERNEL_H_

#include <stddef.h>

/* include/openssl/types.h:136; crypto/evp/evp_local.h:279-287 */
typedef struct evp_Encode_Ctx_st EVP_ENCODE_CTX;
struct evp_Encode_Ctx_st {
    int num;
    unsigned char enc_data[80];
    int line_num;
    unsigned int flags;
};

/* include/crypto/evp.h:627; crypto/evp/encode.c:56-58 */
#define EVP_ENCODE_CTX_USE_SRP_ALPHABET 2
#define B64_WS 0xE0
#define B64_ERROR 0xFF
#define B64_NOT_BASE64(a) (((a) | 0x13) == 0xF3)

/* Upstream static linkage removed for the independent extraction. */
int evp_decodeblock_int(EVP_ENCODE_CTX *ctx, unsigned char *t,
                        const unsigned char *f, int n, int eof);
int evp_decodeblock_int_rvv(EVP_ENCODE_CTX *ctx, unsigned char *t,
                            const unsigned char *f, int n, int eof);

/* Internal bridge: expose the exact upstream reverse tables to the separately
 * compiled RVV implementation without maintaining divergent copies. */
const unsigned char *ossl_base64_decode_table(int srp);
#endif
