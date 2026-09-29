/****************************************************************************
 * Project: FFmpeg n9.0.2
 * Source files: libavcodec/videodsp_template.c,
 *               libavcodec/bit_depth_template.c, libavcodec/videodsp.h
 *
 * Original copyright and license notices:
 *
 * libavcodec/videodsp_template.c:
 * Copyright (c) 2002-2012 Michael Niedermayer
 * Copyright (C) 2012 Ronald S. Bultje
 *
 * libavcodec/videodsp.h:
 * Copyright (C) 2012 Ronald S. Bultje
 *
 * libavcodec/bit_depth_template.c:
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
 ****************************************************************************/

#ifndef KERNELS_20_FFMPEG_EMULATED_EDGE_MC_16_INCLUDE_KERNEL_H_
#define KERNELS_20_FFMPEG_EMULATED_EDGE_MC_16_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

// Expansion of bit_depth_template.c at BIT_DEPTH=16.
#define pixel uint16_t

// Extracted videodsp_template.c entry point (static upstream, exposed here).
void emulated_edge_mc_16_isolated(uint8_t *buf, const uint8_t *src,
                         ptrdiff_t buf_linesize, ptrdiff_t src_linesize,
                         int block_w, int block_h, int src_x, int src_y,
                         int w, int h);

#endif
