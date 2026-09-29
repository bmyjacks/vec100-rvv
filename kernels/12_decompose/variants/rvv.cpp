#include "kernel.h"

#include <riscv_vector.h>

// Scan only the portion that is unconditionally normalization-inert. A trie
// lookup, surrogate pair, or combining-class boundary still goes through the
// original sequential quick-check state machine below.
static const char16_t *low_prefix(const char16_t *src, const char16_t *limit,
                                  uint16_t minNoCP) {
    while (src != limit) {
        size_t vl = __riscv_vsetvl_e16m1(limit - src);
        vuint16m1_t chars = __riscv_vle16_v_u16m1(
            reinterpret_cast<const uint16_t *>(src), vl);
        vbool16_t high = __riscv_vmsgeu_vx_u16m1_b16(chars, minNoCP, vl);
        long first = __riscv_vfirst_m_b16(high, vl);
        if (first >= 0) return src + first;
        src += vl;
    }
    return src;
}

static int32_t small_index(const UCPTrie *trie, int32_t c) {
    int32_t i1 = c >> UCPTRIE_SHIFT_1;
    if (trie->type == UCPTRIE_TYPE_FAST)
        i1 += UCPTRIE_BMP_INDEX_LENGTH - UCPTRIE_OMITTED_BMP_INDEX_1_LENGTH;
    else
        i1 += UCPTRIE_SMALL_INDEX_LENGTH;
    int32_t i3Block = trie->index[(int32_t)trie->index[i1] +
                                  ((c >> UCPTRIE_SHIFT_2) & UCPTRIE_INDEX_2_MASK)];
    int32_t i3 = (c >> UCPTRIE_SHIFT_3) & UCPTRIE_INDEX_3_MASK;
    int32_t dataBlock;
    if ((i3Block & 0x8000) == 0) {
        dataBlock = trie->index[i3Block + i3];
    } else {
        i3Block = (i3Block & 0x7fff) + (i3 & ~7) + (i3 >> 3);
        i3 &= 7;
        dataBlock = ((int32_t)trie->index[i3Block++] << (2 + (2 * i3))) & 0x30000;
        dataBlock |= trie->index[i3Block + i3];
    }
    return dataBlock + (c & UCPTRIE_SMALL_DATA_MASK);
}

static bool most_yes_zero(uint16_t norm16, uint16_t minYesNo) {
    return norm16 < minYesNo || norm16 == MIN_NORMAL_MAYBE_YES || norm16 == JAMO_VT;
}

const char16_t *decomposeQuickCheck_rvv(
    const char16_t *src, const char16_t *limit, const UCPTrie *normTrie,
    char16_t minDecompNoCP, uint16_t minYesNo, uint16_t minMaybeYes, bool error) {
    const int32_t minNoCP = minDecompNoCP;
    if (limit == nullptr) {
        // An unbounded vector load could cross the NUL allocation boundary.
        // Retain the source's byte-exact unbounded scan in this mode.
        char16_t c;
        while ((c = *src++) < minNoCP && c != 0) {}
        --src;
        if (error) return src;
        limit = src;
        while (*limit != 0) ++limit;
    }

    const char16_t *prevSrc;
    int32_t c = 0;
    uint16_t norm16 = 0;
    const char16_t *prevBoundary = src;
    uint8_t prevCC = 0;
    for (;;) {
        for (prevSrc = src; src != limit;) {
            src = low_prefix(src, limit, uint16_t(minNoCP));
            if (src == limit) break;
            if ((c = *src) < minNoCP ||
                most_yes_zero(norm16 = UCPTRIE_FAST_BMP_GET(normTrie, UCPTRIE_16, c), minYesNo)) {
                ++src;
            } else if (!U16_IS_LEAD(c)) {
                break;
            } else {
                char16_t c2;
                if ((src + 1) != limit && U16_IS_TRAIL(c2 = src[1])) {
                    c = U16_GET_SUPPLEMENTARY(c, c2);
                    norm16 = normTrie->data.ptr16[
                        c >= normTrie->highStart ? normTrie->dataLength - UCPTRIE_HIGH_VALUE_NEG_DATA_OFFSET
                                                 : small_index(normTrie, c)];
                    if (most_yes_zero(norm16, minYesNo)) src += 2;
                    else break;
                } else {
                    ++src;
                }
            }
        }
        if (src != prevSrc) { prevCC = 0; prevBoundary = src; }
        if (src == limit) break;
        src += U16_LENGTH(c);
        if (norm16 < minYesNo || minMaybeYes <= norm16) {
            uint8_t cc = norm16 >= MIN_NORMAL_MAYBE_YES
                             ? static_cast<uint8_t>(norm16 >> OFFSET_SHIFT) : 0;
            if (prevCC <= cc || cc == 0) {
                prevCC = cc;
                if (cc <= 1) prevBoundary = src;
                continue;
            }
        }
        return prevBoundary;
    }
    return src;
}
