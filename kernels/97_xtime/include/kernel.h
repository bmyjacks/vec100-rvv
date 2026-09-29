/****************************************************************************
 *
 *
 *  Project: libsodium 1.0.22
 *  Source files:
 *    src/libsodium/include/sodium/private/softaes.h
 *  License file: LICENSE
 *
 *
 *  The upstream source file has no per-file license header. The applicable
 *  project copyright and license notice follows, copied from LICENSE.
 *
 *
 * LICENSE
 *
 * ISC License
 *
 * Copyright (c) 2013-2026
 * Frank Denis <j at pureftpd dot org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef KERNELS_97_XTIME_INCLUDE_KERNEL_H_
#define KERNELS_97_XTIME_INCLUDE_KERNEL_H_

#include <stdint.h>

/*
 * src/libsodium/include/sodium/private/softaes.h:8-13
 */
typedef struct SoftAesBlock {
    uint32_t w0;
    uint32_t w1;
    uint32_t w2;
    uint32_t w3;
} SoftAesBlock;

/*
 * src/libsodium/include/sodium/private/softaes.h:17-19
 */
SoftAesBlock softaes_inv_mix_columns(const SoftAesBlock block);
void softaes_invert_key_schedule128(SoftAesBlock rkeys[11]);
void softaes_invert_key_schedule256(SoftAesBlock rkeys[15]);

#endif // KERNELS_97_XTIME_INCLUDE_KERNEL_H_
