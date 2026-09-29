/****************************************************************************
 *
 *
 *  Project: zstd 1.5.7
 *  Source files:
 *    lib/common/mem.h
 *    lib/common/compiler.h
 *    lib/common/zstd_deps.h
 *    lib/zstd_errors.h
 *    lib/common/error_private.h
 *    lib/compress/hist.h
 *    lib/compress/hist.c
 *
 *
 *  Below are the copyright notices of original files
 *
 *
 * lib/common/mem.h
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 *
 * lib/common/compiler.h
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 *
 * lib/common/zstd_deps.h
 *
 *   This file provides common libc dependencies that zstd requires.
 *   The purpose is to allow replacing this file with a custom implementation
 *   to compile zstd without libc support.
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 *
 * lib/zstd_errors.h
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 *
 * lib/common/error_private.h
 *
 *   Note : this module is expected to remain private, do not expose it
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 *
 * lib/compress/hist.h
 *
 *   hist : Histogram functions
 *   part of Finite State Entropy project
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 *  You can contact the author at :
 *  - FSE source repository : https://github.com/Cyan4973/FiniteStateEntropy
 *  - Public forum : https://groups.google.com/forum/#!forum/lz4c
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 *
 * lib/compress/hist.c
 *
 *   hist : Histogram functions
 *   part of Finite State Entropy project
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 *  You can contact the author at :
 *  - FSE source repository : https://github.com/Cyan4973/FiniteStateEntropy
 *  - Public forum : https://groups.google.com/forum/#!forum/lz4c
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 */

#ifndef KERNELS_49_HIST_COUNT_SIMPLE_INCLUDE_KERNEL_H_
#define KERNELS_49_HIST_COUNT_SIMPLE_INCLUDE_KERNEL_H_

#include <assert.h>
#include <stddef.h>
#include <string.h>

/*
 * lib/common/mem.h:42, 47
 */
#include <stdint.h>
typedef uint8_t BYTE;
typedef uint32_t U32;

/*
 * lib/common/compiler.h:58
 */
#define UNUSED_ATTR __attribute__((unused))

/*
 * lib/common/compiler.h:97
 */
#define MEM_STATIC static __inline UNUSED_ATTR

/*
 * lib/common/zstd_deps.h:45-46
 */
#define ZSTD_memmove(d, s, l) __builtin_memmove((d), (s), (l))
#define ZSTD_memset(p, v, l) __builtin_memset((p), (v), (l))

/*
 * lib/common/mem.h:178, 183 (selected unaligned 32-bit read)
 */
typedef __attribute__((aligned(1))) U32 unalign32;
MEM_STATIC U32 MEM_read32(const void *ptr) { return *(const unalign32 *)ptr; }

/*
 * lib/zstd_errors.h:60-98 (only the used enumerator retained)
 */
typedef enum {
    ZSTD_error_maxSymbolValue_tooSmall = 48,
} ZSTD_ErrorCode;

/*
 * lib/common/error_private.h:42
 */
#define PREFIX(name) ZSTD_error_##name

/*
 * lib/common/error_private.h:48-50
 */
#undef ERROR
#define ERROR(name) ZSTD_ERROR(name)
#define ZSTD_ERROR(name) ((size_t)-PREFIX(name))

/*
 * lib/compress/hist.h:38-39
 */
#define HIST_WKSP_SIZE_U32 1024
#define HIST_WKSP_SIZE (HIST_WKSP_SIZE_U32 * sizeof(unsigned))

/*
 * lib/compress/hist.c:66
 */
typedef enum { trustInput, checkMaxSymbolValue } HIST_checkInput_e;

/*
 * lib/compress/hist.h:74-75
 */
unsigned HIST_count_simple_isolated(unsigned *count, unsigned *maxSymbolValuePtr,
                                    const void *src, size_t srcSize);
unsigned HIST_count_simple_rvv(unsigned *count, unsigned *maxSymbolValuePtr,
                           const void *src, size_t srcSize);

/*
 * lib/compress/hist.h:82
 */
void HIST_add_isolated(unsigned *count, const void *src, size_t srcSize);
void HIST_add_rvv(unsigned *count, const void *src, size_t srcSize);

/*
 * Wrapper for invoking the extracted kernel.
 */
size_t HIST_count_parallel_wksp_bench(unsigned *count,
                                       unsigned *maxSymbolValuePtr,
                                       const void *source, size_t sourceSize,
                                       HIST_checkInput_e check,
                                       U32 *const workSpace);
size_t HIST_count_parallel_wksp_bench_rvv(unsigned *count,
                                          unsigned *maxSymbolValuePtr,
                                          const void *source, size_t sourceSize,
                                          HIST_checkInput_e check,
                                          U32 *const workSpace);

#endif // KERNELS_49_HIST_COUNT_SIMPLE_INCLUDE_KERNEL_H_
