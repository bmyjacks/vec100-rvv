/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb.hh
 *    src/hb-meta.hh
 *    src/hb-machinery.hh
 *    src/hb-algs.hh
 *    src/hb-open-type.hh
 *    src/hb-subset-plan.hh
 *    src/hb-array.hh
 *    src/OT/glyf/SimpleGlyph.hh
 *
 *
 *  The original file copyright and license notices follow.
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
 * src/hb-machinery.hh
 *
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012,2018  Google, Inc.
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
 *
 * src/hb-open-type.hh
 *
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
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
 * src/hb-subset-plan.hh
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
 * Google Author(s): Garret Rieger, Roderick Sheeter
 *
 *
 * src/hb-array.hh
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
 */

#ifndef KERNELS_74_READ_POINTS_INCLUDE_KERNEL_H_
#define KERNELS_74_READ_POINTS_INCLUDE_KERNEL_H_

/*
 * <cstdlib> is included because the original translation unit included it
 * (hb.hh) before hb-algs.hh. On this sysroot it transitively defines
 * __BYTE_ORDER, which makes the verbatim HB_FAST_NUM_ACCESS block below
 * resolve exactly as in the original project build.
 */
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

/*
 * src/hb.hh:251-267
 */
#ifdef __has_builtin
#define hb_has_builtin __has_builtin
#elif defined(_MSC_VER)
#define hb_has_builtin(x) 0
#else
#define hb_has_builtin(x) ((defined(__GNUC__) && __GNUC__ >= 5))
#endif
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
 * src/hb-machinery.hh:97-115
 */
#define _DEFINE_INSTANCE_ASSERTION1(_line, _assertion)                         \
    void _instance_assertion_on_line_##_line() const {                         \
        static_assert((_assertion), "");                                       \
    }
#define _DEFINE_INSTANCE_ASSERTION0(_line, _assertion)                         \
    _DEFINE_INSTANCE_ASSERTION1(_line, _assertion)
#define DEFINE_INSTANCE_ASSERTION(_assertion)                                  \
    _DEFINE_INSTANCE_ASSERTION0(__LINE__, _assertion)
#define _DEFINE_COMPILES_ASSERTION1(_line, _code)                              \
    void _compiles_assertion_on_line_##_line() const { _code; }
#define _DEFINE_COMPILES_ASSERTION0(_line, _code)                              \
    _DEFINE_COMPILES_ASSERTION1(_line, _code)
#define DEFINE_COMPILES_ASSERTION(_code)                                       \
    _DEFINE_COMPILES_ASSERTION0(__LINE__, _code)
#define DEFINE_SIZE_STATIC(size)                                               \
    DEFINE_INSTANCE_ASSERTION(sizeof(*this) == (size))                         \
    size_t get_size() const { return (size); }                                 \
    static constexpr unsigned null_size = (size);                              \
    static constexpr unsigned min_size = (size);                               \
    static constexpr unsigned static_size = (size)

/*
 * src/hb-algs.hh:104-114
 */
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

/*
 * src/hb-algs.hh:116-140
 */
#ifndef HB_FAST_NUM_ACCESS

#if defined(__OPTIMIZE__) && defined(__BYTE_ORDER) &&                          \
    (__BYTE_ORDER == __BIG_ENDIAN ||                                           \
     (__BYTE_ORDER == __LITTLE_ENDIAN && hb_has_builtin(__builtin_bswap16) &&  \
      hb_has_builtin(__builtin_bswap32) && hb_has_builtin(__builtin_bswap64)))
#define HB_FAST_NUM_ACCESS 1
#else
#define HB_FAST_NUM_ACCESS 0
#endif

#if defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 12)
#undef HB_FAST_NUM_ACCESS
#define HB_FAST_NUM_ACCESS 0
#endif

#endif

/*
 * src/hb-algs.hh:141-176
 */
template <bool BE, typename Type, int Bytes = sizeof(Type)> struct HBInt;
template <bool BE, typename Type> struct HBInt<BE, Type, 1> {
  public:
    HBInt() = default;
    constexpr HBInt(Type V) : v{uint8_t(V)} {}
    constexpr operator Type() const { return v; }

  private:
    uint8_t v;
};
template <bool BE, typename Type> struct HBInt<BE, Type, 2> {
  public:
    HBInt() = default;

    HBInt(Type V)
#if HB_FAST_NUM_ACCESS
    {
        if (BE == (__BYTE_ORDER == __BIG_ENDIAN))
            *((hb_packed_t<uint16_t> *)v) = V;
        else
            *((hb_packed_t<uint16_t> *)v) = __builtin_bswap16(V);
    }
#else
        : v{BE ? uint8_t((V >> 8) & 0xFF) : uint8_t((V) & 0xFF),
            BE ? uint8_t((V) & 0xFF) : uint8_t((V >> 8) & 0xFF)} {
    }
#endif

    constexpr operator Type() const {
#if HB_FAST_NUM_ACCESS
        return (BE == (__BYTE_ORDER == __BIG_ENDIAN))
                   ? (uint16_t)*((const hb_packed_t<uint16_t> *)v)
                   : __builtin_bswap16(
                         (uint16_t)*((const hb_packed_t<uint16_t> *)v));
#else
        return (BE ? (v[0] << 8) : (v[0])) + (BE ? (v[1]) : (v[1] << 8));
#endif
    }

  private:
    uint8_t v[2];
};

