/****************************************************************************
 *
 *
 *  Project: ICU4C 78.3
 *  Source files:
 *    icu/source/common/unicode/ucptrie.h
 *    icu/source/common/ucptrie_impl.h
 *    icu/source/common/unicode/utf16.h
 *    icu/source/common/normalizer2impl.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * icu/source/common/unicode/ucptrie.h
 *
 * © 2017 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   ucptrie.h (modified from utrie2.h)
 *   created: 2017dec29 Markus W. Scherer
 *
 * icu/source/common/ucptrie_impl.h
 *
 * © 2017 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   ucptrie_impl.h (modified from utrie2_impl.h)
 *   created: 2017dec29 Markus W. Scherer
 *
 * icu/source/common/unicode/utf16.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1999-2012, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *   file name:  utf16.h
 *   encoding:   UTF-8
 *   tab size:   8 (not used)
 *   indentation:4
 *
 *   created on: 1999sep09
 *   created by: Markus W. Scherer
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
 */

#ifndef KERNELS_12_DECOMPOSE_INCLUDE_KERNEL_H_
#define KERNELS_12_DECOMPOSE_INCLUDE_KERNEL_H_

#include <cstdint>

/*
 * icu/source/common/unicode/ucptrie.h:30-39
 */
typedef union UCPTrieData {
    const void *ptr0;
    const uint16_t *ptr16;
    const uint32_t *ptr32;
    const uint8_t *ptr8;
} UCPTrieData;

/*
 * icu/source/common/unicode/ucptrie.h:59-108
 */
struct UCPTrie {
    const uint16_t *index;
    UCPTrieData data;
    int32_t indexLength;
    int32_t dataLength;
    int32_t highStart;
    uint16_t shifted12HighStart;
    int8_t type;
    int8_t valueWidth;
    uint32_t reserved32;
    uint16_t reserved16;
    uint16_t index3NullOffset;
    int32_t dataNullOffset;
    uint32_t nullValue;
};

/*
 * icu/source/common/unicode/ucptrie.h:119-139
 */
enum UCPTrieType {
    UCPTRIE_TYPE_ANY = -1,
    UCPTRIE_TYPE_FAST,
    UCPTRIE_TYPE_SMALL
};

/*
 * icu/source/common/unicode/ucptrie.h:326
 */
#define UCPTRIE_16(trie, i) ((trie)->data.ptr16[i])

/*
 * icu/source/common/unicode/ucptrie.h:530
 */
#define UCPTRIE_FAST_BMP_GET(trie, dataAccess, c)                              \
    dataAccess(trie, _UCPTRIE_FAST_INDEX(trie, c))

/*
 * icu/source/common/unicode/ucptrie.h:542
 */
#define UCPTRIE_FAST_SUPP_GET(trie, dataAccess, c)                             \
    dataAccess(trie, _UCPTRIE_SMALL_INDEX(trie, c))

/*
 * icu/source/common/unicode/ucptrie.h:546-624
 */
enum {
    UCPTRIE_FAST_SHIFT = 6,
    UCPTRIE_FAST_DATA_BLOCK_LENGTH = 1 << UCPTRIE_FAST_SHIFT,
    UCPTRIE_FAST_DATA_MASK = UCPTRIE_FAST_DATA_BLOCK_LENGTH - 1,
    UCPTRIE_HIGH_VALUE_NEG_DATA_OFFSET = 2
};

#define _UCPTRIE_FAST_INDEX(trie, c)                                           \
    ((int32_t)(trie)->index[(c) >> UCPTRIE_FAST_SHIFT] +                       \
     ((c) & UCPTRIE_FAST_DATA_MASK))

#define _UCPTRIE_SMALL_INDEX(trie, c)                                          \
    ((c) >= (trie)->highStart                                                  \
         ? (trie)->dataLength - UCPTRIE_HIGH_VALUE_NEG_DATA_OFFSET             \
         : ucptrie_internalSmallIndex(trie, c))

