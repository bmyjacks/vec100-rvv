/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    glib/gstrfuncs.c
 *    glib/gmessages.c
 *
 *
 *  The original file copyright and license notices follow.
 *
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
#include <string.h>

/*
 * glib/gmessages.c:3192-3202; use libc diagnostics to isolate g_log.
 */
void g_return_if_fail_warning(const char *log_domain,
                              const char *pretty_function,
                              const char *expression) {
    fprintf(stderr, "%s: assertion '%s' failed\n", pretty_function, expression);
}

/*
 * glib/gstrfuncs.c:1689-1714
 */
gchar *g_strreverse_isolated(gchar *string) {
    g_return_val_if_fail(string != NULL, NULL);

    if (*string) {
        gchar *h, *t;

        h = string;
        t = string + strlen(string) - 1;

        while (h < t) {
            gchar c;

            c = *h;
            *h = *t;
            h++;
            *t = c;
            t--;
        }
    }

    return string;
}
