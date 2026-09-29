#include "kernel.h"
#include <riscv_vector.h>
#include <cstddef>
#include <cstdint>

static bool overlaps(const void *a, size_t an, const void *b, size_t bn) {
    const uintptr_t aa = reinterpret_cast<uintptr_t>(a);
    const uintptr_t bb = reinterpret_cast<uintptr_t>(b);
    return aa < bb + bn && bb < aa + an;
}

uint32_t rdo_recount_sign_rvv(const uint16_t *scan, int16_t *dstCoeff,
                              const int16_t *resiDctCoeff, int bestLastIdx,
                              int coeffCount) {
    if (bestLastIdx == 0)
        return 0;

    // Scattered writes must not modify later scan entries or later residual
    // reads. Reject all overlapping spans, including offset and partial alias.
    const size_t scanBytes = size_t(bestLastIdx) * sizeof(*scan);
    const size_t coeffBytes = size_t(coeffCount) * sizeof(*dstCoeff);
    if (overlaps(dstCoeff, coeffBytes, scan, scanBytes) ||
        overlaps(dstCoeff, coeffBytes, resiDctCoeff, coeffBytes))
        return rdo_recount_sign(scan, dstCoeff, resiDctCoeff, bestLastIdx,
                                 coeffCount);

    // Duplicated destinations are a loop-carried dependency, even across
    // vector chunks. x265 transforms have at most 1024 coefficients.
    uint64_t seen[16] = {};
    for (int pos = 0; pos < bestLastIdx; ++pos) {
        const uint16_t idx = scan[pos];
        const uint64_t bit = uint64_t(1) << (idx & 63);
        if (seen[idx >> 6] & bit)
            return rdo_recount_sign(scan, dstCoeff, resiDctCoeff, bestLastIdx,
                                     coeffCount);
        seen[idx >> 6] |= bit;
    }

    uint32_t numSig = 0;
    for (int pos = 0; pos < bestLastIdx;) {
        const size_t vl = __riscv_vsetvl_e16m1(bestLastIdx - pos);
        const vuint16m1_t idx = __riscv_vle16_v_u16m1(scan + pos, vl);
        const vuint16m1_t bytes = __riscv_vsll_vx_u16m1(idx, 1, vl);
        const vint16m1_t level = __riscv_vluxei16_v_i16m1(dstCoeff, bytes, vl);
        const vint16m1_t resi = __riscv_vluxei16_v_i16m1(resiDctCoeff, bytes, vl);
        numSig += __riscv_vcpop_m_b16(__riscv_vmsne_vx_i16m1_b16(level, 0, vl), vl);
        const vint16m1_t mask = __riscv_vsra_vx_i16m1(resi, 15, vl);
        const vint16m1_t signedLevel = __riscv_vsub_vv_i16m1(
            __riscv_vxor_vv_i16m1(level, mask, vl), mask, vl);
        __riscv_vsuxei16_v_i16m1(dstCoeff, bytes, signedLevel, vl);
        pos += static_cast<int>(vl);
    }
    return numSig;
}
