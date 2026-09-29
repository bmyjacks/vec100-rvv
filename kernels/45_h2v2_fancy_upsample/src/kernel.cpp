/****************************************************************************
 *
 *
 *  Project: libjpeg-turbo 3.2.0
 *  Source files:
 *    src/jdsample.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/jdsample.c
 *
 *   This file contains upsampling routines.
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * libjpeg-turbo Modifications:
 * Copyright 2009 Pierre Ossman <ossman@cendio.se> for Cendio AB
 * Copyright (C) 2010, 2015-2016, 2022, 2024-2026, D. R. Commander.
 * Copyright (C) 2015, Google, Inc.
 * Copyright (C) 2019-2020, Arm Limited.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 *
 */

#include "kernel.h"

/*
 * src/jdsample.c:381-429
 */
METHODDEF(void)
h2v2_fancy_upsample_impl(j_decompress_ptr cinfo, jpeg_component_info *compptr,
                         _JSAMPARRAY input_data, _JSAMPARRAY *output_data_ptr) {
    _JSAMPARRAY output_data = *output_data_ptr;
    _JSAMPROW inptr0, inptr1, outptr;
    int thiscolsum, lastcolsum, nextcolsum;
    JDIMENSION colctr;
    int inrow, outrow, v;

    inrow = outrow = 0;
    while (outrow < cinfo->max_v_samp_factor) {
        for (v = 0; v < 2; v++) {
            inptr0 = input_data[inrow];
            if (v == 0)
                inptr1 = input_data[inrow - 1];
            else
                inptr1 = input_data[inrow + 1];
            outptr = output_data[outrow++];

            thiscolsum = (*inptr0++) * 3 + (*inptr1++);
            nextcolsum = (*inptr0++) * 3 + (*inptr1++);
            *outptr++ = (_JSAMPLE)((thiscolsum * 4 + 8) >> 4);
            *outptr++ = (_JSAMPLE)((thiscolsum * 3 + nextcolsum + 7) >> 4);
            lastcolsum = thiscolsum;
            thiscolsum = nextcolsum;

            for (colctr = compptr->downsampled_width - 2; colctr > 0;
                 colctr--) {
                nextcolsum = (*inptr0++) * 3 + (*inptr1++);
                *outptr++ = (_JSAMPLE)((thiscolsum * 3 + lastcolsum + 8) >> 4);
                *outptr++ = (_JSAMPLE)((thiscolsum * 3 + nextcolsum + 7) >> 4);
                lastcolsum = thiscolsum;
                thiscolsum = nextcolsum;
            }

            *outptr++ = (_JSAMPLE)((thiscolsum * 3 + lastcolsum + 8) >> 4);
            *outptr++ = (_JSAMPLE)((thiscolsum * 4 + 7) >> 4);
        }
        inrow++;
    }
}

/* Wrapper for invoking the extracted kernel. */
void h2v2_fancy_upsample(j_decompress_ptr cinfo, jpeg_component_info *compptr,
                         _JSAMPARRAY input_data, _JSAMPARRAY *output_data_ptr) {
    h2v2_fancy_upsample_impl(cinfo, compptr, input_data, output_data_ptr);
}
