/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb.hh
 *    src/hb-meta.hh
 *    src/hb-algs.hh
 *    src/hb-array.hh
 *    src/hb-subset-plan.hh
 *    src/hb-vector.hh
 *
 *
 *  The original file copyright and license notices follow.
 *
 *  src/hb.hh
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
 *  src/hb-meta.hh
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
 *  src/hb-algs.hh
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
 *  src/hb-array.hh
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
 *  src/hb-subset-plan.hh
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
 *  src/hb-vector.hh
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
 */

#ifndef KERNELS_46_HB_IUP_SEGMENT_INCLUDE_KERNEL_H_
#define KERNELS_46_HB_IUP_SEGMENT_INCLUDE_KERNEL_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <type_traits>
#include <utility>

/* src/hb.hh:258-265 */
#define likely(expr) __builtin_expect(bool(expr), 1)
#define unlikely(expr) __builtin_expect(bool(expr), 0)
/* src/hb.hh:321-329 */
#define HB_ALWAYS_INLINE __attribute__((always_inline)) inline

/* src/hb-meta.hh:76-83, 63-67 */
#define HB_AUTO_RETURN(E) -> decltype((E)) { return (E); }
template <bool B, typename T = void> struct hb_enable_if {};
template <typename T> struct hb_enable_if<true, T> { typedef T type; };
#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type * = nullptr

/* src/hb-meta.hh:116-119 */
template <unsigned Pri> struct hb_priority : hb_priority<Pri - 1> {};
template <> struct hb_priority<0> {};
#define hb_prioritize hb_priority<16>()

/* src/hb-algs.hh:786-795, 867-884 */
struct {
    template <typename T> void operator()(T &a, T &b) const {
        using std::swap;
        swap(a, b);
    }
} static const hb_swap __attribute__((unused));
struct {
    template <typename T, typename T2>
    constexpr auto operator()(T &&a, T2 &&b) const
        HB_AUTO_RETURN(a >= b ? a : b)
} static const hb_max __attribute__((unused));

/* src/hb-algs.hh:1232-1238, 1250-1256, 1289-1302 */
static inline void *hb_memcpy(void *__restrict dst, const void *__restrict src,
                              size_t len) {
    if (unlikely(!len)) return dst;
    return memcpy(dst, src, len);
}
static inline void *hb_memset(void *s, int c, unsigned int n) {
    if (unlikely(!n)) return s;
    return memset(s, c, n);
}
static inline bool hb_unsigned_mul_overflows(unsigned int count,
                                             unsigned int size,
                                             unsigned *result = nullptr) {
    unsigned stack_result;
    if (!result) result = &stack_result;
    return __builtin_mul_overflow(count, size, result);
}

/* src/hb-array.hh:47-61, 359-361; unused methods and iterator base omitted */
template <typename Type> struct hb_array_t {
    hb_array_t() = default;
    constexpr hb_array_t(Type *array_, unsigned int length_)
        : arrayZ(array_), length(length_) {}
    Type *arrayZ = nullptr;
    unsigned int length = 0;
    unsigned int backwards_length = 0;
};

/* src/hb-subset-plan.hh:77-102 */
struct contour_point_t {
    void init(float x_ = 0.f, float y_ = 0.f, bool is_end_point_ = false) {
        flag = 0; x = x_; y = y_; is_end_point = is_end_point_;
    }
    void transform(const float (&matrix)[4]) {
        float x_ = x * matrix[0] + y * matrix[2];
        y = x * matrix[1] + y * matrix[3];
        x = x_;
    }
    void add_delta(float delta_x, float delta_y) { x += delta_x; y += delta_y; }
    HB_ALWAYS_INLINE void translate(const contour_point_t &p) {
        x += p.x; y += p.y;
    }
    float x;
    float y;
    uint8_t flag;
    bool is_end_point;
};

/* src/hb-common.h:522-530; selected default allocator */
extern "C" {
void *hb_malloc(size_t size);
void *hb_realloc(void *ptr, size_t size);
void hb_free(void *ptr);
}

/* src/hb-vector.hh:35-49, 95, 155-178, 342-384, 409-415, 432-447,
 * 501-513, 522-578, 635-667. Only double-instantiated methods retained.
 * Data layout, allocation and resize paths are retained. */
