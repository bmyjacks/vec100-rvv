/****************************************************************************
 *
 *
 *  Project: ICU4C 78.3
 *  Source files:
 *    icu/source/common/unicode/umachine.h
 *    icu/source/common/unicode/platform.h
 *    icu/source/common/unicode/utypes.h
 *    icu/source/common/unicode/utf.h
 *    icu/source/common/unicode/utf8.h
 *    icu/source/common/unicode/utf16.h
 *    icu/source/common/ustr_imp.h
 *    icu/source/common/unicode/ustring.h
 *    icu/source/common/unicode/uvernum.h
 *    icu/source/common/unicode/urename.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * icu/source/common/unicode/umachine.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1999-2015, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/unicode/platform.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1997-2016, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/unicode/utypes.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1996-2016, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/unicode/utf.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1999-2011, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/unicode/utf8.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1999-2015, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/unicode/utf16.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1999-2012, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/ustr_imp.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1999-2015, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/unicode/ustring.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 1998-2014, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/unicode/uvernum.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 2000-2016, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 *
 * icu/source/common/unicode/urename.h
 *
 * © 2016 and later: Unicode, Inc. and others.
 * License & terms of use: http://www.unicode.org/copyright.html
 *
 *   Copyright (C) 2002-2016, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 *
 */

#ifndef KERNELS_85_U_STRFROMUTF8WITHSUB_INCLUDE_KERNEL_H_
#define KERNELS_85_U_STRFROMUTF8WITHSUB_INCLUDE_KERNEL_H_

#include <stdint.h>

/*
 * icu/source/common/unicode/umachine.h:269
 */
typedef int8_t UBool;

/*
 * icu/source/common/unicode/umachine.h:399-403
 */
typedef char16_t UChar;

/*
 * icu/source/common/unicode/umachine.h:449
 */
typedef int32_t UChar32;

/*
 * icu/source/common/unicode/umachine.h:469
 */
#define U_SENTINEL (-1)

/*
 * icu/source/common/unicode/umachine.h:168-179
 */
#ifndef UPRV_BLOCK_MACRO_BEGIN
#define UPRV_BLOCK_MACRO_BEGIN do
#endif

#ifndef UPRV_BLOCK_MACRO_END
#define UPRV_BLOCK_MACRO_END while (false)
#endif

/*
 * icu/source/common/unicode/platform.h:765-781
 */
#define U_EXPORT __attribute__((visibility("default")))

/*
 * icu/source/common/unicode/platform.h:783-790
 */
#define U_EXPORT2

/*
 * icu/source/common/unicode/umachine.h:79-87
 */
#define U_CFUNC extern "C"

/*
 * icu/source/common/unicode/umachine.h:110
 */
#define U_CAPI U_CFUNC U_EXPORT

/*
 * icu/source/common/unicode/uvernum.h:82
 */
#define U_ICU_VERSION_SUFFIX _78

/*
 * icu/source/common/unicode/uvernum.h:104-128
 */
#define U_DEF_ICU_ENTRY_POINT_RENAME(x, y) x##y
#define U_DEF2_ICU_ENTRY_POINT_RENAME(x, y) U_DEF_ICU_ENTRY_POINT_RENAME(x, y)
#define U_ICU_ENTRY_POINT_RENAME(x)                                            \
    U_DEF2_ICU_ENTRY_POINT_RENAME(x, U_ICU_VERSION_SUFFIX)

/*
 * icu/source/common/unicode/urename.h:371
 */
#define u_strFromUTF8WithSub U_ICU_ENTRY_POINT_RENAME(u_strFromUTF8WithSub)

/*
 * icu/source/common/unicode/urename.h:409
 */
#define u_terminateUChars U_ICU_ENTRY_POINT_RENAME(u_terminateUChars)

/*
 * icu/source/common/unicode/urename.h:1922
 */
#define utf8_nextCharSafeBody U_ICU_ENTRY_POINT_RENAME(utf8_nextCharSafeBody)

/*
 * icu/source/common/unicode/utypes.h:509-799
 */
