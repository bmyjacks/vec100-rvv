/* GLib 2.90.0, glib/gtypes.h and glib/gbytes.c.
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 * Copyright © 2009, 2010 Codethink Limited
 * Copyright © 2011 Collabora Ltd.
 * SPDX-License-Identifier: LGPL-2.1-or-later
 * Distributed under the GNU Lesser General Public License, version 2.1
 * or (at your option) any later version. WITHOUT ANY WARRANTY.
 */
#ifndef KERNELS_43_GLIB_STR_HASH_INCLUDE_KERNEL_H_
#define KERNELS_43_GLIB_STR_HASH_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

using guint = unsigned int;
using guint32 = std::uint32_t;
using gsize = std::size_t;
using gconstpointer = const void *;
using gpointer = void *;
using gatomicrefcount = int;
using GDestroyNotify = void (*)(gpointer);

// glib/gbytes.c:79-86; the other fields are not consulted by g_bytes_hash.
struct GBytes {
    gconstpointer data;
    gsize size;
    gatomicrefcount ref_count;
    GDestroyNotify free_func;
    gpointer user_data;
};

guint g_str_hash(gconstpointer v);
guint g_bytes_hash(gconstpointer bytes);
guint g_str_hash_rvv(gconstpointer v);
guint g_bytes_hash_rvv(gconstpointer bytes);
#endif
