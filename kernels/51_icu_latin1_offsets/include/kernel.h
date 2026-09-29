// ICU4C 78.3, icu4c/source/common/unicode/ucnv.h, utypes.h, utf.h, utf16.h,
// and common/ucnv_bld.h: locally reduced declarations for ucnvlat1.cpp.
// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
// Copyright (C) 1996-2016, International Business Machines Corporation
// and others. All Rights Reserved.
#ifndef KERNELS_127_ICU_LATIN1_OFFSETS_KERNEL_H_
#define KERNELS_127_ICU_LATIN1_OFFSETS_KERNEL_H_

#include <cstdint>

using UChar32 = int32_t;
enum UErrorCode {
    U_ZERO_ERROR = 0,
    U_ILLEGAL_CHAR_FOUND = 12,
    U_INVALID_CHAR_FOUND = 10,
    U_BUFFER_OVERFLOW_ERROR = 15
};
#define U_SUCCESS(x) ((x) <= U_ZERO_ERROR)
#define U_IS_SURROGATE(c) (((c) & 0xfffff800) == 0xd800)
#define U_IS_SURROGATE_LEAD(c) (((c) & 0x400) == 0)
#define U16_IS_TRAIL(c) (((c) & 0xfc00) == 0xdc00)
#define U16_GET_SUPPLEMENTARY(lead, trail) \
    (((static_cast<UChar32>(lead) - 0xd800) << 10) + \
     (static_cast<UChar32>(trail) - 0xdc00) + 0x10000)

// Only the fields read/written by the selected ICU function are retained.
struct UConverterSharedData { int tag; };
struct UConverter {
    const UConverterSharedData *sharedData;
    UChar32 fromUChar32;
};
struct UConverterFromUnicodeArgs {
    UConverter *converter;
    const char16_t *source;
    const char16_t *sourceLimit;
    char *target;
    const char *targetLimit;
    int32_t *offsets;
};
inline constexpr UConverterSharedData _Latin1Data{1};
inline constexpr UConverterSharedData _ASCIIData{2};

void _Latin1FromUnicodeWithOffsets(UConverterFromUnicodeArgs *, UErrorCode *);
void _Latin1FromUnicodeWithOffsets_rvv(UConverterFromUnicodeArgs *, UErrorCode *);
#endif
