/****************************************************************************
 *
 *
 *  Project: XZ Utils (xz) 5.8.4
 *  Source files:
 *    src/liblzma/delta/delta_encoder.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/liblzma/delta/delta_encoder.c
 *
 *   Delta filter encoder
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author:     Lasse Collin
 *
 */

#include "kernel.h"

/*
 * src/liblzma/delta/delta_encoder.c:19-31
 */
static void copy_and_encode(lzma_delta_coder *coder,
                            const uint8_t *__restrict__ in,
                            uint8_t *__restrict__ out, size_t size) {
    const size_t distance = coder->distance;

    for (size_t i = 0; i < size; ++i) {
        const uint8_t tmp = coder->history[(distance + coder->pos) & 0xFF];
        coder->history[coder->pos-- & 0xFF] = in[i];
        out[i] = in[i] - tmp;
    }
}

/*
 * src/liblzma/delta/delta_encoder.c:36-47
 */
static void encode_in_place(lzma_delta_coder *coder, uint8_t *buffer,
                            size_t size) {
    const size_t distance = coder->distance;

    for (size_t i = 0; i < size; ++i) {
        const uint8_t tmp = coder->history[(distance + coder->pos) & 0xFF];
        coder->history[coder->pos-- & 0xFF] = buffer[i];
        buffer[i] -= tmp;
    }
}

/*
 * Wrapper for invoking the extracted static function.
 */
void copy_and_encode_isolated(lzma_delta_coder *coder,
                              const uint8_t *__restrict__ in,
                              uint8_t *__restrict__ out, size_t size) {
    copy_and_encode(coder, in, out, size);
}

/*
 * Wrapper for invoking the extracted static function.
 */
void encode_in_place_isolated(lzma_delta_coder *coder, uint8_t *buffer,
                              size_t size) {
    encode_in_place(coder, buffer, size);
}
