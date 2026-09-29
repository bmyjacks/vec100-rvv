/****************************************************************************
 * Project: libjpeg-turbo 3.2.0, src/jcphuff.c:1-21, 777-843
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1995-1997, Thomas G. Lane.
 * Lossless JPEG Modifications: Copyright (C) 1999, Ken Murchison.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2011, 2015, 2018, 2021-2022, 2024-2025, D. R. Commander.
 * Copyright (C) 2016, 2018, 2022, Matthieu Darbois.
 * Copyright (C) 2020, Arm Limited.
 * Copyright (C) 2021, Alex Richardson.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file in the libjpeg-turbo distribution.
 ****************************************************************************/
#include "kernel.h"
#include <limits.h>

/* src/jcphuff.c:780-802; keep the upstream pre-pass and its write order. */
#define COMPUTE_ABSVALUES_AC_REFINE(Sl, koffset) { \
  for (k = 0; k < Sl; k++) { \
    temp = block[jpeg_natural_order_start[k]]; \
    temp2 = temp >> (CHAR_BIT * sizeof(int) - 1); \
    temp ^= temp2; \
    temp -= temp2; \
    temp >>= Al; \
    if (temp != 0) { \
      zerobits |= ((size_t)1U) << k; \
      signbits |= ((size_t)(temp2 + 1)) << k; \
    } \
    absvalues[k] = (UJCOEF)temp; \
    if (temp == 1) \
      EOB = k + koffset; \
  } \
}

/* RV64 LP64: SIZEOF_SIZE_T == 8.  The 32-bit split path is not used here. */
METHODDEF(int)
encode_mcu_AC_refine_prepare_impl(const JCOEF *block,
                                  const int *jpeg_natural_order_start, int Sl,
                                  int Al, UJCOEF *absvalues, size_t *bits)
{
  register int k, temp, temp2;
  int EOB = 0;
  size_t zerobits = 0U, signbits = 0U;
  int Sl0 = Sl;

  COMPUTE_ABSVALUES_AC_REFINE(Sl0, 0);

  bits[0] = zerobits;
  bits[1] = signbits;
  return EOB;
}

int encode_mcu_AC_refine_prepare(const JCOEF *block,
                                 const int *jpeg_natural_order_start, int Sl,
                                 int Al, UJCOEF *absvalues, size_t *bits)
{
  return encode_mcu_AC_refine_prepare_impl(block, jpeg_natural_order_start,
                                           Sl, Al, absvalues, bits);
}
