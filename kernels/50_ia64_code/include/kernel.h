/****************************************************************************
 *
 *
 *  Project: XZ Utils (xz) 5.8.4
 *  Source files:
 *    src/liblzma/api/lzma.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/liblzma/api/lzma.h
 *
 *   The public API of liblzma data compression library
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author: Lasse Collin
 *
 */

#ifndef KERNELS_50_IA64_CODE_INCLUDE_KERNEL_H_
#define KERNELS_50_IA64_CODE_INCLUDE_KERNEL_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * src/liblzma/api/lzma.h:248
 */
#define lzma_attribute(attr) __attribute__(attr)

/*
 * Wrapper for invoking the extracted kernel.
 */
size_t ia64_code_isolated(void *simple, uint32_t now_pos, bool is_encoder,
                          uint8_t *buffer, size_t size);

#endif // KERNELS_50_IA64_CODE_INCLUDE_KERNEL_H_
