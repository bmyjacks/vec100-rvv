/****************************************************************************
 *
 *
 *  Project: libjpeg-turbo 3.2.0
 *  Source files:
 *    src/jcsample.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 * src/jcsample.c
 *
 * This file contains downsampling routines.
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * Lossless JPEG Modifications:
 * Copyright (C) 1999, Ken Murchison.
 * libjpeg-turbo Modifications:
 * Copyright 2009 Pierre Ossman <ossman@cendio.se> for Cendio AB
 * Copyright (C) 2015, 2019, 2022, 2024-2026, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README.ijg
 * file.
 */

#include "kernel.h"

/* src/jcsample.c:99-117 */
static void expand_right_edge(_JSAMPARRAY image_data, int num_rows,
                              JDIMENSION input_cols, JDIMENSION output_cols) {
    _JSAMPROW ptr;
    _JSAMPLE pixval;
    int count;
    int row;
    int numcols = (int)(output_cols - input_cols);

    if (numcols > 0) {
        for (row = 0; row < num_rows; row++) {
            ptr = image_data[row] + input_cols;
            pixval = ptr[-1];
            for (count = numcols; count > 0; count--)
                *ptr++ = pixval;
        }
    }
}

/* src/jcsample.c:152-191 */
static void int_downsample(j_compress_ptr cinfo, jpeg_component_info *compptr,
                           _JSAMPARRAY input_data, _JSAMPARRAY output_data) {
    int inrow, outrow, h_expand, v_expand, numpix, numpix2, h, v;
    JDIMENSION outcol, outcol_h;
    int data_unit = cinfo->master->lossless ? 1 : DCTSIZE;
    JDIMENSION output_cols = compptr->width_in_blocks * data_unit;
    _JSAMPROW inptr, outptr;
    JLONG outvalue;

    h_expand = cinfo->max_h_samp_factor / compptr->h_samp_factor;
    v_expand = cinfo->max_v_samp_factor / compptr->v_samp_factor;
    numpix = h_expand * v_expand;
    numpix2 = numpix / 2;

    expand_right_edge(input_data, cinfo->max_v_samp_factor, cinfo->image_width,
                      output_cols * h_expand);

    inrow = 0;
    for (outrow = 0; outrow < compptr->v_samp_factor; outrow++) {
        outptr = output_data[outrow];
        for (outcol = 0, outcol_h = 0; outcol < output_cols;
             outcol++, outcol_h += h_expand) {
            outvalue = 0;
            for (v = 0; v < v_expand; v++) {
                inptr = input_data[inrow + v] + outcol_h;
                for (h = 0; h < h_expand; h++) {
                    outvalue += (JLONG)(*inptr++);
                }
            }
            *outptr++ = (_JSAMPLE)((outvalue + numpix2) / numpix);
        }
        inrow += v_expand;
    }
}

/* Wrapper for invoking the extracted kernel. */
void jpeg_int_downsample(j_compress_ptr cinfo, jpeg_component_info *compptr,
                         _JSAMPARRAY input_data, _JSAMPARRAY output_data) {
    int_downsample(cinfo, compptr, input_data, output_data);
}
