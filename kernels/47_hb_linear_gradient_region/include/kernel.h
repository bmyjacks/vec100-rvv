/****************************************************************************
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb.hh
 *    src/hb-meta.hh
 *    src/hb-algs.hh
 *    src/hb-paint.h
 *
 *  The original file copyright and license notices follow.
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
 * src/hb-paint.h
 *
 * Copyright © 2022 Matthias Clasen
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

#ifndef KERNELS_47_HB_LINEAR_GRADIENT_REGION_INCLUDE_KERNEL_H_
#define KERNELS_47_HB_LINEAR_GRADIENT_REGION_INCLUDE_KERNEL_H_

#include <cstdint>
#include <utility>

/* src/hb.hh:253-259 (GCC/Clang configuration) */
#define hb_has_builtin __has_builtin

/* src/hb.hh:261-267 */
#if defined(__OPTIMIZE__) && hb_has_builtin(__builtin_expect)
#define unlikely(expr) __builtin_expect(bool(expr), 0)
#else
#define unlikely(expr) (expr)
#endif

/* src/hb.hh:281-286 (GCC/Clang configuration) */
#define HB_UNUSED __attribute__((unused))

/* src/hb.hh:304-309 (GCC/Clang configuration) */
#define static_const static const

/* src/hb.hh:324-330 (GCC/Clang configuration) */
#define HB_ALWAYS_INLINE __attribute__((always_inline)) inline

/* src/hb-meta.hh:76 */
#define HB_AUTO_RETURN(E)                                                      \
    ->decltype((E)) { return (E); }

/* src/hb-meta.hh:83 */
#define HB_FUNCOBJ(x) static_const x HB_UNUSED

/* src/hb-algs.hh:104-114 */
template <typename Type> struct __attribute__((packed)) hb_packed_t {
    hb_packed_t() = default;
    constexpr hb_packed_t(Type V) : v(V) {}
    operator Type() const { return v; }
    hb_packed_t &operator=(Type V) {
        v = V;
        return *this;
    }

  private:
    Type v;
};

/* src/hb-algs.hh:871-891 */
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

/* src/hb-paint.h:452-456 */
typedef enum {
    HB_PAINT_EXTEND_PAD,
    HB_PAINT_EXTEND_REPEAT,
    HB_PAINT_EXTEND_REFLECT
} hb_paint_extend_t;

/* Wrapper for the rectangular LUT pixel loop at
 * src/hb-raster-paint.cc:1333-1340. row is the row pointer AFTER the upstream
 * stride calculation; gx/gy are the upstream first-pixel values after its
 * inverse-transform setup. */
extern "C" void hb_139_linear_gradient_region(
    hb_packed_t<uint32_t> *row, unsigned min_x, unsigned max_x, float gx,
    float gy, float gx0, float gy0, float dx, float dy, float inv_denom,
    float inv_xx, float inv_yx, const uint32_t *lut,
    hb_paint_extend_t extend);

/* Locally written RVV variant, same contract. */
extern "C" void hb_139_linear_gradient_region_rvv(
    hb_packed_t<uint32_t> *row, unsigned min_x, unsigned max_x, float gx,
    float gy, float gx0, float gy0, float dx, float dy, float inv_denom,
    float inv_xx, float inv_yx, const uint32_t *lut,
    hb_paint_extend_t extend);

#endif // KERNELS_47_HB_LINEAR_GRADIENT_REGION_INCLUDE_KERNEL_H_
