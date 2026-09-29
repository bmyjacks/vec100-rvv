/****************************************************************************
 *
 *
 *  Project: FFmpeg n9.0.2
 *  Source files:
 *    libswscale/yuv2rgb.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * libswscale/yuv2rgb.c
 *
 *   software YUV to RGB converter
 *
 * Copyright (C) 2009 Konstantin Shishkov
 *
 * 1,4,8bpp support and context / deglobalize stuff
 * by Michael Niedermayer (michaelni@gmx.at)
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
 * libswscale/yuv2rgb.c:68-73
 */
#define LOADCHROMA(l, i)                                                       \
    U = pu_##l[i];                                                             \
    V = pv_##l[i];                                                             \
    r = reinterpret_cast<decltype(r)>(c->table_rV[V + YUVRGB_TABLE_HEADROOM]); \
    g = reinterpret_cast<decltype(g)>(c->table_gU[U + YUVRGB_TABLE_HEADROOM] + \
                                      c->table_gV[V + YUVRGB_TABLE_HEADROOM]); \
    b = reinterpret_cast<decltype(b)>(c->table_bU[U + YUVRGB_TABLE_HEADROOM]);

/*
 * libswscale/yuv2rgb.c:81-89
 */
#define PUTRGB24(l, i, abase)                                                  \
    Y = py_##l[2 * i];                                                         \
    dst_##l[6 * i + 0] = r[Y];                                                 \
    dst_##l[6 * i + 1] = g[Y];                                                 \
    dst_##l[6 * i + 2] = b[Y];                                                 \
    Y = py_##l[2 * i + 1];                                                     \
    dst_##l[6 * i + 3] = r[Y];                                                 \
    dst_##l[6 * i + 4] = g[Y];                                                 \
    dst_##l[6 * i + 5] = b[Y];

/*
 * libswscale/yuv2rgb.c:137-175
 */
#define YUV2RGBFUNC(func_name, dst_type, alpha, yuv422, nb_dst_planes)         \
    static int func_name(SwsInternal *c, const uint8_t *const src[],           \
                         const int srcStride[], int srcSliceY, int srcSliceH,  \
                         uint8_t *const dst[], const int dstStride[]) {        \
        int y;                                                                 \
                                                                               \
        for (y = 0; y < srcSliceH; y += 2) {                                   \
            int yd = y + srcSliceY;                                            \
            dst_type *dst_1 = (dst_type *)(dst[0] + (yd) * dstStride[0]);      \
            dst_type *dst_2 = (dst_type *)(dst[0] + (yd + 1) * dstStride[0]);  \
            av_unused dst_type *dst1_1, *dst1_2, *dst2_1, *dst2_2;             \
            av_unused dst_type *r, *g, *b;                                     \
            const uint8_t *py_1 = src[0] + y * srcStride[0];                   \
            const uint8_t *py_2 = py_1 + srcStride[0];                         \
            av_unused const uint8_t *pu_1 =                                    \
                src[1] + (y >> !yuv422) * srcStride[1];                        \
            av_unused const uint8_t *pv_1 =                                    \
                src[2] + (y >> !yuv422) * srcStride[2];                        \
            av_unused const uint8_t *pu_2, *pv_2;                              \
            av_unused const uint8_t *pa_1, *pa_2;                              \
            unsigned int h_size = c->opts.dst_w >> 3;                          \
            if (nb_dst_planes > 1) {                                           \
                dst1_1 = (dst_type *)(dst[1] + (yd) * dstStride[1]);           \
                dst1_2 = (dst_type *)(dst[1] + (yd + 1) * dstStride[1]);       \
                dst2_1 = (dst_type *)(dst[2] + (yd) * dstStride[2]);           \
                dst2_2 = (dst_type *)(dst[2] + (yd + 1) * dstStride[2]);       \
            }                                                                  \
            if (yuv422) {                                                      \
                pu_2 = pu_1 + srcStride[1];                                    \
                pv_2 = pv_1 + srcStride[2];                                    \
            }                                                                  \
            if (alpha) {                                                       \
                pa_1 = src[3] + y * srcStride[3];                              \
                pa_2 = pa_1 + srcStride[3];                                    \
            }                                                                  \
            while (h_size--) {                                                 \
                av_unused int U, V, Y;

/*
 * libswscale/yuv2rgb.c:176-199
 */
#define ENDYUV2RGBLINE(dst_delta, ss, alpha, yuv422, nb_dst_planes)            \
    pu_1 += 4 >> ss;                                                           \
    pv_1 += 4 >> ss;                                                           \
    if (yuv422) {                                                              \
        pu_2 += 4 >> ss;                                                       \
        pv_2 += 4 >> ss;                                                       \
    }                                                                          \
    py_1 += 8 >> ss;                                                           \
    py_2 += 8 >> ss;                                                           \
    if (alpha) {                                                               \
        pa_1 += 8 >> ss;                                                       \
        pa_2 += 8 >> ss;                                                       \
    }                                                                          \
    dst_1 += dst_delta >> ss;                                                  \
    dst_2 += dst_delta >> ss;                                                  \
    if (nb_dst_planes > 1) {                                                   \
        dst1_1 += dst_delta >> ss;                                             \
        dst1_2 += dst_delta >> ss;                                             \
        dst2_1 += dst_delta >> ss;                                             \
        dst2_2 += dst_delta >> ss;                                             \
    }                                                                          \
    }                                                                          \
    if (c->opts.dst_w & (4 >> ss)) {                                           \
        av_unused int Y, U, V;

/*
 * libswscale/yuv2rgb.c:201-205
 */
#define ENDYUV2RGBFUNC()                                                       \
    }                                                                          \
    }                                                                          \
    return srcSliceH;                                                          \
    }

