/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-raster-draw.cc
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/hb-raster-draw.cc
 *
 * Copyright © 2026  Behdad Esfahbod
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
 * Author(s): Behdad Esfahbod
 *
 */

#include "kernel.h"

/*
 * src/hb-raster-draw.cc:1330-1339
 */
static int32_t sweep_row_to_alpha_impl(uint8_t *__restrict row_buf,
                                       int32_t *__restrict area,
                                       int16_t *__restrict cover,
                                       unsigned x_min, unsigned x_max) {
    const int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
    int32_t cover_accum = 0;
    unsigned x = x_min;

    /*
     * src/hb-raster-draw.cc:1423-1435
     */
    for (; x <= x_max; x++) {
        cover_accum += cover[x];
        int32_t val = cover_accum * cover_scale - area[x];
        int32_t alpha = val < 0 ? -val : val;
        if (alpha > HB_RASTER_FULL_COVERAGE)
            alpha = HB_RASTER_FULL_COVERAGE;
        row_buf[x] =
            (uint8_t)(((unsigned)alpha * 255 + HB_RASTER_FULL_COVERAGE / 2) >>
                      (2 * HB_RASTER_PIXEL_BITS + 1));
        area[x] = 0;
        cover[x] = 0;
    }

    return cover_accum;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
extern "C" int32_t sweep_row_to_alpha(uint8_t *__restrict row_buf,
                                      int32_t *__restrict area,
                                      int16_t *__restrict cover, unsigned x_min,
                                      unsigned x_max) {
    return sweep_row_to_alpha_impl(row_buf, area, cover, x_min, x_max);
}
