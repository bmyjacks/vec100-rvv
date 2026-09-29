/****************************************************************************
 *
 *
 *  Project: FFmpeg n9.0.2
 *  Source files:
 *    libavutil/attributes.h
 *    libavutil/macros.h
 *    libavutil/mem_internal.h
 *    libavfilter/framepool.h
 *    libswscale/swscale.h
 *    configure
 *    libswscale/swscale_internal.h
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
 * libavutil/macros.h
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
 * libavutil/mem_internal.h
 *
 * Copyright (c) 2002 Fabrice Bellard
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
 * libavfilter/framepool.h
 *
 * This file is part of FFmpeg.
 *
 * Copyright (c) 2015 Matthieu Bouron <matthieu.bouron stupeflix.com>
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
 * libswscale/swscale.h
 *
 * Copyright (C) 2024 Niklas Haas
 * Copyright (C) 2001-2011 Michael Niedermayer <michaelni@gmx.at>
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
 * configure
 *
 * FFmpeg configure script
 *
 * Copyright (c) 2000-2002 Fabrice Bellard
 * Copyright (c) 2005-2008 Diego Biurrun
 * Copyright (c) 2005-2008 Mans Rullgard
 *
 *
 * libswscale/swscale_internal.h
 *
 * Copyright (C) 2001-2011 Michael Niedermayer <michaelni@gmx.at>
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

#ifndef KERNELS_98_YUV2RGB_C_24_RGB_INCLUDE_KERNEL_H_
#define KERNELS_98_YUV2RGB_C_24_RGB_INCLUDE_KERNEL_H_

#include <atomic>
#include <cstddef>
#include <cstdint>

/*
 * libavutil/attributes.h:29-60
 */
