#include "kernel.h"

#include <riscv_vector.h>

// An overlap with either input makes a batched update observable to subsequent
// input reads. In that case preserve the upstream, position-by-position order.
static bool overlaps(const void *a, size_t an, const void *b, size_t bn) {
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return an && bn && (x <= y ? y - x < an : x - y < bn);
}

void FASTCOVER_computeFrequency_bench_rvv(U32 *freqs, const BYTE *samples,
                                      size_t *offsets, size_t nbTrainSamples,
                                      size_t nbSamples, unsigned f, unsigned d,
                                      unsigned skip) {
    assert(nbTrainSamples >= 5 && nbTrainSamples <= nbSamples);
    const size_t stride = static_cast<size_t>(skip) + 1;
    const size_t readLength = MAX(d, 8);
    const size_t bins = size_t(1) << f; // upstream f is in [1,31]
    const bool alias = overlaps(freqs, bins * sizeof(U32), offsets,
                                (nbTrainSamples + 1) * sizeof(size_t)) ||
                       overlaps(freqs, bins * sizeof(U32), samples,
                                offsets[nbTrainSamples]);
    if (alias) {
        for (size_t i = 0; i < nbTrainSamples; ++i) {
            size_t start = offsets[i];
            const size_t end = offsets[i + 1];
            while (start + readLength <= end) {
                const size_t idx = d == 6 ? ZSTD_hash6Ptr(samples + start, f)
                                          : ZSTD_hash8Ptr(samples + start, f);
                ++freqs[idx];
                start += stride;
            }
        }
        return;
    }

    for (size_t i = 0; i < nbTrainSamples; ++i) {
        size_t start = offsets[i];
        const size_t end = offsets[i + 1];
        if (end < readLength || start > end - readLength)
            continue;
        // Limit VL to the temporary index array; the algorithm is VLEN agnostic.
        while (start <= end - readLength) {
            const size_t remaining = 1 + (end - readLength - start) / stride;
            const size_t vl = __riscv_vsetvl_e64m1(remaining < 32 ? remaining : 32);
            const vuint64m1_t words = __riscv_vlse64_v_u64m1(
                reinterpret_cast<const U64 *>(samples + start), stride, vl);
            const vuint64m1_t input =
                d == 6 ? __riscv_vsll_vx_u64m1(words, 16, vl) : words;
            const vuint64m1_t hash = __riscv_vmul_vx_u64m1(
                input, d == 6 ? prime6bytes : prime8bytes, vl);
            const vuint64m1_t indices = __riscv_vsrl_vx_u64m1(hash, 64 - f, vl);
            U64 slots[32];
            __riscv_vse64_v_u64m1(slots, indices, vl);

            // Each distinct index is incremented once by its vector population.
            // A gather/add/scatter would lose increments for repeated indices.
            bool consumed[32] = {};
            for (size_t lane = 0; lane < vl; ++lane) {
                if (consumed[lane])
                    continue;
                const U64 idx = slots[lane];
                const vbool64_t equal = __riscv_vmseq_vx_u64m1_b64(indices, idx, vl);
                const size_t multiplicity = __riscv_vcpop_m_b64(equal, vl);
                freqs[idx] += static_cast<U32>(multiplicity);
                for (size_t other = lane + 1; other < vl; ++other)
                    consumed[other] |= slots[other] == idx;
            }
            start += vl * stride;
        }
    }
}
