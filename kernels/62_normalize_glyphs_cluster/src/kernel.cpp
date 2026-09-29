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
 *
 */

#include "kernel.h"

/*
 * src/hb-buffer.cc:2101-2106
 */
static int compare_info_codepoint(const hb_glyph_info_t *pa,
                                  const hb_glyph_info_t *pb) {
    return (int)pb->codepoint - (int)pa->codepoint;
}

/*
 * src/hb-buffer.cc:2108-2154
 */
static inline void normalize_glyphs_cluster(hb_buffer_t *buffer,
                                            unsigned int start,
                                            unsigned int end, bool backward) {
    hb_glyph_position_t *pos = buffer->pos;

    hb_position_t total_x_advance = 0, total_y_advance = 0;
    for (unsigned int i = start; i < end; i++) {
        total_x_advance = hb_saturate_add(total_x_advance, pos[i].x_advance);
        total_y_advance = hb_saturate_add(total_y_advance, pos[i].y_advance);
    }

    hb_position_t x_advance = 0, y_advance = 0;
    for (unsigned int i = start; i < end; i++) {
        pos[i].x_offset = hb_saturate_add(pos[i].x_offset, x_advance);
        pos[i].y_offset = hb_saturate_add(pos[i].y_offset, y_advance);

        x_advance = hb_saturate_add(x_advance, pos[i].x_advance);
        y_advance = hb_saturate_add(y_advance, pos[i].y_advance);

        pos[i].x_advance = 0;
        pos[i].y_advance = 0;
    }

    if (backward) {
        pos[end - 1].x_advance = total_x_advance;
        pos[end - 1].y_advance = total_y_advance;

        hb_stable_sort(buffer->info + start, end - start - 1,
                       compare_info_codepoint, buffer->pos + start);
    } else {
        pos[start].x_advance =
            hb_saturate_add(pos[start].x_advance, total_x_advance);
        pos[start].y_advance =
            hb_saturate_add(pos[start].y_advance, total_y_advance);
        for (unsigned int i = start + 1; i < end; i++) {
            pos[i].x_offset = hb_saturate_sub(pos[i].x_offset, total_x_advance);
            pos[i].y_offset = hb_saturate_sub(pos[i].y_offset, total_y_advance);
        }
        hb_stable_sort(buffer->info + start + 1, end - start - 1,
                       compare_info_codepoint, buffer->pos + start + 1);
    }
}

/* Local benchmark adapter (not upstream): assemble a buffer around the
 * supplied arrays and forward to the copied static function above. */
extern "C" void normalize_glyphs_cluster_isolated(hb_glyph_info_t *info,
                                         hb_glyph_position_t *pos,
                                         unsigned int start, unsigned int end,
                                         bool backward) {
    hb_buffer_t buffer{};
    buffer.info = info;
    buffer.pos = pos;
    normalize_glyphs_cluster(&buffer, start, end, backward);
}
