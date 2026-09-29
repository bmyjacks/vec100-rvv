/****************************************************************************
 *
 *
 *  Project: FFmpeg n9.0.2
 *  Source files:
 *    libswscale/output.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * libswscale/output.c
 *
 * Copyright (C) 2001-2012 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include "kernel.h"

/*
 * libswscale/output.c:320-325
 */
#define output_pixel(pos, val)                                                 \
    if (big_endian) {                                                          \
        AV_WB16(pos, av_clip_uintp2(val >> shift, output_bits));               \
    } else {                                                                   \
        AV_WL16(pos, av_clip_uintp2(val >> shift, output_bits));               \
    }

/*
 * libswscale/output.c:340-357
 */
static av_always_inline void
yuv2planeX_10_c_template_upstream(const int16_t *filter, int filterSize,
                                  const int16_t **src, uint16_t *dest, int dstW,
                                  int big_endian, int output_bits) {
    int i;
    int shift = 11 + 16 - output_bits;

    for (i = 0; i < dstW; i++) {
        int val = 1 << (shift - 1);
        int j;

        for (j = 0; j < filterSize; j++)
            val += src[j][i] * filter[j];

        output_pixel(&dest[i], val);
    }
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void yuv2planeX_10_c_template(const int16_t *filter, int filterSize,
                              const int16_t **src, uint16_t *dest, int dstW,
                              int big_endian, int output_bits) {
    yuv2planeX_10_c_template_upstream(filter, filterSize, src, dest, dstW,
                                      big_endian, output_bits);
}
