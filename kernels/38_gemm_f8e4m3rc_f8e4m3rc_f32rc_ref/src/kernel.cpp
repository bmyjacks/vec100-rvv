/****************************************************************************
 *
 *
 *  Project: SiFive Kernel Library (SKL) 3.0.0
 *  Source files:
 *    ref/cvt/cvt_ofp8.c
 *    ref/gemm/gemm_f8e4m3rc_f8e4m3rc_f32rc.c
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
 * ref/gemm/gemm_f8e4m3rc_f8e4m3rc_f32rc.c
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
 * ref/gemm/gemm_f8e4m3rc_f8e4m3rc_f32rc.c:11-25
 */
void skl_gemm_f8e4m3rc_f8e4m3rc_f32rc_ref(size_t m, size_t n, size_t k,
                                          float alpha, const uint8_t *a,
                                          size_t rsa, size_t csa,
                                          const uint8_t *b, size_t rsb,
                                          size_t csb, float beta, float *c,
                                          size_t rsc, size_t csc) {
    for (size_t ii = 0; ii < m; ii++) {
        for (size_t jj = 0; jj < n; jj++) {
            float acc = 0;
            for (size_t kk = 0; kk < k; kk++) {
                acc += skl_cvt_f8e4m3_f32(a[ii * rsa + kk * csa]) *
                       skl_cvt_f8e4m3_f32(b[kk * rsb + jj * csb]);
            }
            c[ii * rsc + jj * csc] =
                beta * c[ii * rsc + jj * csc] + alpha * acc;
        }
    }
}