/*
 * src/hb-algs.hh:871-891
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

/*
 * src/hb-open-type.hh:56-113
 */
template <bool BE, typename Type, unsigned int Size = sizeof(Type)>
struct NumType {
    typedef Type type;
    typedef typename std::conditional<
        std::is_integral<Type>::value && sizeof(Type) <= sizeof(int),
        typename std::conditional<std::is_signed<Type>::value, signed,
                                  unsigned>::type,
        Type>::type WideType;

    NumType() = default;
    explicit constexpr NumType(Type V) : v{V} {}
    NumType &operator=(Type V) {
        v = V;
        return *this;
    }

    operator WideType() const { return v; }

    bool operator==(const NumType &o) const { return (Type)v == (Type)o.v; }
    bool operator!=(const NumType &o) const { return !(*this == o); }

    NumType &operator+=(WideType count) {
        *this = *this + count;
        return *this;
    }
    NumType &operator-=(WideType count) {
        *this = *this - count;
        return *this;
    }
    NumType &operator++() {
        *this += 1;
        return *this;
    }
    NumType &operator--() {
        *this -= 1;
        return *this;
    }
    NumType operator++(int) {
        NumType c(*this);
        ++*this;
        return c;
    }
    NumType operator--(int) {
        NumType c(*this);
        --*this;
        return c;
    }

  protected:
    HBInt<BE, Type, Size> v;

  public:
    DEFINE_SIZE_STATIC(Size);
};

/*
 * src/hb-open-type.hh:115-120
 */
typedef NumType<true, uint8_t> HBUINT8;
typedef NumType<true, int16_t> HBINT16;

/*
 * src/hb-subset-plan.hh:77-102
 */
struct contour_point_t {
    void init(float x_ = 0.f, float y_ = 0.f, bool is_end_point_ = false) {
        flag = 0;
        x = x_;
        y = y_;
        is_end_point = is_end_point_;
    }

    void transform(const float (&matrix)[4]) {
        float x_ = x * matrix[0] + y * matrix[2];
        y = x * matrix[1] + y * matrix[3];
        x = x_;
    }

    void add_delta(float delta_x, float delta_y) {
        x += delta_x;
        y += delta_y;
    }

    HB_ALWAYS_INLINE
    void translate(const contour_point_t &p) {
        x += p.x;
        y += p.y;
    }

    float x;
    float y;
    uint8_t flag;
    bool is_end_point;
};

/*
 * src/hb-array.hh:47-61 (reduced to the constructors used by the kernel)
 */
template <typename Type> struct hb_array_t {
    hb_array_t() = default;
    hb_array_t(const hb_array_t &) = default;
    ~hb_array_t() = default;
    hb_array_t &operator=(const hb_array_t &) = default;

    constexpr hb_array_t(Type *array_, unsigned int length_)
        : arrayZ(array_), length(length_) {}

    /*
     * src/hb-array.hh:135-136
     */
    Type *begin() const { return arrayZ; }
    Type *end() const { return arrayZ + length; }

  public:
    /*
     * src/hb-array.hh:359-361
     */
    Type *arrayZ = nullptr;
    unsigned int length = 0;
    unsigned int backwards_length = 0;
};

/*
 * src/OT/glyf/SimpleGlyph.hh:12-24 (reduced to the enum and used methods)
 */
struct SimpleGlyph {
    enum simple_glyph_flag_t {
        FLAG_ON_CURVE = 0x01,
        FLAG_X_SHORT = 0x02,
        FLAG_Y_SHORT = 0x04,
        FLAG_REPEAT = 0x08,
        FLAG_X_SAME = 0x10,
        FLAG_Y_SAME = 0x20,
        FLAG_OVERLAP_SIMPLE = 0x40,
        FLAG_CUBIC = 0x80
    };

    /*
     * src/OT/glyf/SimpleGlyph.hh:126-128
     */
    static bool read_flags(const HBUINT8 *&p,
                           hb_array_t<contour_point_t> points_,
                           const HBUINT8 *end);

    /*
     * src/OT/glyf/SimpleGlyph.hh:149-154
     */
    static bool read_points(const HBUINT8 *&p,
                            hb_array_t<contour_point_t> points_,
                            const HBUINT8 *end, float contour_point_t::*m,
                            const simple_glyph_flag_t short_flag,
                            const simple_glyph_flag_t same_flag);
};

/*
 * Wrappers for invoking the extracted kernel.
 */
extern "C" bool read_flags(const HBUINT8 *&p,
                           hb_array_t<contour_point_t> points_,
                           const HBUINT8 *end);
extern "C" bool read_points(const HBUINT8 *&p,
                            hb_array_t<contour_point_t> points_,
                            const HBUINT8 *end, float contour_point_t::*m,
                            const SimpleGlyph::simple_glyph_flag_t short_flag,
                            const SimpleGlyph::simple_glyph_flag_t same_flag);

#endif // KERNELS_74_READ_POINTS_INCLUDE_KERNEL_H_
