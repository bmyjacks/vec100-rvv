/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-raster-draw.cc
 *    src/hb.hh
 *    src/hb-meta.hh
 *    src/hb-algs.hh
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
 *
 * src/hb.hh
 *
 * Copyright © 2007,2008,2009  Red Hat, Inc.
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
 *
 * src/hb-meta.hh
 *
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 *
 *
 * src/hb-algs.hh
 *
 * Copyright © 2017  Google, Inc.
 * Copyright © 2019  Facebook, Inc.
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
 * Google Author(s): Behdad Esfahbod
 * Facebook Author(s): Behdad Esfahbod
 *
 */

#ifndef KERNELS_14_EDGE_SWEEP_ROW_INCLUDE_KERNEL_H_
#define KERNELS_14_EDGE_SWEEP_ROW_INCLUDE_KERNEL_H_

#include <stdint.h>
#include <utility>

/*
 * src/hb-raster-draw.cc:53-60
 */
struct hb_raster_edge_t {
    int32_t xL, yL;
    int32_t xH, yH;
    int64_t slope;
    int32_t wind;
};

/*
 * src/hb.hh:251-259
 */
#ifdef __has_builtin
#define hb_has_builtin __has_builtin
#elif defined(_MSC_VER)
#define hb_has_builtin(x) 0
#else
#define hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#endif

/*
 * src/hb.hh:261-267
 */
#if defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#define likely(expr) __builtin_expect(bool(expr), 1)
#define unlikely(expr) __builtin_expect(bool(expr), 0)
#else
#define likely(expr) (expr)
#define unlikely(expr) (expr)
#endif

/*
 * src/hb.hh:281-287
 */
#if defined(__GNUC__) && (__GNUC__ >= 4) || (__clang__)
#define HB_UNUSED __attribute__((unused))
#elif defined(_MSC_VER)
#define HB_UNUSED __pragma(warning(suppress : 4100 4101 4189))
#else
#define HB_UNUSED
#endif

/*
 * src/hb.hh:304-309
 */
#if defined(__clang__) && __clang_major__ < 10
#define static_const static
#else
#define static_const static const
#endif

/*
 * src/hb.hh:324-330
 */
#ifndef HB_ALWAYS_INLINE
#if defined(_MSC_VER)
#define HB_ALWAYS_INLINE __forceinline
#else
#define HB_ALWAYS_INLINE __attribute__((always_inline)) inline
#endif
#endif

/*
 * src/hb-meta.hh:76
 */
#define HB_AUTO_RETURN(E)                                                      \
    ->decltype((E)) { return (E); }

/*
 * src/hb-meta.hh:83
 */
#define HB_FUNCOBJ(x) static_const x HB_UNUSED

/*
 * src/hb-algs.hh:871-892
 */
struct {
    template <typename T, typename T2>
    constexpr auto operator()(T &&a, T2 &&b) const
        HB_AUTO_RETURN(a <= b ? a : b)
} HB_FUNCOBJ(hb_min);
struct {
    template <typename T, typename T2>
    constexpr auto operator()(T &&a, T2 &&b) const
        HB_AUTO_RETURN(a >= b ? a : b)
} HB_FUNCOBJ(hb_max);
struct {
    template <typename T, typename T2, typename T3>
    constexpr auto operator()(T &&x, T2 &&min, T3 &&max) const
        HB_AUTO_RETURN(hb_min(hb_max(std::forward<T>(x), std::forward<T2>(min)),
                              std::forward<T3>(max)))
} HB_FUNCOBJ(hb_clamp);

/*
 * src/hb-raster-draw.cc:44-48
 */
#define HB_RASTER_PIXEL_BITS 8
#define HB_RASTER_ONE_PIXEL (1 << HB_RASTER_PIXEL_BITS)
#define HB_RASTER_PIXEL_MASK (HB_RASTER_ONE_PIXEL - 1)

#define HB_RASTER_FULL_COVERAGE (2 * HB_RASTER_ONE_PIXEL * HB_RASTER_ONE_PIXEL)

/*
 * Wrapper for invoking the extracted kernel.
 */
extern "C" void edge_sweep_row_isolated(int32_t *area, int16_t *cover, unsigned width,
                               int x_org, int32_t y_top,
                               const hb_raster_edge_t &edge, unsigned &x_min,
                               unsigned &x_max);

#endif // KERNELS_14_EDGE_SWEEP_ROW_INCLUDE_KERNEL_H_
