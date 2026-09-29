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
 *    src/hb-common.h
 *    src/hb-ot-var-gvar-table.hh
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
 * src/hb-common.h
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

#ifndef KERNELS_10_DECOMPILE_DELTAS_ADD_TO_POINTS_INCLUDE_KERNEL_H_
#define KERNELS_10_DECOMPILE_DELTAS_ADD_TO_POINTS_INCLUDE_KERNEL_H_

/*
 * <cstdlib> is included because the original translation unit included it
 * (hb.hh) before hb-algs.hh. On this sysroot it transitively defines
 * __BYTE_ORDER, which makes the verbatim HB_FAST_NUM_ACCESS block below
 * resolve exactly as in the original project build (verified: the original TU
 * resolves HB_FAST_NUM_ACCESS to 1).
 */
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

/*
 * src/hb.hh:253-259
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
 * src/hb-algs.hh:116-136
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
 * src/hb-algs.hh:138-181
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
 * src/hb-algs.hh:199-237 (the 3- and 8-byte specializations are unused
 * and omitted)
 */
template <bool BE, typename Type> struct HBInt<BE, Type, 4> {
    template <bool, typename, int> friend struct HBFloat;

  public:
    HBInt() = default;

    HBInt(Type V)
#if HB_FAST_NUM_ACCESS
    {
        if (BE == (__BYTE_ORDER == __BIG_ENDIAN))
            *((hb_packed_t<uint32_t> *)v) = V;
        else
            *((hb_packed_t<uint32_t> *)v) = __builtin_bswap32(V);
    }
#else
        : v{BE ? uint8_t((V >> 24) & 0xFF) : uint8_t((V) & 0xFF),
            BE ? uint8_t((V >> 16) & 0xFF) : uint8_t((V >> 8) & 0xFF),
            BE ? uint8_t((V >> 8) & 0xFF) : uint8_t((V >> 16) & 0xFF),
            BE ? uint8_t((V) & 0xFF) : uint8_t((V >> 24) & 0xFF)} {
    }
#endif

    constexpr operator Type() const {
#if HB_FAST_NUM_ACCESS
        return (BE == (__BYTE_ORDER == __BIG_ENDIAN))
                   ? (uint32_t)*((const hb_packed_t<uint32_t> *)v)
                   : __builtin_bswap32(
                         (uint32_t)*((const hb_packed_t<uint32_t> *)v));
#else
        return (BE ? (v[0] << 24) : (v[0])) +
               (BE ? (v[1] << 16) : (v[1] << 8)) +
               (BE ? (v[2] << 8) : (v[2] << 16)) + (BE ? (v[3]) : (v[3] << 24));
#endif
    }

  private:
    uint8_t v[4];
};

/*
 * src/hb-algs.hh:289-340
 */

template <bool BE, typename Type, int Bytes> struct HBFloat {
    using IntType =
        typename std::conditional<Bytes == 4, uint32_t, uint64_t>::type;

  public:
    HBFloat() = default;

    HBFloat(Type V) {
#if HB_FAST_NUM_ACCESS
        {
            if (BE == (__BYTE_ORDER == __BIG_ENDIAN)) {
                *((hb_packed_t<Type> *)v) = V;
                return;
            }
        }
#endif

        union {
            hb_packed_t<Type> f;
            hb_packed_t<IntType> i;
        } u = {{V}};

        const HBInt<BE, IntType> I = (IntType)u.i;
        for (unsigned i = 0; i < Bytes; i++)
            v[i] = I.v[i];
    }

    operator Type() const {
#if HB_FAST_NUM_ACCESS
        {
            if (BE == (__BYTE_ORDER == __BIG_ENDIAN))
                return (Type) * ((const hb_packed_t<Type> *)v);
        }
#endif

        HBInt<BE, IntType> I;
        for (unsigned i = 0; i < Bytes; i++)
            I.v[i] = v[i];

        union {
            hb_packed_t<IntType> i;
            hb_packed_t<Type> f;
        } u = {{I}};

        return (Type)u.f;
    }

  private:
    uint8_t v[Bytes];
};

/*
 * src/hb-algs.hh:871-884
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
 * src/hb-open-type.hh:56-113 (the unused member functions hash(), cmp()
 * and sanitize() are omitted)
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
    typename std::conditional<std::is_integral<Type>::value,
                              HBInt<BE, Type, Size>,
                              HBFloat<BE, Type, Size>>::type v;

  public:
    DEFINE_SIZE_STATIC(Size);
};

/*
 * src/hb-open-type.hh:115-120
 */
typedef NumType<true, uint8_t> HBUINT8;
typedef NumType<true, int8_t> HBINT8;
typedef NumType<true, uint16_t> HBUINT16;
typedef NumType<true, int16_t> HBINT16;
typedef NumType<true, uint32_t> HBUINT32;
typedef NumType<true, int32_t> HBINT32;

/*
 * src/hb-open-type.hh:1499-1509 (the encode_* helpers of TupleValues are
 * unused and omitted)
 */
struct TupleValues {
    enum packed_value_flag_t {
        VALUES_ARE_ZEROS = 0x80,
        VALUES_ARE_BYTES = 0x00,
        VALUES_ARE_WORDS = 0x40,
        VALUES_ARE_LONGS = 0xC0,
        VALUES_SIZE_MASK = 0xC0,
        VALUE_RUN_COUNT_MASK = 0x3F
    };
};

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
 * src/hb-array.hh:47-532 (only the data members and
 * constructors used by the extracted function are kept; member order
 * and types are the original hb_array_t members (arrayZ, length,
 * backwards_length).
 */
template <typename Type> struct hb_array_t {
    hb_array_t() = default;
    hb_array_t(const hb_array_t &) = default;
    ~hb_array_t() = default;
    hb_array_t &operator=(const hb_array_t &) = default;

    constexpr hb_array_t(Type *array_, unsigned int length_)
        : arrayZ(array_), length(length_) {}

  public:
    Type *arrayZ = nullptr;
    unsigned int length = 0;
    unsigned int backwards_length = 0;
};

/*
 * src/hb-common.h:165
 */
typedef uint32_t hb_tag_t;
/*
 * src/hb-common.h:177
 */
#define HB_TAG(c1, c2, c3, c4)                                                 \
    ((hb_tag_t)((((uint32_t)(c1) & 0xFF) << 24) |                              \
                (((uint32_t)(c2) & 0xFF) << 16) |                              \
                (((uint32_t)(c3) & 0xFF) << 8) | ((uint32_t)(c4) & 0xFF)))

/*
 * src/hb-ot-var-gvar-table.hh:39
 */
#define HB_OT_TAG_gvar HB_TAG('g', 'v', 'a', 'r')

/*
 * src/hb-ot-var-gvar-table.hh:307-309
 */
template <typename GidOffsetType, unsigned TableTag> struct gvar_GVAR {
    /*
     * src/hb-ot-var-gvar-table.hh:625
     */
    struct accelerator_t {
        /*
         * src/hb-ot-var-gvar-table.hh:677-688
         */
        template <bool is_x>
        static bool decompile_deltas_add_to_points(
            const HBUINT8 *&p, hb_array_t<contour_point_t> points, float scalar,
            const HBUINT8 *end, unsigned start);
    };
};

/*
 * Wrapper for invoking the extracted member twice (x and y).
 */
extern "C" bool
decompile_deltas_add_to_points(const HBUINT8 *&p,
                               hb_array_t<contour_point_t> points, float scalar,
                               const HBUINT8 *end, unsigned start);

#endif // KERNELS_10_DECOMPILE_DELTAS_ADD_TO_POINTS_INCLUDE_KERNEL_H_
