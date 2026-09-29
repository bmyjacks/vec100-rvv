/****************************************************************************
 *
 *
 *  Project: OpenSSL 4.0.2
 *  Source files:
 *    crypto/ec/curve25519.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * crypto/ec/curve25519.c
 *
 * Copyright 2016-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 *
 */

#ifndef KERNELS_19_FE_MUL_INCLUDE_KERNEL_H_
#define KERNELS_19_FE_MUL_INCLUDE_KERNEL_H_

#include <stdint.h>

/* crypto/ec/curve25519.c:802 */
typedef int32_t fe[10];

/* crypto/ec/curve25519.c:807-808 (the two kTop* constants used by
   fe_mul and fe_sq) */
static const int64_t kTop39Bits = 0xfffffffffe000000LL;
static const int64_t kTop38Bits = 0xfffffffffc000000LL;

/*
 * Wrapper for invoking the extracted kernels.
 */
void fe_mul_isolated(fe h, const fe f, const fe g);
void fe_sq_isolated(fe h, const fe f);

#endif // KERNELS_19_FE_MUL_INCLUDE_KERNEL_H_
