#include "kernel.h"

#include <riscv_vector.h>

static bool overlaps(const void *a, size_t an, const void *b, size_t bn) {
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return an && bn && (x <= y ? y - x < an : x - y < bn);
}

// Count every matching lane, rather than gathering/incrementing/scattering:
// repeated symbols within the same vector must contribute independently.
static U32 occurrences(const BYTE *src, size_t n, ptrdiff_t stride, BYTE symbol) {
    U32 total = 0;
    while (n) {
        const size_t vl = __riscv_vsetvl_e8m1(n);
        const vuint8m1_t bytes = __riscv_vlse8_v_u8m1(src, stride, vl);
        const vbool8_t same = __riscv_vmseq_vx_u8m1_b8(bytes, symbol, vl);
        total += static_cast<U32>(__riscv_vcpop_m_b8(same, vl));
        src += vl * stride;
        n -= vl;
    }
    return total;
}

void HIST_add_rvv(unsigned *count, const void *src, size_t srcSize) {
    const BYTE *bytes = static_cast<const BYTE *>(src);
    if (overlaps(count, 256 * sizeof(*count), src, srcSize)) {
        for (size_t i = 0; i < srcSize; ++i)
            ++count[bytes[i]];
        return;
    }
    for (unsigned s = 0; s < 256; ++s)
        count[s] += occurrences(bytes, srcSize, 1, static_cast<BYTE>(s));
}

unsigned HIST_count_simple_rvv(unsigned *count, unsigned *maxSymbolValuePtr,
                           const void *src, size_t srcSize) {
    unsigned maxSymbolValue = *maxSymbolValuePtr;
    const bool alias = overlaps(count, (size_t(maxSymbolValue) + 1) * sizeof(*count),
                                src, srcSize);
    ZSTD_memset(count, 0, (maxSymbolValue + 1) * sizeof(*count));
    if (!srcSize) {
        *maxSymbolValuePtr = 0;
        return 0;
    }
    const BYTE *bytes = static_cast<const BYTE *>(src);
    if (alias) {
        for (size_t i = 0; i < srcSize; ++i) {
            assert(bytes[i] <= maxSymbolValue);
            ++count[bytes[i]];
        }
    } else {
        for (unsigned s = 0; s <= maxSymbolValue && s < 256; ++s)
            count[s] = occurrences(bytes, srcSize, 1, static_cast<BYTE>(s));
    }
    while (!count[maxSymbolValue])
        --maxSymbolValue;
    *maxSymbolValuePtr = maxSymbolValue;
    unsigned largest = 0;
    for (unsigned s = 0; s <= maxSymbolValue; ++s)
        if (count[s] > largest)
            largest = count[s];
    return largest;
}

static void scalar_stripes(U32 *workSpace, const BYTE *source, size_t sourceSize) {
    const BYTE *ip = source;
    const BYTE *const end = source + sourceSize;
    U32 *const a = workSpace;
    U32 *const b = a + 256;
    U32 *const c = b + 256;
    U32 *const d = c + 256;
    U32 cached = MEM_read32(ip);
    ip += 4;
    while (ip < end - 15) {
        for (int j = 0; j < 4; ++j) {
            const U32 word = cached;
            cached = MEM_read32(ip);
            ip += 4;
            ++a[static_cast<BYTE>(word)];
            ++b[static_cast<BYTE>(word >> 8)];
            ++c[static_cast<BYTE>(word >> 16)];
            ++d[word >> 24];
        }
    }
    ip -= 4;
    while (ip < end)
        ++a[*ip++];
}

size_t HIST_count_parallel_wksp_bench_rvv(unsigned *count,
                                      unsigned *maxSymbolValuePtr,
                                      const void *source, size_t sourceSize,
                                      HIST_checkInput_e check,
                                      U32 *const workSpace) {
    const size_t countSize = (*maxSymbolValuePtr + 1) * sizeof(*count);
    assert(*maxSymbolValuePtr <= 255);
    if (!sourceSize) {
        ZSTD_memset(count, 0, countSize);
        *maxSymbolValuePtr = 0;
        return 0;
    }
    const bool alias = overlaps(workSpace, HIST_WKSP_SIZE, source, sourceSize);
    ZSTD_memset(workSpace, 0, HIST_WKSP_SIZE);
    const BYTE *bytes = static_cast<const BYTE *>(source);
    if (alias) {
        scalar_stripes(workSpace, bytes, sourceSize);
    } else {
        // The first full 16-byte stripe is counted only for N >= 20;
        // the remaining 4..19 bytes all go into Counting1.
        const size_t bulk = sourceSize < 20 ? 0 :
                            16 * (1 + (sourceSize - 20) / 16);
        for (unsigned s = 0; s < 256; ++s) {
            if (bulk)
                for (size_t stripe = 0; stripe < 4; ++stripe)
                    workSpace[stripe * 256 + s] = occurrences(
                        bytes + stripe, bulk / 4, 4, static_cast<BYTE>(s));
            workSpace[s] += occurrences(bytes + bulk, sourceSize - bulk, 1,
                                        static_cast<BYTE>(s));
        }
    }
    unsigned max = 0;
    for (unsigned s = 0; s < 256; ++s) {
        workSpace[s] += workSpace[256 + s] + workSpace[512 + s] + workSpace[768 + s];
        if (workSpace[s] > max)
            max = workSpace[s];
    }
    unsigned symbol = 255;
    while (!workSpace[symbol])
        --symbol;
    if (check && symbol > *maxSymbolValuePtr)
        return ERROR(maxSymbolValue_tooSmall);
    *maxSymbolValuePtr = symbol;
    ZSTD_memmove(count, workSpace, countSize);
    return max;
}
