/****************************************************************************
 *
 *
 *  Project: libsodium 1.0.22
 *  Source files:
 *    src/libsodium/include/sodium/private/common.h
 *    src/libsodium/crypto_stream/chacha20/ref/chacha20_ref.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/libsodium/include/sodium/private/common.h
 *
 * src/libsodium/crypto_stream/chacha20/ref/chacha20_ref.c
 *
 * chacha-merged.c version 20080118
 * D. J. Bernstein
 * Public domain.
 *
 */

#ifndef KERNELS_05_CHACHA20_ENCRYPT_BYTES_INCLUDE_KERNEL_H_
#define KERNELS_05_CHACHA20_ENCRYPT_BYTES_INCLUDE_KERNEL_H_

#include <stdint.h>
#include <string.h>

/*
 * src/libsodium/include/sodium/private/common.h:42-47
 */
#define ROTL32(X, B) rotl32((X), (B))
static inline uint32_t rotl32(const uint32_t x, const int b) {
    return (x << b) | (x >> (32 - b));
}

/*
 * src/libsodium/include/sodium/private/common.h:111-118
 */
#define LOAD32_LE(SRC) load32_le(SRC)
static inline uint32_t load32_le(const uint8_t src[4]) {
    uint32_t w;
    memcpy(&w, src, sizeof w);
    return w;
}

/*
 * src/libsodium/include/sodium/private/common.h:128-134
 */
#define STORE32_LE(DST, W) store32_le((DST), (W))
static inline void store32_le(uint8_t dst[4], uint32_t w) {
    memcpy(dst, &w, sizeof w);
}

/*
 * src/libsodium/crypto_stream/chacha20/ref/chacha20_ref.c:20-24
 */
struct chacha_ctx {
    uint32_t input[16];
};

typedef struct chacha_ctx chacha_ctx;

/*
 * Wrapper for invoking the extracted static function.
 */
void chacha20_encrypt_bytes_isolated(chacha_ctx *ctx, const uint8_t *m,
                                     uint8_t *c, unsigned long long bytes);

#endif // KERNELS_05_CHACHA20_ENCRYPT_BYTES_INCLUDE_KERNEL_H_
