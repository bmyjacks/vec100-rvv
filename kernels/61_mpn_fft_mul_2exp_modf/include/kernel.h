/****************************************************************************
 *
 *
 *  Project: GNU MP (GMP) 6.3.0
 *  Source files:
 *    gmp-impl.h
 *    gmp-h.in
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * gmp-impl.h
 *
 * Include file for internal GNU MP types and definitions.
 *
 *    THE CONTENTS OF THIS FILE ARE FOR INTERNAL USE AND ARE ALMOST CERTAIN TO
 *    BE SUBJECT TO INCOMPATIBLE CHANGES IN FUTURE GNU MP RELEASES.
 *
 * Copyright 1991-2018, 2021, 2022 Free Software Foundation, Inc.
 *
 * This file is part of the GNU MP Library.
 *
 * The GNU MP Library is free software; you can redistribute it and/or modify
 * it under the terms of either:
 *
 *   * the GNU Lesser General Public License as published by the Free
 *     Software Foundation; either version 3 of the License, or (at your
 *     option) any later version.
 *
 * or
 *
 *   * the GNU General Public License as published by the Free Software
 *     Foundation; either version 2 of the License, or (at your option) any
 *     later version.
 *
 * or both in parallel, as here.
 *
 * The GNU MP Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received copies of the GNU General Public License and the
 * GNU Lesser General Public License along with the GNU MP Library.  If not,
 * see https://www.gnu.org/licenses/.
 *
 *
 * gmp-h.in
 *
 * Definitions for GNU multiple precision functions.   -*- mode: c -*-
 *
 * Copyright 1991, 1993-1997, 1999-2016, 2020, 2021 Free Software
 * Foundation, Inc.
 *
 * This file is part of the GNU MP Library.
 *
 * The GNU MP Library is free software; you can redistribute it and/or modify
 * it under the terms of either:
 *
 *   * the GNU Lesser General Public License as published by the Free
 *     Software Foundation; either version 3 of the License, or (at your
 *     option) any later version.
 *
 * or
 *
 *   * the GNU General Public License as published by the Free Software
 *     Foundation; either version 2 of the License, or (at your option) any
 *     later version.
 *
 * or both in parallel, as here.
 *
 * The GNU MP Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received copies of the GNU General Public License and the
 * GNU Lesser General Public License along with the GNU MP Library.  If not,
 * see https://www.gnu.org/licenses/.
 *
 */

#ifndef KERNELS_61_MPN_FFT_MUL_2EXP_MODF_INCLUDE_KERNEL_H_
#define KERNELS_61_MPN_FFT_MUL_2EXP_MODF_INCLUDE_KERNEL_H_

#include <climits>

/*
 * gmp-h.in:134-146 (64-bit long limbs)
 */
typedef unsigned long int mp_limb_t;
typedef unsigned long int mp_bitcnt_t;
/*
 * gmp-h.in:167-178
 */
typedef mp_limb_t *mp_ptr;
typedef const mp_limb_t *mp_srcptr;
typedef long int mp_size_t;

/*
 * gmp-h.in:41-50 (no nail bits)
 */
#define GMP_LIMB_BITS (sizeof(mp_limb_t) * CHAR_BIT)
#define GMP_NAIL_BITS 0
#define GMP_NUMB_BITS (GMP_LIMB_BITS - GMP_NAIL_BITS)
#define GMP_NUMB_MASK (~(mp_limb_t)0)

/*
 * gmp-impl.h:568
 */
#define MP_LIMB_T_MAX (~(mp_limb_t)0)
/*
 * gmp-impl.h:576
 */
#define GMP_LIMB_HIGHBIT (MP_LIMB_T_MAX ^ (MP_LIMB_T_MAX >> 1))

/*
 * gmp-h.in:454-461 (GCC/Clang)
 */
#define UNLIKELY(cond) __builtin_expect((cond) != 0, 0)

/*
 * gmp-impl.h:2507-2511 (non-assert configuration)
 */
#define ASSERT(expr)                                                           \
    do {                                                                       \
    } while (0)
/*
 * gmp-impl.h:1914-1920
 */
#define MPN_COPY(d, s, n)                                                      \
    do {                                                                       \
        ASSERT(MPN_SAME_OR_SEPARATE_P(d, s, n));                               \
        MPN_COPY_INCR(d, s, n);                                                \
    } while (0)
