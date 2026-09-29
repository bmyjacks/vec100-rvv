/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    glib/glibconfig.h.in
 *    glib/gtypes.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * glib/glibconfig.h.in
 *
 *   This is a generated file.  Please modify 'glibconfig.h.in'
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

#ifndef KERNELS_24_FIND_UNORDERED_GUINT8_INCLUDE_KERNEL_H_
#define KERNELS_24_FIND_UNORDERED_GUINT8_INCLUDE_KERNEL_H_

#include <stddef.h>
#include <stdint.h>

/*
 * glib/glibconfig.h.in:44
 */
typedef unsigned char guint8;

/*
 * glib/glibconfig.h.in:47
 */
typedef unsigned short guint16;

/*
 * glib/glibconfig.h.in:55
 */
typedef unsigned int guint32;

/*
 * glib/glibconfig.h.in:65
 */
typedef unsigned long guint64;

/*
 * glib/glibconfig.h.in:81
 */
typedef size_t gsize;

/*
 * glib/glibconfig.h.in:154-167 (little-endian configuration)
 */
#define GUINT16_TO_LE(val) ((guint16)(val))
#define GUINT32_TO_LE(val) ((guint32)(val))
#define GUINT64_TO_LE(val) ((guint64)(val))

/*
 * glib/gtypes.h:413
 */
#define GUINT16_FROM_LE(val) (GUINT16_TO_LE(val))

/*
 * glib/gtypes.h:417
 */
#define GUINT32_FROM_LE(val) (GUINT32_TO_LE(val))

/*
 * glib/gtypes.h:422
 */
#define GUINT64_FROM_LE(val) (GUINT64_TO_LE(val))

/* Wrapper declarations for invoking the extracted kernels. */
gsize find_unordered_guint8_isolated(const guint8 *data, gsize start,
                                     gsize len);
gsize find_unordered_guint16_isolated(const guint8 *data, gsize start,
                                      gsize len);
gsize find_unordered_guint32_isolated(const guint8 *data, gsize start,
                                      gsize len);
gsize find_unordered_guint64_isolated(const guint8 *data, gsize start,
                                      gsize len);

#endif // KERNELS_24_FIND_UNORDERED_GUINT8_INCLUDE_KERNEL_H_