template <typename Type, bool sorted = false> struct hb_vector_t {
    static constexpr bool realloc_move = true;
    typedef Type item_t;
    static constexpr unsigned item_size = sizeof(Type);
    hb_vector_t() = default;
    hb_vector_t(const hb_vector_t &) = delete;
    hb_vector_t &operator=(const hb_vector_t &) = delete;
    ~hb_vector_t() { fini(); }

    int allocated = 0;
    unsigned int length = 0;
    Type *arrayZ = nullptr;

    void init() { allocated = length = 0; arrayZ = nullptr; }
    void init0() {}
    void fini() {
        if (is_owned()) { shrink_vector(0); hb_free(arrayZ); }
        init();
    }
    bool is_owned() const { return allocated != 0 && allocated != -1; }
    bool in_error() const { return allocated < 0; }
    void set_error() { assert(allocated >= 0); allocated = -allocated - 1; }
    void reset_error() { assert(allocated < 0); allocated = -(allocated + 1); }
    void ensure_error() { if (!in_error()) set_error(); }
    Type *_realloc(unsigned new_allocated) {
        if (!new_allocated) {
            if (is_owned()) hb_free(arrayZ);
            return nullptr;
        }
        if (!allocated && arrayZ) {
            Type *new_array = (Type *)hb_malloc(new_allocated * sizeof(Type));
            if (unlikely(!new_array)) return nullptr;
            hb_memcpy((void *)new_array, (const void *)arrayZ,
                      length * sizeof(Type));
            return new_array;
        }
        return (Type *)hb_realloc(arrayZ, new_allocated * sizeof(Type));
    }
    template <typename T = Type, hb_enable_if(std::is_trivially_copy_assignable<T>::value)>
    Type *realloc_vector(unsigned new_allocated, hb_priority<0>) {
        return _realloc(new_allocated);
    }
    template <typename T = Type, hb_enable_if(std::is_trivially_constructible<T>::value)>
    void grow_vector(unsigned size, hb_priority<0>) {
        hb_memset(arrayZ + length, 0, (size - length) * sizeof(*arrayZ));
        length = size;
    }
    void shrink_vector(unsigned size) {
        assert(size <= length);
        if (!std::is_trivially_destructible<Type>::value) {
            unsigned count = length - size;
            Type *p = arrayZ + length;
            while (count--) (--p)->~Type();
        }
        length = size;
    }
    bool alloc(unsigned int size, bool exact = false) {
        if (unlikely(in_error())) return false;
        unsigned int new_allocated;
        if (exact) {
            size = hb_max(size, length);
            if (size <= (unsigned)allocated &&
                size >= (unsigned)allocated >> 2) return true;
            new_allocated = size;
        } else {
            if (likely(size <= (unsigned)allocated)) return true;
            new_allocated = allocated;
            while (size > new_allocated)
                new_allocated += (new_allocated >> 1) + 8;
        }
        bool overflows = (int)in_error() || (new_allocated < size) ||
                         hb_unsigned_mul_overflows(new_allocated, sizeof(Type));
        if (unlikely(overflows)) { set_error(); return false; }
        Type *new_array = realloc_vector(new_allocated, hb_prioritize);
        if (unlikely(new_allocated && !new_array)) {
            if (new_allocated <= (unsigned)allocated) return true;
            set_error(); return false;
        }
        arrayZ = new_array;
        allocated = new_allocated;
        return true;
    }
    bool resize_full(int size_, bool initialize, bool exact) {
        if (unlikely(size_ < 0)) return false;
        unsigned int size = (unsigned int)size_;
        if (!alloc(size, exact)) return false;
        if (size > length) {
            if (initialize) grow_vector(size, hb_prioritize);
        } else if (size < length) {
            if (initialize) shrink_vector(size);
        }
        length = size;
        return true;
    }
    bool resize_dirty(int size_) { return resize_full(size_, false, false); }
};

/* Wrapper for invoking src/hb-subset-instancer-iup.cc:181-263. */
extern "C" bool hb_012_iup_segment_interpolate_isolated(
    const hb_array_t<const contour_point_t> contour_points,
    const hb_array_t<const int> x_deltas,
    const hb_array_t<const int> y_deltas,
    const contour_point_t &p1, const contour_point_t &p2,
    int p1_dx, int p2_dx, int p1_dy, int p2_dy, double tolerance_sq,
    hb_vector_t<double> &interp_x_deltas,
    hb_vector_t<double> &interp_y_deltas);
extern "C" bool hb_012_iup_segment_interpolate_rvv(
    const hb_array_t<const contour_point_t> contour_points,
    const hb_array_t<const int> x_deltas,
    const hb_array_t<const int> y_deltas,
    const contour_point_t &p1, const contour_point_t &p2,
    int p1_dx, int p2_dx, int p1_dy, int p2_dy, double tolerance_sq,
    hb_vector_t<double> &interp_x_deltas,
    hb_vector_t<double> &interp_y_deltas);

#endif // KERNELS_46_HB_IUP_SEGMENT_INCLUDE_KERNEL_H_
