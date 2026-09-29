/****************************************************************************
 *
 *
 *  Project: libsodium 1.0.22
 *  Source files:
 *    src/libsodium/crypto_stream/chacha20/ref/chacha20_ref.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/libsodium/crypto_stream/chacha20/ref/chacha20_ref.c
 *
 * chacha-merged.c version 20080118
 * D. J. Bernstein
 * Public domain.
 *
 */

#include <stdint.h>
#include <string.h>

#include "kernel.h"

/*
 * src/libsodium/crypto_stream/chacha20/ref/chacha20_ref.c:26-43
 */
#define U32C(v) (v##U)

#define U32V(v) ((uint32_t)(v) & U32C(0xFFFFFFFF))

#define ROTATE(v, c) (ROTL32(v, c))
#define XOR(v, w) ((v) ^ (w))
#define PLUS(v, w) (U32V((v) + (w)))
#define PLUSONE(v) (PLUS((v), 1))

#define QUARTERROUND(a, b, c, d)                                               \
    a = PLUS(a, b);                                                            \
    d = ROTATE(XOR(d, a), 16);                                                 \
    c = PLUS(c, d);                                                            \
    b = ROTATE(XOR(b, c), 12);                                                 \
    a = PLUS(a, b);                                                            \
    d = ROTATE(XOR(d, a), 8);                                                  \
    c = PLUS(c, d);                                                            \
    b = ROTATE(XOR(b, c), 7);

/*
 * src/libsodium/crypto_stream/chacha20/ref/chacha20_ref.c:80-221
 */
static void chacha20_encrypt_bytes(chacha_ctx *ctx, const uint8_t *m,
                                   uint8_t *c, unsigned long long bytes) {
    uint32_t x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14,
        x15;
    uint32_t j0, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14,
        j15;
    uint8_t *ctarget = NULL;
    uint8_t tmp[64];
    unsigned int i;

    if (!bytes) {
        return;
    }
    j0 = ctx->input[0];
    j1 = ctx->input[1];
    j2 = ctx->input[2];
    j3 = ctx->input[3];
    j4 = ctx->input[4];
    j5 = ctx->input[5];
    j6 = ctx->input[6];
    j7 = ctx->input[7];
    j8 = ctx->input[8];
    j9 = ctx->input[9];
    j10 = ctx->input[10];
    j11 = ctx->input[11];
    j12 = ctx->input[12];
    j13 = ctx->input[13];
    j14 = ctx->input[14];
    j15 = ctx->input[15];

    for (;;) {
        if (bytes < 64) {
            memset(tmp, 0, 64);
            for (i = 0; i < bytes; ++i) {
                tmp[i] = m[i];
            }
            m = tmp;
            ctarget = c;
            c = tmp;
        }
        x0 = j0;
        x1 = j1;
        x2 = j2;
        x3 = j3;
        x4 = j4;
        x5 = j5;
        x6 = j6;
        x7 = j7;
        x8 = j8;
        x9 = j9;
        x10 = j10;
        x11 = j11;
        x12 = j12;
        x13 = j13;
        x14 = j14;
        x15 = j15;
        for (i = 20; i > 0; i -= 2) {
            QUARTERROUND(x0, x4, x8, x12)
            QUARTERROUND(x1, x5, x9, x13)
            QUARTERROUND(x2, x6, x10, x14)
            QUARTERROUND(x3, x7, x11, x15)
            QUARTERROUND(x0, x5, x10, x15)
            QUARTERROUND(x1, x6, x11, x12)
            QUARTERROUND(x2, x7, x8, x13)
            QUARTERROUND(x3, x4, x9, x14)
        }
        x0 = PLUS(x0, j0);
        x1 = PLUS(x1, j1);
        x2 = PLUS(x2, j2);
        x3 = PLUS(x3, j3);
        x4 = PLUS(x4, j4);
        x5 = PLUS(x5, j5);
        x6 = PLUS(x6, j6);
        x7 = PLUS(x7, j7);
        x8 = PLUS(x8, j8);
        x9 = PLUS(x9, j9);
        x10 = PLUS(x10, j10);
        x11 = PLUS(x11, j11);
        x12 = PLUS(x12, j12);
        x13 = PLUS(x13, j13);
        x14 = PLUS(x14, j14);
        x15 = PLUS(x15, j15);

        x0 = XOR(x0, LOAD32_LE(m + 0));
        x1 = XOR(x1, LOAD32_LE(m + 4));
        x2 = XOR(x2, LOAD32_LE(m + 8));
        x3 = XOR(x3, LOAD32_LE(m + 12));
        x4 = XOR(x4, LOAD32_LE(m + 16));
        x5 = XOR(x5, LOAD32_LE(m + 20));
        x6 = XOR(x6, LOAD32_LE(m + 24));
        x7 = XOR(x7, LOAD32_LE(m + 28));
        x8 = XOR(x8, LOAD32_LE(m + 32));
        x9 = XOR(x9, LOAD32_LE(m + 36));
        x10 = XOR(x10, LOAD32_LE(m + 40));
        x11 = XOR(x11, LOAD32_LE(m + 44));
        x12 = XOR(x12, LOAD32_LE(m + 48));
        x13 = XOR(x13, LOAD32_LE(m + 52));
        x14 = XOR(x14, LOAD32_LE(m + 56));
        x15 = XOR(x15, LOAD32_LE(m + 60));

        j12 = PLUSONE(j12);
        if (!j12) {
            j13 = PLUSONE(j13);
        }

        STORE32_LE(c + 0, x0);
        STORE32_LE(c + 4, x1);
        STORE32_LE(c + 8, x2);
        STORE32_LE(c + 12, x3);
        STORE32_LE(c + 16, x4);
        STORE32_LE(c + 20, x5);
        STORE32_LE(c + 24, x6);
        STORE32_LE(c + 28, x7);
        STORE32_LE(c + 32, x8);
        STORE32_LE(c + 36, x9);
        STORE32_LE(c + 40, x10);
        STORE32_LE(c + 44, x11);
        STORE32_LE(c + 48, x12);
        STORE32_LE(c + 52, x13);
        STORE32_LE(c + 56, x14);
        STORE32_LE(c + 60, x15);

        if (bytes <= 64) {
            if (bytes < 64) {
                for (i = 0; i < (unsigned int)bytes; ++i) {
                    ctarget[i] = c[i];
                }
            }
            ctx->input[12] = j12;
            ctx->input[13] = j13;

            return;
        }
        bytes -= 64;
        c += 64;
        m += 64;
    }
}

/*
 * Wrapper for invoking the extracted static function.
 */
void chacha20_encrypt_bytes_isolated(chacha_ctx *ctx, const uint8_t *m,
                                     uint8_t *c, unsigned long long bytes) {
    chacha20_encrypt_bytes(ctx, m, c, bytes);
}