#ifdef __GNUC__
#define AV_GCC_VERSION_AT_LEAST(x, y)                                          \
    (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#define AV_GCC_VERSION_AT_MOST(x, y)                                           \
    (__GNUC__ < (x) || __GNUC__ == (x) && __GNUC_MINOR__ <= (y))
#else
#define AV_GCC_VERSION_AT_LEAST(x, y) 0
#define AV_GCC_VERSION_AT_MOST(x, y) 0
#endif

#ifdef __has_builtin
#define AV_HAS_BUILTIN(x) __has_builtin(x)
#else
#define AV_HAS_BUILTIN(x) 0
#endif

#ifdef __has_attribute
#define AV_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#define AV_HAS_ATTRIBUTE(x) 0
#endif

#if defined(__cplusplus) && defined(__has_cpp_attribute) &&                    \
    __cplusplus >= 201103L
#define AV_HAS_STD_ATTRIBUTE(x) __has_cpp_attribute(x)
#elif !defined(__cplusplus) && defined(__has_c_attribute) &&                   \
    defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define AV_HAS_STD_ATTRIBUTE(x) __has_c_attribute(x)
#else
#define AV_HAS_STD_ATTRIBUTE(x) 0
#endif

/*
 * libavutil/attributes.h:161-167
 */
#if AV_HAS_STD_ATTRIBUTE(maybe_unused)
#define av_unused [[maybe_unused]]
#elif defined(__GNUC__) || defined(__clang__)
#define av_unused __attribute__((unused))
#else
#define av_unused
#endif

/*
 * libavutil/attributes.h:174-178
 */
#if AV_GCC_VERSION_AT_LEAST(3, 1) || defined(__clang__)
#define av_used __attribute__((used))
#else
#define av_used
#endif

/*
 * libavutil/macros.h:49
 */
#define FFMIN(a, b) ((a) > (b) ? (b) : (a))

/*
 * libavutil/mem_internal.h:79-114
 */
#if defined(__DJGPP__)
#define DECLARE_ALIGNED_T(n, t, v) alignas(FFMIN(n, 16)) t v
#define DECLARE_ASM_ALIGNED(n, t, v) alignas(FFMIN(n, 16)) t av_used v
#define DECLARE_ASM_CONST(n, t, v)                                             \
    alignas(FFMIN(n, 16)) static const t av_used v
#elif defined(_MSC_VER)
#define DECLARE_ALIGNED_T(n, t, v) __declspec(align(n)) t v
#define DECLARE_ASM_ALIGNED(n, t, v) __declspec(align(n)) t av_used v
#define DECLARE_ASM_CONST(n, t, v) __declspec(align(n)) static const t av_used v
#else
#define DECLARE_ALIGNED_T(n, t, v) alignas(n) t v
#define DECLARE_ASM_ALIGNED(n, t, v) alignas(n) t av_used v
#define DECLARE_ASM_CONST(n, t, v) alignas(n) static const t av_used v
#endif

#if HAVE_SIMD_ALIGN_64
#define ALIGN_64 64
#define ALIGN_32 32
#elif HAVE_SIMD_ALIGN_32
#define ALIGN_64 32
#define ALIGN_32 32
#else
#define ALIGN_64 16
#define ALIGN_32 16
#endif

#define DECLARE_ALIGNED(n, t, v) DECLARE_ALIGNED_V(n, t, v)

#define DECLARE_ALIGNED_V(n, t, v) DECLARE_ALIGNED_##n(t, v)

#define DECLARE_ALIGNED_4(t, v) DECLARE_ALIGNED_T(4, t, v)
#define DECLARE_ALIGNED_8(t, v) DECLARE_ALIGNED_T(8, t, v)
#define DECLARE_ALIGNED_16(t, v) DECLARE_ALIGNED_T(16, t, v)
#define DECLARE_ALIGNED_32(t, v) DECLARE_ALIGNED_T(ALIGN_32, t, v)
#define DECLARE_ALIGNED_64(t, v) DECLARE_ALIGNED_T(ALIGN_64, t, v)

/*
 * The full upstream structures below need enum types normally supplied by
 * libavutil headers. Their enumerators are not used here; fixed-width opaque
 * enum declarations preserve their int-sized storage in this C++ extraction.
 * AVClass, AVFrame, AVBufferPool, AVSliceThread, SwsGraph, Half2FloatTables,
 * SwsSlice and SwsFilterDescriptor are only used through pointers.
 */
enum AVMediaType : int;
enum AVPixelFormat : int;
enum AVSampleFormat : int;
struct AVClass;
struct AVFrame;
struct AVBufferPool;
struct AVSliceThread;
struct SwsGraph;
struct Half2FloatTables;
struct SwsSlice;
struct SwsFilterDescriptor;

/*
 * libavfilter/framepool.h:32-54
 */
typedef struct FFFramePool {
    enum AVMediaType type;
    union {
        enum AVPixelFormat pix_fmt;
        enum AVSampleFormat sample_fmt;
    };
    int width;
    int height;
    int planes;
    int channels;
    int nb_samples;
    int align;
    int linesize[4];
    AVBufferPool *pools[4];
} FFFramePool;

/*
 * libswscale/swscale.h:77-85
 */
typedef enum SwsDither {
    SWS_DITHER_NONE = 0,
    SWS_DITHER_AUTO,
    SWS_DITHER_BAYER,
    SWS_DITHER_ED,
    SWS_DITHER_A_DITHER,
    SWS_DITHER_X_DITHER,
    SWS_DITHER_NB,
    SWS_DITHER_MAX_ENUM = 0x7FFFFFFF,
} SwsDither;

/*
 * libswscale/swscale.h:88-94
 */
typedef enum SwsAlphaBlend {
    SWS_ALPHA_BLEND_NONE = 0,
    SWS_ALPHA_BLEND_UNIFORM,
    SWS_ALPHA_BLEND_CHECKERBOARD,
    SWS_ALPHA_BLEND_NB,
    SWS_ALPHA_BLEND_MAX_ENUM = 0x7FFFFFFF,
} SwsAlphaBlend;

/*
 * libswscale/swscale.h:96-108
 */
typedef enum SwsScaler {
    SWS_SCALE_AUTO = 0,
    SWS_SCALE_BILINEAR,
    SWS_SCALE_BICUBIC,
    SWS_SCALE_POINT,
    SWS_SCALE_AREA,
    SWS_SCALE_GAUSSIAN,
    SWS_SCALE_SINC,
    SWS_SCALE_LANCZOS,
    SWS_SCALE_SPLINE,
    SWS_SCALE_NB,
    SWS_SCALE_MAX_ENUM = 0x7FFFFFFF,
} SwsScaler;

/*
 * libswscale/swscale.h:110-129
 */
typedef enum SwsBackend {
    SWS_BACKEND_LEGACY = (1 << 0),
    SWS_BACKEND_STABLE = SWS_BACKEND_LEGACY,
    SWS_BACKEND_C = (1 << 1),
    SWS_BACKEND_MEMCPY = (1 << 2),
    SWS_BACKEND_X86 = (1 << 3),
    SWS_BACKEND_AARCH64 = (1 << 4),
    SWS_BACKEND_SPIRV = (1 << 5),
    SWS_BACKEND_UNSTABLE = SWS_BACKEND_C | SWS_BACKEND_MEMCPY |
                           SWS_BACKEND_X86 | SWS_BACKEND_AARCH64 |
                           SWS_BACKEND_SPIRV,
    SWS_BACKEND_ALL = SWS_BACKEND_STABLE | SWS_BACKEND_UNSTABLE,
    SWS_BACKEND_MAX_ENUM = 0x7FFFFFFF,
} SwsBackend;

/*
 * libswscale/swscale.h:227-315
 */
typedef struct SwsContext {
    const AVClass *av_class;
    void *opaque;
    unsigned flags;
#define SWS_NUM_SCALER_PARAMS 2
    double scaler_params[SWS_NUM_SCALER_PARAMS];
    int threads;
    SwsDither dither;
    SwsAlphaBlend alpha_blend;
    int gamma_flag;
    int src_w, src_h;
    int dst_w, dst_h;
    int src_format;
    int dst_format;
    int src_range;
    int dst_range;
    int src_v_chr_pos;
    int src_h_chr_pos;
    int dst_v_chr_pos;
    int dst_h_chr_pos;
    int intent;
    SwsScaler scaler;
    SwsScaler scaler_sub;
    SwsBackend backends;
} SwsContext;

/*
 * libswscale/swscale_internal.h:52
 */
#define YUVRGB_TABLE_HEADROOM 512

/*
 * configure:4476-4477,8781 (generates SWS_MAX_FILTER_SIZE in config.h;
 * 256 is the upstream default configuration)
 */
#ifndef SWS_MAX_FILTER_SIZE
#define SWS_MAX_FILTER_SIZE 256
#endif

/*
 * libswscale/swscale_internal.h:55
 */
#define MAX_FILTER_SIZE SWS_MAX_FILTER_SIZE

/*
 * libswscale/swscale_internal.h:521
 */
#define DITHER32_INT (11 * 8 + 4 * 4 * MAX_FILTER_SIZE * 3 + 80)

/*
 * libswscale/swscale_internal.h:77
 */
typedef struct SwsInternal SwsInternal;

/*
 * libswscale/swscale_internal.h:86-89
 */
typedef struct Range {
    unsigned int start;
    unsigned int len;
} Range;

/*
 * libswscale/swscale_internal.h:91-95
 */
typedef struct RangeList {
    Range *ranges;
    unsigned int nb_ranges;
    int ranges_allocated;
} RangeList;

/*
 * libswscale/swscale_internal.h:99-114
 */
typedef int (*SwsFunc)(SwsInternal *c, const uint8_t *const src[],
                       const int srcStride[], int srcSliceY, int srcSliceH,
                       uint8_t *const dst[], const int dstStride[]);
typedef void (*SwsColorFunc)(const SwsInternal *c, uint8_t *dst, int dst_stride,
                             const uint8_t *src, int src_stride, int w, int h);
typedef struct SwsLuts {
    uint16_t *in;
    uint16_t *out;
} SwsLuts;
typedef struct SwsColorXform {
    SwsLuts gamma;
    int16_t mat[3][3];
} SwsColorXform;

/*
 * libswscale/swscale_internal.h:128-129
 */
typedef void (*yuv2planar1_fn)(const int16_t *src, uint8_t *dest, int dstW,
                               const uint8_t *dither, int offset);
/*
 * libswscale/swscale_internal.h:144-146
 */
typedef void (*yuv2planarX_fn)(const int16_t *filter, int filterSize,
                               const int16_t **src, uint8_t *dest, int dstW,
                               const uint8_t *dither, int offset);
/*
 * libswscale/swscale_internal.h:164-170
 */
typedef void (*yuv2interleavedX_fn)(enum AVPixelFormat dstFormat,
                                    const uint8_t *chrDither,
                                    const int16_t *chrFilter, int chrFilterSize,
                                    const int16_t **chrUSrc,
                                    const int16_t **chrVSrc, uint8_t *dest,
                                    int dstW);
/*
 * libswscale/swscale_internal.h:201-205
 */
typedef void (*yuv2packed1_fn)(SwsInternal *c, const int16_t *lumSrc,
                               const int16_t *chrUSrc[2],
                               const int16_t *chrVSrc[2], const int16_t *alpSrc,
                               uint8_t *dest, int dstW, int uvalpha, int y);
/*
 * libswscale/swscale_internal.h:234-239
 */
typedef void (*yuv2packed2_fn)(SwsInternal *c, const int16_t *lumSrc[2],
                               const int16_t *chrUSrc[2],
                               const int16_t *chrVSrc[2],
                               const int16_t *alpSrc[2], uint8_t *dest,
                               int dstW, int yalpha, int uvalpha, int y);
/*
 * libswscale/swscale_internal.h:266-272
 */
typedef void (*yuv2packedX_fn)(SwsInternal *c, const int16_t *lumFilter,
                               const int16_t **lumSrc, int lumFilterSize,
                               const int16_t *chrFilter,
                               const int16_t **chrUSrc, const int16_t **chrVSrc,
                               int chrFilterSize, const int16_t **alpSrc,
                               uint8_t *dest, int dstW, int y);
/*
 * libswscale/swscale_internal.h:300-306
 */
typedef void (*yuv2anyX_fn)(SwsInternal *c, const int16_t *lumFilter,
                            const int16_t **lumSrc, int lumFilterSize,
                            const int16_t *chrFilter, const int16_t **chrUSrc,
                            const int16_t **chrVSrc, int chrFilterSize,
                            const int16_t **alpSrc, uint8_t **dest, int dstW,
                            int y);
/*
 * libswscale/swscale_internal.h:311-313
 */
typedef void (*planar1_YV12_fn)(uint8_t *dst, const uint8_t *src,
                                const uint8_t *src2, const uint8_t *src3,
                                int width, uint32_t *pal, void *opaque);
/*
 * libswscale/swscale_internal.h:318-320
 */
typedef void (*planar2_YV12_fn)(uint8_t *dst, uint8_t *dst2, const uint8_t *src,
                                const uint8_t *src2, const uint8_t *src3,
                                int width, uint32_t *pal, void *opaque);
/*
 * libswscale/swscale_internal.h:326-327
 */
typedef void (*planarX_YV12_fn)(uint8_t *dst, const uint8_t *src[4], int width,
                                int32_t *rgb2yuv, void *opaque);
/*
 * libswscale/swscale_internal.h:329-331
 */
typedef void (*planarX2_YV12_fn)(uint8_t *dst, uint8_t *dst2,
                                 const uint8_t *src[4], int width,
                                 int32_t *rgb2yuv, void *opaque);

/*
 * libswscale/swscale_internal.h:337-707
 * Complete field order; upstream implementation comments omitted.
 * std::atomic<int> is the C++14 counterpart to C11 atomic_int. No
 * layout-dependent upstream allocator or scaler code is linked into this
 * isolated extraction.
 */
struct SwsInternal {
    SwsContext opts;
    SwsContext *parent;
    AVSliceThread *slicethread;
    SwsContext **slice_ctx;
    int *slice_err;
    int nb_slice_ctx;
    SwsGraph *graph[2];
    int dst_slice_start;
    int dst_slice_height;
    SwsFunc convert_unscaled;
    int chrSrcW;
    int chrSrcH;
    int chrDstW;
    int chrDstH;
    int lumXInc, chrXInc;
    int lumYInc, chrYInc;
    int dstFormatBpp;
    int srcFormatBpp;
    int dstBpc, srcBpc;
    int chrSrcHSubSample;
    int chrSrcVSubSample;
    int chrDstHSubSample;
    int chrDstVSubSample;
    int vChrDrop;
    int sliceDir;
    AVFrame *frame_src;
    AVFrame *frame_dst;
    RangeList src_ranges;
    SwsContext *cascaded_context[3];
    int cascaded_tmpStride[2][4];
    uint8_t *cascaded_tmp[2][4];
    int cascaded_mainindex;
    double gamma_value;
    int is_internal_gamma;
    uint16_t *gamma;
    uint16_t *inv_gamma;
    int numDesc;
    int descIndex[2];
    int numSlice;
    struct SwsSlice *slice;
    struct SwsFilterDescriptor *desc;
    uint32_t pal_yuv[256];
    uint32_t pal_rgb[256];
    float uint2float_lut[256];
    int lastInLumBuf;
    int lastInChrBuf;
    uint8_t *formatConvBuffer;
    int needAlpha;
    int16_t *hLumFilter;
    int16_t *hChrFilter;
    int16_t *vLumFilter;
    int16_t *vChrFilter;
    int32_t *hLumFilterPos;
    int32_t *hChrFilterPos;
    int32_t *vLumFilterPos;
    int32_t *vChrFilterPos;
    int hLumFilterSize;
    int hChrFilterSize;
    int vLumFilterSize;
    int vChrFilterSize;
    int lumMmxextFilterCodeSize;
    int chrMmxextFilterCodeSize;
    uint8_t *lumMmxextFilterCode;
    uint8_t *chrMmxextFilterCode;
    int canMMXEXTBeUsed;
    int warned_unuseable_bilinear;
    int dstY;
    void *yuvTable;
    DECLARE_ALIGNED(16, int, table_gV)[256 + 2 * YUVRGB_TABLE_HEADROOM];
    uint8_t *table_rV[256 + 2 * YUVRGB_TABLE_HEADROOM];
    uint8_t *table_gU[256 + 2 * YUVRGB_TABLE_HEADROOM];
    uint8_t *table_bU[256 + 2 * YUVRGB_TABLE_HEADROOM];
    DECLARE_ALIGNED(16, int32_t, input_rgb2yuv_table)[16 + 40 * 4];
    int *dither_error[4];
    int contrast, brightness, saturation;
    int srcColorspaceTable[4];
    int dstColorspaceTable[4];
    int src0Alpha;
    int dst0Alpha;
    int srcXYZ;
    int dstXYZ;
    int yuv2rgb_y_offset;
    int yuv2rgb_y_coeff;
    int yuv2rgb_v2r_coeff;
    int yuv2rgb_v2g_coeff;
    int yuv2rgb_u2g_coeff;
    int yuv2rgb_u2b_coeff;
    DECLARE_ALIGNED(8, uint64_t, redDither);
    DECLARE_ALIGNED(8, uint64_t, greenDither);
    DECLARE_ALIGNED(8, uint64_t, blueDither);
    DECLARE_ALIGNED(8, uint64_t, yCoeff);
    DECLARE_ALIGNED(8, uint64_t, vrCoeff);
    DECLARE_ALIGNED(8, uint64_t, ubCoeff);
    DECLARE_ALIGNED(8, uint64_t, vgCoeff);
    DECLARE_ALIGNED(8, uint64_t, ugCoeff);
    DECLARE_ALIGNED(8, uint64_t, yOffset);
    DECLARE_ALIGNED(8, uint64_t, uOffset);
    DECLARE_ALIGNED(8, uint64_t, vOffset);
    int32_t lumMmxFilter[4 * MAX_FILTER_SIZE];
    int32_t chrMmxFilter[4 * MAX_FILTER_SIZE];
    int dstW_mmx;
    DECLARE_ALIGNED(8, uint64_t, esp);
    DECLARE_ALIGNED(8, uint64_t, vRounder);
    DECLARE_ALIGNED(8, uint64_t, u_temp);
    DECLARE_ALIGNED(8, uint64_t, v_temp);
    DECLARE_ALIGNED(8, uint64_t, y_temp);
    int32_t alpMmxFilter[4 * MAX_FILTER_SIZE];
    DECLARE_ALIGNED(8, ptrdiff_t, uv_off);
    DECLARE_ALIGNED(8, ptrdiff_t, uv_offx2);
    DECLARE_ALIGNED(8, uint16_t, dither16)[8];
    DECLARE_ALIGNED(8, uint32_t, dither32)[8];
    const uint8_t *chrDither8, *lumDither8;
    int use_mmx_vfilter;
    SwsColorFunc xyz12Torgb48;
    SwsColorFunc rgb48Toxyz12;
    SwsColorXform xyz2rgb;
    SwsColorXform rgb2xyz;
    yuv2planar1_fn yuv2plane1;
    yuv2planarX_fn yuv2planeX;
    yuv2interleavedX_fn yuv2nv12cX;
    yuv2packed1_fn yuv2packed1;
    yuv2packed2_fn yuv2packed2;
    yuv2packedX_fn yuv2packedX;
    yuv2anyX_fn yuv2anyX;
    void *input_opaque;
    planar1_YV12_fn lumToYV12;
    planar1_YV12_fn alpToYV12;
    planar2_YV12_fn chrToYV12;
    planarX_YV12_fn readLumPlanar;
    planarX_YV12_fn readAlpPlanar;
    planarX2_YV12_fn readChrPlanar;
    void (*hyscale_fast)(SwsInternal *c, int16_t *dst, int dstWidth,
                         const uint8_t *src, int srcW, int xInc);
    void (*hcscale_fast)(SwsInternal *c, int16_t *dst1, int16_t *dst2,
                         int dstWidth, const uint8_t *src1, const uint8_t *src2,
                         int srcW, int xInc);
    void (*hyScale)(SwsInternal *c, int16_t *dst, int dstW, const uint8_t *src,
                    const int16_t *filter, const int32_t *filterPos,
                    int filterSize);
    void (*hcScale)(SwsInternal *c, int16_t *dst, int dstW, const uint8_t *src,
                    const int16_t *filter, const int32_t *filterPos,
                    int filterSize);
    void (*lumConvertRange)(int16_t *dst, int width, uint32_t coeff,
                            int64_t offset);
    void (*chrConvertRange)(int16_t *dst1, int16_t *dst2, int width,
                            uint32_t coeff, int64_t offset);
    uint32_t lumConvertRange_coeff;
    uint32_t chrConvertRange_coeff;
    int64_t lumConvertRange_offset;
    int64_t chrConvertRange_offset;
    int needs_hcscale;
    uint8_t *rgb0_scratch;
    unsigned int rgb0_scratch_allocated;
    uint8_t *xyz_scratch;
    unsigned int xyz_scratch_allocated;
    unsigned int dst_slice_align;
    std::atomic<int> stride_unaligned_warned;
    std::atomic<int> data_unaligned_warned;
    int color_conversion_warned;
    Half2FloatTables *h2f_tables;
    void *hw_priv;
    int is_legacy_init;
    FFFramePool frame_pool;
};

/*
 * libswscale/swscale_internal.h:710-711
 */
static_assert(
    offsetof(SwsInternal, redDither) + DITHER32_INT ==
        offsetof(SwsInternal, dither32),
    "dither32 must be at the same offset as redDither + DITHER32_INT");

/*
 * Wrapper for invoking the extracted kernel.
 */
int yuv2rgb_c_24_rgb(SwsInternal *c, const uint8_t *const src[],
                     const int srcStride[], int srcSliceY, int srcSliceH,
                     uint8_t *const dst[], const int dstStride[]);

#endif // KERNELS_98_YUV2RGB_C_24_RGB_INCLUDE_KERNEL_H_
