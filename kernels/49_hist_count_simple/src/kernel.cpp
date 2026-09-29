/****************************************************************************
 *
 *
 *  Project: zstd 1.5.7
 *  Source files:
 *    lib/compress/hist.c
 *
 *
 *  Below are the copyright notice of original file
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

#include "kernel.h"

/*
 * lib/compress/hist.c:29-37
 */
void HIST_add_isolated(unsigned *count, const void *src, size_t srcSize) {
    const BYTE *ip = (const BYTE *)src;
    const BYTE *const end = ip + srcSize;

    while (ip < end) {
        count[*ip++]++;
    }
}

/*
 * lib/compress/hist.c:39-64
 */
unsigned HIST_count_simple_isolated(unsigned *count, unsigned *maxSymbolValuePtr,
                           const void *src, size_t srcSize) {
    const BYTE *ip = (const BYTE *)src;
    const BYTE *const end = ip + srcSize;
    unsigned maxSymbolValue = *maxSymbolValuePtr;
    unsigned largestCount = 0;

    ZSTD_memset(count, 0, (maxSymbolValue + 1) * sizeof(*count));
    if (srcSize == 0) {
        *maxSymbolValuePtr = 0;
        return 0;
    }

    while (ip < end) {
        assert(*ip <= maxSymbolValue);
        count[*ip++]++;
    }

    while (!count[maxSymbolValue])
        maxSymbolValue--;
    *maxSymbolValuePtr = maxSymbolValue;

    {
        U32 s;
        for (s = 0; s <= maxSymbolValue; s++)
            if (count[s] > largestCount)
                largestCount = count[s];
    }

    return largestCount;
}

/*
 * lib/compress/hist.c:76-143
 */
static size_t HIST_count_parallel_wksp(unsigned *count,
                                       unsigned *maxSymbolValuePtr,
                                       const void *source, size_t sourceSize,
                                       HIST_checkInput_e check,
                                       U32 *const workSpace) {
    const BYTE *ip = (const BYTE *)source;
    const BYTE *const iend = ip + sourceSize;
    size_t const countSize = (*maxSymbolValuePtr + 1) * sizeof(*count);
    unsigned max = 0;
    U32 *const Counting1 = workSpace;
    U32 *const Counting2 = Counting1 + 256;
    U32 *const Counting3 = Counting2 + 256;
    U32 *const Counting4 = Counting3 + 256;

    assert(*maxSymbolValuePtr <= 255);
    if (!sourceSize) {
        ZSTD_memset(count, 0, countSize);
        *maxSymbolValuePtr = 0;
        return 0;
    }
    ZSTD_memset(workSpace, 0, 4 * 256 * sizeof(unsigned));

    {
        U32 cached = MEM_read32(ip);
        ip += 4;
        while (ip < iend - 15) {
            U32 c = cached;
            cached = MEM_read32(ip);
            ip += 4;
            Counting1[(BYTE)c]++;
            Counting2[(BYTE)(c >> 8)]++;
            Counting3[(BYTE)(c >> 16)]++;
            Counting4[c >> 24]++;
            c = cached;
            cached = MEM_read32(ip);
            ip += 4;
            Counting1[(BYTE)c]++;
            Counting2[(BYTE)(c >> 8)]++;
            Counting3[(BYTE)(c >> 16)]++;
            Counting4[c >> 24]++;
            c = cached;
            cached = MEM_read32(ip);
            ip += 4;
            Counting1[(BYTE)c]++;
            Counting2[(BYTE)(c >> 8)]++;
            Counting3[(BYTE)(c >> 16)]++;
            Counting4[c >> 24]++;
            c = cached;
            cached = MEM_read32(ip);
            ip += 4;
            Counting1[(BYTE)c]++;
            Counting2[(BYTE)(c >> 8)]++;
            Counting3[(BYTE)(c >> 16)]++;
            Counting4[c >> 24]++;
        }
        ip -= 4;
    }

    while (ip < iend)
        Counting1[*ip++]++;

    {
        U32 s;
        for (s = 0; s < 256; s++) {
            Counting1[s] += Counting2[s] + Counting3[s] + Counting4[s];
            if (Counting1[s] > max)
                max = Counting1[s];
        }
    }

    {
        unsigned maxSymbolValue = 255;
        while (!Counting1[maxSymbolValue])
            maxSymbolValue--;
        if (check && maxSymbolValue > *maxSymbolValuePtr)
            return ERROR(maxSymbolValue_tooSmall);
        *maxSymbolValuePtr = maxSymbolValue;
        ZSTD_memmove(count, Counting1, countSize);
    }
    return (size_t)max;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
size_t HIST_count_parallel_wksp_bench(unsigned *count,
                                      unsigned *maxSymbolValuePtr,
                                      const void *source, size_t sourceSize,
                                      HIST_checkInput_e check,
                                      U32 *const workSpace) {
    return HIST_count_parallel_wksp(count, maxSymbolValuePtr, source,
                                    sourceSize, check, workSpace);
}
