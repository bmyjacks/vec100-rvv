#include "kernel.h"

#include <climits>
#include <cstdint>
#include <riscv_vector.h>

// Compile the exact extracted decoder and its two helpers into this translation
// unit with private names. This preserves the scalar paths for preflight,
// malformed sequences, NUL-terminated input and aliasing without requiring
// src/kernel.cpp to be linked. The original symbols remain available to the
// differential test from its independently compiled scalar object.
#undef U_CAPI
#define U_CAPI static
#undef utf8_nextCharSafeBody
#undef u_terminateUChars
#undef u_strFromUTF8WithSub
#define utf8_nextCharSafeBody utf8_nextCharSafeBody_local
#define u_terminateUChars u_terminateUChars_local
#define u_strFromUTF8WithSub u_strFromUTF8WithSub_local
#include "../src/kernel.cpp"
#undef utf8_nextCharSafeBody
#undef u_terminateUChars
#undef u_strFromUTF8WithSub

static bool overlaps(const void *a, size_t na, const void *b, size_t nb) {
    const uintptr_t x = reinterpret_cast<uintptr_t>(a);
    const uintptr_t y = reinterpret_cast<uintptr_t>(b);
    return x <= UINTPTR_MAX - na && y <= UINTPTR_MAX - nb &&
           x < y + nb && y < x + na;
}

UChar *u_strFromUTF8WithSub_rvv(UChar *dest, int32_t destCapacity,
                                int32_t *pDestLength, const char *src,
                                int32_t srcLength, UChar32 subchar,
                                int32_t *pNumSubstitutions,
                                UErrorCode *pErrorCode) {
    // Retain the scalar entry's argument checks, preflight/NUL-terminated
    // paths, error ordering, partial writes, and possible in-place reads.
    if (srcLength <= 0 || srcLength > INT32_MAX / 2 ||
        destCapacity <= srcLength || !src || !dest || !pErrorCode ||
        U_FAILURE(*pErrorCode) || subchar > 0x10ffff || U_IS_SURROGATE(subchar)) {
        return u_strFromUTF8WithSub_local(dest, destCapacity, pDestLength, src,
                                          srcLength, subchar, pNumSubstitutions,
                                          pErrorCode);
    }

    const size_t inputBytes = static_cast<size_t>(srcLength);
    const size_t outputBytes = static_cast<size_t>(destCapacity) * sizeof(UChar);
    if (overlaps(src, inputBytes, dest, outputBytes) ||
        overlaps(pErrorCode, sizeof(*pErrorCode), src, inputBytes) ||
        overlaps(pErrorCode, sizeof(*pErrorCode), dest, outputBytes) ||
        (pDestLength &&
         (overlaps(pDestLength, sizeof(*pDestLength), src, inputBytes) ||
          overlaps(pDestLength, sizeof(*pDestLength), dest, outputBytes) ||
          overlaps(pDestLength, sizeof(*pDestLength), pErrorCode,
                   sizeof(*pErrorCode)))) ||
        (pNumSubstitutions &&
         (overlaps(pNumSubstitutions, sizeof(*pNumSubstitutions), src, inputBytes) ||
          overlaps(pNumSubstitutions, sizeof(*pNumSubstitutions), dest, outputBytes) ||
          overlaps(pNumSubstitutions, sizeof(*pNumSubstitutions), pErrorCode,
                   sizeof(*pErrorCode)) ||
          (pDestLength && overlaps(pNumSubstitutions,
                                   sizeof(*pNumSubstitutions), pDestLength,
                                   sizeof(*pDestLength)))))) {
        return u_strFromUTF8WithSub_local(dest, destCapacity, pDestLength, src,
                                          srcLength, subchar, pNumSubstitutions,
                                          pErrorCode);
    }

    int32_t pos = 0;
    while (pos < srcLength) {
        const size_t vl = __riscv_vsetvl_e8m1(srcLength - pos);
        const auto bytes = __riscv_vle8_v_u8m1(
            reinterpret_cast<const uint8_t *>(src + pos), vl);
        const auto high = __riscv_vmsgeu_vx_u8m1_b8(bytes, 128, vl);
        const long first = __riscv_vfirst_m_b8(high, vl);
        const size_t ascii = first < 0 ? vl : static_cast<size_t>(first);
        if (ascii) {
            const auto widened = __riscv_vzext_vf2_u16m2(bytes, ascii);
            __riscv_vse16_v_u16m2(reinterpret_cast<uint16_t *>(dest + pos),
                                  widened, ascii);
            pos += static_cast<int32_t>(ascii);
        }
        if (first >= 0) break;
    }

    int32_t suffixLength = 0;
    UChar *result = u_strFromUTF8WithSub_local(
        dest + pos, destCapacity - pos, &suffixLength, src + pos,
        srcLength - pos, subchar, pNumSubstitutions, pErrorCode);
    if (!result) return nullptr;
    if (pDestLength) *pDestLength = pos + suffixLength;
    return dest;
}
