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
 *    src/hb-null.hh
 *    src/hb-vector.hh
 *    src/hb-ot-var-common.hh
 *    src/hb-common.h
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
 * src/hb-null.hh
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
 * src/hb-vector.hh
 *
 * Copyright © 2017,2018  Google, Inc.
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
 */

#ifndef KERNELS_11_DECOMPILE_POINTS_INCLUDE_KERNEL_H_
#define KERNELS_11_DECOMPILE_POINTS_INCLUDE_KERNEL_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>
#include <utility>

/*
 * src/hb.hh:253-267
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
 * src/hb.hh:289-302
 */
#ifndef HB_INTERNAL
#if !defined(HB_NO_VISIBILITY) && !defined(__MINGW32__) &&                     \
    !defined(__CYGWIN__) && !defined(_MSC_VER) && !defined(__SUNPRO_CC)
#define HB_INTERNAL __attribute__((__visibility__("hidden")))
#elif defined(__MINGW32__)
#define HB_INTERNAL
#elif defined(_MSC_VER) && defined(HB_DLL_EXPORT)
#define HB_INTERNAL
#else
#define HB_INTERNAL
#define HB_NO_VISIBILITY 1
#endif
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
 * src/hb-meta.hh:83
 */
#define HB_FUNCOBJ(x) static_const x HB_UNUSED

/*
 * src/hb-meta.hh:45-46
 */
template <typename... Ts> struct _hb_void_t {
    typedef void type;
};
template <typename... Ts> using hb_void_t = typename _hb_void_t<Ts...>::type;

/*
 * src/hb-meta.hh:48-49
 */
template <typename Head, typename... Ts> struct _hb_head_t {
    typedef Head type;
};
template <typename... Ts> using hb_head_t = typename _hb_head_t<Ts...>::type;

/*
 * src/hb-meta.hh:51-54
 */
template <typename T, T v> struct hb_integral_constant {
    static constexpr T value = v;
};
template <bool b> using hb_bool_constant = hb_integral_constant<bool, b>;
using hb_true_type = hb_bool_constant<true>;
using hb_false_type = hb_bool_constant<false>;

/*
 * src/hb-meta.hh:63-71
 */
template <bool B, typename T = void> struct hb_enable_if {};
template <typename T> struct hb_enable_if<true, T> {
    typedef T type;
};
#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type * = nullptr
#define hb_requires(Cond) hb_enable_if((Cond))

template <typename T, typename T2> struct hb_is_same : hb_false_type {};
template <typename T> struct hb_is_same<T, T> : hb_true_type {};
#define hb_is_same(T, T2) hb_is_same<T, T2>::value

/*
 * src/hb-meta.hh:117
 */
#define hb_is_convertible(From, To) std::is_convertible<From, To>::value

/*
 * src/hb-meta.hh:75-81
 */
#define HB_RETURN(Ret, E)                                                      \
    ->hb_head_t<Ret, decltype((E))> { return (E); }
#define HB_AUTO_RETURN(E)                                                      \
    ->decltype((E)) { return (E); }
#define HB_VOID_RETURN(E)                                                      \
    ->hb_void_t<decltype((E))> { (E); }

template <unsigned Pri> struct hb_priority : hb_priority<Pri - 1> {};
template <> struct hb_priority<0> {};
#define hb_prioritize hb_priority<16>()

/*
 * src/hb-meta.hh:202-214
 */
#if defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#define hb_is_trivially_copyable(T) __has_trivial_copy(T)
#define hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#define hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#define hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#define hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#else
#define hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#define hb_is_trivially_copy_assignable(T)                                     \
    std::is_trivially_copy_assignable<T>::value
#define hb_is_trivially_constructible(T)                                       \
    std::is_trivially_constructible<T>::value
#define hb_is_trivially_copy_constructible(T)                                  \
    std::is_trivially_copy_constructible<T>::value
#define hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#endif

/*
 * src/hb-null.hh:85-91
 */
template <typename T, typename>
struct _hb_static_size : hb_integral_constant<unsigned, sizeof(T)> {};
template <typename T>
struct _hb_static_size<T, hb_void_t<decltype(T::static_size)>>
    : hb_integral_constant<unsigned, T::static_size> {};
template <typename T> using hb_static_size = _hb_static_size<T, void>;
#define hb_static_size(T) hb_static_size<T>::value

/*
 * src/hb-algs.hh:1232-1238
 */
static inline void *hb_memcpy(void *__restrict dst, const void *__restrict src,
                              size_t len) {
    if (unlikely(!len))
        return dst;
    return memcpy(dst, src, len);
}

/*
 * src/hb-algs.hh:1251-1256
 */
static inline void *hb_memset(void *s, int c, unsigned int n) {
    if (unlikely(!n))
        return s;
    return memset(s, c, n);
}

