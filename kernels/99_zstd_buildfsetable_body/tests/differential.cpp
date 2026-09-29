#include "kernel.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

void ZSTD_buildFSETable_rvv(ZSTD_seqSymbol *, const short *, unsigned,
                             const U32 *, const U8 *, unsigned, void *, size_t, int);

int main() {
    std::mt19937 rng(0x99f5e);
    for (unsigned log = 5; log <= MaxFSELog; ++log) {
        for (unsigned trial = 0; trial < 140; ++trial) {
            const unsigned maxSymbol = trial % (MaxSeq + 1);
            const unsigned tableSize = 1u << log;
            short norm[MaxSeq + 1] = {};
            U32 base[MaxSeq + 1];
            U8 extra[MaxSeq + 1];
            for (unsigned i = 0; i <= MaxSeq; ++i) {
                base[i] = rng(); extra[i] = rng() % 32;
            }
            int remaining = tableSize;
            if (maxSymbol && (trial % 3 == 0)) {
                for (unsigned i = 0; i < maxSymbol && i < 5; ++i) {
                    norm[i] = -1; --remaining;
                }
            }
            // Supply a valid normalized distribution (sum of positive counts
            // plus one per -1 equals tableSize), including zero-frequency
            // symbols, large counts, and both spread algorithms.
            for (unsigned i = 0; i < maxSymbol; ++i) {
                if (norm[i] < 0) continue;
                int count = rng() % (remaining + 1);
                norm[i] = count;
                remaining -= count;
            }
            norm[maxSymbol] = remaining;
            std::vector<ZSTD_seqSymbol> a(tableSize + 4), b(tableSize + 4);
            for (auto &v : a) {
                v.nextState = rng(); v.nbBits = rng();
                v.nbAdditionalBits = rng(); v.baseValue = rng();
            }
            b = a;
            alignas(8) unsigned char work1[ZSTD_BUILD_FSE_TABLE_WKSP_SIZE];
            alignas(8) unsigned char work2[ZSTD_BUILD_FSE_TABLE_WKSP_SIZE];
            for (auto &v : work1) v = rng();
            std::memcpy(work2, work1, sizeof work1);
            ZSTD_buildFSETable(a.data(), norm, maxSymbol, base, extra, log,
                               work1, sizeof work1, trial & 1);
            ZSTD_buildFSETable_rvv(b.data(), norm, maxSymbol, base, extra, log,
                                   work2, sizeof work2, trial & 1);
            if (std::memcmp(a.data(), b.data(), a.size() * sizeof a[0]) ||
                std::memcmp(work1, work2, sizeof work1)) {
                std::fprintf(stderr, "fse mismatch log=%u trial=%u max=%u\n", log, trial, maxSymbol);
                std::abort();
            }
        }
    }
    std::puts("ZSTD_buildFSETable: pass");
}
