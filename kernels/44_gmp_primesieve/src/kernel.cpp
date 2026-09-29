/****************************************************************************
 *
 *
 *  Project: GNU MP (GMP) 6.3.0
 *  Source files:
 *    primesieve.c
 *    mpn/generic/popham.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * primesieve.c
 *
 * primesieve (BIT_ARRAY, N) -- Fills the BIT_ARRAY with a mask for primes up to N.
 *
 * Contributed to the GNU project by Marco Bodrato.
 *
 * THE FUNCTION IN THIS FILE IS INTERNAL WITH A MUTABLE INTERFACE.
 * IT IS ONLY SAFE TO REACH IT THROUGH DOCUMENTED INTERFACES.
 * IN FACT, IT IS ALMOST GUARANTEED THAT IT WILL CHANGE OR
 * DISAPPEAR IN A FUTURE GNU MP RELEASE.
 *
 * Copyright 2010-2012, 2015, 2016, 2021, 2022 Free Software Foundation, Inc.
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
 * mpn/generic/popham.c
 *
 * mpn_popcount, mpn_hamdist -- mpn bit population count/hamming distance.
 *
 * Copyright 1994, 1996, 2000-2002, 2005, 2011, 2012 Free Software Foundation,
 * Inc.
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

#include "kernel.h"

/*
 * primesieve.c:45-55
 */
static mp_limb_t id_to_n(mp_limb_t id) { return id * 3 + 1 + (id & 1); }

static mp_limb_t n_fto_bit(mp_limb_t n) { return ((n - 5) | 1) / 3U; }

static mp_limb_t n_cto_bit(mp_limb_t n) { return (n | 1) / 3U - 1; }

/*
 * primesieve.c:62-143
 */
#define SET_OFF1(m1, m2, M1, M2, off, BITS)                                    \
    if (off) {                                                                 \
        if (off < GMP_LIMB_BITS) {                                             \
            m1 = (M1 >> off) | (M2 << (GMP_LIMB_BITS - off));                  \
            if (off <= BITS - GMP_LIMB_BITS) {                                 \
                m2 = M1 << (BITS - GMP_LIMB_BITS - off) | M2 >> off;           \
            } else {                                                           \
                m1 |= M1 << (BITS - off);                                      \
                m2 = M1 >> (off + GMP_LIMB_BITS - BITS);                       \
            }                                                                  \
        } else {                                                               \
            m1 = M1 << (BITS - off) | M2 >> (off - GMP_LIMB_BITS);             \
            m2 = M2 << (BITS - off) | M1 >> (off + GMP_LIMB_BITS - BITS);      \
        }                                                                      \
    } else {                                                                   \
        m1 = M1;                                                               \
        m2 = M2;                                                               \
    }

#define SET_OFF2(m1, m2, m3, M1, M2, M3, off, BITS)                            \
    if (off) {                                                                 \
        if (off <= GMP_LIMB_BITS) {                                            \
            m1 = M2 << (GMP_LIMB_BITS - off);                                  \
            m2 = M3 << (GMP_LIMB_BITS - off);                                  \
            if (off != GMP_LIMB_BITS) {                                        \
                m1 |= (M1 >> off);                                             \
                m2 |= (M2 >> off);                                             \
            }                                                                  \
            if (off <= BITS - 2 * GMP_LIMB_BITS) {                             \
                m3 = M1 << (BITS - 2 * GMP_LIMB_BITS - off) | M3 >> off;       \
            } else {                                                           \
                m2 |= M1 << (BITS - GMP_LIMB_BITS - off);                      \
                m3 = M1 >> (off + 2 * GMP_LIMB_BITS - BITS);                   \
            }                                                                  \
        } else if (off < 2 * GMP_LIMB_BITS) {                                  \
            m1 =                                                               \
                M2 >> (off - GMP_LIMB_BITS) | M3 << (2 * GMP_LIMB_BITS - off); \
            if (off <= BITS - GMP_LIMB_BITS) {                                 \
                m2 = M3 >> (off - GMP_LIMB_BITS) |                             \
                     M1 << (BITS - GMP_LIMB_BITS - off);                       \
                m3 = M2 << (BITS - GMP_LIMB_BITS - off);                       \
                if (off != BITS - GMP_LIMB_BITS) {                             \
                    m3 |= M1 >> (off + 2 * GMP_LIMB_BITS - BITS);              \
                }                                                              \
            } else {                                                           \
                m1 |= M1 << (BITS - off);                                      \
                m2 = M2 << (BITS - off) | M1 >> (GMP_LIMB_BITS - BITS + off);  \
                m3 = M2 >> (GMP_LIMB_BITS - BITS + off);                       \
            }                                                                  \
        } else {                                                               \
            m1 = M1 << (BITS - off) | M3 >> (off - 2 * GMP_LIMB_BITS);         \
            m2 = M2 << (BITS - off) | M1 >> (off + GMP_LIMB_BITS - BITS);      \
            m3 = M3 << (BITS - off) | M2 >> (off + GMP_LIMB_BITS - BITS);      \
        }                                                                      \
    } else {                                                                   \
        m1 = M1;                                                               \
        m2 = M2;                                                               \
        m3 = M3;                                                               \
    }