/*
 * src/hb-algs.hh:1289-1302
 */
static inline bool hb_unsigned_mul_overflows(unsigned int count,
                                             unsigned int size,
                                             unsigned *result = nullptr) {
#if hb_has_builtin(__builtin_mul_overflow)
    unsigned stack_result;
    if (!result)
        result = &stack_result;
    return __builtin_mul_overflow(count, size, result);
#endif

    if (result)
        *result = count * size;
    return (size > 0) && (count >= ((unsigned int)-1) / size);
}

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
 * src/hb-algs.hh:199-237
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
 * src/hb-algs.hh:238-285
 */
template <bool BE, typename Type> struct HBInt<BE, Type, 8> {
    template <bool, typename, int> friend struct HBFloat;

  public:
    HBInt() = default;

    HBInt(Type V)
#if HB_FAST_NUM_ACCESS
    {
        if (BE == (__BYTE_ORDER == __BIG_ENDIAN))
            *((hb_packed_t<uint64_t> *)v) = V;
        else
            *((hb_packed_t<uint64_t> *)v) = __builtin_bswap64(V);
    }
#else
        : v{BE ? uint8_t((V >> 56) & 0xFF) : uint8_t((V) & 0xFF),
            BE ? uint8_t((V >> 48) & 0xFF) : uint8_t((V >> 8) & 0xFF),
            BE ? uint8_t((V >> 40) & 0xFF) : uint8_t((V >> 16) & 0xFF),
            BE ? uint8_t((V >> 32) & 0xFF) : uint8_t((V >> 24) & 0xFF),
            BE ? uint8_t((V >> 24) & 0xFF) : uint8_t((V >> 32) & 0xFF),
            BE ? uint8_t((V >> 16) & 0xFF) : uint8_t((V >> 40) & 0xFF),
            BE ? uint8_t((V >> 8) & 0xFF) : uint8_t((V >> 48) & 0xFF),
            BE ? uint8_t((V) & 0xFF) : uint8_t((V >> 56) & 0xFF)} {
    }
#endif

    constexpr operator Type() const {
#if HB_FAST_NUM_ACCESS
        return (BE == (__BYTE_ORDER == __BIG_ENDIAN))
                   ? (uint64_t)*((const hb_packed_t<uint64_t> *)v)
                   : __builtin_bswap64(
                         (uint64_t)*((const hb_packed_t<uint64_t> *)v));
#else
        return (BE ? (uint64_t(v[0]) << 56) : (uint64_t(v[0]))) +
               (BE ? (uint64_t(v[1]) << 48) : (uint64_t(v[1]) << 8)) +
               (BE ? (uint64_t(v[2]) << 40) : (uint64_t(v[2]) << 16)) +
               (BE ? (uint64_t(v[3]) << 32) : (uint64_t(v[3]) << 24)) +
               (BE ? (uint64_t(v[4]) << 24) : (uint64_t(v[4]) << 32)) +
               (BE ? (uint64_t(v[5]) << 16) : (uint64_t(v[5]) << 40)) +
               (BE ? (uint64_t(v[6]) << 8) : (uint64_t(v[6]) << 48)) +
               (BE ? (uint64_t(v[7])) : (uint64_t(v[7]) << 56));
#endif
    }

  private:
    uint8_t v[8];
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
 * src/hb-machinery.hh:92-94
 */
#ifndef HB_VAR_ARRAY
#define HB_VAR_ARRAY 1
#endif
/*
 * src/hb-machinery.hh:97-141
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

#define DEFINE_SIZE_UNION(size, _member)                                       \
    DEFINE_COMPILES_ASSERTION((void)this->u._member.static_size)               \
    DEFINE_INSTANCE_ASSERTION(sizeof(this->u._member) == (size))               \
    static constexpr unsigned null_size = (size);                              \
    static constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size)                                                  \
    DEFINE_INSTANCE_ASSERTION(sizeof(*this) >= (size))                         \
    static constexpr unsigned null_size = (size);                              \
    static constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size)                                            \
    DEFINE_INSTANCE_ASSERTION(sizeof(*this) >= (size))                         \
    static constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array)                                         \
    DEFINE_COMPILES_ASSERTION((void)(array)[0].static_size)                    \
    DEFINE_INSTANCE_ASSERTION(                                                 \
        sizeof(*this) == (size) + (HB_VAR_ARRAY + 0) * sizeof((array)[0]))     \
    static constexpr unsigned null_size = (size);                              \
    static constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array)                                   \
    size_t get_size() const {                                                  \
        return (size - (array).min_size + (array).get_size());                 \
    }                                                                          \
    DEFINE_SIZE_ARRAY(size, array)

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

    HB_INTERNAL static int cmp(const NumType *a, const NumType *b) {
        return b->cmp(*a);
    }
    HB_INTERNAL static int cmp(const void *a, const void *b) {
        NumType *pa = (NumType *)a;
        NumType *pb = (NumType *)b;

        return pb->cmp(*pa);
    }
    template <typename Type2, hb_enable_if(hb_is_convertible(Type2, Type))>
    int cmp(Type2 a) const {
        Type b = v;
        return (a > b) - (a < b);
    }

  protected:
    typename std::conditional<std::is_integral<Type>::value,
                              HBInt<BE, Type, Size>,
                              HBFloat<BE, Type, Size>>::type v;

  public:
    DEFINE_SIZE_STATIC(Size);
};

/*
 * src/hb-open-type.hh:115-122
 */
typedef NumType<true, uint8_t> HBUINT8;
typedef NumType<true, int8_t> HBINT8;
typedef NumType<true, uint16_t> HBUINT16;
typedef NumType<true, int16_t> HBINT16;
typedef NumType<true, uint32_t> HBUINT32;
typedef NumType<true, int32_t> HBINT32;
typedef NumType<true, uint64_t> HBUINT64;
typedef NumType<true, int64_t> HBINT64;

/*
 * src/hb.hh:533-553
 */
#if !defined(HB_CUSTOM_MALLOC) && defined(hb_malloc_impl) &&                   \
    defined(hb_calloc_impl) && defined(hb_realloc_impl) &&                     \
    defined(hb_free_impl)
#define HB_CUSTOM_MALLOC
#endif

#ifdef HB_CUSTOM_MALLOC
extern "C" void *hb_malloc_impl(size_t size);
extern "C" void *hb_calloc_impl(size_t nmemb, size_t size);
extern "C" void *hb_realloc_impl(void *ptr, size_t size);
extern "C" void hb_free_impl(void *ptr);
#else
#define hb_malloc_impl malloc
#define hb_calloc_impl calloc
#define hb_realloc_impl realloc
#define hb_free_impl free
#endif

/*
 * src/hb-common.h:36-38
 */
#ifndef HB_EXTERN
#define HB_EXTERN extern
#endif

/*
 * src/hb-common.h:523-530
 */
HB_EXTERN void *hb_malloc(size_t size);
HB_EXTERN void *hb_calloc(size_t nmemb, size_t size);
HB_EXTERN void *hb_realloc(void *ptr, size_t size);
HB_EXTERN void hb_free(void *ptr);

/*
 * src/hb-vector.hh:35-40
 */
#if 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#else
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

/*
 * src/hb-vector.hh:42-49
 */
template <typename Type, bool sorted = false> struct hb_vector_t {
    static constexpr bool realloc_move = true;

    typedef Type item_t;
    static constexpr unsigned item_size = hb_static_size(Type);

    /*
     * src/hb-vector.hh:53
     */
    hb_vector_t() = default;
    /*
     * src/hb-vector.hh:95
     */
    ~hb_vector_t() { fini(); }

    /*
     * src/hb-vector.hh:155-159
     */
  public:
    int allocated = 0;
    unsigned int length = 0;

  public:
    Type *arrayZ = nullptr;

    /*
     * src/hb-vector.hh:161-178
     */
    void init() {
        allocated = length = 0;
        arrayZ = nullptr;
    }
    void init0() {}

    void fini() {
        if (is_owned()) {
            shrink_vector(0);
            hb_free(arrayZ);
        }
        init();
    }

    /*
     * src/hb-vector.hh:342-362
     */
    bool is_owned() const { return allocated != 0 && allocated != -1; }

    bool in_error() const { return allocated < 0; }
    void set_error() {
        assert(allocated >= 0);
        allocated = -allocated - 1;
    }
    void reset_error() {
        assert(allocated < 0);
        allocated = -(allocated + 1);
    }
    void ensure_error() {
        if (!in_error())
            set_error();
    }

    /*
     * src/hb-vector.hh:364-407
     */
    Type *_realloc(unsigned new_allocated) {
        if (!new_allocated) {
            if (is_owned())
                hb_free(arrayZ);
            return nullptr;
        }
        if (!allocated && arrayZ) {
            Type *new_array = (Type *)hb_malloc(new_allocated * sizeof(Type));
            if (unlikely(!new_array))
                return nullptr;
            hb_memcpy((void *)new_array, (const void *)arrayZ,
                      length * sizeof(Type));
            return new_array;
        }
        return (Type *)hb_realloc(arrayZ, new_allocated * sizeof(Type));
    }
    Type *_malloc_move(unsigned new_allocated) {
        if (!new_allocated) {
            if (is_owned())
                hb_free(arrayZ);
            return nullptr;
        }
        Type *new_array = (Type *)hb_malloc(new_allocated * sizeof(Type));
        if (likely(new_array)) {
            for (unsigned i = 0; i < length; i++) {
                new (std::addressof(new_array[i])) Type();
                new_array[i] = std::move(arrayZ[i]);
                arrayZ[i].~Type();
            }
            if (is_owned())
                hb_free(arrayZ);
        }
        return new_array;
    }

    /*
     * src/hb-vector.hh:409-430
     */
    template <typename T = Type,
              hb_enable_if(hb_is_trivially_copy_assignable(T))>
    Type *realloc_vector(unsigned new_allocated, hb_priority<0>) {
        return _realloc(new_allocated);
    }
    template <typename T = Type,
              hb_enable_if(!hb_is_trivially_copy_assignable(T))>
    Type *realloc_vector(unsigned new_allocated, hb_priority<0>) {
        return _malloc_move(new_allocated);
    }
    template <typename T = Type, hb_enable_if(T::realloc_move)>
    Type *realloc_vector(unsigned new_allocated, hb_priority<1>) {
        return _realloc(new_allocated);
    }

    /*
     * src/hb-vector.hh:432-447
     */
    template <typename T = Type, hb_enable_if(hb_is_trivially_constructible(T))>
    void grow_vector(unsigned size, hb_priority<0>) {
        hb_memset(arrayZ + length, 0, (size - length) * sizeof(*arrayZ));
        length = size;
    }
    template <typename T = Type,
              hb_enable_if(!hb_is_trivially_constructible(T))>
    void grow_vector(unsigned size, hb_priority<0>) {
        for (; length < size; length++)
            new (std::addressof(arrayZ[length])) Type();
    }

    /*
     * src/hb-vector.hh:501-513
     */
    void shrink_vector(unsigned size) {
        assert(size <= length);
        if (!hb_is_trivially_destructible(Type)) {
            unsigned count = length - size;
            Type *p = arrayZ + length;
            while (count--)
                (--p)->~Type();
        }
        length = size;
    }

    /*
     * src/hb-vector.hh:522-583
     */
    HB_ALWAYS_INLINE_VECTOR_ALLOCS
    bool alloc(unsigned int size, bool exact = false) {
        if (unlikely(in_error()))
            return false;

        unsigned int new_allocated;
        if (exact) {
            size = hb_max(size, length);
            if (size <= (unsigned)allocated && size >= (unsigned)allocated >> 2)
                return true;

            new_allocated = size;
        } else {
            if (likely(size <= (unsigned)allocated))
                return true;

            new_allocated = allocated;
            while (size > new_allocated)
                new_allocated += (new_allocated >> 1) + 8;
        }

        bool overflows = (int)in_error() || (new_allocated < size) ||
                         hb_unsigned_mul_overflows(new_allocated, sizeof(Type));

        if (unlikely(overflows)) {
            set_error();
            return false;
        }

        Type *new_array = realloc_vector(new_allocated, hb_prioritize);

        if (unlikely(new_allocated && !new_array)) {
            if (new_allocated <= (unsigned)allocated)
                return true;

            set_error();
            return false;
        }

        arrayZ = new_array;
        allocated = new_allocated;

        return true;
    }
    HB_ALWAYS_INLINE_VECTOR_ALLOCS
    bool alloc_exact(unsigned int size) { return alloc(size, true); }

    /*
     * src/hb-vector.hh:635-667
     */
    HB_ALWAYS_INLINE_VECTOR_ALLOCS
    bool resize_full(int size_, bool initialize, bool exact) {
        if (unlikely(size_ < 0))
            return false;
        unsigned int size = (unsigned int)size_;
        if (!alloc(size, exact))
            return false;

        if (size > length) {
            if (initialize)
                grow_vector(size, hb_prioritize);
        } else if (size < length) {
            if (initialize)
                shrink_vector(size);
        }

        length = size;
        return true;
    }
    HB_ALWAYS_INLINE_VECTOR_ALLOCS
    bool resize(int size_) { return resize_full(size_, true, false); }
    HB_ALWAYS_INLINE_VECTOR_ALLOCS
    bool resize_dirty(int size_) { return resize_full(size_, false, false); }
};

/*
 * src/hb-ot-var-common.hh:939-940
 */
template <typename OffType = HBUINT16> struct TupleVariationData {
    /*
     * src/hb-ot-var-common.hh:1644-1646
     */
    static bool decompile_points(const HBUINT8 *&p,
                                 hb_vector_t<unsigned int> &points,
                                 const HBUINT8 *end);
};

/*
 * Wrapper for invoking the extracted member.
 */
extern "C" bool decompile_points(const HBUINT8 *&p,
                                 hb_vector_t<unsigned int> &points,
                                 const HBUINT8 *end);

#endif // KERNELS_11_DECOMPILE_POINTS_INCLUDE_KERNEL_H_
