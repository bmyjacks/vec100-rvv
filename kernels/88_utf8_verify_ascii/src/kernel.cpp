/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    glib/gutf8.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * glib/gutf8.c
 *
 *   gutf8.c - Operations on UTF-8 strings.
 *
 * Copyright (C) 1999 Tom Tromey
 * Copyright (C) 2000, 2015-2022 Red Hat, Inc.
 * Copyright (C) 2022-2023 David Rheinsberg
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * SIMD-based UTF-8 validation originates in the c-utf8 project from
 * https://github.com/c-util/c-utf8/ from the following authors:
 *
 *   David Rheinsberg <david@readahead.eu>
 *   Evgeny Vereshchagin <evvers@ya.ru>
 *   Jan Engelhardt <jengelh@inai.de>
 *   Tom Gundersen <teg@jklm.no>
 *
 * It has been adapted for portability and integration.
 * The original code is dual-licensed Apache-2.0 or LGPLv2.1+
 *
 */

#include "kernel.h"

#include <string.h>

/*
 * glib/gutf8.c:1987
 */
#define align_to(_val, _to) (((_val) + (_to) - 1) & ~((_to) - 1))

/*
 * glib/gutf8.c:1989-1994
 */
static inline guint8 load_u8(gconstpointer memory, gsize offset) {
    return ((const guint8 *)memory)[offset];
}

/*
 * glib/gutf8.c:1996-2002
 */
#define _attribute_aligned(n) __attribute__((aligned(n)))

/*
 * glib/gutf8.c:2004-2021
 */
static inline gsize load_word(gconstpointer memory, gsize offset) {
    _attribute_aligned(8) const guint8 *m = ((const guint8 *)memory) + offset;

    return ((guint64)m[0] << 0) | ((guint64)m[1] << 8) | ((guint64)m[2] << 16) |
           ((guint64)m[3] << 24) | ((guint64)m[4] << 32) |
           ((guint64)m[5] << 40) | ((guint64)m[6] << 48) |
           ((guint64)m[7] << 56);
}

/*
 * glib/gutf8.c:2024-2025
 */
#define UTF8_ASCII_MASK ((gsize)0x8080808080808080L)
#define UTF8_ASCII_SUB ((gsize)0x0101010101010101L)

/*
 * glib/gutf8.c:2027-2032
 */
static inline int utf8_word_is_ascii(gsize word) {

    return ((((word - UTF8_ASCII_SUB) | word) & UTF8_ASCII_MASK) == 0);
}

/*
 * glib/gutf8.c:2034-2079
 */
static void utf8_verify_ascii(const char **strp, gsize *lenp) {
    const char *str = *strp;
    gsize len = lenp ? *lenp : strlen(str);

    while (len > 0 && load_u8(str, 0) < 128) {
        if ((gpointer)align_to((guintptr)str, sizeof(gsize)) == str) {
            while (len >= 2 * sizeof(gsize)) {
                if (!utf8_word_is_ascii(load_word(str, 0)) ||
                    !utf8_word_is_ascii(load_word(str, sizeof(gsize))))
                    break;

                str += 2 * sizeof(gsize);
                len -= 2 * sizeof(gsize);
            }

            while (len > 0 && load_u8(str, 0) < 128) {
                if G_UNLIKELY (load_u8(str, 0) == 0x00)
                    goto out;

                ++str;
                --len;
            }
        } else {
            if G_UNLIKELY (load_u8(str, 0) == 0x00)
                goto out;

            ++str;
            --len;
        }
    }

out:
    *strp = str;

    if (lenp)
        *lenp = len;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void utf8_verify_ascii_isolated(const char **strp, gsize *lenp) {
    utf8_verify_ascii(strp, lenp);
}
