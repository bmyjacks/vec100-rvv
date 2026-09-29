#include "kernel.h"

#include <riscv_vector.h>

void ZSTD_buildFSETable_rvv(ZSTD_seqSymbol *dt, const short *normalizedCounter,
                             unsigned maxSymbolValue, const U32 *baseValue,
                             const U8 *nbAdditionalBits, unsigned tableLog,
                             void *wksp, size_t wkspSize, int bmi2) {
    (void)bmi2;
    ZSTD_seqSymbol *const tableDecode = dt + 1;
    const U32 tableSize = 1u << tableLog;
    U16 *symbolNext = (U16 *)wksp;
    BYTE *spread = (BYTE *)(symbolNext + MaxSeq + 1);
    U32 highThreshold = tableSize - 1;
    assert(maxSymbolValue <= MaxSeq);
    assert(tableLog <= MaxFSELog);
    assert(wkspSize >= ZSTD_BUILD_FSE_TABLE_WKSP_SIZE);
    (void)wkspSize;

    ZSTD_seqSymbol_header header;
    header.tableLog = tableLog;
    header.fastMode = 1;
    const S16 largeLimit = (S16)(1 << (tableLog - 1));
    for (U32 s = 0; s <= maxSymbolValue; ++s) {
        if (normalizedCounter[s] == -1) {
            tableDecode[highThreshold--].baseValue = s;
            symbolNext[s] = 1;
        } else {
            if (normalizedCounter[s] >= largeLimit) header.fastMode = 0;
            assert(normalizedCounter[s] >= 0);
            symbolNext[s] = (U16)normalizedCounter[s];
        }
    }
    ZSTD_memcpy(dt, &header, sizeof(header));
    assert(tableSize <= 512);

    const U32 mask = tableSize - 1;
    const U32 step = FSE_TABLESTEP(tableSize);
    if (highThreshold == tableSize - 1) {
        // Reproduce the upstream 64-bit overlapping writes, including their
        // writes past the end of each short run into the workspace padding.
        size_t pos = 0;
        U64 sv = 0;
        const U64 add = 0x0101010101010101ull;
        for (U32 s = 0; s <= maxSymbolValue; ++s, sv += add) {
            const int n = normalizedCounter[s];
            MEM_write64(spread + pos, sv);
            for (int i = 8; i < n; i += 8) MEM_write64(spread + pos + i, sv);
            assert(n >= 0);
            pos += (size_t)n;
        }
        // The FSE step is coprime to the power-of-two table size. Generate
        // independent permutation indices for each batch and scatter symbols.
        U32 position = 0;
        for (U32 s = 0; s < tableSize;) {
            const size_t vl = __riscv_vsetvl_e32m1(tableSize - s);
            const auto lane = __riscv_vid_v_u32m1(vl);
            const auto positions = __riscv_vand_vx_u32m1(
                __riscv_vadd_vx_u32m1(
                    __riscv_vmul_vx_u32m1(lane, step, vl), position, vl), mask, vl);
            const auto offsets = __riscv_vsll_vx_u32m1(positions, 3, vl);
            const auto bytes = __riscv_vle8_v_u8mf4(spread + s, vl);
            const auto symbols = __riscv_vzext_vf4_u32m1(bytes, vl);
            __riscv_vsuxei32_v_u32m1(&tableDecode[0].baseValue, offsets, symbols, vl);
            position = (position + vl * step) & mask;
            s += vl;
        }
        assert(position == 0);
    } else {
        // -1 symbols reserve the high states. The next free position depends
        // on the preceding placement; preserve the upstream serial walk.
        U32 position = 0;
        for (U32 s = 0; s <= maxSymbolValue; ++s) {
            const int n = normalizedCounter[s];
            for (int i = 0; i < n; ++i) {
                tableDecode[position].baseValue = s;
                position = (position + step) & mask;
                while (UNLIKELY(position > highThreshold))
                    position = (position + step) & mask;
            }
        }
        assert(position == 0);
    }
    // symbolNext[symbol]++ depends on the exact visitation order, including
    // repetitions across vector chunks; leave this final pass serial.
    for (U32 u = 0; u < tableSize; ++u) {
        const U32 symbol = tableDecode[u].baseValue;
        const U32 nextState = symbolNext[symbol]++;
        tableDecode[u].nbBits = (BYTE)(tableLog - ZSTD_highbit32(nextState));
        tableDecode[u].nextState = (U16)((nextState << tableDecode[u].nbBits) - tableSize);
        assert(nbAdditionalBits[symbol] < 255);
        tableDecode[u].nbAdditionalBits = nbAdditionalBits[symbol];
        tableDecode[u].baseValue = baseValue[symbol];
    }
}