/*
 * gmp-impl.h:1834-1858
 */
#define MPN_COPY_INCR(dst, src, n)                                             \
    do {                                                                       \
        ASSERT((n) >= 0);                                                      \
        ASSERT(MPN_SAME_OR_INCR_P(dst, src, n));                               \
        if ((n) != 0) {                                                        \
            mp_size_t __n = (n) - 1;                                           \
            mp_ptr __dst = (dst);                                              \
            mp_srcptr __src = (src);                                           \
            mp_limb_t __x;                                                     \
            __x = *__src++;                                                    \
            if (__n != 0) {                                                    \
                do {                                                           \
                    *__dst++ = __x;                                            \
                    __x = *__src++;                                            \
                } while (--__n);                                               \
            }                                                                  \
            *__dst++ = __x;                                                    \
        }                                                                      \
    } while (0)

/*
 * gmp-impl.h:2453-2454
 */
#define MPN_OVERLAP_P(xp, xsize, yp, ysize)                                    \
    ((xp) + (xsize) > (yp) && (yp) + (ysize) > (xp))
/*
 * gmp-impl.h:2461-2476
 */
#define MPN_SAME_OR_SEPARATE_P(xp, yp, size)                                   \
    MPN_SAME_OR_SEPARATE2_P(xp, size, yp, size)
#define MPN_SAME_OR_SEPARATE2_P(xp, xsize, yp, ysize)                          \
    ((xp) == (yp) || !MPN_OVERLAP_P(xp, xsize, yp, ysize))
#define MPN_SAME_OR_INCR2_P(dst, dsize, src, ssize)                            \
    ((dst) <= (src) || !MPN_OVERLAP_P(dst, dsize, src, ssize))
#define MPN_SAME_OR_INCR_P(dst, src, size)                                     \
    MPN_SAME_OR_INCR2_P(dst, size, src, size)
#define MPN_SAME_OR_DECR2_P(dst, dsize, src, ssize)                            \
    ((dst) >= (src) || !MPN_OVERLAP_P(dst, dsize, src, ssize))
#define MPN_SAME_OR_DECR_P(dst, src, size)                                     \
    MPN_SAME_OR_DECR2_P(dst, size, src, size)

/*
 * gmp-impl.h:2618-2632 (generic complement)
 */
#define mpn_com(d, s, n)                                                       \
    do {                                                                       \
        mp_ptr __d = (d);                                                      \
        mp_srcptr __s = (s);                                                   \
        mp_size_t __n = (n);                                                   \
        ASSERT(__n >= 1);                                                      \
        ASSERT(MPN_SAME_OR_SEPARATE_P(__d, __s, __n));                         \
        do                                                                     \
            *__d++ = (~*__s++) & GMP_NUMB_MASK;                                \
        while (--__n);                                                         \
    } while (0)

/*
 * gmp-impl.h:2820-2861 (zero nail bits)
 */
#define mpn_incr_u(p, incr)                                                    \
    do {                                                                       \
        mp_limb_t __x;                                                         \
        mp_ptr __p = (p);                                                      \
        if (__builtin_constant_p(incr) && (incr) == 1) {                       \
            while (++(*(__p++)) == 0)                                          \
                ;                                                              \
        } else {                                                               \
            __x = *__p + (incr);                                               \
            *__p = __x;                                                        \
            if (__x < (incr))                                                  \
                while (++(*(++__p)) == 0)                                      \
                    ;                                                          \
        }                                                                      \
    } while (0)
#define mpn_decr_u(p, incr)                                                    \
    do {                                                                       \
        mp_limb_t __x;                                                         \
        mp_ptr __p = (p);                                                      \
        if (__builtin_constant_p(incr) && (incr) == 1) {                       \
            while ((*(__p++))-- == 0)                                          \
                ;                                                              \
        } else {                                                               \
            __x = *__p;                                                        \
            *__p = __x - (incr);                                               \
            if (__x < (incr))                                                  \
                while ((*(++__p))-- == 0)                                      \
                    ;                                                          \
        }                                                                      \
    } while (0)

/*
 * gmp-impl.h:2926-2948 (assertions disabled)
 */
#define MPN_INCR_U(ptr, size, n) mpn_incr_u(ptr, n)
#define MPN_DECR_U(ptr, size, n) mpn_decr_u(ptr, n)