/*
 * icu/source/common/ucptrie_impl.h:73-125
 */
constexpr int32_t UCPTRIE_BMP_INDEX_LENGTH = 0x10000 >> UCPTRIE_FAST_SHIFT;
constexpr int32_t UCPTRIE_SMALL_LIMIT = 0x1000;
constexpr int32_t UCPTRIE_SMALL_INDEX_LENGTH =
    UCPTRIE_SMALL_LIMIT >> UCPTRIE_FAST_SHIFT;
constexpr int32_t UCPTRIE_SHIFT_3 = 4;
constexpr int32_t UCPTRIE_SHIFT_2 = 5 + UCPTRIE_SHIFT_3;
constexpr int32_t UCPTRIE_SHIFT_1 = 5 + UCPTRIE_SHIFT_2;
constexpr int32_t UCPTRIE_SHIFT_2_3 = UCPTRIE_SHIFT_2 - UCPTRIE_SHIFT_3;
constexpr int32_t UCPTRIE_SHIFT_1_2 = UCPTRIE_SHIFT_1 - UCPTRIE_SHIFT_2;
constexpr int32_t UCPTRIE_OMITTED_BMP_INDEX_1_LENGTH =
    0x10000 >> UCPTRIE_SHIFT_1;
constexpr int32_t UCPTRIE_INDEX_2_BLOCK_LENGTH = 1 << UCPTRIE_SHIFT_1_2;
constexpr int32_t UCPTRIE_INDEX_2_MASK = UCPTRIE_INDEX_2_BLOCK_LENGTH - 1;
constexpr int32_t UCPTRIE_INDEX_3_BLOCK_LENGTH = 1 << UCPTRIE_SHIFT_2_3;
constexpr int32_t UCPTRIE_INDEX_3_MASK = UCPTRIE_INDEX_3_BLOCK_LENGTH - 1;
constexpr int32_t UCPTRIE_SMALL_DATA_BLOCK_LENGTH = 1 << UCPTRIE_SHIFT_3;
constexpr int32_t UCPTRIE_SMALL_DATA_MASK = UCPTRIE_SMALL_DATA_BLOCK_LENGTH - 1;

/*
 * icu/source/common/unicode/utf16.h:59
 */
#define U16_IS_LEAD(c) (((c) & 0xfffffc00) == 0xd800)

/*
 * icu/source/common/unicode/utf16.h:67
 */
#define U16_IS_TRAIL(c) (((c) & 0xfffffc00) == 0xdc00)

/*
 * icu/source/common/unicode/utf16.h:99
 */
#define U16_SURROGATE_OFFSET ((0xd800 << 10UL) + 0xdc00 - 0x10000)

/*
 * icu/source/common/unicode/utf16.h:112-113
 */
#define U16_GET_SUPPLEMENTARY(lead, trail)                                     \
    (((int32_t)(lead) << 10UL) + (int32_t)(trail) - U16_SURROGATE_OFFSET)

/*
 * icu/source/common/unicode/utf16.h:141
 */
#define U16_LENGTH(c) ((uint32_t)(c) <= 0xffff ? 1 : 2)

/*
 * icu/source/common/normalizer2impl.h:409-430
 */
enum { JAMO_VT = 0xfe00, MIN_NORMAL_MAYBE_YES = 0xfc00, OFFSET_SHIFT = 1 };

/*
 * Wrapper for the buffer == nullptr quick-check configuration of decompose.
 * A null limit denotes a NUL-terminated UTF-16 input.
 */
const char16_t *decomposeQuickCheck(const char16_t *src, const char16_t *limit,
                                    const UCPTrie *normTrie,
                                    char16_t minDecompNoCP, uint16_t minYesNo,
                                    uint16_t minMaybeYes, bool error = false);

#endif // KERNELS_12_DECOMPOSE_INCLUDE_KERNEL_H_
