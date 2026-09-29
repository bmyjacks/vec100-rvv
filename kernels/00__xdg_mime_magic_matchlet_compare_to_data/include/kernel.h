/****************************************************************************
 *
 *
 *  Project: GLib 2.90.0
 *  Source files:
 *    gio/xdgmime/xdgmimemagic.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * gio/xdgmime/xdgmimemagic.c
 *
 * xdgmimemagic.: Private file.  Datastructure for storing magic files.
 *
 * More info can be found at http://www.freedesktop.org/standards/
 *
 * Copyright (C) 2003  Red Hat, Inc.
 * Copyright (C) 2003  Jonathan Blandford <jrb@alum.mit.edu>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later or AFL-2.0
 *
 */

#ifndef KERNELS_00__XDG_MIME_MAGIC_MATCHLET_COMPARE_TO_DATA_INCLUDE_KERNEL_H_
#define KERNELS_00__XDG_MIME_MAGIC_MATCHLET_COMPARE_TO_DATA_INCLUDE_KERNEL_H_

#include <stddef.h>

/*
 * gio/xdgmime/xdgmimemagic.c:39
 */
typedef struct XdgMimeMagicMatchlet XdgMimeMagicMatchlet;

/*
 * gio/xdgmime/xdgmimemagic.c:58-68
 */
struct XdgMimeMagicMatchlet {
    int indent;
    int offset;
    unsigned int value_length;
    unsigned char *value;
    unsigned char *mask;
    unsigned int range_length;
    unsigned int word_size;
    XdgMimeMagicMatchlet *next;
};

/*
 * Wrapper for invoking the extracted kernel.
 */
int xdg_mime_magic_matchlet_compare_to_data_isolated(
    XdgMimeMagicMatchlet *matchlet, const void *data, size_t len);

#endif // KERNELS_00__XDG_MIME_MAGIC_MATCHLET_COMPARE_TO_DATA_INCLUDE_KERNEL_H_