/*
 * gmp-impl.h:4002-4006 (long limbs)
 */
#define CNST_LIMB(C) ((mp_limb_t)C##L)

/*
 * gmp-h.in:2118-2131
 */
#define __GMPN_COPY_REST(dst, src, size, start)                                \
    do {                                                                       \
        mp_size_t __gmp_j;                                                     \
        for (__gmp_j = (start); __gmp_j < (size); __gmp_j++)                   \
            (dst)[__gmp_j] = (src)[__gmp_j];                                   \
    } while (0)
/*
 * gmp-h.in:1993-2030 (zero nail bits)
 */
#define __GMPN_AORS_1(cout, dst, src, n, v, OP, CB)                            \
    do {                                                                       \
        mp_size_t __gmp_i;                                                     \
        mp_limb_t __gmp_x, __gmp_r;                                            \
        __gmp_x = (src)[0];                                                    \
        __gmp_r = __gmp_x OP(v);                                               \
        (dst)[0] = __gmp_r;                                                    \
        if (CB(__gmp_r, __gmp_x, (v))) {                                       \
            (cout) = 1;                                                        \
            for (__gmp_i = 1; __gmp_i < (n);) {                                \
                __gmp_x = (src)[__gmp_i];                                      \
                __gmp_r = __gmp_x OP 1;                                        \
                (dst)[__gmp_i] = __gmp_r;                                      \
                ++__gmp_i;                                                     \
                if (!CB(__gmp_r, __gmp_x, 1)) {                                \
                    if ((src) != (dst))                                        \
                        __GMPN_COPY_REST(dst, src, n, __gmp_i);                \
                    (cout) = 0;                                                \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        } else {                                                               \
            if ((src) != (dst))                                                \
                __GMPN_COPY_REST(dst, src, n, 1);                              \
            (cout) = 0;                                                        \
        }                                                                      \
    } while (0)
/*
 * gmp-h.in:2071-2077
 */
#define __GMPN_ADDCB(r, x, y) ((r) < (y))
#define __GMPN_SUBCB(r, x, y) ((x) < (y))
#define __GMPN_ADD_1(cout, dst, src, n, v)                                     \
    __GMPN_AORS_1(cout, dst, src, n, v, +, __GMPN_ADDCB)
#define __GMPN_SUB_1(cout, dst, src, n, v)                                     \
    __GMPN_AORS_1(cout, dst, src, n, v, -, __GMPN_SUBCB)

/*
 * gmp-h.in:2154-2166
 */
static inline mp_limb_t mpn_add_1(mp_ptr __gmp_dst, mp_srcptr __gmp_src,
                                  mp_size_t __gmp_size, mp_limb_t __gmp_n) {
    mp_limb_t __gmp_c;
    __GMPN_ADD_1(__gmp_c, __gmp_dst, __gmp_src, __gmp_size, __gmp_n);
    return __gmp_c;
}
/*
 * gmp-h.in:2209-2221
 */
static inline mp_limb_t mpn_sub_1(mp_ptr __gmp_dst, mp_srcptr __gmp_src,
                                  mp_size_t __gmp_size, mp_limb_t __gmp_n) {
    mp_limb_t __gmp_c;
    __GMPN_SUB_1(__gmp_c, __gmp_dst, __gmp_src, __gmp_size, __gmp_n);
    return __gmp_c;
}

/*
 * gmp-h.in:1480-1481
 */
mp_limb_t mpn_add_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t);
/*
 * gmp-h.in:1620-1621
 */
mp_limb_t mpn_sub_n(mp_ptr, mp_srcptr, mp_srcptr, mp_size_t);
/*
 * gmp-h.in:1544-1545
 */
mp_limb_t mpn_lshift(mp_ptr, mp_srcptr, mp_size_t, unsigned int);
/*
 * gmp-impl.h:1097-1100
 */
mp_limb_t mpn_lshiftc(mp_ptr, mp_srcptr, mp_size_t, unsigned int);

/*
 * Wrapper for invoking the extracted kernel.
 */
void mpn_fft_fft_isolated(mp_ptr *Ap, mp_size_t K, int **ll, mp_size_t omega,
                          mp_size_t n, mp_size_t inc, mp_ptr tp);

#endif // KERNELS_61_MPN_FFT_MUL_2EXP_MODF_INCLUDE_KERNEL_H_
