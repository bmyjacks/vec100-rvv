/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    gobject/gparam.h
 *    glib/glibconfig.h.in
 *
 *  The original file copyright and license notices follow.
 *
 * gobject/gparam.h
 *
 * GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 1997-1999, 2000-2001 Tim Janik and Red Hat, Inc.
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
 * gparam.h: GParamSpec base class implementation
 *
 * glib/glibconfig.h.in
 *
 *   This is a generated file.  Please modify 'glibconfig.h.in'
 */

#ifndef KERNELS_42_GLIB_NOTIFY_REVERSE_INCLUDE_KERNEL_H_
#define KERNELS_42_GLIB_NOTIFY_REVERSE_INCLUDE_KERNEL_H_

/* gobject/gparam.h:204 */
typedef struct _GParamSpec GParamSpec;

/* glib/glibconfig.h.in:47, configured with gint16=short on RV64 */
typedef unsigned short guint16;

/* Wrapper for the reversal in g_object_notify_queue_thaw. */
void glib_notify_reverse(GParamSpec **pspecs, guint16 len);
void glib_notify_reverse_rvv(GParamSpec **pspecs, guint16 len);

#endif // KERNELS_42_GLIB_NOTIFY_REVERSE_INCLUDE_KERNEL_H_
