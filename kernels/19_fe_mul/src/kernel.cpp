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

#include "kernel.h"

/* crypto/ec/curve25519.c:1105-1344 */
static void fe_mul(fe h, const fe f, const fe g) {
    int32_t f0 = f[0];
    int32_t f1 = f[1];
    int32_t f2 = f[2];
    int32_t f3 = f[3];
    int32_t f4 = f[4];
    int32_t f5 = f[5];
    int32_t f6 = f[6];
    int32_t f7 = f[7];
    int32_t f8 = f[8];
    int32_t f9 = f[9];
    int32_t g0 = g[0];
    int32_t g1 = g[1];
    int32_t g2 = g[2];
    int32_t g3 = g[3];
    int32_t g4 = g[4];
    int32_t g5 = g[5];
    int32_t g6 = g[6];
    int32_t g7 = g[7];
    int32_t g8 = g[8];
    int32_t g9 = g[9];
    int32_t g1_19 = 19 * g1;
    int32_t g2_19 = 19 * g2;
    int32_t g3_19 = 19 * g3;
    int32_t g4_19 = 19 * g4;
    int32_t g5_19 = 19 * g5;
    int32_t g6_19 = 19 * g6;
    int32_t g7_19 = 19 * g7;
    int32_t g8_19 = 19 * g8;
    int32_t g9_19 = 19 * g9;
    int32_t f1_2 = 2 * f1;
    int32_t f3_2 = 2 * f3;
    int32_t f5_2 = 2 * f5;
    int32_t f7_2 = 2 * f7;
    int32_t f9_2 = 2 * f9;
    int64_t f0g0 = f0 * (int64_t)g0;
    int64_t f0g1 = f0 * (int64_t)g1;
    int64_t f0g2 = f0 * (int64_t)g2;
    int64_t f0g3 = f0 * (int64_t)g3;
    int64_t f0g4 = f0 * (int64_t)g4;
    int64_t f0g5 = f0 * (int64_t)g5;
    int64_t f0g6 = f0 * (int64_t)g6;
    int64_t f0g7 = f0 * (int64_t)g7;
    int64_t f0g8 = f0 * (int64_t)g8;
    int64_t f0g9 = f0 * (int64_t)g9;
    int64_t f1g0 = f1 * (int64_t)g0;
    int64_t f1g1_2 = f1_2 * (int64_t)g1;
    int64_t f1g2 = f1 * (int64_t)g2;
    int64_t f1g3_2 = f1_2 * (int64_t)g3;
    int64_t f1g4 = f1 * (int64_t)g4;
    int64_t f1g5_2 = f1_2 * (int64_t)g5;
    int64_t f1g6 = f1 * (int64_t)g6;
    int64_t f1g7_2 = f1_2 * (int64_t)g7;
    int64_t f1g8 = f1 * (int64_t)g8;
    int64_t f1g9_38 = f1_2 * (int64_t)g9_19;
    int64_t f2g0 = f2 * (int64_t)g0;
    int64_t f2g1 = f2 * (int64_t)g1;
    int64_t f2g2 = f2 * (int64_t)g2;
    int64_t f2g3 = f2 * (int64_t)g3;
    int64_t f2g4 = f2 * (int64_t)g4;
    int64_t f2g5 = f2 * (int64_t)g5;
    int64_t f2g6 = f2 * (int64_t)g6;
    int64_t f2g7 = f2 * (int64_t)g7;
    int64_t f2g8_19 = f2 * (int64_t)g8_19;
    int64_t f2g9_19 = f2 * (int64_t)g9_19;
    int64_t f3g0 = f3 * (int64_t)g0;
    int64_t f3g1_2 = f3_2 * (int64_t)g1;
    int64_t f3g2 = f3 * (int64_t)g2;
    int64_t f3g3_2 = f3_2 * (int64_t)g3;
    int64_t f3g4 = f3 * (int64_t)g4;
    int64_t f3g5_2 = f3_2 * (int64_t)g5;
    int64_t f3g6 = f3 * (int64_t)g6;
    int64_t f3g7_38 = f3_2 * (int64_t)g7_19;
    int64_t f3g8_19 = f3 * (int64_t)g8_19;
    int64_t f3g9_38 = f3_2 * (int64_t)g9_19;
    int64_t f4g0 = f4 * (int64_t)g0;
    int64_t f4g1 = f4 * (int64_t)g1;
    int64_t f4g2 = f4 * (int64_t)g2;
    int64_t f4g3 = f4 * (int64_t)g3;
    int64_t f4g4 = f4 * (int64_t)g4;
    int64_t f4g5 = f4 * (int64_t)g5;
    int64_t f4g6_19 = f4 * (int64_t)g6_19;
    int64_t f4g7_19 = f4 * (int64_t)g7_19;
    int64_t f4g8_19 = f4 * (int64_t)g8_19;
    int64_t f4g9_19 = f4 * (int64_t)g9_19;
    int64_t f5g0 = f5 * (int64_t)g0;
    int64_t f5g1_2 = f5_2 * (int64_t)g1;
    int64_t f5g2 = f5 * (int64_t)g2;
    int64_t f5g3_2 = f5_2 * (int64_t)g3;
    int64_t f5g4 = f5 * (int64_t)g4;
    int64_t f5g5_38 = f5_2 * (int64_t)g5_19;
    int64_t f5g6_19 = f5 * (int64_t)g6_19;
    int64_t f5g7_38 = f5_2 * (int64_t)g7_19;
    int64_t f5g8_19 = f5 * (int64_t)g8_19;
    int64_t f5g9_38 = f5_2 * (int64_t)g9_19;
    int64_t f6g0 = f6 * (int64_t)g0;
    int64_t f6g1 = f6 * (int64_t)g1;
    int64_t f6g2 = f6 * (int64_t)g2;
    int64_t f6g3 = f6 * (int64_t)g3;
    int64_t f6g4_19 = f6 * (int64_t)g4_19;
    int64_t f6g5_19 = f6 * (int64_t)g5_19;
    int64_t f6g6_19 = f6 * (int64_t)g6_19;
    int64_t f6g7_19 = f6 * (int64_t)g7_19;
    int64_t f6g8_19 = f6 * (int64_t)g8_19;
    int64_t f6g9_19 = f6 * (int64_t)g9_19;
    int64_t f7g0 = f7 * (int64_t)g0;
    int64_t f7g1_2 = f7_2 * (int64_t)g1;
    int64_t f7g2 = f7 * (int64_t)g2;
    int64_t f7g3_38 = f7_2 * (int64_t)g3_19;
    int64_t f7g4_19 = f7 * (int64_t)g4_19;
    int64_t f7g5_38 = f7_2 * (int64_t)g5_19;
    int64_t f7g6_19 = f7 * (int64_t)g6_19;
    int64_t f7g7_38 = f7_2 * (int64_t)g7_19;
    int64_t f7g8_19 = f7 * (int64_t)g8_19;
    int64_t f7g9_38 = f7_2 * (int64_t)g9_19;
    int64_t f8g0 = f8 * (int64_t)g0;
    int64_t f8g1 = f8 * (int64_t)g1;
    int64_t f8g2_19 = f8 * (int64_t)g2_19;
    int64_t f8g3_19 = f8 * (int64_t)g3_19;
    int64_t f8g4_19 = f8 * (int64_t)g4_19;
    int64_t f8g5_19 = f8 * (int64_t)g5_19;
    int64_t f8g6_19 = f8 * (int64_t)g6_19;
    int64_t f8g7_19 = f8 * (int64_t)g7_19;
    int64_t f8g8_19 = f8 * (int64_t)g8_19;
    int64_t f8g9_19 = f8 * (int64_t)g9_19;
    int64_t f9g0 = f9 * (int64_t)g0;
    int64_t f9g1_38 = f9_2 * (int64_t)g1_19;
    int64_t f9g2_19 = f9 * (int64_t)g2_19;
    int64_t f9g3_38 = f9_2 * (int64_t)g3_19;
    int64_t f9g4_19 = f9 * (int64_t)g4_19;
    int64_t f9g5_38 = f9_2 * (int64_t)g5_19;
    int64_t f9g6_19 = f9 * (int64_t)g6_19;
    int64_t f9g7_38 = f9_2 * (int64_t)g7_19;
    int64_t f9g8_19 = f9 * (int64_t)g8_19;
    int64_t f9g9_38 = f9_2 * (int64_t)g9_19;
    int64_t h0 = f0g0 + f1g9_38 + f2g8_19 + f3g7_38 + f4g6_19 + f5g5_38 +
                 f6g4_19 + f7g3_38 + f8g2_19 + f9g1_38;
    int64_t h1 = f0g1 + f1g0 + f2g9_19 + f3g8_19 + f4g7_19 + f5g6_19 + f6g5_19 +
                 f7g4_19 + f8g3_19 + f9g2_19;
    int64_t h2 = f0g2 + f1g1_2 + f2g0 + f3g9_38 + f4g8_19 + f5g7_38 + f6g6_19 +
                 f7g5_38 + f8g4_19 + f9g3_38;
    int64_t h3 = f0g3 + f1g2 + f2g1 + f3g0 + f4g9_19 + f5g8_19 + f6g7_19 +
                 f7g6_19 + f8g5_19 + f9g4_19;
    int64_t h4 = f0g4 + f1g3_2 + f2g2 + f3g1_2 + f4g0 + f5g9_38 + f6g8_19 +
                 f7g7_38 + f8g6_19 + f9g5_38;
    int64_t h5 = f0g5 + f1g4 + f2g3 + f3g2 + f4g1 + f5g0 + f6g9_19 + f7g8_19 +
                 f8g7_19 + f9g6_19;
    int64_t h6 = f0g6 + f1g5_2 + f2g4 + f3g3_2 + f4g2 + f5g1_2 + f6g0 +
                 f7g9_38 + f8g8_19 + f9g7_38;
    int64_t h7 = f0g7 + f1g6 + f2g5 + f3g4 + f4g3 + f5g2 + f6g1 + f7g0 +
                 f8g9_19 + f9g8_19;
    int64_t h8 = f0g8 + f1g7_2 + f2g6 + f3g5_2 + f4g4 + f5g3_2 + f6g2 + f7g1_2 +
                 f8g0 + f9g9_38;
    int64_t h9 =
        f0g9 + f1g8 + f2g7 + f3g6 + f4g5 + f5g4 + f6g3 + f7g2 + f8g1 + f9g0;
    int64_t carry0;
    int64_t carry1;
    int64_t carry2;
    int64_t carry3;
    int64_t carry4;
    int64_t carry5;
    int64_t carry6;
    int64_t carry7;
    int64_t carry8;
    int64_t carry9;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;
    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;

    carry1 = h1 + (1 << 24);
    h2 += carry1 >> 25;
    h1 -= carry1 & kTop39Bits;
    carry5 = h5 + (1 << 24);
    h6 += carry5 >> 25;
    h5 -= carry5 & kTop39Bits;

    carry2 = h2 + (1 << 25);
    h3 += carry2 >> 26;
    h2 -= carry2 & kTop38Bits;
    carry6 = h6 + (1 << 25);
    h7 += carry6 >> 26;
    h6 -= carry6 & kTop38Bits;

    carry3 = h3 + (1 << 24);
    h4 += carry3 >> 25;
    h3 -= carry3 & kTop39Bits;
    carry7 = h7 + (1 << 24);
    h8 += carry7 >> 25;
    h7 -= carry7 & kTop39Bits;

    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;
    carry8 = h8 + (1 << 25);
    h9 += carry8 >> 26;
    h8 -= carry8 & kTop38Bits;

    carry9 = h9 + (1 << 24);
    h0 += (carry9 >> 25) * 19;
    h9 -= carry9 & kTop39Bits;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;

    h[0] = (int32_t)h0;
    h[1] = (int32_t)h1;
    h[2] = (int32_t)h2;
    h[3] = (int32_t)h3;
    h[4] = (int32_t)h4;
    h[5] = (int32_t)h5;
    h[6] = (int32_t)h6;
    h[7] = (int32_t)h7;
    h[8] = (int32_t)h8;
    h[9] = (int32_t)h9;
}

