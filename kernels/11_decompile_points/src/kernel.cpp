/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-ot-var-common.hh
 *    src/hb-common.cc
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/hb-ot-var-common.hh
 *
 * Copyright © 2021  Google, Inc.
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
 * src/hb-common.cc
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
 *
 */

#include "kernel.h"

/*
 * src/hb-ot-var-common.hh:1644-1694
 */
template <typename OffType>
bool TupleVariationData<OffType>::decompile_points(
    const HBUINT8 *&p, hb_vector_t<unsigned int> &points, const HBUINT8 *end) {
    enum packed_point_flag_t {
        POINTS_ARE_WORDS = 0x80,
        POINT_RUN_COUNT_MASK = 0x7F
    };

    if (unlikely(p + 1 > end))
        return false;

    unsigned count = *p++;
    if (count & POINTS_ARE_WORDS) {
        if (unlikely(p + 1 > end))
            return false;
        count = ((count & POINT_RUN_COUNT_MASK) << 8) | *p++;
    }
    if (unlikely(!points.resize_dirty(count)))
        return false;

    unsigned n = 0;
    unsigned i = 0;
    while (i < count) {
        if (unlikely(p + 1 > end))
            return false;
        unsigned control = *p++;
        unsigned run_count = (control & POINT_RUN_COUNT_MASK) + 1;
        unsigned stop = i + run_count;
        if (unlikely(stop > count))
            return false;
        if (control & POINTS_ARE_WORDS) {
            if (unlikely(p + run_count * HBUINT16::static_size > end))
                return false;
            for (; i < stop; i++) {
                n += *(const HBUINT16 *)p;
                points.arrayZ[i] = n;
                p += HBUINT16::static_size;
            }
        } else {
            if (unlikely(p + run_count > end))
                return false;
            for (; i < stop; i++) {
                n += *p++;
                points.arrayZ[i] = n;
            }
        }
    }
    return true;
}

/*
 * Wrapper for invoking the extracted member.
 */
extern "C" bool decompile_points(const HBUINT8 *&p,
                                 hb_vector_t<unsigned int> &points,
                                 const HBUINT8 *end) {
    return TupleVariationData<HBUINT16>::decompile_points(p, points, end);
}

/*
 * src/hb-common.cc:1200
 */
void *hb_malloc(size_t size) { return hb_malloc_impl(size); }

/*
 * src/hb-common.cc:1214
 */
void *hb_calloc(size_t nmemb, size_t size) {
    return hb_calloc_impl(nmemb, size);
}

/*
 * src/hb-common.cc:1228
 */
void *hb_realloc(void *ptr, size_t size) { return hb_realloc_impl(ptr, size); }

/*
 * src/hb-common.cc:1239
 */
void hb_free(void *ptr) { hb_free_impl(ptr); }
