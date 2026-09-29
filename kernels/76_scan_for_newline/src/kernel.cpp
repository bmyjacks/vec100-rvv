/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    gio/gbufferedinputstream.c
 *    gio/gdatainputstream.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * gio/gbufferedinputstream.c
 *
 *   GIO - GLib Input, Output and Streaming Library
 *
 * Copyright (C) 2006-2007 Red Hat, Inc.
 * Copyright (C) 2007 Jürg Billeter
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
 * Author: Christian Kellner <gicmo@gnome.org>
 *
 *
 * gio/gdatainputstream.c
 *
 *   GIO - GLib Input, Output and Streaming Library
 *
 * Copyright (C) 2006-2007 Red Hat, Inc.
 * Copyright (C) 2007 Jürg Billeter
 * Copyright © 2009 Codethink Limited
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
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Alexander Larsson <alexl@redhat.com>
 *
 */

#include "kernel.h"

#include <string.h>

/*
 * gio/gbufferedinputstream.c:57-63 (members used by the accessor)
 */
struct _GBufferedInputStreamPrivate {
    unsigned char *buffer;
    gsize len;
    gsize pos;
    gsize end;
};

/*
 * gio/gbufferedinputstream.c:616-630 (G_DISABLE_CHECKS configuration)
 */
static const void *
g_buffered_input_stream_peek_buffer(GBufferedInputStream *stream,
                                    gsize *count) {
    GBufferedInputStreamPrivate *priv;

    priv = stream->priv;

    if (count)
        *count = priv->end - priv->pos;

    return priv->buffer + priv->pos;
}

/*
 * gio/gdatainputstream.c:42-45
 */
struct _GDataInputStreamPrivate {
    GDataStreamByteOrder byte_order;
    GDataStreamNewlineType newline_type;
};

/*
 * gio/gdatainputstream.c:618-714
 */
static gssize scan_for_newline(GDataInputStream *stream, gsize *checked_out,
                               gboolean *last_saw_cr_out,
                               int *newline_len_out) {
    GBufferedInputStream *bstream;
    GDataInputStreamPrivate *priv;
    const char *buffer;
    gsize start, end, peeked;
    gsize i;
    gssize found_pos;
    int newline_len;
    gsize available, checked;
    gboolean last_saw_cr;

    priv = stream->priv;

    bstream = G_BUFFERED_INPUT_STREAM(stream);

    checked = *checked_out;
    last_saw_cr = *last_saw_cr_out;
    found_pos = -1;
    newline_len = 0;

    start = checked;
    buffer =
        (const char *)g_buffered_input_stream_peek_buffer(bstream, &available) +
        start;
    end = available;
    peeked = end - start;

    for (i = 0; checked < available && i < peeked; i++) {
        switch (priv->newline_type) {
        case G_DATA_STREAM_NEWLINE_TYPE_LF:
            if (buffer[i] == 10) {
                found_pos = start + i;
                newline_len = 1;
            }
            break;
        case G_DATA_STREAM_NEWLINE_TYPE_CR:
            if (buffer[i] == 13) {
                found_pos = start + i;
                newline_len = 1;
            }
            break;
        case G_DATA_STREAM_NEWLINE_TYPE_CR_LF:
            if (last_saw_cr && buffer[i] == 10) {
                found_pos = start + i - 1;
                newline_len = 2;
            }
            break;
        default:
        case G_DATA_STREAM_NEWLINE_TYPE_ANY:
            if (buffer[i] == 10) {
                if (last_saw_cr) {

                    found_pos = start + i - 1;
                    newline_len = 2;
                } else {

                    found_pos = start + i;
                    newline_len = 1;
                }
            } else if (last_saw_cr) {

                found_pos = start + i - 1;
                newline_len = 1;
            }

            break;
        }

        last_saw_cr = (buffer[i] == 13);

        if (found_pos != -1) {
            *newline_len_out = newline_len;
            return found_pos;
        }
    }

    checked = end;

    *checked_out = checked;
    *last_saw_cr_out = last_saw_cr;
    return -1;
}

/*
 * gio/gdatainputstream.c:853-902
 */
static gssize scan_for_chars(GDataInputStream *stream, gsize *checked_out,
                             const char *stop_chars, gsize stop_chars_len) {
    GBufferedInputStream *bstream;
    const char *buffer;
    gsize start, end, peeked;
    gsize i;
    gsize available, checked;

    bstream = G_BUFFERED_INPUT_STREAM(stream);

    checked = *checked_out;

    start = checked;
    buffer =
        (const char *)g_buffered_input_stream_peek_buffer(bstream, &available) +
        start;
    end = available;
    peeked = end - start;

    if (stop_chars_len == 1) {
        const char *p = (const char *)memchr(buffer, stop_chars[0], peeked);

        if (p != NULL)
            return start + (p - buffer);
    } else {
        for (i = 0; checked < available && i < peeked; i++) {

            const char *p =
                (const char *)memchr(stop_chars, buffer[i], stop_chars_len);

            if (p != NULL)
                return (start + i);
        }
    }

    checked = end;

    *checked_out = checked;
    return -1;
}

/*
 * Wrappers for invoking the extracted kernel.
 */
gssize scan_for_newline_isolated(GDataInputStream *stream, gsize *checked_out,
                                 gboolean *last_saw_cr_out,
                                 int *newline_len_out) {
    return scan_for_newline(stream, checked_out, last_saw_cr_out,
                            newline_len_out);
}

gssize scan_for_chars_isolated(GDataInputStream *stream, gsize *checked_out,
                               const char *stop_chars, gsize stop_chars_len) {
    return scan_for_chars(stream, checked_out, stop_chars, stop_chars_len);
}

/*
 * Wrappers for invoking the extracted kernels with a pre-filled buffer.
 */
gssize scan_for_newline_bench(const char *buffer, gsize size,
                              GDataStreamNewlineType type, gsize *checked_out,
                              gboolean *last_saw_cr_out, int *newline_len_out) {
    GBufferedInputStreamPrivate buffered_priv = {(unsigned char *)buffer, size,
                                                 0, size};
    GDataInputStreamPrivate data_priv = {G_DATA_STREAM_BYTE_ORDER_HOST_ENDIAN,
                                         type};
    GDataInputStream stream = {};
    stream.parent_instance.priv = &buffered_priv;
    stream.priv = &data_priv;
    return scan_for_newline(&stream, checked_out, last_saw_cr_out,
                            newline_len_out);
}

gssize scan_for_chars_bench(const char *buffer, gsize size, gsize *checked_out,
                            const char *stop_chars, gsize stop_chars_len) {
    GBufferedInputStreamPrivate buffered_priv = {(unsigned char *)buffer, size,
                                                 0, size};
    GDataInputStream stream = {};
    stream.parent_instance.priv = &buffered_priv;
    return scan_for_chars(&stream, checked_out, stop_chars, stop_chars_len);
}
