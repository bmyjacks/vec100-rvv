/****************************************************************************
 * Project: libjpeg-turbo 3.2.0
 * Source files: src/jmorecfg.h, src/jpeglib.h, src/jchuff.h
 *
 * Original file notices follow.
 *
 * src/jmorecfg.h
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * Modified 1997-2009 by Guido Vollbeding.
 * Lossless JPEG Modifications:
 * Copyright (C) 1999, Ken Murchison.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2009, 2011, 2014-2015, 2018, 2020, 2022, 2026,
 *           D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 * src/jpeglib.h
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1998, Thomas G. Lane.
 * Modified 2002-2009 by Guido Vollbeding.
 * Lossless JPEG Modifications:
 * Copyright (C) 1999, Ken Murchison.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2009-2011, 2013-2014, 2016-2017, 2020, 2022-2024,
 *           D. R. Commander.
 * Copyright (C) 2015, Google, Inc.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 * src/jchuff.h
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2022, 2025, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 ****************************************************************************/
#ifndef KERNELS_55_JPEG_AC_REFINE_INCLUDE_KERNEL_H_
#define KERNELS_55_JPEG_AC_REFINE_INCLUDE_KERNEL_H_

#include <stddef.h>

/* src/jmorecfg.h:78,169; src/jchuff.h:34; src/jpeglib.h:67 */
typedef short JCOEF;
typedef unsigned short UJCOEF;
#define METHODDEF(type) static type
#define DCTSIZE2 64

/* The exact callback signature in src/jcphuff.c:59-61. */
int encode_mcu_AC_refine_prepare(const JCOEF *block,
                                 const int *jpeg_natural_order_start, int Sl,
                                 int Al, UJCOEF *absvalues, size_t *bits);

#endif
