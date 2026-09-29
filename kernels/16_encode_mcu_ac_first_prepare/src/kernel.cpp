/****************************************************************************
 *
 *
 *  Project: libjpeg-turbo 3.2.0
 *  Source files:
 *    src/jcphuff.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/jcphuff.c
 *
 *   This file contains Huffman entropy encoding routines for progressive JPEG.
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1995-1997, Thomas G. Lane.
 * Lossless JPEG Modifications:
 * Copyright (C) 1999, Ken Murchison.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2011, 2015, 2018, 2021-2022, 2024-2025, D. R. Commander.
 * Copyright (C) 2016, 2018, 2022, Matthieu Darbois.
 * Copyright (C) 2020, Arm Limited.
 * Copyright (C) 2021, Alex Richardson.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 */

#include "kernel.h"
#include <limits.h>

/*
 * src/jcphuff.c:542-565
 */
#define COMPUTE_ABSVALUES_AC_FIRST(Sl)                                         \
    {                                                                          \
        for (k = 0; k < Sl; k++) {                                             \
            temp = block[jpeg_natural_order_start[k]];                         \
            if (temp == 0)                                                     \
                continue;                                                      \
            temp2 = temp >> (CHAR_BIT * sizeof(int) - 1);                      \
            temp ^= temp2;                                                     \
            temp -= temp2;                                                     \
            temp >>= Al;                                                       \
            if (temp == 0)                                                     \
                continue;                                                      \
            temp2 ^= temp;                                                     \
            values[k] = (UJCOEF)temp;                                          \
            values[k + DCTSIZE2] = (UJCOEF)temp2;                              \
            zerobits |= ((size_t)1U) << k;                                     \
        }                                                                      \
    }

/*
 * src/jcphuff.c:567-596
 */
METHODDEF(void)
encode_mcu_AC_first_prepare_impl(const JCOEF *block,
                                 const int *jpeg_natural_order_start, int Sl,
                                 int Al, UJCOEF *values, size_t *bits) {
    register int k, temp, temp2;
    size_t zerobits = 0U;
    int Sl0 = Sl;

    COMPUTE_ABSVALUES_AC_FIRST(Sl0);

    bits[0] = zerobits;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void encode_mcu_AC_first_prepare_isolated(const JCOEF *block,
                                 const int *jpeg_natural_order_start, int Sl,
                                 int Al, UJCOEF *values, size_t *bits) {
    encode_mcu_AC_first_prepare_impl(block, jpeg_natural_order_start, Sl, Al,
                                     values, bits);
}