typedef enum UErrorCode {

    U_USING_FALLBACK_WARNING = -128,

    U_ERROR_WARNING_START = -128,

    U_USING_DEFAULT_WARNING = -127,

    U_SAFECLONE_ALLOCATED_WARNING = -126,

    U_STATE_OLD_WARNING = -125,

    U_STRING_NOT_TERMINATED_WARNING = -124,

    U_SORT_KEY_TOO_SHORT_WARNING = -123,

    U_AMBIGUOUS_ALIAS_WARNING = -122,

    U_DIFFERENT_UCA_VERSION = -121,

    U_PLUGIN_CHANGED_LEVEL_WARNING = -120,

#ifndef U_HIDE_DEPRECATED_API
    U_ERROR_WARNING_LIMIT,
#endif

    U_ZERO_ERROR = 0,

    U_ILLEGAL_ARGUMENT_ERROR = 1,
    U_MISSING_RESOURCE_ERROR = 2,
    U_INVALID_FORMAT_ERROR = 3,
    U_FILE_ACCESS_ERROR = 4,
    U_INTERNAL_PROGRAM_ERROR = 5,
    U_MESSAGE_PARSE_ERROR = 6,
    U_MEMORY_ALLOCATION_ERROR = 7,
    U_INDEX_OUTOFBOUNDS_ERROR = 8,
    U_PARSE_ERROR = 9,
    U_INVALID_CHAR_FOUND = 10,
    U_TRUNCATED_CHAR_FOUND = 11,
    U_ILLEGAL_CHAR_FOUND = 12,
    U_INVALID_TABLE_FORMAT = 13,
    U_INVALID_TABLE_FILE = 14,
    U_BUFFER_OVERFLOW_ERROR = 15,
    U_UNSUPPORTED_ERROR = 16,
    U_RESOURCE_TYPE_MISMATCH = 17,
    U_ILLEGAL_ESCAPE_SEQUENCE = 18,
    U_UNSUPPORTED_ESCAPE_SEQUENCE = 19,
    U_NO_SPACE_AVAILABLE = 20,
    U_CE_NOT_FOUND_ERROR = 21,
    U_PRIMARY_TOO_LONG_ERROR = 22,
    U_STATE_TOO_OLD_ERROR = 23,
    U_TOO_MANY_ALIASES_ERROR = 24,
    U_ENUM_OUT_OF_SYNC_ERROR = 25,
    U_INVARIANT_CONVERSION_ERROR = 26,
    U_INVALID_STATE_ERROR = 27,
    U_COLLATOR_VERSION_MISMATCH = 28,
    U_USELESS_COLLATOR_ERROR = 29,
    U_NO_WRITE_PERMISSION = 30,
    U_INPUT_TOO_LONG_ERROR = 31,

#ifndef U_HIDE_DEPRECATED_API
    U_STANDARD_ERROR_LIMIT = 32,
#endif

    U_BAD_VARIABLE_DEFINITION = 0x10000,
    U_PARSE_ERROR_START = 0x10000,
    U_MALFORMED_RULE,
    U_MALFORMED_SET,
    U_MALFORMED_SYMBOL_REFERENCE,
    U_MALFORMED_UNICODE_ESCAPE,
    U_MALFORMED_VARIABLE_DEFINITION,
    U_MALFORMED_VARIABLE_REFERENCE,
    U_MISMATCHED_SEGMENT_DELIMITERS,
    U_MISPLACED_ANCHOR_START,
    U_MISPLACED_CURSOR_OFFSET,
    U_MISPLACED_QUANTIFIER,
    U_MISSING_OPERATOR,
    U_MISSING_SEGMENT_CLOSE,
    U_MULTIPLE_ANTE_CONTEXTS,
    U_MULTIPLE_CURSORS,
    U_MULTIPLE_POST_CONTEXTS,
    U_TRAILING_BACKSLASH,
    U_UNDEFINED_SEGMENT_REFERENCE,
    U_UNDEFINED_VARIABLE,
    U_UNQUOTED_SPECIAL,
    U_UNTERMINATED_QUOTE,
    U_RULE_MASK_ERROR,
    U_MISPLACED_COMPOUND_FILTER,
    U_MULTIPLE_COMPOUND_FILTERS,
    U_INVALID_RBT_SYNTAX,
    U_INVALID_PROPERTY_PATTERN,
    U_MALFORMED_PRAGMA,
    U_UNCLOSED_SEGMENT,
    U_ILLEGAL_CHAR_IN_SEGMENT,
    U_VARIABLE_RANGE_EXHAUSTED,
    U_VARIABLE_RANGE_OVERLAP,
    U_ILLEGAL_CHARACTER,
    U_INTERNAL_TRANSLITERATOR_ERROR,
    U_INVALID_ID,
    U_INVALID_FUNCTION,
#ifndef U_HIDE_DEPRECATED_API
    U_PARSE_ERROR_LIMIT,
#endif

    U_UNEXPECTED_TOKEN = 0x10100,
    U_FMT_PARSE_ERROR_START = 0x10100,
    U_MULTIPLE_DECIMAL_SEPARATORS,
    U_MULTIPLE_DECIMAL_SEPERATORS = U_MULTIPLE_DECIMAL_SEPARATORS,
    U_MULTIPLE_EXPONENTIAL_SYMBOLS,
    U_MALFORMED_EXPONENTIAL_PATTERN,
    U_MULTIPLE_PERCENT_SYMBOLS,
    U_MULTIPLE_PERMILL_SYMBOLS,
    U_MULTIPLE_PAD_SPECIFIERS,
    U_PATTERN_SYNTAX_ERROR,
    U_ILLEGAL_PAD_POSITION,
    U_UNMATCHED_BRACES,
    U_UNSUPPORTED_PROPERTY,
    U_UNSUPPORTED_ATTRIBUTE,
    U_ARGUMENT_TYPE_MISMATCH,
    U_DUPLICATE_KEYWORD,
    U_UNDEFINED_KEYWORD,
    U_DEFAULT_KEYWORD_MISSING,
    U_DECIMAL_NUMBER_SYNTAX_ERROR,
    U_FORMAT_INEXACT_ERROR,
    U_NUMBER_ARG_OUTOFBOUNDS_ERROR,
    U_NUMBER_SKELETON_SYNTAX_ERROR,

    U_MF_UNRESOLVED_VARIABLE_ERROR,
    U_MF_SYNTAX_ERROR,
    U_MF_UNKNOWN_FUNCTION_ERROR,
    U_MF_VARIANT_KEY_MISMATCH_ERROR,
    U_MF_FORMATTING_ERROR,
    U_MF_NONEXHAUSTIVE_PATTERN_ERROR,
    U_MF_DUPLICATE_OPTION_NAME_ERROR,
    U_MF_SELECTOR_ERROR,
    U_MF_MISSING_SELECTOR_ANNOTATION_ERROR,
    U_MF_DUPLICATE_DECLARATION_ERROR,
    U_MF_OPERAND_MISMATCH_ERROR,
    U_MF_DUPLICATE_VARIANT_ERROR,
    U_MF_BAD_OPTION,
#ifndef U_HIDE_DEPRECATED_API
    U_FMT_PARSE_ERROR_LIMIT = 0x10121,
#endif

    U_BRK_INTERNAL_ERROR = 0x10200,
    U_BRK_ERROR_START = 0x10200,
    U_BRK_HEX_DIGITS_EXPECTED,
    U_BRK_SEMICOLON_EXPECTED,
    U_BRK_RULE_SYNTAX,
    U_BRK_UNCLOSED_SET,
    U_BRK_ASSIGN_ERROR,
    U_BRK_VARIABLE_REDFINITION,
    U_BRK_MISMATCHED_PAREN,
    U_BRK_NEW_LINE_IN_QUOTED_STRING,
    U_BRK_UNDEFINED_VARIABLE,
    U_BRK_INIT_ERROR,
    U_BRK_RULE_EMPTY_SET,
    U_BRK_UNRECOGNIZED_OPTION,
    U_BRK_MALFORMED_RULE_TAG,
#ifndef U_HIDE_DEPRECATED_API
    U_BRK_ERROR_LIMIT,
#endif

    U_REGEX_INTERNAL_ERROR = 0x10300,
    U_REGEX_ERROR_START = 0x10300,
    U_REGEX_RULE_SYNTAX,
    U_REGEX_INVALID_STATE,
    U_REGEX_BAD_ESCAPE_SEQUENCE,
    U_REGEX_PROPERTY_SYNTAX,
    U_REGEX_UNIMPLEMENTED,
    U_REGEX_MISMATCHED_PAREN,
    U_REGEX_NUMBER_TOO_BIG,
    U_REGEX_BAD_INTERVAL,
    U_REGEX_MAX_LT_MIN,
    U_REGEX_INVALID_BACK_REF,
    U_REGEX_INVALID_FLAG,
    U_REGEX_LOOK_BEHIND_LIMIT,
    U_REGEX_SET_CONTAINS_STRING,
#ifndef U_HIDE_DEPRECATED_API
    U_REGEX_OCTAL_TOO_BIG,
#endif
    U_REGEX_MISSING_CLOSE_BRACKET = U_REGEX_SET_CONTAINS_STRING + 2,
    U_REGEX_INVALID_RANGE,
    U_REGEX_STACK_OVERFLOW,
    U_REGEX_TIME_OUT,
    U_REGEX_STOPPED_BY_CALLER,
    U_REGEX_PATTERN_TOO_BIG,
    U_REGEX_INVALID_CAPTURE_GROUP_NAME,
#ifndef U_HIDE_DEPRECATED_API
    U_REGEX_ERROR_LIMIT = U_REGEX_STOPPED_BY_CALLER + 3,
#endif

    U_IDNA_PROHIBITED_ERROR = 0x10400,
    U_IDNA_ERROR_START = 0x10400,
    U_IDNA_UNASSIGNED_ERROR,
    U_IDNA_CHECK_BIDI_ERROR,
    U_IDNA_STD3_ASCII_RULES_ERROR,
    U_IDNA_ACE_PREFIX_ERROR,
    U_IDNA_VERIFICATION_ERROR,
    U_IDNA_LABEL_TOO_LONG_ERROR,
    U_IDNA_ZERO_LENGTH_LABEL_ERROR,
    U_IDNA_DOMAIN_NAME_TOO_LONG_ERROR,
#ifndef U_HIDE_DEPRECATED_API
    U_IDNA_ERROR_LIMIT,
#endif
    U_STRINGPREP_PROHIBITED_ERROR = U_IDNA_PROHIBITED_ERROR,
    U_STRINGPREP_UNASSIGNED_ERROR = U_IDNA_UNASSIGNED_ERROR,
    U_STRINGPREP_CHECK_BIDI_ERROR = U_IDNA_CHECK_BIDI_ERROR,

    U_PLUGIN_ERROR_START = 0x10500,
    U_PLUGIN_TOO_HIGH = 0x10500,
    U_PLUGIN_DIDNT_SET_LEVEL,
#ifndef U_HIDE_DEPRECATED_API
    U_PLUGIN_ERROR_LIMIT,
#endif

#ifndef U_HIDE_DEPRECATED_API
    U_ERROR_LIMIT = U_PLUGIN_ERROR_LIMIT
#endif
} UErrorCode;

