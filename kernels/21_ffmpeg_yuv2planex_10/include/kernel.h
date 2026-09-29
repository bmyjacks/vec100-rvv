/****************************************************************************
 *
 *
 *  Project: FFmpeg n9.0.2
 *  Source files:
 *    libavutil/attributes.h
 *    libavutil/intreadwrite.h
 *    libavutil/bswap.h
 *    libavutil/common.h
 *
 *
 *  The original file copyright and license notices follow.
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
 * libavutil/bswap.h
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
 *
 * libavutil/common.h
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

#ifndef KERNELS_21_FFMPEG_YUV2PLANEX_10_INCLUDE_KERNEL_H_
#define KERNELS_21_FFMPEG_YUV2PLANEX_10_INCLUDE_KERNEL_H_

#include <cstdint>

/*
 * libavutil/attributes.h:70-78 (GCC/Clang selection)
 */
#define av_always_inline __attribute__((always_inline)) inline

/*
 * libavutil/attributes.h:110-114 (GCC/Clang selection)
 */
#define av_const __attribute__((const))

/*
 * libavutil/attributes.h:180-184 (GCC/Clang selection)
 */
#define av_alias __attribute__((may_alias))

/*
 * libavutil/intreadwrite.h:214-221 (GCC/Clang selection)
 */
union unaligned_16 {
    uint16_t l;
} __attribute__((packed)) av_alias;
#define AV_WN(s, p, v) ((((union unaligned_##s *)(p))->l) = (v))

/*
 * libavutil/intreadwrite.h:367-369
 */
#define AV_WN16(p, v) AV_WN(16, p, v)

/*
 * libavutil/intreadwrite.h:379-389 (little-endian selection)
 */
#define AV_WB(s, p, v) AV_WN##s(p, av_bswap##s(v))
#define AV_WL(s, p, v) AV_WN##s(p, v)

/*
 * libavutil/intreadwrite.h:400-408
 */
#define AV_WB16(p, v) AV_WB(16, p, v)
#define AV_WL16(p, v) AV_WL(16, p, v)

/*
 * libavutil/bswap.h:53-59
 */
static av_always_inline av_const uint16_t av_bswap16(uint16_t x) {
    x = (x >> 8) | (x << 8);
    return x;
}

/*
 * libavutil/common.h:123-125
 */
#define av_clip_uintp2 av_clip_uintp2_c

/*
 * libavutil/common.h:274-284
 */
static av_always_inline av_const unsigned av_clip_uintp2_c(int a, int p) {
    if (a & ~((1U << p) - 1))
        return (~a) >> 31 & ((1U << p) - 1);
    else
        return a;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void yuv2planeX_10_c_template(const int16_t *filter, int filterSize,
                              const int16_t **src, uint16_t *dest, int dstW,
                              int big_endian, int output_bits);

#endif // KERNELS_21_FFMPEG_YUV2PLANEX_10_INCLUDE_KERNEL_H_
