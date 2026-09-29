/****************************************************************************
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-raster.hh
 *    src/hb-raster-paint.cc
 *
 *  The original file copyright and license notices follow.
 *
 * src/hb-raster.hh
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
 * src/hb-raster-paint.cc
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
 */

#include "kernel.h"
#include <cmath>

/* src/hb-raster.hh:63-74 */
static HB_ALWAYS_INLINE uint8_t hb_raster_div255(unsigned a) {
    if (true) {
        return (a + 255) >> 8;
    }
    return (uint8_t)((a + 128 + ((a + 128) >> 8)) >> 8);
}

/* src/hb-raster.hh:83-95 */
static HB_ALWAYS_INLINE uint32_t hb_raster_src_over(uint32_t src,
                                                     uint32_t dst) {
    uint8_t sa = (uint8_t)(src >> 24);
    if (sa == 255)
        return src;
    if (sa == 0)
        return dst;
    unsigned inv_sa = 255 - sa;
    uint8_t rb =
        hb_raster_div255((dst & 0xFF) * inv_sa) + (uint8_t)(src & 0xFF);
    uint8_t rg = hb_raster_div255(((dst >> 8) & 0xFF) * inv_sa) +
                 (uint8_t)((src >> 8) & 0xFF);
    uint8_t rr = hb_raster_div255(((dst >> 16) & 0xFF) * inv_sa) +
                 (uint8_t)((src >> 16) & 0xFF);
    uint8_t ra = hb_raster_div255(((dst >> 24) & 0xFF) * inv_sa) + sa;
    return (uint32_t)rb | ((uint32_t)rg << 8) | ((uint32_t)rr << 16) |
           ((uint32_t)ra << 24);
}

/* src/hb-raster-paint.cc:1130 */
#define GRADIENT_LUT_SIZE 256

/* src/hb-raster-paint.cc:1133-1138 */
static HB_ALWAYS_INLINE float reflect_gradient_t(float t) {
    t = fmodf(fabsf(t), 2.f);
    return t > 1.f ? 2.f - t : t;
}

/* src/hb-raster-paint.cc:1207-1223 */
static HB_ALWAYS_INLINE float normalize_gradient_t(float t,
                                                    hb_paint_extend_t extend) {
    if (unlikely(!std::isfinite(t)))
        return 0.f;
    if (extend == HB_PAINT_EXTEND_PAD)
        return hb_clamp(t, 0.f, 1.f);
    if (extend == HB_PAINT_EXTEND_REPEAT) {
        t = t - floorf(t);
        return t < 0.f ? t + 1.f : t;
    }
    return reflect_gradient_t(t);
}

/* src/hb-raster-paint.cc:1237-1249 */
static HB_ALWAYS_INLINE uint32_t lookup_gradient_lut(const uint32_t *lut,
                                                      float t,
                                                      hb_paint_extend_t extend) {
    float u = normalize_gradient_t(t, extend);
    if (unlikely(!std::isfinite(u)))
        u = 0.f;
    else
        u = hb_clamp(u, 0.f, 1.f);
    unsigned idx = (unsigned)(u * (GRADIENT_LUT_SIZE - 1) + 0.5f);
    return lut[idx];
}

/* Wrapper for the selected upstream inner loop. The only replacements are
 * clip.min_x/max_x -> min_x/max_x and the precomputed locals -> parameters. */
extern "C" void hb_139_linear_gradient_region(
    hb_packed_t<uint32_t> *row, unsigned min_x, unsigned max_x, float gx,
    float gy, float gx0, float gy0, float dx, float dy, float inv_denom,
    float inv_xx, float inv_yx, const uint32_t *lut,
    hb_paint_extend_t extend) {
    /* src/hb-raster-paint.cc:1333-1340 */
    for (unsigned px = min_x; px < max_x; px++) {
        float proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
        uint32_t src = lookup_gradient_lut(lut, proj_t, extend);
        row[px] = hb_packed_t<uint32_t>(
            hb_raster_src_over(src, (uint32_t)row[px]));
        gx += inv_xx;
        gy += inv_yx;
    }
}
