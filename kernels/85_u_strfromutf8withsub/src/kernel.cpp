/****************************************************************************
 *
 *
 *  Project: ICU4C 78.3
 *  Source files:
 *    icu/source/common/utf_impl.cpp
 *    icu/source/common/ustring.cpp
 *    icu/source/common/ustrtrns.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * icu/source/common/utf_impl.cpp
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1999-2012, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *   file name:  utf_impl.cpp
 *   encoding:   UTF-8
 *   tab size:   8 (not used)
 *   indentation:4
 *
 *   created on: 1999sep13
 *   created by: Markus W. Scherer
 *
 *   This file provides implementation functions for macros in the utfXX.h
 *   that would otherwise be too long as macros.
 *
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
 * icu/source/common/ustrtrns.cpp
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 2001-2016, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 * File ustrtrns.cpp
 *
 * Modification History:
 *
 *   Date        Name        Description
 *   9/10/2001    Ram    Creation.
 *
 */

#include "kernel.h"

/*
 * icu/source/common/utf_impl.cpp:87-104
 */
static const UChar32 utf8_errorValue[6] = {0x15, 0x9f, 0xffff, 0x10ffff};

static UChar32 errorValue(int32_t count, int8_t strict) {
    if (strict >= 0) {
        return utf8_errorValue[count];
    } else if (strict == -3) {
        return 0xfffd;
    } else {
        return U_SENTINEL;
    }
}

/*
 * icu/source/common/utf_impl.cpp:128-186
 */
U_CAPI UChar32 U_EXPORT2 utf8_nextCharSafeBody(const uint8_t *s, int32_t *pi,
                                               int32_t length, UChar32 c,
                                               int8_t strict) {
    int32_t i = *pi;
    if (i == length || c > 0xf4) {
    } else if (c >= 0xf0) {
        uint8_t t1 = s[i], t2, t3;
        c &= 7;
        if (U8_IS_VALID_LEAD4_AND_T1(c, t1) && ++i != length &&
            (t2 = s[i] - 0x80) <= 0x3f && ++i != length &&
            (t3 = s[i] - 0x80) <= 0x3f) {
            ++i;
            c = (c << 18) | ((t1 & 0x3f) << 12) | (t2 << 6) | t3;
            if (strict <= 0 || !U_IS_UNICODE_NONCHAR(c)) {
                *pi = i;
                return c;
            }
        }
    } else if (c >= 0xe0) {
        c &= 0xf;
        if (strict != -2) {
            uint8_t t1 = s[i], t2;
            if (U8_IS_VALID_LEAD3_AND_T1(c, t1) && ++i != length &&
                (t2 = s[i] - 0x80) <= 0x3f) {
                ++i;
                c = (c << 12) | ((t1 & 0x3f) << 6) | t2;
                if (strict <= 0 || !U_IS_UNICODE_NONCHAR(c)) {
                    *pi = i;
                    return c;
                }
            }
        } else {
            uint8_t t1 = s[i] - 0x80, t2;
            if (t1 <= 0x3f && (c > 0 || t1 >= 0x20) && ++i != length &&
                (t2 = s[i] - 0x80) <= 0x3f) {
                *pi = i + 1;
                return (c << 12) | (t1 << 6) | t2;
            }
        }
    } else if (c >= 0xc2) {
        uint8_t t1 = s[i] - 0x80;
        if (t1 <= 0x3f) {
            *pi = i + 1;
            return ((c - 0xc0) << 6) | t1;
        }
    }
    c = errorValue(i - *pi, strict);
    *pi = i;
    return c;
}

/*
 * icu/source/common/ustring.cpp:1437-1458
 */
#define __TERMINATE_STRING(dest, destCapacity, length, pErrorCode)             \
    UPRV_BLOCK_MACRO_BEGIN {                                                   \
        if (pErrorCode != nullptr && U_SUCCESS(*pErrorCode)) {                 \
            if (length < 0) {                                                  \
            } else if (length < destCapacity) {                                \
                dest[length] = 0;                                              \
                if (*pErrorCode == U_STRING_NOT_TERMINATED_WARNING) {          \
                    *pErrorCode = U_ZERO_ERROR;                                \
                }                                                              \
            } else if (length == destCapacity) {                               \
                *pErrorCode = U_STRING_NOT_TERMINATED_WARNING;                 \
            } else {                                                           \
                *pErrorCode = U_BUFFER_OVERFLOW_ERROR;                         \
            }                                                                  \
        }                                                                      \
    }                                                                          \
    UPRV_BLOCK_MACRO_END

