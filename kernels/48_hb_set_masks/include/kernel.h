/****************************************************************************
 *
 *
 *  Project: HarfBuzz 14.4.0
 *  Source files:
 *    src/hb-common.h
 *    src/hb-buffer.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 * src/hb-common.h
 *
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 *
 * src/hb-buffer.h
 *
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2004,2007,2009,2010  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
 * Red Hat Author(s): Owen Taylor, Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef KERNELS_48_HB_SET_MASKS_INCLUDE_KERNEL_H_
#define KERNELS_48_HB_SET_MASKS_INCLUDE_KERNEL_H_

#include <cstdint>

/*
 * src/hb-common.h:104
 */
typedef uint32_t hb_codepoint_t;

/*
 * src/hb-common.h:130-139
 */
typedef uint32_t hb_mask_t;

typedef union _hb_var_int_t {
    uint32_t u32;
    int32_t i32;
    uint16_t u16[2];
    int16_t i16[2];
    uint8_t u8[4];
    int8_t i8[4];
} hb_var_int_t;

/*
 * src/hb-buffer.h:62-72 (only documentation comments removed)
 */
typedef struct hb_glyph_info_t {
    hb_codepoint_t codepoint;
    hb_mask_t mask;
    uint32_t cluster;

    hb_var_int_t var1;
    hb_var_int_t var2;
} hb_glyph_info_t;

/*
 * Wrapper for the two selected loops in src/hb-buffer.cc:548-557.
 * info is an array of len live hb_glyph_info_t records (or may be null if
 * len == 0); all other fields are preserved. Inputs are copied by value.
 * Updates are in place; there is no separate input/output buffer to alias.
 */
extern "C" void hb_017_buffer_set_masks_isolated(hb_glyph_info_t *info, unsigned int len,
                                         hb_mask_t value, hb_mask_t mask,
                                         unsigned int cluster_start,
                                         unsigned int cluster_end);
extern "C" void hb_017_buffer_set_masks_rvv(hb_glyph_info_t *info, unsigned int len,
                                             hb_mask_t value, hb_mask_t mask,
                                             unsigned int cluster_start,
                                             unsigned int cluster_end);

#endif // KERNELS_48_HB_SET_MASKS_INCLUDE_KERNEL_H_
