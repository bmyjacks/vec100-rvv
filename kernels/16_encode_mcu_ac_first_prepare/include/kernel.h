/****************************************************************************
 *
 *
 *  Project: libjpeg-turbo 3.2.0
 *  Source files:
 *    src/jmorecfg.h
 *    src/jpeglib.h
 *    src/jchuff.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/jmorecfg.h
 *
 *   This file contains additional configuration options that customize the
 *   JPEG software for special applications or support machine-dependent
 *   optimizations.  Most users will not need to touch this file.
 *
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
 *
 * src/jpeglib.h
 *
 *   This file defines the application interface for the JPEG library.
 *   Most applications using the library need only include this file,
 *   and perhaps jerror.h if they want to know the exact error codes.
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1998, Thomas G. Lane.
 * Modified 2002-2009 by Guido Vollbeding.
 * Lossless JPEG Modifications:
 * Copyright (C) 1999, Ken Murchison.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2009-2011, 2013-2014, 2016-2017, 2020, 2022-2024,
 *              D. R. Commander.
 * Copyright (C) 2015, Google, Inc.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 *
 * src/jchuff.h
 *
 *   This file contains declarations for Huffman entropy encoding routines
 *   that are shared between the sequential encoder (jchuff.c) and the
 *   progressive encoder (jcphuff.c).  No other modules need to see these.
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2022, 2025, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 */

#ifndef KERNELS_16_ENCODE_MCU_AC_FIRST_PREPARE_INCLUDE_KERNEL_H_
#define KERNELS_16_ENCODE_MCU_AC_FIRST_PREPARE_INCLUDE_KERNEL_H_

#include <stddef.h>

/*
 * src/jmorecfg.h:78
 */
typedef short JCOEF;

/*
 * src/jmorecfg.h:169
 */
#define METHODDEF(type) static type

/*
 * src/jpeglib.h:67
 */
#define DCTSIZE2 64

/*
 * src/jchuff.h:34
 */
typedef unsigned short UJCOEF;

/*
 * Wrapper for invoking the extracted kernel.
 */
void encode_mcu_AC_first_prepare_isolated(const JCOEF *block,
                                 const int *jpeg_natural_order_start, int Sl,
                                 int Al, UJCOEF *values, size_t *bits);

#endif // KERNELS_16_ENCODE_MCU_AC_FIRST_PREPARE_INCLUDE_KERNEL_H_
