/****************************************************************************
 *
 *
 *  Project: zstd 1.5.7
 *  Source files:
 *    lib/decompress/zstd_decompress_block.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * lib/decompress/zstd_decompress_block.c
 *
 *   zstd_decompress_block :
 *   this module takes care of decompressing _compressed_ block
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

#include "kernel.h"

/*
 * lib/decompress/zstd_decompress_block.c:484-603
 */
FORCE_INLINE_TEMPLATE
void ZSTD_buildFSETable_body(ZSTD_seqSymbol *dt, const short *normalizedCounter,
                             unsigned maxSymbolValue, const U32 *baseValue,
                             const U8 *nbAdditionalBits, unsigned tableLog,
                             void *wksp, size_t wkspSize) {
    ZSTD_seqSymbol *const tableDecode = dt + 1;
    U32 const maxSV1 = maxSymbolValue + 1;
    U32 const tableSize = 1 << tableLog;

    U16 *symbolNext = (U16 *)wksp;
    BYTE *spread = (BYTE *)(symbolNext + MaxSeq + 1);
    U32 highThreshold = tableSize - 1;

    assert(maxSymbolValue <= MaxSeq);
    assert(tableLog <= MaxFSELog);
    assert(wkspSize >= ZSTD_BUILD_FSE_TABLE_WKSP_SIZE);
    (void)wkspSize;

    {
        ZSTD_seqSymbol_header DTableH;
        DTableH.tableLog = tableLog;
        DTableH.fastMode = 1;
        {
            S16 const largeLimit = (S16)(1 << (tableLog - 1));
            U32 s;
            for (s = 0; s < maxSV1; s++) {
                if (normalizedCounter[s] == -1) {
                    tableDecode[highThreshold--].baseValue = s;
                    symbolNext[s] = 1;
                } else {
                    if (normalizedCounter[s] >= largeLimit)
                        DTableH.fastMode = 0;
                    assert(normalizedCounter[s] >= 0);
                    symbolNext[s] = (U16)normalizedCounter[s];
                }
            }
        }
        ZSTD_memcpy(dt, &DTableH, sizeof(DTableH));
    }

    assert(tableSize <= 512);

    if (highThreshold == tableSize - 1) {
        size_t const tableMask = tableSize - 1;
        size_t const step = FSE_TABLESTEP(tableSize);

        {
            U64 const add = 0x0101010101010101ull;
            size_t pos = 0;
            U64 sv = 0;
            U32 s;
            for (s = 0; s < maxSV1; ++s, sv += add) {
                int i;
                int const n = normalizedCounter[s];
                MEM_write64(spread + pos, sv);
                for (i = 8; i < n; i += 8) {
                    MEM_write64(spread + pos + i, sv);
                }
                assert(n >= 0);
                pos += (size_t)n;
            }
        }

        {
            size_t position = 0;
            size_t s;
            size_t const unroll = 2;
            assert(tableSize % unroll == 0);
            for (s = 0; s < (size_t)tableSize; s += unroll) {
                size_t u;
                for (u = 0; u < unroll; ++u) {
                    size_t const uPosition =
                        (position + (u * step)) & tableMask;
                    tableDecode[uPosition].baseValue = spread[s + u];
                }
                position = (position + (unroll * step)) & tableMask;
            }
            assert(position == 0);
        }
    } else {
        U32 const tableMask = tableSize - 1;
        U32 const step = FSE_TABLESTEP(tableSize);
        U32 s, position = 0;
        for (s = 0; s < maxSV1; s++) {
            int i;
            int const n = normalizedCounter[s];
            for (i = 0; i < n; i++) {
                tableDecode[position].baseValue = s;
                position = (position + step) & tableMask;
                while (UNLIKELY(position > highThreshold))
                    position = (position + step) & tableMask;
            }
        }
        assert(position == 0);
    }

    {
        U32 u;
        for (u = 0; u < tableSize; u++) {
            U32 const symbol = tableDecode[u].baseValue;
            U32 const nextState = symbolNext[symbol]++;
            tableDecode[u].nbBits =
                (BYTE)(tableLog - ZSTD_highbit32(nextState));
            tableDecode[u].nextState =
                (U16)((nextState << tableDecode[u].nbBits) - tableSize);
            assert(nbAdditionalBits[symbol] < 255);
            tableDecode[u].nbAdditionalBits = nbAdditionalBits[symbol];
            tableDecode[u].baseValue = baseValue[symbol];
        }
    }
}

/*
 * lib/decompress/zstd_decompress_block.c:605-613
 */
static void ZSTD_buildFSETable_body_default(
    ZSTD_seqSymbol *dt, const short *normalizedCounter, unsigned maxSymbolValue,
    const U32 *baseValue, const U8 *nbAdditionalBits, unsigned tableLog,
    void *wksp, size_t wkspSize) {
    ZSTD_buildFSETable_body(dt, normalizedCounter, maxSymbolValue, baseValue,
                            nbAdditionalBits, tableLog, wksp, wkspSize);
}

/*
 * lib/decompress/zstd_decompress_block.c:626-641
 */
void ZSTD_buildFSETable(ZSTD_seqSymbol *dt, const short *normalizedCounter,
                        unsigned maxSymbolValue, const U32 *baseValue,
                        const U8 *nbAdditionalBits, unsigned tableLog,
                        void *wksp, size_t wkspSize, int bmi2) {
    (void)bmi2;
    ZSTD_buildFSETable_body_default(dt, normalizedCounter, maxSymbolValue,
                                    baseValue, nbAdditionalBits, tableLog, wksp,
                                    wkspSize);
}
