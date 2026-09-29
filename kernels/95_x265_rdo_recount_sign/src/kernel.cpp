#include "kernel.h"

/* x265 3.4 source/common/quant.cpp:1253-1260; codeParams.scan and
 * m_resiDctCoeff supplied as arguments, bestLastIdx supplied by RDO search.
 * The original numSig = 0 is represented by the local accumulator. */
uint32_t rdo_recount_sign(const uint16_t *scan, int16_t *dstCoeff,
                          const int16_t *resiDctCoeff, int bestLastIdx,
                          int /* coeffCount */) {
    uint32_t numSig = 0;
    for (int pos = 0; pos < bestLastIdx; pos++) {
        int blkPos = scan[pos];
        int level = dstCoeff[blkPos];
        numSig += (level != 0);
        uint32_t mask = (int32_t)resiDctCoeff[blkPos] >> 31;
        dstCoeff[blkPos] = (int16_t)((level ^ mask) - mask);
    }
    return numSig;
}
