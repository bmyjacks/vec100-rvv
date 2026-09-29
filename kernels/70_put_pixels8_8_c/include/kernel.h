/****************************************************************************
 *
 *
 *  Project: FFmpeg n9.0.2
 *  Source files:
 *    libavcodec/bit_depth_template.c
 *    libavutil/intreadwrite.h
 *    libavutil/attributes.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * libavcodec/bit_depth_template.c
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
 *
 * libavutil/intreadwrite.h
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
 *
 * libavutil/attributes.h
 *
 * copyright (c) 2006 Michael Niedermayer <michaelni@gmx.at>
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

#ifndef KERNELS_70_PUT_PIXELS8_8_C_INCLUDE_KERNEL_H_
#define KERNELS_70_PUT_PIXELS8_8_C_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

/*
 * libavutil/attributes.h:180-184
 */
#define av_alias __attribute__((may_alias))

/*
 * libavutil/intreadwrite.h:214-221
 */
union unaligned_32 {
    uint32_t l;
} __attribute__((packed)) av_alias;

#define AV_RN(s, p) (((const union unaligned_##s *)(p))->l)

/*
 * libavutil/intreadwrite.h:359-361
 */
#define AV_RN32(p) AV_RN(32, p)

/*
 * libavcodec/bit_depth_template.c:82-84
 */
#define pixel uint8_t
#define pixel4 uint32_t

/*
 * libavcodec/bit_depth_template.c:91
 */
#define AV_RN4P AV_RN32

/*
 * Wrapper for invoking the extracted kernel.
 */
void put_pixels8_8_c(uint8_t *block, const uint8_t *pixels, ptrdiff_t line_size,
                     int h);

#endif // KERNELS_70_PUT_PIXELS8_8_C_INCLUDE_KERNEL_H_
