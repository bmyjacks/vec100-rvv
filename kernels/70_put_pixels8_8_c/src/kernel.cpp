/****************************************************************************
 *
 *
 *  Project: FFmpeg n9.0.2
 *  Source files:
 *    libavcodec/pel_template.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * libavcodec/pel_template.c
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
 * libavcodec/pel_template.c:55-68
 * Selected put_pixels8_8_c expansion of DEF_PEL(put, op_put).
 */
static inline void put_pixels8_8_c_impl(uint8_t *block, const uint8_t *pixels,
                                        ptrdiff_t line_size, int h) {
    int i;
    for (i = 0; i < h; i++) {
        *((pixel4 *)block) = AV_RN4P(pixels);
        *((pixel4 *)(block + 4 * sizeof(pixel))) =
            AV_RN4P(pixels + 4 * sizeof(pixel));
        pixels += line_size;
        block += line_size;
    }
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void put_pixels8_8_c(uint8_t *block, const uint8_t *pixels, ptrdiff_t line_size,
                     int h) {
    put_pixels8_8_c_impl(block, pixels, line_size, h);
}
