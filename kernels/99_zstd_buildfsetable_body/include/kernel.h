/****************************************************************************
 *
 *
 *  Project: zstd 1.5.7
 *  Source files:
 *    lib/common/compiler.h
 *    lib/common/mem.h
 *    lib/common/zstd_deps.h
 *    lib/common/zstd_internal.h
 *    lib/common/fse.h
 *    lib/common/bits.h
 *    lib/decompress/zstd_decompress_internal.h
 *    lib/decompress/zstd_decompress_block.h
 *
 *
 *  The original file copyright and license notices follow.
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
 *
 * lib/common/fse.h
 *
 *   FSE : Finite State Entropy codec
 *   Public Prototypes declaration
 *
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * You can contact the author at :
 * - Source repository : https://github.com/Cyan4973/FiniteStateEntropy
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 *
 *
 * lib/common/bits.h
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
 * lib/decompress/zstd_decompress_internal.h
 *
 *   zstd_decompress_internal:
 *   objects and definitions shared within lib/decompress modules
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
 * lib/decompress/zstd_decompress_block.h
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

#ifndef KERNELS_99_ZSTD_BUILDFSETABLE_BODY_INCLUDE_KERNEL_H_
#define KERNELS_99_ZSTD_BUILDFSETABLE_BODY_INCLUDE_KERNEL_H_

#include <assert.h>
#include <stddef.h>

/*
 * lib/common/compiler.h:23-43
 */
#define INLINE_KEYWORD inline
#define FORCE_INLINE_ATTR __attribute__((always_inline))

/*
 * lib/common/compiler.h:56-68
 */
#define UNUSED_ATTR __attribute__((unused))
#define FORCE_INLINE_TEMPLATE                                                  \
    static INLINE_KEYWORD FORCE_INLINE_ATTR UNUSED_ATTR

/*
 * lib/common/compiler.h:95-107
 */
#define MEM_STATIC static __inline UNUSED_ATTR

/*
 * lib/common/compiler.h:186-192
 */
#define UNLIKELY(x) (__builtin_expect((x), 0))

/*
 * lib/common/mem.h:36-73
 */
#include <stdint.h>
typedef uint8_t BYTE;
typedef uint8_t U8;
typedef uint16_t U16;
typedef int16_t S16;
typedef uint32_t U32;
typedef uint64_t U64;

/*
 * lib/common/zstd_internal.h:52-55
 */
#undef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*
 * lib/common/zstd_internal.h:103-111
 */
#define MaxML 52
#define MaxLL 35
#define MaxSeq MAX(MaxLL, MaxML)
#define MLFSELog 9
#define LLFSELog 9
#define OffFSELog 8
#define MaxFSELog MAX(MAX(MLFSELog, LLFSELog), OffFSELog)

/*
 * lib/common/zstd_deps.h:43-51
 */
#define ZSTD_memcpy(d, s, l) __builtin_memcpy((d), (s), (l))

/*
 * lib/common/mem.h:131-135
 */
/*
 * lib/common/mem.h:179
 */
typedef __attribute__((aligned(1))) U64 unalign64;

/*
 * lib/common/mem.h:189
 */
MEM_STATIC void MEM_write64(void *memPtr, U64 value) {
    *(unalign64 *)memPtr = value;
}

/*
 * lib/common/fse.h:623
 */
#define FSE_TABLESTEP(tableSize) (((tableSize) >> 1) + ((tableSize) >> 3) + 3)

/*
 * lib/common/bits.h:68-91
 */
MEM_STATIC unsigned ZSTD_countLeadingZeros32(U32 val) {
    assert(val != 0);
    return (unsigned)__builtin_clz(val);
}

/*
 * lib/common/bits.h:174-178
 */
MEM_STATIC unsigned ZSTD_highbit32(U32 val) {
    assert(val != 0);
    return 31 - ZSTD_countLeadingZeros32(val);
}

/*
 * lib/decompress/zstd_decompress_internal.h:62-76
 */
typedef struct {
    U32 fastMode;
    U32 tableLog;
} ZSTD_seqSymbol_header;

typedef struct {
    U16 nextState;
    BYTE nbAdditionalBits;
    BYTE nbBits;
    U32 baseValue;
} ZSTD_seqSymbol;

#define ZSTD_BUILD_FSE_TABLE_WKSP_SIZE                                         \
    (sizeof(S16) * (MaxSeq + 1) + (1u << MaxFSELog) + sizeof(U64))

/*
 * lib/decompress/zstd_decompress_block.h:61-65
 */
void ZSTD_buildFSETable(ZSTD_seqSymbol *dt, const short *normalizedCounter,
                        unsigned maxSymbolValue, const U32 *baseValue,
                        const U8 *nbAdditionalBits, unsigned tableLog,
                        void *wksp, size_t wkspSize, int bmi2);

#endif // KERNELS_99_ZSTD_BUILDFSETABLE_BODY_INCLUDE_KERNEL_H_
