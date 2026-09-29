/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    glib/gvariant-serialiser.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * glib/gvariant-serialiser.c
 *
 * Copyright © 2007, 2008 Ryan Lortie
 * Copyright © 2010 Codethink Limited
 * Copyright © 2020 William Manley
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Ryan Lortie <desrt@desrt.ca>
 *
 */

#include "kernel.h"

#include <string.h>

/*
 * glib/gvariant-serialiser.c:717-735
 */
#define DEFINE_FIND_UNORDERED(type, le_to_native)                              \
    static gsize find_unordered_##type(const guint8 *data, gsize start,        \
                                       gsize len) {                            \
        gsize off;                                                             \
        type current_le, previous_le, current, previous;                       \
                                                                               \
        memcpy(&previous_le, data + start * sizeof(current), sizeof(current)); \
        previous = le_to_native(previous_le);                                  \
        for (off = (start + 1) * sizeof(current); off < len * sizeof(current); \
             off += sizeof(current)) {                                         \
            memcpy(&current_le, data + off, sizeof(current));                  \
            current = le_to_native(current_le);                                \
            if (current < previous)                                            \
                break;                                                         \
            previous = current;                                                \
        }                                                                      \
        return off / sizeof(current) - 1;                                      \
    }

/*
 * glib/gvariant-serialiser.c:737-741
 */
#define NO_CONVERSION(x) (x)
DEFINE_FIND_UNORDERED(guint8, NO_CONVERSION);
DEFINE_FIND_UNORDERED(guint16, GUINT16_FROM_LE);
DEFINE_FIND_UNORDERED(guint32, GUINT32_FROM_LE);
DEFINE_FIND_UNORDERED(guint64, GUINT64_FROM_LE);

/* Wrapper for invoking the extracted macro-generated kernels. */
gsize find_unordered_guint8_isolated(const guint8 *data, gsize start,
                                     gsize len) {
    return find_unordered_guint8(data, start, len);
}

gsize find_unordered_guint16_isolated(const guint8 *data, gsize start,
                                      gsize len) {
    return find_unordered_guint16(data, start, len);
}

gsize find_unordered_guint32_isolated(const guint8 *data, gsize start,
                                      gsize len) {
    return find_unordered_guint32(data, start, len);
}

gsize find_unordered_guint64_isolated(const guint8 *data, gsize start,
                                      gsize len) {
    return find_unordered_guint64(data, start, len);
}
