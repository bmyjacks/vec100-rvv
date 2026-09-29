/****************************************************************************
 *
 *
 *  Project: ICU4C 78.3
 *  Source files:
 *    icu/source/common/ucptrie.cpp
 *    icu/source/common/ustring.cpp
 *    icu/source/common/normalizer2impl.h
 *    icu/source/common/normalizer2impl.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * icu/source/common/ucptrie.cpp
 *
 * © 2017 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   ucptrie.cpp (modified from utrie2.cpp)
 *   created: 2017dec29 Markus W. Scherer
 *
 * icu/source/common/ustring.cpp
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1998-2016, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 * File ustring.cpp
 *
 * Modification History:
 *
 *   Date        Name        Description
 *   12/07/98    bertrand    Creation.
 *
 * icu/source/common/normalizer2impl.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 2009-2014, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *   file name:  normalizer2impl.h
 *   encoding:   UTF-8
 *   tab size:   8 (not used)
 *   indentation:4
 *
 *   created on: 2009nov22
 *   created by: Markus W. Scherer
 *
 * icu/source/common/normalizer2impl.cpp
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 2009-2014, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *   file name:  normalizer2impl.cpp
 *   encoding:   UTF-8
 *   tab size:   8 (not used)
 *   indentation:4
 *
 *   created on: 2009nov22
 *   created by: Markus W. Scherer
 *
 */

#include "kernel.h"

/*
 * icu/source/common/ucptrie.cpp:160-185
 */
static int32_t ucptrie_internalSmallIndex(const UCPTrie *trie, int32_t c) {
    int32_t i1 = c >> UCPTRIE_SHIFT_1;
    if (trie->type == UCPTRIE_TYPE_FAST) {
        i1 += UCPTRIE_BMP_INDEX_LENGTH - UCPTRIE_OMITTED_BMP_INDEX_1_LENGTH;
    } else {
        i1 += UCPTRIE_SMALL_INDEX_LENGTH;
    }
    int32_t i3Block =
        trie->index[(int32_t)trie->index[i1] +
                    ((c >> UCPTRIE_SHIFT_2) & UCPTRIE_INDEX_2_MASK)];
    int32_t i3 = (c >> UCPTRIE_SHIFT_3) & UCPTRIE_INDEX_3_MASK;
    int32_t dataBlock;
    if ((i3Block & 0x8000) == 0) {
        dataBlock = trie->index[i3Block + i3];
    } else {
        i3Block = (i3Block & 0x7fff) + (i3 & ~7) + (i3 >> 3);
        i3 &= 7;
        dataBlock =
            ((int32_t)trie->index[i3Block++] << (2 + (2 * i3))) & 0x30000;
        dataBlock |= trie->index[i3Block + i3];
    }
    return dataBlock + (c & UCPTRIE_SMALL_DATA_MASK);
}

/*
 * icu/source/common/ustring.cpp:199-217 (c == 0, the non-surrogate path)
 */
static const char16_t *u_strchr_zero(const char16_t *s) {
    char16_t cs;
    for (;;) {
        if ((cs = *s) == 0) {
            return s;
        }
        ++s;
    }
}

/*
 * icu/source/common/normalizer2impl.h:624-626
 */
static bool isMostDecompYesAndZeroCC(uint16_t norm16, uint16_t minYesNo) {
    return norm16 < minYesNo || norm16 == MIN_NORMAL_MAYBE_YES ||
           norm16 == JAMO_VT;
}

/*
 * icu/source/common/normalizer2impl.h:290-292
 */
static bool isDecompYes(uint16_t norm16, uint16_t minYesNo,
                        uint16_t minMaybeYes) {
    return norm16 < minYesNo || minMaybeYes <= norm16;
}

/*
 * icu/source/common/normalizer2impl.h:303-308
 */
static uint8_t getCCFromYesOrMaybeYes(uint16_t norm16) {
    return norm16 >= MIN_NORMAL_MAYBE_YES
               ? static_cast<uint8_t>(norm16 >> OFFSET_SHIFT)
               : 0;
}

/*
 * icu/source/common/normalizer2impl.cpp:526-547 (buffer == nullptr)
 */
static const char16_t *copyLowPrefixFromNulTerminated(const char16_t *src,
                                                      int32_t minNeedDataCP) {
    char16_t c;
    while ((c = *src++) < minNeedDataCP && c != 0) {
    }
    --src;
    return src;
}

/*
 * icu/source/common/normalizer2impl.cpp:584-665 (buffer == nullptr)
 */
static const char16_t *
decompose_quick_check(const char16_t *src, const char16_t *limit,
                      const UCPTrie *normTrie, char16_t minDecompNoCP,
                      uint16_t minYesNo, uint16_t minMaybeYes, bool error) {
    int32_t minNoCP = minDecompNoCP;
    if (limit == nullptr) {
        src = copyLowPrefixFromNulTerminated(src, minNoCP);
        if (error) {
            return src;
        }
        limit = u_strchr_zero(src);
    }

    const char16_t *prevSrc;
    int32_t c = 0;
    uint16_t norm16 = 0;

    const char16_t *prevBoundary = src;
    uint8_t prevCC = 0;

    for (;;) {
        for (prevSrc = src; src != limit;) {
            if ((c = *src) < minNoCP ||
                isMostDecompYesAndZeroCC(
                    norm16 = UCPTRIE_FAST_BMP_GET(normTrie, UCPTRIE_16, c),
                    minYesNo)) {
                ++src;
            } else if (!U16_IS_LEAD(c)) {
                break;
            } else {
                char16_t c2;
                if ((src + 1) != limit && U16_IS_TRAIL(c2 = src[1])) {
                    c = U16_GET_SUPPLEMENTARY(c, c2);
                    norm16 = UCPTRIE_FAST_SUPP_GET(normTrie, UCPTRIE_16, c);
                    if (isMostDecompYesAndZeroCC(norm16, minYesNo)) {
                        src += 2;
                    } else {
                        break;
                    }
                } else {
                    ++src;
                }
            }
        }
        if (src != prevSrc) {
            prevCC = 0;
            prevBoundary = src;
        }
        if (src == limit) {
            break;
        }

        src += U16_LENGTH(c);
        if (isDecompYes(norm16, minYesNo, minMaybeYes)) {
            uint8_t cc = getCCFromYesOrMaybeYes(norm16);
            if (prevCC <= cc || cc == 0) {
                prevCC = cc;
                if (cc <= 1) {
                    prevBoundary = src;
                }
                continue;
            }
        }
        return prevBoundary;
    }
    return src;
}

/*
 * Wrapper for the selected quick-check path.
 */
const char16_t *decomposeQuickCheck(const char16_t *src, const char16_t *limit,
                                    const UCPTrie *normTrie,
                                    char16_t minDecompNoCP, uint16_t minYesNo,
                                    uint16_t minMaybeYes, bool error) {
    return decompose_quick_check(src, limit, normTrie, minDecompNoCP, minYesNo,
                                 minMaybeYes, error);
}
