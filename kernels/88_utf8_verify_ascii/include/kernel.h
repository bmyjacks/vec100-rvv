/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    glib/gmacros.h
 *    glib/glibconfig.h.in
 *    glib/gtypes.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * glib/gmacros.h
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
 *
 * glib/glibconfig.h.in
 *
 *   This is a generated file.  Please modify 'glibconfig.h.in'
 *
 *
 * glib/gtypes.h
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

#ifndef KERNELS_88_UTF8_VERIFY_ASCII_INCLUDE_KERNEL_H_
#define KERNELS_88_UTF8_VERIFY_ASCII_INCLUDE_KERNEL_H_

/*
 * glib/gmacros.h:61-65
 */
#define G_GNUC_EXTENSION __extension__

/*
 * glib/gmacros.h:902-903
 */
#define G_PASTE_ARGS(identifier1, identifier2) identifier1##identifier2
#define G_PASTE(identifier1, identifier2) G_PASTE_ARGS(identifier1, identifier2)

/*
 * glib/gmacros.h:1275-1289
 */
#define _G_BOOLEAN_EXPR_IMPL(uniq, expr)                                       \
    G_GNUC_EXTENSION({                                                         \
        int G_PASTE(_g_boolean_var_, uniq) = 0;                                \
        if (expr)                                                              \
            G_PASTE(_g_boolean_var_, uniq) = 1;                                \
        G_PASTE(_g_boolean_var_, uniq);                                        \
    })
#define _G_BOOLEAN_EXPR(expr) _G_BOOLEAN_EXPR_IMPL(__COUNTER__, expr)
#define G_LIKELY(expr) (__builtin_expect(_G_BOOLEAN_EXPR(expr), 1))
#define G_UNLIKELY(expr) (__builtin_expect(_G_BOOLEAN_EXPR(expr), 0))

/*
 * glib/glibconfig.h.in:44
 */
typedef unsigned char guint8;

/*
 * glib/glibconfig.h.in:65
 */
typedef unsigned long guint64;

/*
 * glib/glibconfig.h.in:81
 */
typedef unsigned long gsize;

/*
 * glib/glibconfig.h.in:108
 */
typedef unsigned long guintptr;

/*
 * glib/gtypes.h:109-110
 */
typedef void *gpointer;
typedef const void *gconstpointer;

/*
 * Wrapper for invoking the extracted kernel.
 */
void utf8_verify_ascii_isolated(const char **strp, gsize *lenp);

#endif // KERNELS_88_UTF8_VERIFY_ASCII_INCLUDE_KERNEL_H_
