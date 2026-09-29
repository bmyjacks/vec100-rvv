/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    glib/gmem.c
 *    glib/gmem.h
 *    glib/gstrfuncs.c
 *    glib/gmessages.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * glib/gmem.c
 *
 *   GLIB - Library of useful routines for C programming
 *
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
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
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 *
 * glib/gmem.h
 *
 *   GLIB - Library of useful routines for C programming
 *
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
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
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 *
 * glib/gstrfuncs.c
 *
 *   GLIB - Library of useful routines for C programming
 *
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
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
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 *
 * glib/gmessages.c
 *
 *   GLIB - Library of useful routines for C programming
 *
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
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
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 *
 */

#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>

/*
 * glib/gmem.c:93-112; the disabled trace probes and g_error's OOM diagnostic
 * are isolated; the upstream zero-size and failed-allocation branches remain.
 */
static void *g_malloc(gsize n_bytes) {
    if (G_LIKELY(n_bytes)) {
        void *mem;

        mem = malloc(n_bytes);
        if (mem)
            return mem;

        abort();
    }

    return NULL;
}

/*
 * glib/gmem.h:261-275,318 (the sizeof(char/gchar) == 1 branch of _G_NEW).
 */
#define g_new(struct_type, n_structs)                                          \
    ((struct_type *)g_malloc((gsize)(n_structs)))

/*
 * glib/gstrfuncs.c:314-330
 */
gchar *(g_strdup)(const gchar *str) {
    gchar *new_str;
    gsize length;

    if (G_LIKELY(str)) {
        length = strlen(str) + 1;
        new_str = g_new(char, length);
        memcpy(new_str, str, length);
    } else
        new_str = NULL;

    return new_str;
}

/*
 * glib/gstrfuncs.c:411-429
 */
gchar *g_strndup(const gchar *str, gsize n) {
    gchar *new_str;

    if (str) {
        g_return_val_if_fail(n < G_MAXSIZE, NULL);

        new_str = g_new(gchar, n + 1);
        strncpy(new_str, str, n);
        new_str[n] = '\0';
    } else
        new_str = NULL;

    return new_str;
}

/*
 * glib/gmessages.c:3192-3202; use libc diagnostics to isolate g_log.
 */
void g_return_if_fail_warning(const char *log_domain,
                              const char *pretty_function,
                              const char *expression) {
    fprintf(stderr, "%s: assertion '%s' failed\n", pretty_function, expression);
}

/*
 * glib/gstrfuncs.c:262-280
 */
static const guint16 ascii_table_data[256] = {
    0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x104, 0x104,
    0x004, 0x104, 0x104, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
    0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x140,
    0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
    0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459,
    0x459, 0x459, 0x459, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x653,
    0x653, 0x653, 0x653, 0x653, 0x653, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
    0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
    0x253, 0x253, 0x253, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x473, 0x473,
    0x473, 0x473, 0x473, 0x473, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
    0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
    0x073, 0x073, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x004};

/*
 * glib/gstrfuncs.c:282
 */
const guint16 *const g_ascii_table = ascii_table_data;

/*
 * glib/gstrfuncs.c:1564-1581
 */
gchar *g_ascii_strdown(const gchar *str, gssize len) {
    gchar *result, *s;

    g_return_val_if_fail(str != NULL, NULL);

    if (len < 0)
        result = g_strdup(str);
    else
        result = g_strndup(str, (gsize)len);

    for (s = result; *s; s++)
        *s = g_ascii_tolower(*s);

    return result;
}

/*
 * glib/gstrfuncs.c:1595-1612
 */
gchar *g_ascii_strup(const gchar *str, gssize len) {
    gchar *result, *s;

    g_return_val_if_fail(str != NULL, NULL);

    if (len < 0)
        result = g_strdup(str);
    else
        result = g_strndup(str, (gsize)len);

    for (s = result; *s; s++)
        *s = g_ascii_toupper(*s);

    return result;
}

/*
 * glib/gstrfuncs.c:1733-1737
 */
gchar g_ascii_tolower(gchar c) {
    return g_ascii_isupper(c) ? c - 'A' + 'a' : c;
}

/*
 * glib/gstrfuncs.c:1756-1760
 */
gchar g_ascii_toupper(gchar c) {
    return g_ascii_islower(c) ? c - 'a' + 'A' : c;
}