/*
 * libswscale/yuv2rgb.c:207-236
 */
#define YUV420FUNC(func_name, dst_type, alpha, abase, PUTFUNC, dst_delta,      \
                   nb_dst_planes)                                              \
    YUV2RGBFUNC(func_name, dst_type, alpha, 0, nb_dst_planes)                  \
    LOADCHROMA(1, 0);                                                          \
    PUTFUNC(1, 0, abase);                                                      \
    PUTFUNC(2, 0, abase);                                                      \
                                                                               \
    LOADCHROMA(1, 1);                                                          \
    PUTFUNC(2, 1, abase);                                                      \
    PUTFUNC(1, 1, abase);                                                      \
                                                                               \
    LOADCHROMA(1, 2);                                                          \
    PUTFUNC(1, 2, abase);                                                      \
    PUTFUNC(2, 2, abase);                                                      \
                                                                               \
    LOADCHROMA(1, 3);                                                          \
    PUTFUNC(2, 3, abase);                                                      \
    PUTFUNC(1, 3, abase);                                                      \
    ENDYUV2RGBLINE(dst_delta, 0, alpha, 0, nb_dst_planes)                      \
    LOADCHROMA(1, 0);                                                          \
    PUTFUNC(1, 0, abase);                                                      \
    PUTFUNC(2, 0, abase);                                                      \
                                                                               \
    LOADCHROMA(1, 1);                                                          \
    PUTFUNC(2, 1, abase);                                                      \
    PUTFUNC(1, 1, abase);                                                      \
    ENDYUV2RGBLINE(dst_delta, 1, alpha, 0, nb_dst_planes)                      \
    LOADCHROMA(1, 0);                                                          \
    PUTFUNC(1, 0, abase);                                                      \
    PUTFUNC(2, 0, abase);                                                      \
    ENDYUV2RGBFUNC()

/*
 * libswscale/yuv2rgb.c:530
 */
YUV420FUNC(yuv2rgb_c_24_rgb_upstream, uint8_t, 0, 0, PUTRGB24, 24, 1)

/*
 * Wrapper for invoking the extracted kernel.
 */
int yuv2rgb_c_24_rgb(SwsInternal *c, const uint8_t *const src[],
                     const int srcStride[], int srcSliceY, int srcSliceH,
                     uint8_t *const dst[], const int dstStride[]) {
    return yuv2rgb_c_24_rgb_upstream(c, src, srcStride, srcSliceY, srcSliceH,
                                     dst, dstStride);
}