#define ROTATE1(m1, m2, BITS)                                                  \
    do {                                                                       \
        mp_limb_t __tmp;                                                       \
        __tmp = m1 >> (2 * GMP_LIMB_BITS - BITS);                              \
        m1 = (m1 << (BITS - GMP_LIMB_BITS)) | m2;                              \
        m2 = __tmp;                                                            \
    } while (0)

#define ROTATE2(m1, m2, m3, BITS)                                              \
    do {                                                                       \
        mp_limb_t __tmp;                                                       \
        __tmp = m2 >> (3 * GMP_LIMB_BITS - BITS);                              \
        m2 = m2 << (BITS - GMP_LIMB_BITS * 2) |                                \
             m1 >> (3 * GMP_LIMB_BITS - BITS);                                 \
        m1 = m1 << (BITS - GMP_LIMB_BITS * 2) | m3;                            \
        m3 = __tmp;                                                            \
    } while (0)

/*
 * primesieve.c:145-194
 */
static mp_limb_t fill_bitpattern(mp_ptr bit_array, mp_size_t limbs,
                                 mp_limb_t offset) {
    mp_limb_t m11, m12, m21, m22, m23;

    {
        mp_limb_t off1 = offset % (11 * 5 * 2);
        SET_OFF1(m11, m12, SIEVE_MASK1, SIEVE_MASKT, off1, 11 * 5 * 2);
        offset %= 13 * 7 * 2;
        SET_OFF2(m21, m22, m23, SIEVE_2MSK1, SIEVE_2MSK2, SIEVE_2MSKT, offset,
                 13 * 7 * 2);
    }

    do {
        bit_array[0] = m11 | m21;
        if (--limbs == 0)
            break;
        ROTATE1(m11, m12, 11 * 5 * 2);
        bit_array[1] = m11 | m22;
        bit_array += 2;
        ROTATE1(m11, m12, 11 * 5 * 2);
        ROTATE2(m21, m22, m23, 13 * 7 * 2);
    } while (--limbs != 0);
    return n_cto_bit(13 + 1);
}

/*
 * primesieve.c:196-259
 */
static void block_resieve(mp_ptr bit_array, mp_size_t limbs, mp_limb_t offset,
                          mp_srcptr sieve) {
    mp_size_t bits, off = offset;
    mp_limb_t mask, i;

    ASSERT(limbs > 0);

    bits = limbs * GMP_LIMB_BITS - 1;

    i = fill_bitpattern(bit_array, limbs, offset);

    ASSERT(i < GMP_LIMB_BITS);

    mask = CNST_LIMB(1) << i;
    do {
        ++i;
        if ((*sieve & mask) == 0) {
            mp_size_t step, lindex;
            mp_limb_t lmask;
            unsigned maskrot;

            step = id_to_n(i);

            lindex = i * (step + 1) - 1 + (-(i & 1) & (i + 1));

            if (lindex > bits + off)
                break;

            step <<= 1;
            maskrot = step % GMP_LIMB_BITS;

            if (lindex < off)
                lindex += step * ((off - lindex - 1) / step + 1);

            lindex -= off;

            lmask = CNST_LIMB(1) << (lindex % GMP_LIMB_BITS);
            for (; lindex <= bits; lindex += step) {
                bit_array[lindex / GMP_LIMB_BITS] |= lmask;
                lmask = lmask << maskrot | lmask >> (GMP_LIMB_BITS - maskrot);
            };

            lindex = i * (i * 3 + 6) + (i & 1);

            if (lindex < off)
                lindex += step * ((off - lindex - 1) / step + 1);

            lindex -= off;

            lmask = CNST_LIMB(1) << (lindex % GMP_LIMB_BITS);
            for (; lindex <= bits; lindex += step) {
                bit_array[lindex / GMP_LIMB_BITS] |= lmask;
                lmask = lmask << maskrot | lmask >> (GMP_LIMB_BITS - maskrot);
            };
        }
        mask = mask << 1 | mask >> (GMP_LIMB_BITS - 1);
        sieve += mask & 1;
    } while (1);
}

/*
 * primesieve.c:261
 */
