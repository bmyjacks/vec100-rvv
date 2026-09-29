/****************************************************************************
 *
 *
 *  Project: SiFive Kernel Library (SKL) 3.0.0
 *  Source files:
 *    ref/cvt/cvt_ofp8.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * ref/cvt/cvt_ofp8.c
 *
 * Copyright (c) 2026 SiFive, Inc. All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 * SPDX-License-Identifier: MIT
 *
 */

#include "kernel.h"

/*
 * ref/cvt/cvt_ofp8.c:12-16
 */
static float skl_u32_as_float(uint32_t x) {
    float y;
    __builtin_memcpy(&y, &x, sizeof(float));
    return y;
}

/*
 * ref/cvt/cvt_ofp8.c:18-52
 */
float skl_cvt_f8e4m3_f32(uint8_t in) {
    uint32_t result = 0;

    uint32_t sign = ((uint32_t)in & 0x80U) << 24U;

    uint32_t exponent = ((uint32_t)in >> 3U) & 0x0FU;
    uint32_t mantissa = (uint32_t)in & 0x07U;

    if (exponent == 0U) {
        if (mantissa != 0U) {
            int lz = __builtin_clz(mantissa << 29U);
            uint32_t msk = ~0U << (uint32_t)(2 - lz);
            uint32_t man = (mantissa & ~msk) << (uint32_t)(20 + lz + 1);
            uint32_t exp = (uint32_t)(1 - (lz + 1) - 7 + 127) << 23U;
            result = sign | exp | man;
        } else {
            result = sign;
        }
    } else if (exponent == 0x0FU && mantissa == 0x7U) {
        result = (0xFFU << 23U) | (0x1U << 22U);
    } else {
        uint32_t unbiased_exp = exponent - 7U + 127U;
        result = sign | (unbiased_exp << 23U) | (mantissa << 20U);
    }

    return skl_u32_as_float(result);
}

/*
 * ref/cvt/cvt_ofp8.c:230-235
 */
void skl_cvt_f8e4m3_bf16_ref(__bf16 *pDst, const uint8_t *pSrc, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        pDst[i] = (__bf16)skl_cvt_f8e4m3_f32(pSrc[i]);
    }
}