/*
 * icu/source/common/unicode/utypes.h:804-828
 */
static inline UBool U_SUCCESS(UErrorCode code) { return code <= U_ZERO_ERROR; }
static inline UBool U_FAILURE(UErrorCode code) { return code > U_ZERO_ERROR; }

/*
 * icu/source/common/unicode/utf.h:161-163
 */
#define U_IS_UNICODE_NONCHAR(c)                                                \
    ((c) >= 0xfdd0 && ((c) <= 0xfdef || ((c) & 0xfffe) == 0xfffe) &&           \
     (c) <= 0x10ffff)

/*
 * icu/source/common/unicode/utf.h:224
 */
#define U_IS_SURROGATE(c) (((c) & 0xfffff800) == 0xd800)

/*
 * icu/source/common/unicode/utf8.h:91
 */
#define U8_LEAD3_T1_BITS                                                       \
    "\x20\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x10\x30\x30"

/*
 * icu/source/common/unicode/utf8.h:98
 */
#define U8_IS_VALID_LEAD3_AND_T1(lead, t1)                                     \
    (U8_LEAD3_T1_BITS[(lead) & 0xf] & (1 << ((uint8_t)(t1) >> 5)))

/*
 * icu/source/common/unicode/utf8.h:108
 */
#define U8_LEAD4_T1_BITS                                                       \
    "\x00\x00\x00\x00\x00\x00\x00\x00\x1E\x0F\x0F\x0F\x00\x00\x00\x00"

