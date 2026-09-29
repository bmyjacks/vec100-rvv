/****************************************************************************
 * Project: x265 3.4
 * Source files: source/common/quant.cpp
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
 */
#include "kernel.h"

/* Wrapper for the two upstream DC loops. In the selected 8-bit configuration
 * source/common/quant.cpp:486 makes shift = X265_DEPTH - 8 = 0. */
void ssim_distortion_dc(const pixel *fenc, uint32_t fStride,
                        const pixel *recon, std::intptr_t rstride, int trSize,
                        uint64_t *ssDcOut, uint64_t *dcOut) {
    uint64_t ssDc = 0;
    /* source/common/quant.cpp:493-500 */
    for (int y = 0; y < trSize; y += 4) {
        for (int x = 0; x < trSize; x += 4) {
            int temp = fenc[y * fStride + x] - recon[y * rstride + x];
            ssDc += temp * temp;
        }
    }
    uint64_t dc_k = 0;
    /* source/common/quant.cpp:513-521 */
    for (int block_yy = 0; block_yy < trSize; block_yy += 4) {
        for (int block_xx = 0; block_xx < trSize; block_xx += 4) {
            uint32_t temp = fenc[block_yy * fStride + block_xx] >> 0;
            dc_k += temp * temp;
        }
    }
    *ssDcOut = ssDc;
    *dcOut = dc_k;
}