/*
 * icu/source/common/ustring.cpp:1468-1472
 */
U_CAPI int32_t U_EXPORT2 u_terminateUChars(char16_t *dest, int32_t destCapacity,
                                           int32_t length,
                                           UErrorCode *pErrorCode) {
    __TERMINATE_STRING(dest, destCapacity, length, pErrorCode);
    return length;
}

/*
 * icu/source/common/ustrtrns.cpp:259-537
 */
U_CAPI char16_t *U_EXPORT2
u_strFromUTF8WithSub(char16_t *dest, int32_t destCapacity, int32_t *pDestLength,
                     const char *src, int32_t srcLength, UChar32 subchar,
                     int32_t *pNumSubstitutions, UErrorCode *pErrorCode) {

    if (U_FAILURE(*pErrorCode)) {
        return nullptr;
    }
    if ((src == nullptr && srcLength != 0) || srcLength < -1 ||
        (destCapacity < 0) || (dest == nullptr && destCapacity > 0) ||
        subchar > 0x10ffff || U_IS_SURROGATE(subchar)) {
        *pErrorCode = U_ILLEGAL_ARGUMENT_ERROR;
        return nullptr;
    }

    if (pNumSubstitutions != nullptr) {
        *pNumSubstitutions = 0;
    }
    char16_t *pDest = dest;
    char16_t *pDestLimit = dest + destCapacity;
    int32_t reqLength = 0;
    int32_t numSubstitutions = 0;

    if (srcLength < 0) {

        int32_t i;
        UChar32 c;
        for (i = 0; (c = (uint8_t)src[i]) != 0 && (pDest < pDestLimit);) {

            ++i;
            if (U8_IS_SINGLE(c)) {
                *pDest++ = (char16_t)c;
            } else {
                uint8_t __t1, __t2;
                if ((0xe0 <= (c) && (c) < 0xf0) &&
                    U8_IS_VALID_LEAD3_AND_T1((c), src[i]) &&
                    (__t2 = src[(i) + 1] - 0x80) <= 0x3f) {
                    *pDest++ =
                        (((c) & 0xf) << 12) | ((src[i] & 0x3f) << 6) | __t2;
                    i += 2;
                } else if (((c) < 0xe0 && (c) >= 0xc2) &&
                           (__t1 = src[i] - 0x80) <= 0x3f) {
                    *pDest++ = (((c) & 0x1f) << 6) | __t1;
                    ++(i);
                } else {

                    (c) = utf8_nextCharSafeBody((const uint8_t *)src, &(i), -1,
                                                c, -1);
                    if (c < 0 && (++numSubstitutions, c = subchar) < 0) {
                        *pErrorCode = U_INVALID_CHAR_FOUND;
                        return nullptr;
                    } else if (c <= 0xFFFF) {
                        *(pDest++) = (char16_t)c;
                    } else {
                        *(pDest++) = U16_LEAD(c);
                        if (pDest < pDestLimit) {
                            *(pDest++) = U16_TRAIL(c);
                        } else {
                            reqLength++;
                            break;
                        }
                    }
                }
            }
        }

        while ((c = (uint8_t)src[i]) != 0) {

            ++i;
            if (U8_IS_SINGLE(c)) {
                ++reqLength;
            } else {
                uint8_t __t1, __t2;
                if ((0xe0 <= (c) && (c) < 0xf0) &&
                    U8_IS_VALID_LEAD3_AND_T1((c), src[i]) &&
                    (__t2 = src[(i) + 1] - 0x80) <= 0x3f) {
                    ++reqLength;
                    i += 2;
                } else if (((c) < 0xe0 && (c) >= 0xc2) &&
                           (__t1 = src[i] - 0x80) <= 0x3f) {
                    ++reqLength;
                    ++(i);
                } else {

                    (c) = utf8_nextCharSafeBody((const uint8_t *)src, &(i), -1,
                                                c, -1);
                    if (c < 0 && (++numSubstitutions, c = subchar) < 0) {
                        *pErrorCode = U_INVALID_CHAR_FOUND;
                        return nullptr;
                    }
                    reqLength += U16_LENGTH(c);
                }
            }
        }
    } else {

        int32_t i = 0;
        UChar32 c;
        for (;;) {

            int32_t count = (int32_t)(pDestLimit - pDest);
            int32_t count2 = (srcLength - i) / 3;
            if (count > count2) {
                count = count2;
            }
            if (count < 3) {

                break;
            }

            do {

                c = (uint8_t)src[i++];
                if (U8_IS_SINGLE(c)) {
                    *pDest++ = (char16_t)c;
                } else {
                    uint8_t __t1, __t2;
                    if ((0xe0 <= (c) && (c) < 0xf0) && ((i) + 1) < srcLength &&
                        U8_IS_VALID_LEAD3_AND_T1((c), src[i]) &&
                        (__t2 = src[(i) + 1] - 0x80) <= 0x3f) {
                        *pDest++ =
                            (((c) & 0xf) << 12) | ((src[i] & 0x3f) << 6) | __t2;
                        i += 2;
                    } else if (((c) < 0xe0 && (c) >= 0xc2) &&
                               ((i) != srcLength) &&
                               (__t1 = src[i] - 0x80) <= 0x3f) {
                        *pDest++ = (((c) & 0x1f) << 6) | __t1;
                        ++(i);
                    } else {
                        if (c >= 0xf0 || subchar > 0xffff) {

                            if (--count == 0) {
                                --i;
                                break;
                            }
                        }

                        (c) = utf8_nextCharSafeBody((const uint8_t *)src, &(i),
                                                    srcLength, c, -1);
                        if (c < 0 && (++numSubstitutions, c = subchar) < 0) {
                            *pErrorCode = U_INVALID_CHAR_FOUND;
                            return nullptr;
                        } else if (c <= 0xFFFF) {
                            *(pDest++) = (char16_t)c;
                        } else {
                            *(pDest++) = U16_LEAD(c);
                            *(pDest++) = U16_TRAIL(c);
                        }
                    }
                }
            } while (--count > 0);
        }

        while (i < srcLength && (pDest < pDestLimit)) {

            c = (uint8_t)src[i++];
            if (U8_IS_SINGLE(c)) {
                *pDest++ = (char16_t)c;
            } else {
                uint8_t __t1, __t2;
                if ((0xe0 <= (c) && (c) < 0xf0) && ((i) + 1) < srcLength &&
                    U8_IS_VALID_LEAD3_AND_T1((c), src[i]) &&
                    (__t2 = src[(i) + 1] - 0x80) <= 0x3f) {
                    *pDest++ =
                        (((c) & 0xf) << 12) | ((src[i] & 0x3f) << 6) | __t2;
                    i += 2;
                } else if (((c) < 0xe0 && (c) >= 0xc2) && ((i) != srcLength) &&
                           (__t1 = src[i] - 0x80) <= 0x3f) {
                    *pDest++ = (((c) & 0x1f) << 6) | __t1;
                    ++(i);
                } else {

                    (c) = utf8_nextCharSafeBody((const uint8_t *)src, &(i),
                                                srcLength, c, -1);
                    if (c < 0 && (++numSubstitutions, c = subchar) < 0) {
                        *pErrorCode = U_INVALID_CHAR_FOUND;
                        return nullptr;
                    } else if (c <= 0xFFFF) {
                        *(pDest++) = (char16_t)c;
                    } else {
                        *(pDest++) = U16_LEAD(c);
                        if (pDest < pDestLimit) {
                            *(pDest++) = U16_TRAIL(c);
                        } else {
                            reqLength++;
                            break;
                        }
                    }
                }
            }
        }

        while (i < srcLength) {

            c = (uint8_t)src[i++];
            if (U8_IS_SINGLE(c)) {
                ++reqLength;
            } else {
                uint8_t __t1, __t2;
                if ((0xe0 <= (c) && (c) < 0xf0) && ((i) + 1) < srcLength &&
                    U8_IS_VALID_LEAD3_AND_T1((c), src[i]) &&
                    (__t2 = src[(i) + 1] - 0x80) <= 0x3f) {
                    ++reqLength;
                    i += 2;
                } else if (((c) < 0xe0 && (c) >= 0xc2) && ((i) != srcLength) &&
                           (__t1 = src[i] - 0x80) <= 0x3f) {
                    ++reqLength;
                    ++(i);
                } else {

                    (c) = utf8_nextCharSafeBody((const uint8_t *)src, &(i),
                                                srcLength, c, -1);
                    if (c < 0 && (++numSubstitutions, c = subchar) < 0) {
                        *pErrorCode = U_INVALID_CHAR_FOUND;
                        return nullptr;
                    }
                    reqLength += U16_LENGTH(c);
                }
            }
        }
    }

    reqLength += (int32_t)(pDest - dest);

    if (pNumSubstitutions != nullptr) {
        *pNumSubstitutions = numSubstitutions;
    }

    if (pDestLength) {
        *pDestLength = reqLength;
    }

    u_terminateUChars(dest, destCapacity, reqLength, pErrorCode);

    return dest;
}