/*
 * icu/source/common/unicode/utf8.h:115
 */
#define U8_IS_VALID_LEAD4_AND_T1(lead, t1)                                     \
    (U8_LEAD4_T1_BITS[(uint8_t)(t1) >> 4] & (1 << ((lead) & 7)))

/*
 * icu/source/common/unicode/utf8.h:173
 */
#define U8_IS_SINGLE(c) ((int8_t)(c) >= 0)

/*
 * icu/source/common/unicode/utf8.h:126-127
 */
U_CAPI UChar32 U_EXPORT2 utf8_nextCharSafeBody(const uint8_t *s, int32_t *pi,
                                               int32_t length, UChar32 c,
                                               int8_t strict);

/*
 * icu/source/common/unicode/utf16.h:123
 */
#define U16_LEAD(supplementary) (UChar)(((supplementary) >> 10) + 0xd7c0)

/*
 * icu/source/common/unicode/utf16.h:132
 */
#define U16_TRAIL(supplementary) (UChar)(((supplementary) & 0x3ff) | 0xdc00)

/*
 * icu/source/common/unicode/utf16.h:141
 */
#define U16_LENGTH(c) ((uint32_t)(c) <= 0xffff ? 1 : 2)

/*
 * icu/source/common/ustr_imp.h:73-74
 */
U_CAPI int32_t U_EXPORT2 u_terminateUChars(UChar *dest, int32_t destCapacity,
                                           int32_t length,
                                           UErrorCode *pErrorCode);

/*
 * icu/source/common/unicode/ustring.h:1377-1384
 */
U_CAPI UChar *U_EXPORT2 u_strFromUTF8WithSub(UChar *dest, int32_t destCapacity,
                                             int32_t *pDestLength,
                                             const char *src, int32_t srcLength,
                                             UChar32 subchar,
                                             int32_t *pNumSubstitutions,
                                             UErrorCode *pErrorCode);

#endif // KERNELS_85_U_STRFROMUTF8WITHSUB_INCLUDE_KERNEL_H_
