// clang-format off
/****************************************************************************
 *
 *
 *  Project: GNU MP (GMP) 6.3.0
 *  Source files:
 *    mpn/generic/mul_fft.c
 *    mpn/generic/add_n.c
 *    mpn/generic/sub_n.c
 *    mpn/generic/lshift.c
 *    mpn/generic/lshiftc.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * mpn/generic/mul_fft.c
 *
 * Schoenhage's fast multiplication modulo 2^N+1.
 *
 *    Contributed by Paul Zimmermann.
 *
 *    THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH MUTABLE INTERFACES.  IT IS ONLY
 *    SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
 *    GUARANTEED THAT THEY WILL CHANGE OR DISAPPEAR IN A FUTURE GNU MP RELEASE.
 *
 * Copyright 1998-2010, 2012, 2013, 2018, 2020, 2022 Free Software
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
 * mpn/generic/add_n.c
 *
 * mpn_add_n -- Add equal length limb vectors.
 *
 * Copyright 1992-1994, 1996, 2000, 2002, 2009 Free Software Foundation, Inc.
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
 * mpn/generic/sub_n.c
 *
 * mpn_sub_n -- Subtract equal length limb vectors.
 *
 * Copyright 1992-1994, 1996, 2000, 2002, 2009 Free Software Foundation, Inc.
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
 * mpn/generic/lshift.c
 *
 * mpn_lshift -- Shift left low level.
 *
 * Copyright 1991, 1993, 1994, 1996, 2000-2002 Free Software Foundation, Inc.
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
 * mpn/generic/lshiftc.c
 *
 * mpn_lshiftc -- Shift left low level with complement.
 *
 * Copyright 1991, 1993, 1994, 1996, 2000-2002, 2009 Free Software Foundation,
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
// clang-format on

#include "kernel.h"

/*
 * mpn/generic/mul_fft.c:204-299
 */
static void mpn_fft_mul_2exp_modF(mp_ptr r, mp_srcptr a, mp_bitcnt_t d,
                                  mp_size_t n) {
    unsigned int sh;
    mp_size_t m;
    mp_limb_t cc, rd;

    sh = d % GMP_NUMB_BITS;
    m = d / GMP_NUMB_BITS;

    if (m >= n) {

        m -= n;
        if (sh != 0) {

            mpn_lshift(r, a + n - m, m + 1, sh);
            rd = r[m];
            cc = mpn_lshiftc(r + m, a, n - m, sh);
        } else {
            MPN_COPY(r, a + n - m, m);
            rd = a[n];
            mpn_com(r + m, a, n - m);
            cc = 0;
        }

        r[n] = 0;

        ++cc;
        MPN_INCR_U(r, n + 1, cc);

        ++rd;

        cc = rd + (rd == 0);
        r = r + m + (rd == 0);
        MPN_INCR_U(r, n + 1 - m - (rd == 0), cc);
    } else {

        if (sh != 0) {

            mpn_lshiftc(r, a + n - m, m + 1, sh);
            rd = ~r[m];

            cc = mpn_lshift(r + m, a, n - m, sh);
        } else {

            mpn_com(r, a + n - m, m + 1);
            rd = a[n];
            MPN_COPY(r + m, a, n - m);
            cc = 0;
        }

        if (m != 0) {

            if (cc-- == 0)
                cc = mpn_add_1(r, r, n, CNST_LIMB(1));
            cc = mpn_sub_1(r, r, m, cc) + 1;
        }

        r[n] = 2;
        MPN_DECR_U(r + m, n - m + 1, cc);
        MPN_DECR_U(r + m, n - m + 1, rd);

        if (UNLIKELY((r[n] -= 2) != 0)) {
            mp_limb_t cy = -r[n];

            r[n] = 0;
            MPN_INCR_U(r, n + 1, cy);
        }
    }
}

/*
 * mpn/generic/mul_fft.c:320-383 (selected non-ADDSUB implementation)
 */
static inline void mpn_fft_add_modF(mp_ptr r, mp_srcptr a, mp_srcptr b,
                                    mp_size_t n) {
    mp_limb_t c, x;

    c = a[n] + b[n] + mpn_add_n(r, a, b, n);

    x = (c - 1) & -(c != 0);
    r[n] = c - x;
    MPN_DECR_U(r, n + 1, x);
}

static inline void mpn_fft_sub_modF(mp_ptr r, mp_srcptr a, mp_srcptr b,
                                    mp_size_t n) {
    mp_limb_t c, x;

    c = a[n] - b[n] - mpn_sub_n(r, a, b, n);

    x = (-c) & -((c & GMP_LIMB_HIGHBIT) != 0);
    r[n] = x + c;
    MPN_INCR_U(r, n + 1, x);
}

/*
 * mpn/generic/mul_fft.c:385-438
 */
