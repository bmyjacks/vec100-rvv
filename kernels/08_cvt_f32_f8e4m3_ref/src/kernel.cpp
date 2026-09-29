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

#include <math.h>

/*
 * ref/cvt/cvt_ofp8.c:97-134
 */
uint8_t skl_cvt_f32_f8e4m3(float in, bool is_sat) {
    const uint8_t nan_expr = 0x7F;
    const uint8_t max_bits = 0x7E;

    uint32_t in_bits;
    __builtin_memcpy(&in_bits, &in, sizeof(in));
    uint8_t sign_bits = in_bits & 0x80000000 ? 0x80 : 0x00;

    uint8_t inf_expr = is_sat ? sign_bits | max_bits : nan_expr;

    if (isinf(in)) {
        return inf_expr;
    }
    if (isnan(in)) {
        return nan_expr;
    }

    float abs_in = fabsf(in);
    int32_t exp = (int32_t)floorf(log2f(abs_in));

    if (exp < -6) {
        uint8_t mantissa = (uint8_t)nearbyintf(abs_in * powf(2.0f, 6 + 3));
        return sign_bits | mantissa;
    }
    if (exp > 8) {
        return inf_expr;
    }

    uint8_t mantissa =
        (uint8_t)nearbyintf((abs_in * powf(2.0f, (float)(-exp)) - 1.0f) * 8);
    uint8_t mag_bits = ((uint8_t)(exp + 7) << 3U) + mantissa;
    if (mag_bits > max_bits) {
        return inf_expr;
    }
    return sign_bits | mag_bits;
}

/*
 * ref/cvt/cvt_ofp8.c:174-179
 */
void skl_cvt_f32_f8e4m3_ref(uint8_t *pDst, const float *pSrc,
                            float scaling_factor, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        pDst[i] = skl_cvt_f32_f8e4m3(pSrc[i] * scaling_factor, false);
    }
}
