/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb.hh
 *    src/hb-meta.hh
 *    src/hb-algs.hh
 *    src/hb-common.h
 *    src/hb-script-list.h
 *    src/hb-buffer.h
 *    src/hb-atomic.hh
 *    src/hb-object.hh
 *    src/hb-set-digest.hh
 *    src/hb-buffer.hh
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
 *
 * src/hb-buffer.h
 *
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2004,2007,2009  Red Hat, Inc.
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
 * Red Hat Author(s): Owen Taylor, Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 *
 *
 * src/hb-buffer.hh
 *
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2004,2007,2009,2010  Red Hat, Inc.
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
 * Red Hat Author(s): Owen Taylor, Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 *
 * src/hb-script-list.h
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
 * src/hb-atomic.hh
 *
 * Copyright © 2007  Chris Wilson
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
 * Contributor(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 *
 *
 * src/hb-object.hh
 *
 * Copyright © 2007  Chris Wilson
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
 * Contributor(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 *
 *
 * src/hb-set-digest.hh
 *
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
 * Google Author(s): Behdad Esfahbod
 *
 *
 */

#ifndef KERNELS_62_NORMALIZE_GLYPHS_CLUSTER_INCLUDE_KERNEL_H_
#define KERNELS_62_NORMALIZE_GLYPHS_CLUSTER_INCLUDE_KERNEL_H_

#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

/*
 * src/hb.hh:261-267
 */
#define likely(expr) __builtin_expect(bool(expr), 1)
#define unlikely(expr) __builtin_expect(bool(expr), 0)

/*
 * src/hb.hh:324-330
 */
#define HB_ALWAYS_INLINE __attribute__((always_inline)) inline

/*
 * src/hb-meta.hh:51
 */
template <typename T, T v> struct hb_integral_constant {
    static constexpr T value = v;
};

/*
 * src/hb-meta.hh:174-201
 */
template <typename T> struct hb_int_min;
template <>
struct hb_int_min<signed int> : hb_integral_constant<signed int, INT_MIN> {};
#define hb_int_min(T) hb_int_min<T>::value
template <typename T> struct hb_int_max;
template <>
struct hb_int_max<signed int> : hb_integral_constant<signed int, INT_MAX> {};
#define hb_int_max(T) hb_int_max<T>::value

/*
 * src/hb-meta.hh:202-214
 */
#define hb_is_trivially_copy_assignable(T)                                     \
    std::is_trivially_copy_assignable<T>::value

/*
 * src/hb-algs.hh:894-938
 */
template <typename T> static HB_ALWAYS_INLINE T hb_saturate_add(T a, T b) {
    static_assert(std::is_integral<T>::value && std::is_signed<T>::value, "");

    T result;
    if (likely(!__builtin_add_overflow(a, b, &result)))
        return result;

    return a < 0 ? hb_int_min(T) : hb_int_max(T);
}

template <typename T> static HB_ALWAYS_INLINE T hb_saturate_sub(T a, T b) {
    static_assert(std::is_integral<T>::value && std::is_signed<T>::value, "");

    T result;
    if (likely(!__builtin_sub_overflow(a, b, &result)))
        return result;

    return b < 0 ? hb_int_max(T) : hb_int_min(T);
}

/*
 * src/hb-algs.hh:1556-1582
 */
template <typename T, typename T2, typename T3 = int>
static inline void hb_stable_sort(T *array, unsigned int len,
                                  int (*compar)(const T2 *, const T2 *),
                                  T3 *array2 = nullptr) {
    static_assert(hb_is_trivially_copy_assignable(T), "");
    static_assert(hb_is_trivially_copy_assignable(T3), "");

    for (unsigned int i = 1; i < len; i++) {
        unsigned int j = i;
        while (j && compar(&array[j - 1], &array[i]) > 0)
            j--;
        if (i == j)
            continue;
        {
            T t = array[i];
            memmove(&array[j + 1], &array[j], (i - j) * sizeof(T));
            array[j] = t;
        }
        if (array2) {
            T3 t = array2[i];
            memmove(&array2[j + 1], &array2[j], (i - j) * sizeof(T3));
            array2[j] = t;
        }
    }
}

/*
 * src/hb-common.h:104
 */
typedef uint32_t hb_codepoint_t;

/*
 * src/hb-common.h:123
 */
typedef int32_t hb_position_t;

/*
 * src/hb-common.h:130
 */
typedef uint32_t hb_mask_t;

/*
 * src/hb-common.h:132-139
 */
typedef union _hb_var_int_t {
    uint32_t u32;
    int32_t i32;
    uint16_t u16[2];
    int16_t i16[2];
    uint8_t u8[4];
    int8_t i8[4];
} hb_var_int_t;

/*
 * src/hb-buffer.h:62-72
 */
typedef struct hb_glyph_info_t {
    hb_codepoint_t codepoint;
    hb_mask_t mask;
    uint32_t cluster;

    hb_var_int_t var1;
    hb_var_int_t var2;
} hb_glyph_info_t;

/*
 * src/hb-buffer.h:191-199
 */
typedef struct hb_glyph_position_t {
    hb_position_t x_advance;
    hb_position_t y_advance;
    hb_position_t x_offset;
    hb_position_t y_offset;

    hb_var_int_t var;
} hb_glyph_position_t;

/* src/hb-common.h:95, 152-165, 237-243, 317, 378;
 * src/hb-script-list.h:53-493. The script enum is only stored, never inspected:
 * its fixed-width declaration has the same signed-int representation as the
 * upstream enum (whose maximum enumerator is 0x7fffffff).
 */
typedef int hb_bool_t;
typedef uint32_t hb_tag_t;
typedef enum {
    HB_DIRECTION_INVALID = 0,
    HB_DIRECTION_LTR = 4,
    HB_DIRECTION_RTL,
    HB_DIRECTION_TTB,
    HB_DIRECTION_BTT
} hb_direction_t;
enum hb_script_t : int;
typedef const struct hb_language_impl_t *hb_language_t;
typedef void (*hb_destroy_func_t)(void *user_data);

/* src/hb-buffer.h:211-218, 291-295, 396-408, 461-467, 863-866. */
typedef struct hb_segment_properties_t {
    hb_direction_t direction;
    hb_script_t script;
    hb_language_t language;
    void *reserved1;
    void *reserved2;
} hb_segment_properties_t;
typedef enum {
    HB_BUFFER_CONTENT_TYPE_INVALID = 0,
    HB_BUFFER_CONTENT_TYPE_UNICODE,
    HB_BUFFER_CONTENT_TYPE_GLYPHS
} hb_buffer_content_type_t;
typedef enum {
    HB_BUFFER_FLAG_DEFAULT = 0x00000000u,
    HB_BUFFER_FLAG_BOT = 0x00000001u,
    HB_BUFFER_FLAG_EOT = 0x00000002u,
    HB_BUFFER_FLAG_PRESERVE_DEFAULT_IGNORABLES = 0x00000004u,
    HB_BUFFER_FLAG_REMOVE_DEFAULT_IGNORABLES = 0x00000008u,
    HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE = 0x00000010u,
    HB_BUFFER_FLAG_VERIFY = 0x00000020u,
    HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT = 0x00000040u,
    HB_BUFFER_FLAG_PRODUCE_SAFE_TO_INSERT_TATWEEL = 0x00000080u,
    HB_BUFFER_FLAG_DEFINED = 0x000000FFu
} hb_buffer_flags_t;
typedef enum {
    HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES = 0,
    HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS = 1,
    HB_BUFFER_CLUSTER_LEVEL_CHARACTERS = 2,
    HB_BUFFER_CLUSTER_LEVEL_GRAPHEMES = 3,
    HB_BUFFER_CLUSTER_LEVEL_DEFAULT = HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES
} hb_buffer_cluster_level_t;

/* src/hb-atomic.hh:45-93 (native atomic implementation).
 */
#define hb_atomic_int_impl_add(AI, V)                                          \
    __atomic_fetch_add((AI), (V), __ATOMIC_ACQ_REL)
#define hb_atomic_int_impl_set_relaxed(AI, V)                                  \
    __atomic_store_n((AI), (V), __ATOMIC_RELAXED)
#define hb_atomic_int_impl_set(AI, V)                                          \
    __atomic_store_n((AI), (V), __ATOMIC_RELEASE)
#define hb_atomic_int_impl_get_relaxed(AI)                                     \
    __atomic_load_n((AI), __ATOMIC_RELAXED)
#define hb_atomic_int_impl_get(AI) __atomic_load_n((AI), __ATOMIC_ACQUIRE)
#define hb_atomic_ptr_impl_set_relaxed(P, V)                                   \
    __atomic_store_n((P), (V), __ATOMIC_RELAXED)
#define hb_atomic_ptr_impl_get_relaxed(P) __atomic_load_n((P), __ATOMIC_RELAXED)
#define hb_atomic_ptr_impl_get(P) __atomic_load_n((P), __ATOMIC_ACQUIRE)
/*
 * src/hb-atomic.hh:145-215 (native atomic representation)
 */
template <typename T> struct hb_atomic_t {
    hb_atomic_t() = default;
    constexpr hb_atomic_t(T v) : v(v) {}
    hb_atomic_t &operator=(T v_) {
        set_relaxed(v_);
        return *this;
    }
    operator T() const { return get_relaxed(); }
    void set_relaxed(T v_) { hb_atomic_int_impl_set_relaxed(&v, v_); }
    void set_release(T v_) { hb_atomic_int_impl_set(&v, v_); }
    T get_relaxed() const { return hb_atomic_int_impl_get_relaxed(&v); }
    T get_acquire() const { return hb_atomic_int_impl_get(&v); }
    T inc() { return hb_atomic_int_impl_add(&v, 1); }
    T dec() { return hb_atomic_int_impl_add(&v, -1); }
    int operator++(int) { return inc(); }
    int operator--(int) { return dec(); }
    T v = 0;
};
template <typename T> struct hb_atomic_t<T *> {
    hb_atomic_t() = default;
    constexpr hb_atomic_t(T *v) : v(v) {}
    hb_atomic_t(const hb_atomic_t &other) = delete;
    void init(T *v_ = nullptr) { set_relaxed(v_); }
    void set_relaxed(T *v_) { hb_atomic_ptr_impl_set_relaxed(&v, v_); }
    T *get_relaxed() const { return (T *)hb_atomic_ptr_impl_get_relaxed(&v); }
    T *get_acquire() const { return (T *)hb_atomic_ptr_impl_get((void **)&v); }
    operator bool() const { return get_acquire() != nullptr; }
    T *operator->() const { return get_acquire(); }
    template <typename C> operator C *() const { return get_acquire(); }
    T *v = nullptr;
};

/* src/hb-object.hh:143-155, 213-220: only the header's stored members and
 * reference count operations are needed for this independent buffer value. */
struct hb_user_data_array_t;
struct hb_reference_count_t {
    mutable hb_atomic_t<int> ref_count;
    void init(int v = 1) { ref_count = v; }
    int get_relaxed() const { return ref_count; }
    int inc() const { return ref_count.inc(); }
    int dec() const { return ref_count.dec(); }
    void fini() { ref_count = -0x0000DEAD; }
    bool is_inert() const { return !ref_count; }
    bool is_valid() const { return ref_count > 0; }
};
struct hb_object_header_t {
    hb_reference_count_t ref_count;
    mutable hb_atomic_t<bool> writable = false;
    hb_atomic_t<hb_user_data_array_t *> user_data;
    bool is_inert() const { return !ref_count.get_relaxed(); }
};

/* src/hb-set-digest.hh:66-77, 169-172. The digest's stored masks are
 * uint64_t[3], initialized as in upstream; its methods are not called here. */
static constexpr unsigned hb_set_digest_shifts[] = {4, 0, 6};
struct hb_set_digest_t {
    using mask_t = uint64_t;
    static constexpr unsigned n =
        sizeof(hb_set_digest_shifts) / sizeof(hb_set_digest_shifts[0]);

  private:
    mask_t masks[n] = {};
};

/* src/hb-buffer.hh:46-63, 70-140: complete upstream data-member layout.
 * The rest of hb_buffer_t consists of methods unused by this region.
 */
struct hb_unicode_funcs_t;
struct hb_font_t;
struct hb_buffer_t;
typedef hb_bool_t (*hb_buffer_message_func_t)(hb_buffer_t *, hb_font_t *,
                                              const char *, void *);
enum hb_buffer_scratch_flags_t {
    HB_BUFFER_SCRATCH_FLAG_DEFAULT = 0x00000000u,
    HB_BUFFER_SCRATCH_FLAG_HAS_FRACTION_SLASH = 0x00000001u,
    HB_BUFFER_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES = 0x00000002u,
    HB_BUFFER_SCRATCH_FLAG_HAS_SPACE_FALLBACK = 0x00000004u,
    HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT = 0x00000008u,
    HB_BUFFER_SCRATCH_FLAG_HAS_CGJ = 0x00000010u,
    HB_BUFFER_SCRATCH_FLAG_HAS_BROKEN_SYLLABLE = 0x00000020u,
    HB_BUFFER_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK = 0x00000040u,
    HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS = 0x00000080u,
    HB_BUFFER_SCRATCH_FLAG_SHAPER0 = 0x01000000u,
    HB_BUFFER_SCRATCH_FLAG_SHAPER1 = 0x02000000u,
    HB_BUFFER_SCRATCH_FLAG_SHAPER2 = 0x04000000u,
    HB_BUFFER_SCRATCH_FLAG_SHAPER3 = 0x08000000u,
};
struct hb_buffer_t {
    hb_object_header_t header;
    hb_unicode_funcs_t *unicode;
    hb_buffer_flags_t flags;
    hb_buffer_cluster_level_t cluster_level;
    hb_codepoint_t replacement;
    hb_codepoint_t invisible;
    hb_codepoint_t not_found;
    hb_codepoint_t not_found_variation_selector;
    hb_buffer_content_type_t content_type;
    hb_segment_properties_t props;
    bool successful;
    bool have_output;
    bool have_positions;
    unsigned int idx;
    unsigned int len;
    unsigned int out_len;
    unsigned int allocated;
    hb_glyph_info_t *info;
    hb_glyph_info_t *out_info;
    hb_glyph_position_t *pos;
    static constexpr unsigned CONTEXT_LENGTH = 5u;
    hb_codepoint_t context[2][CONTEXT_LENGTH];
    unsigned int context_len[2];
    hb_set_digest_t digest;
    uint8_t allocated_var_bits;
    uint8_t serial;
    uint32_t random_state;
    hb_buffer_scratch_flags_t scratch_flags;
    unsigned int max_len;
    int max_ops;
    typedef void (*changed_func_t)(hb_buffer_t *buffer, void *user_data);
    hb_buffer_message_func_t message_func;
    void *message_data;
    hb_destroy_func_t message_destroy;
    changed_func_t changed_func;
    void *changed_data;
    unsigned message_depth;
};

static_assert(sizeof(hb_glyph_info_t) == 20 &&
                  sizeof(hb_glyph_position_t) == 20,
              "HarfBuzz glyph layout");
static_assert(sizeof(hb_object_header_t) == 16 &&
                  sizeof(hb_segment_properties_t) == 32 &&
                  sizeof(hb_set_digest_t) == 24,
              "HarfBuzz dependent layouts");
static_assert(offsetof(hb_buffer_t, info) == 112 &&
                  offsetof(hb_buffer_t, pos) == 128 &&
                  offsetof(hb_buffer_t, digest) == 184,
              "HarfBuzz rv64 buffer layout");
static_assert(sizeof(hb_buffer_t) == 280, "HarfBuzz rv64 buffer size");

/*
 * Local benchmark adapter (glue, not upstream code): packs the two arrays
 * into a complete hb_buffer_t and forwards to the copied static upstream
 * function in src/kernel.cpp.
 */
extern "C" void normalize_glyphs_cluster_isolated(hb_glyph_info_t *info,
                                         hb_glyph_position_t *pos,
                                         unsigned int start, unsigned int end,
                                         bool backward);

#endif // KERNELS_62_NORMALIZE_GLYPHS_CLUSTER_INCLUDE_KERNEL_H_
