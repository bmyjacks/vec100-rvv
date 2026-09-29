/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    gobject/gobject.c
 *
 *  The original file copyright and license notices follow.
 *
 * gobject/gobject.c
 *
 * GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 1998-1999, 2000-2001 Tim Janik and Red Hat, Inc.
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
 */

#include "kernel.h"

/* Wrapper for gobject/gobject.c:743-756; queue fields become parameters. */
void glib_notify_reverse(GParamSpec **pspecs, guint16 len) {
    if (len > 0) {
        guint16 i;
        guint16 j;

        for (i = 0, j = len - 1u; i < j; i++, j--) {
            GParamSpec *tmp;

            tmp = pspecs[i];
            pspecs[i] = pspecs[j];
            pspecs[j] = tmp;
        }
    }
}
