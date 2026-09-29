/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-ot-var-gvar-table.hh
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/hb-ot-var-gvar-table.hh
 *
 * Copyright © 2019  Adobe Inc.
 * Copyright © 2019  Ebrahim Byagowi
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
 * Adobe Author(s): Michiharu Ariza
 *
 */

#include "kernel.h"

/*
 * src/hb-ot-var-gvar-table.hh:677-753
 */
template <typename GidOffsetType, unsigned TableTag>
template <bool is_x>
bool gvar_GVAR<GidOffsetType, TableTag>::accelerator_t::
    decompile_deltas_add_to_points(const HBUINT8 *&p,
                                   hb_array_t<contour_point_t> points,
                                   float scalar, const HBUINT8 *end,
                                   unsigned start) {
    unsigned i = 0;
    unsigned count = points.length;
    while (i < count) {
        if (unlikely(p + 1 > end))
            return false;
        unsigned control = *p++;
        unsigned run_count = (control & TupleValues::VALUE_RUN_COUNT_MASK) + 1;
        unsigned stop = i + run_count;
        if (unlikely(stop > count))
            return false;

        unsigned skip = i < start ? hb_min(start - i, run_count) : 0;
        i += skip;

        switch (control & TupleValues::VALUES_SIZE_MASK) {
        case TupleValues::VALUES_ARE_ZEROS:
            i = stop;
            break;
        case TupleValues::VALUES_ARE_WORDS: {
            if (unlikely(p + run_count * HBINT16::static_size > end))
                return false;
            p += skip * HBINT16::static_size;
            const auto *pp = (const HBINT16 *)p;
            for (; i < stop; i++) {
                float v = *pp++ * scalar;
                if (is_x)
                    points.arrayZ[i].x += v;
                else
                    points.arrayZ[i].y += v;
            }
            p = (const HBUINT8 *)pp;
        } break;
        case TupleValues::VALUES_ARE_LONGS: {
            if (unlikely(p + run_count * HBINT32::static_size > end))
                return false;
            p += skip * HBINT32::static_size;
            const auto *pp = (const HBINT32 *)p;
            for (; i < stop; i++) {
                float v = *pp++ * scalar;
                if (is_x)
                    points.arrayZ[i].x += v;
                else
                    points.arrayZ[i].y += v;
            }
            p = (const HBUINT8 *)pp;
        } break;
        case TupleValues::VALUES_ARE_BYTES: {
            if (unlikely(p + run_count > end))
                return false;
            p += skip * HBINT8::static_size;
            const auto *pp = (const HBINT8 *)p;
            for (; i < stop; i++) {
                float v = *pp++ * scalar;
                if (is_x)
                    points.arrayZ[i].x += v;
                else
                    points.arrayZ[i].y += v;
            }
            p = (const HBUINT8 *)pp;
        } break;
        }
    }
    return true;
}

/*
 * Wrapper for invoking the extracted member twice (x and y).
 */
extern "C" bool
decompile_deltas_add_to_points(const HBUINT8 *&p,
                               hb_array_t<contour_point_t> points, float scalar,
                               const HBUINT8 *end, unsigned start) {
    using accelerator_t = gvar_GVAR<HBUINT16, HB_OT_TAG_gvar>::accelerator_t;
    if (unlikely(!accelerator_t::decompile_deltas_add_to_points<true>(
            p, points, scalar, end, start)))
        return false;
    if (unlikely(!accelerator_t::decompile_deltas_add_to_points<false>(
            p, points, scalar, end, start)))
        return false;
    return true;
}
