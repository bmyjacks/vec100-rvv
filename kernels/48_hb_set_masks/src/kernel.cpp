/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-buffer.cc
 *
 *
 *  The original file copyright and license notices follow.
 *
 * src/hb-buffer.cc
 *
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2004,2007,2009,2010  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Red Hat Author(s): Owen Taylor, Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#include "kernel.h"

/*
 * Wrapper for src/hb-buffer.cc:530-558: this->info and this->len are explicit
 * arguments. Lines 542-544 update max_ops/successful on the owning buffer but
 * do not feed either selected loop, so that unrelated bookkeeping is omitted.
 * The statements of both loops and their runtime branch are preserved.
 */
extern "C" void hb_017_buffer_set_masks_isolated(hb_glyph_info_t *info, unsigned int len,
                                        hb_mask_t value, hb_mask_t mask,
                                        unsigned int cluster_start,
                                        unsigned int cluster_end) {
    if (!mask)
        return;

    hb_mask_t not_mask = ~mask;
    value &= mask;

    unsigned int count = len;

    if (cluster_start == 0 && cluster_end == (unsigned int)-1) {
        for (unsigned int i = 0; i < count; i++)
            info[i].mask = (info[i].mask & not_mask) | value;
        return;
    }

    for (unsigned int i = 0; i < count; i++)
        if (cluster_start <= info[i].cluster && info[i].cluster < cluster_end)
            info[i].mask = (info[i].mask & not_mask) | value;
}
