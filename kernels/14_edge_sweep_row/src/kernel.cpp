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
 * src/hb-raster-draw.cc:1210-1235
 */
static HB_ALWAYS_INLINE void cell_add(int32_t *area, int16_t *cover,
                                      unsigned width, int col, int32_t fx0,
                                      int32_t fy0, int32_t fx1, int32_t fy1,
                                      int32_t wind, unsigned &x_min,
                                      unsigned &x_max) {
    if (unlikely((unsigned)col >= width)) {
        if (unlikely(col < 0)) {
            int32_t dy = fy1 - fy0;
            cover[0] += (int16_t)(dy * wind);
            x_min = hb_min(x_min, 0u);
            x_max = hb_max(x_max, 0u);
        }
        return;
    }
    int32_t dy = fy1 - fy0;
    area[col] += (fx0 + fx1) * dy * wind;
    cover[col] += (int16_t)(dy * wind);
    x_min = hb_min(x_min, (unsigned)col);
    x_max = hb_max(x_max, (unsigned)col);
}

/*
 * src/hb-raster-draw.cc:1239-1326
 */
static HB_ALWAYS_INLINE void
edge_sweep_row_impl(int32_t *area, int16_t *cover, unsigned width, int x_org,
                    int32_t y_top, const hb_raster_edge_t &edge,
                    unsigned &x_min, unsigned &x_max) {
    int32_t y_bot = (int32_t)hb_min((int64_t)y_top + HB_RASTER_ONE_PIXEL,
                                    (int64_t)INT32_MAX);

    int32_t ey0 = hb_max(edge.yL, y_top);
    int32_t ey1 = hb_min(edge.yH, y_bot);
    if (ey0 >= ey1)
        return;

    int64_t x0_64 = (int64_t)edge.xL +
                    ((((int64_t)ey0 - (int64_t)edge.yL) * edge.slope) >> 16);
    int64_t x1_64 = (int64_t)edge.xL +
                    ((((int64_t)ey1 - (int64_t)edge.yL) * edge.slope) >> 16);
    int32_t x0 =
        (int32_t)hb_clamp(x0_64, (int64_t)INT32_MIN, (int64_t)INT32_MAX);
    int32_t x1 =
        (int32_t)hb_clamp(x1_64, (int64_t)INT32_MIN, (int64_t)INT32_MAX);

    int32_t fy0 = ey0 - y_top;
    int32_t fy1 = ey1 - y_top;

    int32_t cx0 = x0 >> HB_RASTER_PIXEL_BITS;
    int32_t fx0 = x0 & HB_RASTER_PIXEL_MASK;
    int32_t cx1 = x1 >> HB_RASTER_PIXEL_BITS;
    int32_t fx1 = x1 & HB_RASTER_PIXEL_MASK;
    int32_t wind = edge.wind;

    if (cx0 == cx1) {
        cell_add(area, cover, width, cx0 - x_org, fx0, fy0, fx1, fy1, wind,
                 x_min, x_max);
        return;
    }

    int64_t total_dx = (int64_t)x1 - (int64_t)x0;
    int64_t total_dy = (int64_t)fy1 - (int64_t)fy0;

    int32_t delta_fy =
        (int32_t)((int64_t)HB_RASTER_ONE_PIXEL * total_dy / total_dx);

    if (total_dx > 0) {
        int32_t x_b =
            (int32_t)hb_clamp(((int64_t)cx0 + 1) * HB_RASTER_ONE_PIXEL,
                              (int64_t)INT32_MIN, (int64_t)INT32_MAX);
        int32_t fy_b =
            fy0 +
            (int32_t)((((int64_t)x_b - (int64_t)x0) * total_dy) / total_dx);
        cell_add(area, cover, width, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL,
                 fy_b, wind, x_min, x_max);

        int32_t fy_prev = fy_b;
        for (int32_t cx = cx0 + 1; cx < cx1; cx++) {
            fy_b = fy_prev + delta_fy;
            cell_add(area, cover, width, cx - x_org, 0, fy_prev,
                     HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
            fy_prev = fy_b;
        }

        cell_add(area, cover, width, cx1 - x_org, 0, fy_prev, fx1, fy1, wind,
                 x_min, x_max);
    } else {
        int32_t x_b = (int32_t)hb_clamp((int64_t)cx0 * HB_RASTER_ONE_PIXEL,
                                        (int64_t)INT32_MIN, (int64_t)INT32_MAX);
        int32_t fy_b =
            fy0 +
            (int32_t)((((int64_t)x_b - (int64_t)x0) * total_dy) / total_dx);
        cell_add(area, cover, width, cx0 - x_org, fx0, fy0, 0, fy_b, wind,
                 x_min, x_max);

        int32_t fy_prev = fy_b;
        for (int32_t cx = cx0 - 1; cx > cx1; cx--) {
            fy_b = fy_prev - delta_fy;
            cell_add(area, cover, width, cx - x_org, HB_RASTER_ONE_PIXEL,
                     fy_prev, 0, fy_b, wind, x_min, x_max);
            fy_prev = fy_b;
        }

        cell_add(area, cover, width, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev,
                 fx1, fy1, wind, x_min, x_max);
    }
}

/*
 * Wrapper for invoking the extracted kernel.
 */
extern "C" void edge_sweep_row_isolated(int32_t *area, int16_t *cover, unsigned width,
                               int x_org, int32_t y_top,
                               const hb_raster_edge_t &edge, unsigned &x_min,
                               unsigned &x_max) {
    edge_sweep_row_impl(area, cover, width, x_org, y_top, edge, x_min, x_max);
}
