/* GLib 2.90.0, glib/ghash.c and glib/gbytes.c.
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 * Copyright © 2009, 2010 Codethink Limited
 * Copyright © 2011 Collabora Ltd.
 * SPDX-License-Identifier: LGPL-2.1-or-later
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "kernel.h"

// glib/ghash.c:2475-2485. The signed char is essential for bytes >= 128.
guint g_str_hash(gconstpointer v) {
    const signed char *p;
    guint32 h = 5381;

    for (p = static_cast<const signed char *>(v); *p != '\0'; p++)
        h = (h << 5) + h + *p;

    return h;
}

// glib/gbytes.c:431-444. GLib's g_return_val_if_fail also emits a critical
// diagnostic for a null *bytes* argument; standalone extraction returns 0.
guint g_bytes_hash(gconstpointer bytes) {
    const GBytes *a = static_cast<const GBytes *>(bytes);
    const signed char *p, *e;
    guint32 h = 5381;

    if (bytes == nullptr)
        return 0;

    // Avoid null + 0 for an empty GBytes; no bytes are read in this case.
    if (a->size == 0)
        return h;
    for (p = static_cast<const signed char *>(a->data), e = p + a->size;
         p != e; p++)
        h = (h << 5) + h + *p;

    return h;
}