static void mpn_fft_fft(mp_ptr *Ap, mp_size_t K, int **ll, mp_size_t omega,
                        mp_size_t n, mp_size_t inc, mp_ptr tp) {
    if (K == 2) {
        mp_limb_t cy;
        MPN_COPY(tp, Ap[0], n + 1);
        mpn_add_n(Ap[0], Ap[0], Ap[inc], n + 1);
        cy = mpn_sub_n(Ap[inc], tp, Ap[inc], n + 1);
        if (Ap[0][n] > 1) {
            mp_limb_t cc = Ap[0][n] - 1;
            Ap[0][n] = 1;
            MPN_DECR_U(Ap[0], n + 1, cc);
        }
        if (cy) {
            mp_limb_t cc = ~Ap[inc][n] + 1;
            Ap[inc][n] = 0;
            MPN_INCR_U(Ap[inc], n + 1, cc);
        }
    } else {
        mp_size_t j, K2 = K >> 1;
        int *lk = *ll;

        mpn_fft_fft(Ap, K2, ll - 1, 2 * omega, n, inc * 2, tp);
        mpn_fft_fft(Ap + inc, K2, ll - 1, 2 * omega, n, inc * 2, tp);

        for (j = 0; j < K2; j++, lk += 2, Ap += 2 * inc) {

            mpn_fft_mul_2exp_modF(tp, Ap[inc], lk[0] * omega, n);
            mpn_fft_sub_modF(Ap[inc], Ap[0], tp, n);
            mpn_fft_add_modF(Ap[0], Ap[0], tp, n);
        }
    }
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void mpn_fft_fft_isolated(mp_ptr *Ap, mp_size_t K, int **ll, mp_size_t omega,
                          mp_size_t n, mp_size_t inc, mp_ptr tp) {
    mpn_fft_fft(Ap, K, ll, omega, n, inc, tp);
}

/*
 * mpn/generic/add_n.c:34-61 (zero nail bits)
 */
mp_limb_t mpn_add_n(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n) {
    mp_limb_t ul, vl, sl, rl, cy, cy1, cy2;

    ASSERT(n >= 1);
    ASSERT(MPN_SAME_OR_INCR_P(rp, up, n));
    ASSERT(MPN_SAME_OR_INCR_P(rp, vp, n));

    cy = 0;
    do {
        ul = *up++;
        vl = *vp++;
        sl = ul + vl;
        cy1 = sl < ul;
        rl = sl + cy;
        cy2 = rl < sl;
        cy = cy1 | cy2;
        *rp++ = rl;
    } while (--n != 0);

    return cy;
}

/*
 * mpn/generic/sub_n.c:34-61 (zero nail bits)
 */
mp_limb_t mpn_sub_n(mp_ptr rp, mp_srcptr up, mp_srcptr vp, mp_size_t n) {
    mp_limb_t ul, vl, sl, rl, cy, cy1, cy2;

    ASSERT(n >= 1);
    ASSERT(MPN_SAME_OR_INCR_P(rp, up, n));
    ASSERT(MPN_SAME_OR_INCR_P(rp, vp, n));

    cy = 0;
    do {
        ul = *up++;
        vl = *vp++;
        sl = ul - vl;
        cy1 = sl > ul;
        rl = sl - cy;
        cy2 = rl > sl;
        cy = cy1 | cy2;
        *rp++ = rl;
    } while (--n != 0);

    return cy;
}

/*
 * mpn/generic/lshift.c:42-72
 */
mp_limb_t mpn_lshift(mp_ptr rp, mp_srcptr up, mp_size_t n, unsigned int cnt) {
    mp_limb_t high_limb, low_limb;
    unsigned int tnc;
    mp_size_t i;
    mp_limb_t retval;

    ASSERT(n >= 1);
    ASSERT(cnt >= 1);
    ASSERT(cnt < GMP_NUMB_BITS);
    ASSERT(MPN_SAME_OR_DECR_P(rp, up, n));

    up += n;
    rp += n;

    tnc = GMP_NUMB_BITS - cnt;
    low_limb = *--up;
    retval = low_limb >> tnc;
    high_limb = (low_limb << cnt) & GMP_NUMB_MASK;

    for (i = n - 1; i != 0; i--) {
        low_limb = *--up;
        *--rp = high_limb | (low_limb >> tnc);
        high_limb = (low_limb << cnt) & GMP_NUMB_MASK;
    }
    *--rp = high_limb;

    return retval;
}

/*
 * mpn/generic/lshiftc.c:43-73
 */
mp_limb_t mpn_lshiftc(mp_ptr rp, mp_srcptr up, mp_size_t n, unsigned int cnt) {
    mp_limb_t high_limb, low_limb;
    unsigned int tnc;
    mp_size_t i;
    mp_limb_t retval;

    ASSERT(n >= 1);
    ASSERT(cnt >= 1);
    ASSERT(cnt < GMP_NUMB_BITS);
    ASSERT(MPN_SAME_OR_DECR_P(rp, up, n));

    up += n;
    rp += n;

    tnc = GMP_NUMB_BITS - cnt;
    low_limb = *--up;
    retval = low_limb >> tnc;
    high_limb = (low_limb << cnt);

    for (i = n - 1; i != 0; i--) {
        low_limb = *--up;
        *--rp = (~(high_limb | (low_limb >> tnc))) & GMP_NUMB_MASK;
        high_limb = low_limb << cnt;
    }
    *--rp = (~high_limb) & GMP_NUMB_MASK;

    return retval;
}