/* crypto/ec/curve25519.c:1359-1513 */
static void fe_sq(fe h, const fe f) {
    int32_t f0 = f[0];
    int32_t f1 = f[1];
    int32_t f2 = f[2];
    int32_t f3 = f[3];
    int32_t f4 = f[4];
    int32_t f5 = f[5];
    int32_t f6 = f[6];
    int32_t f7 = f[7];
    int32_t f8 = f[8];
    int32_t f9 = f[9];
    int32_t f0_2 = 2 * f0;
    int32_t f1_2 = 2 * f1;
    int32_t f2_2 = 2 * f2;
    int32_t f3_2 = 2 * f3;
    int32_t f4_2 = 2 * f4;
    int32_t f5_2 = 2 * f5;
    int32_t f6_2 = 2 * f6;
    int32_t f7_2 = 2 * f7;
    int32_t f5_38 = 38 * f5;
    int32_t f6_19 = 19 * f6;
    int32_t f7_38 = 38 * f7;
    int32_t f8_19 = 19 * f8;
    int32_t f9_38 = 38 * f9;
    int64_t f0f0 = f0 * (int64_t)f0;
    int64_t f0f1_2 = f0_2 * (int64_t)f1;
    int64_t f0f2_2 = f0_2 * (int64_t)f2;
    int64_t f0f3_2 = f0_2 * (int64_t)f3;
    int64_t f0f4_2 = f0_2 * (int64_t)f4;
    int64_t f0f5_2 = f0_2 * (int64_t)f5;
    int64_t f0f6_2 = f0_2 * (int64_t)f6;
    int64_t f0f7_2 = f0_2 * (int64_t)f7;
    int64_t f0f8_2 = f0_2 * (int64_t)f8;
    int64_t f0f9_2 = f0_2 * (int64_t)f9;
    int64_t f1f1_2 = f1_2 * (int64_t)f1;
    int64_t f1f2_2 = f1_2 * (int64_t)f2;
    int64_t f1f3_4 = f1_2 * (int64_t)f3_2;
    int64_t f1f4_2 = f1_2 * (int64_t)f4;
    int64_t f1f5_4 = f1_2 * (int64_t)f5_2;
    int64_t f1f6_2 = f1_2 * (int64_t)f6;
    int64_t f1f7_4 = f1_2 * (int64_t)f7_2;
    int64_t f1f8_2 = f1_2 * (int64_t)f8;
    int64_t f1f9_76 = f1_2 * (int64_t)f9_38;
    int64_t f2f2 = f2 * (int64_t)f2;
    int64_t f2f3_2 = f2_2 * (int64_t)f3;
    int64_t f2f4_2 = f2_2 * (int64_t)f4;
    int64_t f2f5_2 = f2_2 * (int64_t)f5;
    int64_t f2f6_2 = f2_2 * (int64_t)f6;
    int64_t f2f7_2 = f2_2 * (int64_t)f7;
    int64_t f2f8_38 = f2_2 * (int64_t)f8_19;
    int64_t f2f9_38 = f2 * (int64_t)f9_38;
    int64_t f3f3_2 = f3_2 * (int64_t)f3;
    int64_t f3f4_2 = f3_2 * (int64_t)f4;
    int64_t f3f5_4 = f3_2 * (int64_t)f5_2;
    int64_t f3f6_2 = f3_2 * (int64_t)f6;
    int64_t f3f7_76 = f3_2 * (int64_t)f7_38;
    int64_t f3f8_38 = f3_2 * (int64_t)f8_19;
    int64_t f3f9_76 = f3_2 * (int64_t)f9_38;
    int64_t f4f4 = f4 * (int64_t)f4;
    int64_t f4f5_2 = f4_2 * (int64_t)f5;
    int64_t f4f6_38 = f4_2 * (int64_t)f6_19;
    int64_t f4f7_38 = f4 * (int64_t)f7_38;
    int64_t f4f8_38 = f4_2 * (int64_t)f8_19;
    int64_t f4f9_38 = f4 * (int64_t)f9_38;
    int64_t f5f5_38 = f5 * (int64_t)f5_38;
    int64_t f5f6_38 = f5_2 * (int64_t)f6_19;
    int64_t f5f7_76 = f5_2 * (int64_t)f7_38;
    int64_t f5f8_38 = f5_2 * (int64_t)f8_19;
    int64_t f5f9_76 = f5_2 * (int64_t)f9_38;
    int64_t f6f6_19 = f6 * (int64_t)f6_19;
    int64_t f6f7_38 = f6 * (int64_t)f7_38;
    int64_t f6f8_38 = f6_2 * (int64_t)f8_19;
    int64_t f6f9_38 = f6 * (int64_t)f9_38;
    int64_t f7f7_38 = f7 * (int64_t)f7_38;
    int64_t f7f8_38 = f7_2 * (int64_t)f8_19;
    int64_t f7f9_76 = f7_2 * (int64_t)f9_38;
    int64_t f8f8_19 = f8 * (int64_t)f8_19;
    int64_t f8f9_38 = f8 * (int64_t)f9_38;
    int64_t f9f9_38 = f9 * (int64_t)f9_38;
    int64_t h0 = f0f0 + f1f9_76 + f2f8_38 + f3f7_76 + f4f6_38 + f5f5_38;
    int64_t h1 = f0f1_2 + f2f9_38 + f3f8_38 + f4f7_38 + f5f6_38;
    int64_t h2 = f0f2_2 + f1f1_2 + f3f9_76 + f4f8_38 + f5f7_76 + f6f6_19;
    int64_t h3 = f0f3_2 + f1f2_2 + f4f9_38 + f5f8_38 + f6f7_38;
    int64_t h4 = f0f4_2 + f1f3_4 + f2f2 + f5f9_76 + f6f8_38 + f7f7_38;
    int64_t h5 = f0f5_2 + f1f4_2 + f2f3_2 + f6f9_38 + f7f8_38;
    int64_t h6 = f0f6_2 + f1f5_4 + f2f4_2 + f3f3_2 + f7f9_76 + f8f8_19;
    int64_t h7 = f0f7_2 + f1f6_2 + f2f5_2 + f3f4_2 + f8f9_38;
    int64_t h8 = f0f8_2 + f1f7_4 + f2f6_2 + f3f5_4 + f4f4 + f9f9_38;
    int64_t h9 = f0f9_2 + f1f8_2 + f2f7_2 + f3f6_2 + f4f5_2;
    int64_t carry0;
    int64_t carry1;
    int64_t carry2;
    int64_t carry3;
    int64_t carry4;
    int64_t carry5;
    int64_t carry6;
    int64_t carry7;
    int64_t carry8;
    int64_t carry9;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;
    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;

    carry1 = h1 + (1 << 24);
    h2 += carry1 >> 25;
    h1 -= carry1 & kTop39Bits;
    carry5 = h5 + (1 << 24);
    h6 += carry5 >> 25;
    h5 -= carry5 & kTop39Bits;

    carry2 = h2 + (1 << 25);
    h3 += carry2 >> 26;
    h2 -= carry2 & kTop38Bits;
    carry6 = h6 + (1 << 25);
    h7 += carry6 >> 26;
    h6 -= carry6 & kTop38Bits;

    carry3 = h3 + (1 << 24);
    h4 += carry3 >> 25;
    h3 -= carry3 & kTop39Bits;
    carry7 = h7 + (1 << 24);
    h8 += carry7 >> 25;
    h7 -= carry7 & kTop39Bits;

    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;
    carry8 = h8 + (1 << 25);
    h9 += carry8 >> 26;
    h8 -= carry8 & kTop38Bits;

    carry9 = h9 + (1 << 24);
    h0 += (carry9 >> 25) * 19;
    h9 -= carry9 & kTop39Bits;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;

    h[0] = (int32_t)h0;
    h[1] = (int32_t)h1;
    h[2] = (int32_t)h2;
    h[3] = (int32_t)h3;
    h[4] = (int32_t)h4;
    h[5] = (int32_t)h5;
    h[6] = (int32_t)h6;
    h[7] = (int32_t)h7;
    h[8] = (int32_t)h8;
    h[9] = (int32_t)h9;
}

/* Wrapper for invoking the extracted kernels fe_mul and fe_sq. */
void fe_mul_isolated(fe h, const fe f, const fe g) { fe_mul(h, f, g); }
void fe_sq_isolated(fe h, const fe f) { fe_sq(h, f); }
