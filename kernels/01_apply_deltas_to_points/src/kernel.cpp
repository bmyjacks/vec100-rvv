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
 * src/hb-ot-var-gvar-table.hh:651-672
 */
static float infer_delta(const hb_array_t<contour_point_t> points,
                         const hb_array_t<contour_point_t> deltas,
                         unsigned int target, unsigned int prev,
                         unsigned int next, float contour_point_t::*m) {
    float target_val = points.arrayZ[target].*m;
    float prev_val = points.arrayZ[prev].*m;
    float next_val = points.arrayZ[next].*m;
    float prev_delta = deltas.arrayZ[prev].*m;
    float next_delta = deltas.arrayZ[next].*m;

    if (prev_val == next_val)
        return (prev_delta == next_delta) ? prev_delta : 0.f;
    else if (target_val <= hb_min(prev_val, next_val))
        return (prev_val < next_val) ? prev_delta : next_delta;
    else if (target_val >= hb_max(prev_val, next_val))
        return (prev_val > next_val) ? prev_delta : next_delta;

    float r = (target_val - prev_val) / (next_val - prev_val);
    return prev_delta + r * (next_delta - prev_delta);
}

/*
 * src/hb-ot-var-gvar-table.hh:674-675
 */
static unsigned int next_index(unsigned int i, unsigned int start,
                               unsigned int end) {
    return (i >= end) ? start : (i + 1);
}

/*
 * Wrapper for invoking the extracted IUP interpolation block.
 */
void gvar_iup_interpolate(const hb_array_t<contour_point_t> points,
                          hb_array_t<contour_point_t> deltas,
                          hb_array_t<contour_point_t> orig_points,
                          bool apply_to_all, bool phantom_only) {
    /*
     * src/hb-ot-var-gvar-table.hh:799
     */
    unsigned count = points.length;

    /*
     * src/hb-ot-var-gvar-table.hh:949-1003
     */
    if (!apply_to_all && !phantom_only) {
        unsigned start_point = 0;
        unsigned end_point = 0;
        while (true) {
            while (end_point < count && !points.arrayZ[end_point].is_end_point)
                end_point++;
            if (unlikely(end_point == count))
                break;

            unsigned unref_count = 0;
            for (unsigned i = start_point; i < end_point + 1; i++)
                unref_count += deltas.arrayZ[i].flag;
            unref_count = (end_point - start_point + 1) - unref_count;

            unsigned j = start_point;
            if (unref_count == 0 || unref_count > end_point - start_point)
                goto no_more_gaps;

            for (;;) {
                unsigned int prev, next, i;
                for (;;) {
                    i = j;
                    j = next_index(i, start_point, end_point);
                    if (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag)
                        break;
                }
                prev = j = i;
                for (;;) {
                    i = j;
                    j = next_index(i, start_point, end_point);
                    if (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag)
                        break;
                }
                next = j;

                i = prev;
                for (;;) {
                    i = next_index(i, start_point, end_point);
                    if (i == next)
                        break;
                    deltas.arrayZ[i].x =
                        infer_delta(orig_points, deltas, i, prev, next,
                                    &contour_point_t::x);
                    deltas.arrayZ[i].y =
                        infer_delta(orig_points, deltas, i, prev, next,
                                    &contour_point_t::y);
                    if (--unref_count == 0)
                        goto no_more_gaps;
                }
            }
        no_more_gaps:
            start_point = end_point = end_point + 1;
        }
    }
}