#define BLOCK_SIZE 2048

/*
 * primesieve.c:279-309
 */
mp_limb_t gmp_primesieve(mp_ptr bit_array, mp_limb_t n) {
    mp_size_t size;
    mp_limb_t bits;
    static mp_limb_t presieved[] = {PRIMESIEVE_INIT_TABLE};

    ASSERT(n > 4);

    bits = n_fto_bit(n);
    size = bits / GMP_LIMB_BITS + 1;

    for (mp_size_t j = 0, lim = MIN(size, PRIMESIEVE_NUMBEROF_TABLE); j < lim;
         ++j)
        bit_array[j] = presieved[j];

    if (size > PRIMESIEVE_NUMBEROF_TABLE) {
        mp_size_t off;
        off = size > 2 * BLOCK_SIZE ? BLOCK_SIZE + (size % BLOCK_SIZE) : size;
        block_resieve(bit_array + PRIMESIEVE_NUMBEROF_TABLE,
                      off - PRIMESIEVE_NUMBEROF_TABLE,
                      GMP_LIMB_BITS * PRIMESIEVE_NUMBEROF_TABLE, bit_array);
        for (; off < size; off += BLOCK_SIZE)
            block_resieve(bit_array + off, BLOCK_SIZE, off * GMP_LIMB_BITS,
                          bit_array);
    }

    if ((bits + 1) % GMP_LIMB_BITS != 0)
        bit_array[size - 1] |= MP_LIMB_T_MAX << ((bits + 1) % GMP_LIMB_BITS);

    return size * GMP_LIMB_BITS - mpn_popcount(bit_array, size);
}

/* mpn/generic/popham.c:34-36 */
#define FNAME mpn_popcount
#define POPHAM(u, v) u

/* mpn/generic/popham.c:44-125 (popcount configuration) */
mp_bitcnt_t FNAME(mp_srcptr up, mp_size_t n) __GMP_NOTHROW {
    mp_bitcnt_t result = 0;
    mp_limb_t p0, p1, p2, p3, x, p01, p23;
    mp_size_t i;

    ASSERT(n >= 1);

    for (i = n >> 2; i != 0; i--) {
        p0 = POPHAM(up[0], vp[0]);
        p0 -= (p0 >> 1) & MP_LIMB_T_MAX / 3;
        p0 = ((p0 >> 2) & MP_LIMB_T_MAX / 5) + (p0 & MP_LIMB_T_MAX / 5);

        p1 = POPHAM(up[1], vp[1]);
        p1 -= (p1 >> 1) & MP_LIMB_T_MAX / 3;
        p1 = ((p1 >> 2) & MP_LIMB_T_MAX / 5) + (p1 & MP_LIMB_T_MAX / 5);

        p01 = p0 + p1;
        p01 = ((p01 >> 4) & MP_LIMB_T_MAX / 17) + (p01 & MP_LIMB_T_MAX / 17);

        p2 = POPHAM(up[2], vp[2]);
        p2 -= (p2 >> 1) & MP_LIMB_T_MAX / 3;
        p2 = ((p2 >> 2) & MP_LIMB_T_MAX / 5) + (p2 & MP_LIMB_T_MAX / 5);

        p3 = POPHAM(up[3], vp[3]);
        p3 -= (p3 >> 1) & MP_LIMB_T_MAX / 3;
        p3 = ((p3 >> 2) & MP_LIMB_T_MAX / 5) + (p3 & MP_LIMB_T_MAX / 5);

        p23 = p2 + p3;
        p23 = ((p23 >> 4) & MP_LIMB_T_MAX / 17) + (p23 & MP_LIMB_T_MAX / 17);

        x = p01 + p23;
        x = (x >> 8) + x;
        x = (x >> 16) + x;
#if GMP_LIMB_BITS > 32
        x = ((x >> 32) & 0xff) + (x & 0xff);
        result += x;
#else
        result += x & 0xff;
#endif
        up += 4;
    }

    n &= 3;
    if (n != 0) {
        x = 0;
        do {
            p0 = POPHAM(up[0], vp[0]);
            p0 -= (p0 >> 1) & MP_LIMB_T_MAX / 3;
            p0 = ((p0 >> 2) & MP_LIMB_T_MAX / 5) + (p0 & MP_LIMB_T_MAX / 5);
            p0 = ((p0 >> 4) + p0) & MP_LIMB_T_MAX / 17;

            x += p0;
            up += 1;
        } while (--n);

        x = (x >> 8) + x;
        x = (x >> 16) + x;
#if GMP_LIMB_BITS > 32
        x = (x >> 32) + x;
#endif
        result += x & 0xff;
    }

    return result;
}
