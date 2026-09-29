/****************************************************************************
 *
 *
 *  Project: zstd 1.5.7
 *  Source files:
 *    lib/common/mem.h
 *    lib/common/compiler.h
 *    lib/common/zstd_internal.h
 *    lib/compress/zstd_compress_internal.h
 *
 *
 *  The original file copyright and license notices follow.
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
 * lib/common/zstd_internal.h
 *
 *   this module contains definitions which must be identical
 *   across compression, decompression and dictBuilder.
 *   It also contains a few functions useful to at least 2 of them
 *   and which benefit from being inlined
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 * lib/compress/zstd_compress_internal.h
 *
 *   This header contains definitions
 *   that shall **only** be used by modules within lib/compress.
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 */

#ifndef KERNELS_18_FASTCOVER_COMPUTEFREQUENCY_INCLUDE_KERNEL_H_
#define KERNELS_18_FASTCOVER_COMPUTEFREQUENCY_INCLUDE_KERNEL_H_

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/*
 * lib/common/mem.h:42
 */
typedef uint8_t BYTE;

/*
 * lib/common/mem.h:47
 */
typedef uint32_t U32;

/*
 * lib/common/mem.h:49
 */
typedef uint64_t U64;

/*
 * lib/common/compiler.h:56-61
 */
#define UNUSED_ATTR __attribute__((unused))

/*
 * lib/common/compiler.h:95-100
 */
#define MEM_STATIC static __inline UNUSED_ATTR

/*
 * lib/common/zstd_internal.h:54-55
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*
 * lib/common/mem.h:140-160
 */
MEM_STATIC unsigned MEM_isLittleEndian(void) { return 1; }

/*
 * lib/common/mem.h:131-135, :254-266 (GCC/Clang unaligned-access selection)
 */
typedef __attribute__((aligned(1))) U64 unalign64;
MEM_STATIC U64 MEM_read64(const void *ptr) { return *(const unalign64 *)ptr; }

/*
 * lib/common/mem.h:267-277
 */
MEM_STATIC U64 MEM_swap64(U64 in) { return __builtin_bswap64(in); }

/*
 * lib/common/mem.h:337-343
 */
MEM_STATIC U64 MEM_readLE64(const void *memPtr) {
    if (MEM_isLittleEndian())
        return MEM_read64(memPtr);
    else
        return MEM_swap64(MEM_read64(memPtr));
}

/*
 * lib/compress/zstd_compress_internal.h:913-915
 */
static const U64 prime6bytes = 227718039650203ULL;
static size_t ZSTD_hash6(U64 u, U32 h, U64 s) {
    assert(h <= 64);
    return (size_t)((((u << (64 - 48)) * prime6bytes) ^ s) >> (64 - h));
}
static size_t ZSTD_hash6Ptr(const void *p, U32 h) {
    return ZSTD_hash6(MEM_readLE64(p), h, 0);
}

/*
 * lib/compress/zstd_compress_internal.h:923-925
 */
static const U64 prime8bytes = 0xCF1BBCDCB7A56463ULL;
static size_t ZSTD_hash8(U64 u, U32 h, U64 s) {
    assert(h <= 64);
    return (size_t)((((u)*prime8bytes) ^ s) >> (64 - h));
}
static size_t ZSTD_hash8Ptr(const void *p, U32 h) {
    return ZSTD_hash8(MEM_readLE64(p), h, 0);
}

/*
 * Wrapper for invoking the extracted kernel.
 */
void FASTCOVER_computeFrequency_bench_isolated(U32 *freqs, const BYTE *samples,
                                      size_t *offsets, size_t nbTrainSamples,
                                      size_t nbSamples, unsigned f, unsigned d,
                                      unsigned skip);

#endif // KERNELS_18_FASTCOVER_COMPUTEFREQUENCY_INCLUDE_KERNEL_H_
