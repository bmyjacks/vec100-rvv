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

#include "kernel.h"

/*
 * gio/xdgmime/xdgmimemagic.c:515-555
 */
static int
_xdg_mime_magic_matchlet_compare_to_data(XdgMimeMagicMatchlet *matchlet,
                                         const void *data, size_t len) {
    unsigned int i, j;
    for (i = matchlet->offset; i < matchlet->offset + matchlet->range_length;
         i++) {
        int valid_matchlet = 1;

        if (i + matchlet->value_length > len)
            return 0;

        if (matchlet->mask) {
            for (j = 0; j < matchlet->value_length; j++) {
                if ((matchlet->value[j] & matchlet->mask[j]) !=
                    ((((unsigned char *)data)[j + i]) & matchlet->mask[j])) {
                    valid_matchlet = 0;
                    break;
                }
            }
        } else {
            for (j = 0; j < matchlet->value_length; j++) {
                if (matchlet->value[j] != ((unsigned char *)data)[j + i]) {
                    valid_matchlet = 0;
                    break;
                }
            }
        }
        if (valid_matchlet)
            return 1;
    }
    return 0;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
int xdg_mime_magic_matchlet_compare_to_data_isolated(
    XdgMimeMagicMatchlet *matchlet, const void *data, size_t len) {
    return _xdg_mime_magic_matchlet_compare_to_data(matchlet, data, len);
}
