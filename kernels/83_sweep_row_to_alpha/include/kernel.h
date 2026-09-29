/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-raster-draw.cc
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/hb-raster-draw.cc
 *
 * Copyright © 2026  Behdad Esfahbod
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Author(s): Behdad Esfahbod
 *
 */

#ifndef KERNELS_83_SWEEP_ROW_TO_ALPHA_INCLUDE_KERNEL_H_
#define KERNELS_83_SWEEP_ROW_TO_ALPHA_INCLUDE_KERNEL_H_

#include <stdint.h>

/*
 * src/hb-raster-draw.cc:44-48
 */
#define HB_RASTER_PIXEL_BITS 8
#define HB_RASTER_ONE_PIXEL (1 << HB_RASTER_PIXEL_BITS)

#define HB_RASTER_FULL_COVERAGE (2 * HB_RASTER_ONE_PIXEL * HB_RASTER_ONE_PIXEL)

/*
 * Wrapper for invoking the extracted kernel.
 */
extern "C" int32_t sweep_row_to_alpha(uint8_t *__restrict row_buf,
                                      int32_t *__restrict area,
                                      int16_t *__restrict cover, unsigned x_min,
                                      unsigned x_max);

#endif // KERNELS_83_SWEEP_ROW_TO_ALPHA_INCLUDE_KERNEL_H_
