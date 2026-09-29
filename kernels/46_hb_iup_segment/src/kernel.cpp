/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-subset-instancer-iup.cc
 *    src/hb-common.cc
 *
 *
 *  The original file copyright and license notices follow.
 *
 *  src/hb-subset-instancer-iup.cc
 *
 * Copyright © 2024  Google, Inc.
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
 *  src/hb-common.cc
 *
 * Copyright © 2009,2010  Red Hat, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#include "kernel.h"

/* src/hb-subset-instancer-iup.cc:181-263; implementation comments removed. */
static bool _iup_segment(const hb_array_t<const contour_point_t> contour_points,
                         const hb_array_t<const int> x_deltas,
                         const hb_array_t<const int> y_deltas,
                         const contour_point_t &p1, const contour_point_t &p2,
                         int p1_dx, int p2_dx, int p1_dy, int p2_dy,
                         double tolerance_sq,
                         hb_vector_t<double> &interp_x_deltas,
                         hb_vector_t<double> &interp_y_deltas) {
    unsigned n = contour_points.length;
    if (unlikely(!interp_x_deltas.resize_dirty(n) ||
                 !interp_y_deltas.resize_dirty(n)))
        return false;

    for (unsigned j = 0; j < 2; j++) {
        float contour_point_t::*xp;
        double x1, x2, d1, d2;
        const int *in;
        double *out;
        if (j == 0) {
            xp = &contour_point_t::x;
            x1 = static_cast<double>(p1.x);
            x2 = static_cast<double>(p2.x);
            d1 = p1_dx;
            d2 = p2_dx;
            in = x_deltas.arrayZ;
            out = interp_x_deltas.arrayZ;
        } else {
            xp = &contour_point_t::y;
            x1 = static_cast<double>(p1.y);
            x2 = static_cast<double>(p2.y);
            d1 = p1_dy;
            d2 = p2_dy;
            in = y_deltas.arrayZ;
            out = interp_y_deltas.arrayZ;
        }

        if (x1 == x2) {
            if (d1 == d2) {
                for (unsigned i = 0; i < n; i++)
                    out[i] = d1;
            } else {
                for (unsigned i = 0; i < n; i++)
                    out[i] = 0.0;
            }
            continue;
        }

        if (x1 > x2) {
            hb_swap(x1, x2);
            hb_swap(d1, d2);
        }

        double scale = (d2 - d1) / (x2 - x1);
        for (unsigned i = 0; i < n; i++) {
            double x = (double)(contour_points.arrayZ[i].*xp);
            double d;
            if (x <= x1)
                d = d1;
            else if (x >= x2)
                d = d2;
            else
                d = d1 + (x - x1) * scale;

            out[i] = d;
            double err = d - in[i];
            if (err * err > tolerance_sq)
                return false;
        }
    }
    return true;
}

/* Wrapper for invoking the extracted kernel. */
extern "C" bool hb_012_iup_segment_interpolate_isolated(
    const hb_array_t<const contour_point_t> contour_points,
    const hb_array_t<const int> x_deltas, const hb_array_t<const int> y_deltas,
    const contour_point_t &p1, const contour_point_t &p2, int p1_dx, int p2_dx,
    int p1_dy, int p2_dy, double tolerance_sq,
    hb_vector_t<double> &interp_x_deltas,
    hb_vector_t<double> &interp_y_deltas) {
    return _iup_segment(contour_points, x_deltas, y_deltas, p1, p2, p1_dx,
                        p2_dx, p1_dy, p2_dy, tolerance_sq, interp_x_deltas,
                        interp_y_deltas);
}

/* src/hb-common.cc:1200,1228,1239; default allocator selected from
 * src/hb.hh:533-553 (HB_CUSTOM_MALLOC not defined). */
void *hb_malloc(size_t size) { return malloc(size); }
void *hb_realloc(void *ptr, size_t size) { return realloc(ptr, size); }
void hb_free(void *ptr) { free(ptr); }
