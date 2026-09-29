/****************************************************************************
 *
 *
 *  Project: libjpeg-turbo 3.2.0
 *  Source files:
 *    src/jmorecfg.h
 *    src/jpeglib.h
 *    src/jsamplecomp.h
 *    src/jpegint.h
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
 * src/jsamplecomp.h
 *
 * Copyright (C) 2022, 2026, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 *
 * src/jpegint.h
 *
 *   This file provides common declarations for the various JPEG modules.
 *   These declarations are considered internal to the JPEG library; most
 *   applications using the library shouldn't need to include this file.
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * Modified 1997-2009 by Guido Vollbeding.
 * Lossless JPEG Modifications:
 * Copyright (C) 1999, Ken Murchison.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2015-2017, 2019, 2021-2022, 2024-2026, D. R. Commander.
 * Copyright (C) 2015, Google, Inc.
 * Copyright (C) 2021, Alex Richardson.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 */

#ifndef KERNELS_45_H2V2_FANCY_UPSAMPLE_INCLUDE_KERNEL_H_
#define KERNELS_45_H2V2_FANCY_UPSAMPLE_INCLUDE_KERNEL_H_

/* src/jmorecfg.h:49 */
typedef unsigned char JSAMPLE;

/* src/jmorecfg.h:156 */
typedef unsigned int JDIMENSION;

/* src/jmorecfg.h:169 */
#define METHODDEF(type) static type

/* src/jpeglib.h:89-90 */
typedef JSAMPLE *JSAMPROW;
typedef JSAMPROW *JSAMPARRAY;

/* src/jsamplecomp.h:328,333-334 (BITS_IN_JSAMPLE == 8) */
#define _JSAMPLE JSAMPLE
#define _JSAMPROW JSAMPROW
#define _JSAMPARRAY JSAMPARRAY

/* src/jpeglib.h:340 */
typedef struct jpeg_decompress_struct *j_decompress_ptr;

/* src/jpeglib.h:205 (only the read member of jpeg_component_info) */
typedef struct {
    JDIMENSION downsampled_width;
} jpeg_component_info;

/* src/jpeglib.h:695 (only the read member of jpeg_decompress_struct) */
struct jpeg_decompress_struct {
    int max_v_samp_factor;
};

/* Wrapper for invoking the extracted kernel. */
void h2v2_fancy_upsample(j_decompress_ptr cinfo, jpeg_component_info *compptr,
                         _JSAMPARRAY input_data, _JSAMPARRAY *output_data_ptr);

#endif // KERNELS_45_H2V2_FANCY_UPSAMPLE_INCLUDE_KERNEL_H_
