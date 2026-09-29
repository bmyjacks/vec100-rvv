/****************************************************************************
 *
 *
 *  Project: x265 3.4
 *  Source files:
 *    source/common/common.h
 *    source/encoder/slicetype.h
 *    source/common/frame.h
 *    source/common/picyuv.h
 *    source/x265.h
 *    source/encoder/slicetype.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * source/common/common.h
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Deepthi Nandakumar <deepthi@multicorewareinc.com>
 *          Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *
 *
 * source/encoder/slicetype.h
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Steve Borho <steve@borho.org>
 *          Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *
 *
 * source/common/frame.h
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Author: Steve Borho <steve@borho.org>
 *         Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *
 *
 * source/common/picyuv.h
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Steve Borho <steve@borho.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *
 *
 * source/x265.h
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Steve Borho <steve@borho.org>
 *          Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *
 * source/encoder/slicetype.cpp
 *
 * Copyright (C) 2013-2020 MulticoreWare, Inc
 *
 * Authors: Gopu Govindaswamy <gopu@multicorewareinc.com>
 *          Steve Borho <steve@borho.org>
 *          Ashok Kumar Mishra <ashok@multicorewareinc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *
 */

#ifndef KERNELS_15_EDGEFILTER_GAUSSIAN5X5_INCLUDE_KERNEL_H_
#define KERNELS_15_EDGEFILTER_GAUSSIAN5X5_INCLUDE_KERNEL_H_

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

/*
 * source/common/common.h:126-142
 */
typedef uint8_t pixel;

/*
 * source/x265.h:681
 */
struct x265_param;

/*
 * source/common/frame.h:36
 */
class PicYuv;

/*
 * source/x265.h:744-1912 (struct x265_param reduced to the fields read by
 * edgeFilter and general_log)
 */
typedef struct x265_param {
    /*
     * source/x265.h:843
     */
    int logLevel;
    /*
     * source/x265.h:1049
     */
    uint32_t maxCUSize;
} x265_param;

/*
 * source/common/common.h:423-425
 */
#define x265_log(param, ...) general_log(param, "x265", __VA_ARGS__)
void general_log(const x265_param *param, const char *caller, int level,
                 const char *fmt, ...);

/*
 * source/x265.h:543-547
 */
#define X265_LOG_ERROR 0
#define X265_LOG_WARNING 1
#define X265_LOG_INFO 2
#define X265_LOG_DEBUG 3
#define X265_LOG_FULL 4

/*
 * source/encoder/slicetype.h:46-51
 */
#define EDGE_THRESHOLD 255.0
#define PI 3.14159265

/*
 * source/common/frame.h:75-138 (class Frame reduced to the members read by
 * edgeFilter)
 */
class Frame {
  public:
    PicYuv *m_fencPic;
    pixel *m_edgePic;
    pixel *m_gaussianPic;
    pixel *m_thetaPic;
};

/*
 * source/common/picyuv.h:38-60 (class PicYuv reduced to the members read by
 * edgeFilter)
 */
class PicYuv {
  public:
    pixel *m_picOrg[3];
    uint32_t m_picWidth;
    uint32_t m_picHeight;
    intptr_t m_stride;
    uint32_t m_lumaMarginX;
    uint32_t m_lumaMarginY;
};

/*
 * source/encoder/slicetype.h:268
 */
bool computeEdge(pixel *edgePic, pixel *refPic, pixel *edgeTheta,
                 intptr_t stride, int height, int width, bool bcalcTheta,
                 pixel whitePixel = EDGE_THRESHOLD);

/*
 * source/encoder/slicetype.cpp:151
 */
void edgeFilter_isolated(Frame *curFrame, x265_param *param);

#endif // KERNELS_15_EDGEFILTER_GAUSSIAN5X5_INCLUDE_KERNEL_H_
