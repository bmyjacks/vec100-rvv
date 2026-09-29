/****************************************************************************
 *
 *
 *  Project: PCRE2 10.48
 *  Source files:
 *    src/pcre2_internal.h
 *    src/pcre2_ucp.h
 *    src/pcre2_util.h
 *    src/pcre2_compile.h
 *    src/pcre2.h.generic
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/pcre2_internal.h
 *
 *   PCRE2 is a library of functions to support regular expressions whose syntax
 *   and semantics are as close as possible to those of the Perl 5 language.
 *
 * Written by Philip Hazel
 * Original API code Copyright (c) 1997-2012 University of Cambridge
 * New API code Copyright (c) 2016-2024 University of Cambridge
 *
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the University of Cambridge nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -----------------------------------------------------------------------------
 *
 *
 * src/pcre2_ucp.h
 *
 *   This file contains definitions of the Unicode property values that are
 *   returned by the UCD access macros and used throughout PCRE2.
 *
 *   IMPORTANT: The specific values of the first two enums (general and
 * particular character categories) are assumed by the table called catposstab
 * in the file pcre2_auto_possess.c. They are unlikely to change, but should be
 * checked after an update.
 *
 * Written by Philip Hazel
 * Original API code Copyright (c) 1997-2012 University of Cambridge
 * New API code Copyright (c) 2016-2022 University of Cambridge
 *
 * This module is auto-generated from Unicode data files. DO NOT EDIT MANUALLY!
 * Instead, modify the maint/GenerateUcpHeader.py script and run it to generate
 * a new version of this code.
 *
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the University of Cambridge nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -----------------------------------------------------------------------------
 *
 *
 * src/pcre2_util.h
 *
 *   PCRE2 is a library of functions to support regular expressions whose syntax
 *   and semantics are as close as possible to those of the Perl 5 language.
 *
 * Written by Philip Hazel
 * Original API code Copyright (c) 1997-2012 University of Cambridge
 * New API code Copyright (c) 2016-2024 University of Cambridge
 *
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the University of Cambridge nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -----------------------------------------------------------------------------
 *
 *
 * src/pcre2_compile.h
 *
 *   PCRE2 is a library of functions to support regular expressions whose syntax
 *   and semantics are as close as possible to those of the Perl 5 language.
 *
 * Written by Philip Hazel
 * Original API code Copyright (c) 1997-2012 University of Cambridge
 * New API code Copyright (c) 2016-2024 University of Cambridge
 *
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the University of Cambridge nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -----------------------------------------------------------------------------
 *
 *
 * src/pcre2.h.generic
 *
 *   This is the public header file for the PCRE library, second API, to be
 *   #included by applications that call PCRE2 functions.
 *
 * Copyright (c) 2016-2024 University of Cambridge
 *
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the University of Cambridge nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -----------------------------------------------------------------------------
 *
 */

#ifndef KERNELS_86_UPDATE_CLASSBITS_INCLUDE_KERNEL_H_
#define KERNELS_86_UPDATE_CLASSBITS_INCLUDE_KERNEL_H_

#include <stdint.h>
#include <string.h>

/* Selected width-8 Unicode configuration. */
#define PCRE2_CODE_UNIT_WIDTH 8

/*
 * src/pcre2.h.generic:879-881
 */
#define PCRE2_JOIN(a, b) a##b
#define PCRE2_GLUE(a, b) PCRE2_JOIN(a, b)
#define PCRE2_SUFFIX(a) PCRE2_GLUE(a, PCRE2_CODE_UNIT_WIDTH)

/*
 * src/pcre2_internal.h:93-97
 */
typedef int BOOL;
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

/*
 * src/pcre2_internal.h:193-195
 */
#ifndef PRIV
#define PRIV(name) _pcre2_##name
#endif

/*
 * src/pcre2_internal.h:2222-2233
 */
#define _pcre2_ucd_boolprop_sets PCRE2_SUFFIX(_pcre2_ucd_boolprop_sets_)
#define _pcre2_ucd_script_sets PCRE2_SUFFIX(_pcre2_ucd_script_sets_)
#define _pcre2_ucd_records PCRE2_SUFFIX(_pcre2_ucd_records_)
#define _pcre2_ucd_stage1 PCRE2_SUFFIX(_pcre2_ucd_stage1_)
#define _pcre2_ucd_stage2 PCRE2_SUFFIX(_pcre2_ucd_stage2_)
#define _pcre2_ucp_gentype PCRE2_SUFFIX(_pcre2_ucp_gentype_)

/*
 * src/pcre2_compile.h:270
 */
#define _pcre2_update_classbits PCRE2_SUFFIX(_pcre2_update_classbits_)

/*
 * src/pcre2_ucp.h:59-67
 */
enum {
    ucp_C,
    ucp_L,
    ucp_M,
    ucp_N,
    ucp_P,
    ucp_S,
    ucp_Z,
};

/*
 * src/pcre2_ucp.h:71-102
 */
enum {
    ucp_Cc,
    ucp_Cf,
    ucp_Cn,
    ucp_Co,
    ucp_Cs,
    ucp_Ll,
    ucp_Lm,
    ucp_Lo,
    ucp_Lt,
    ucp_Lu,
    ucp_Mc,
    ucp_Me,
    ucp_Mn,
    ucp_Nd,
    ucp_Nl,
    ucp_No,
    ucp_Pc,
    ucp_Pd,
    ucp_Pe,
    ucp_Pf,
    ucp_Pi,
    ucp_Po,
    ucp_Ps,
    ucp_Sc,
    ucp_Sk,
    ucp_Sm,
    ucp_So,
    ucp_Zl,
    ucp_Zp,
    ucp_Zs,
};

/*
 * src/pcre2_ucp.h:106-170
 */
enum {
    ucp_ASCII,
    ucp_ASCII_Hex_Digit,
    ucp_Alphabetic,
    ucp_Bidi_Control,
    ucp_Bidi_Mirrored,
    ucp_Case_Ignorable,
    ucp_Cased,
    ucp_Changes_When_Casefolded,
    ucp_Changes_When_Casemapped,
    ucp_Changes_When_Lowercased,
    ucp_Changes_When_Titlecased,
    ucp_Changes_When_Uppercased,
    ucp_Dash,
    ucp_Default_Ignorable_Code_Point,
    ucp_Deprecated,
    ucp_Diacritic,
    ucp_Emoji,
    ucp_Emoji_Component,
    ucp_Emoji_Modifier,
    ucp_Emoji_Modifier_Base,
    ucp_Emoji_Presentation,
    ucp_Extended_Pictographic,
    ucp_Extender,
    ucp_Grapheme_Base,
    ucp_Grapheme_Extend,
    ucp_Grapheme_Link,
    ucp_Hex_Digit,
    ucp_IDS_Binary_Operator,
    ucp_IDS_Trinary_Operator,
    ucp_IDS_Unary_Operator,
    ucp_ID_Compat_Math_Continue,
    ucp_ID_Compat_Math_Start,
    ucp_ID_Continue,
    ucp_ID_Start,
    ucp_Ideographic,
    ucp_InCB,
    ucp_Join_Control,
    ucp_Logical_Order_Exception,
    ucp_Lowercase,
    ucp_Math,
    ucp_Modifier_Combining_Mark,
    ucp_Noncharacter_Code_Point,
    ucp_Pattern_Syntax,
    ucp_Pattern_White_Space,
    ucp_Prepended_Concatenation_Mark,
    ucp_Quotation_Mark,
    ucp_Radical,
    ucp_Regional_Indicator,
    ucp_Sentence_Terminal,
    ucp_Soft_Dotted,
    ucp_Terminal_Punctuation,
    ucp_Unified_Ideograph,
    ucp_Uppercase,
    ucp_Variation_Selector,
    ucp_White_Space,
    ucp_XID_Continue,
    ucp_XID_Start,
    ucp_Bprop_Count
};

/*
 * src/pcre2_ucp.h:170
 */
#define ucd_boolprop_sets_item_size 2

/*
 * src/pcre2_ucp.h:174-198
 */
enum {
    ucp_bidiAL,
    ucp_bidiAN,
    ucp_bidiB,
    ucp_bidiBN,
    ucp_bidiCS,
    ucp_bidiEN,
    ucp_bidiES,
    ucp_bidiET,
    ucp_bidiFSI,
    ucp_bidiL,
    ucp_bidiLRE,
    ucp_bidiLRI,
    ucp_bidiLRO,
    ucp_bidiNSM,
    ucp_bidiON,
    ucp_bidiPDF,
    ucp_bidiPDI,
    ucp_bidiR,
    ucp_bidiRLE,
    ucp_bidiRLI,
    ucp_bidiRLO,
    ucp_bidiS,
    ucp_bidiWS,
};

/*
 * src/pcre2_ucp.h:203-219
 */
enum {
    ucp_gbCR,
    ucp_gbLF,
    ucp_gbControl,
    ucp_gbExtend,
    ucp_gbPrepend,
    ucp_gbSpacingMark,
    ucp_gbL,
    ucp_gbV,
    ucp_gbT,
    ucp_gbLV,
    ucp_gbLVT,
    ucp_gbRegional_Indicator,
    ucp_gbOther,
    ucp_gbZWJ,
    ucp_gbExtended_Pictographic,
};

/*
 * src/pcre2_ucp.h:223-405
 */
enum {
    ucp_Latin,
    ucp_Greek,
    ucp_Cyrillic,
    ucp_Armenian,
    ucp_Hebrew,
    ucp_Arabic,
    ucp_Syriac,
    ucp_Thaana,
    ucp_Devanagari,
    ucp_Bengali,
    ucp_Gurmukhi,
    ucp_Gujarati,
    ucp_Oriya,
    ucp_Tamil,
    ucp_Telugu,
    ucp_Kannada,
    ucp_Malayalam,
    ucp_Sinhala,
    ucp_Thai,
    ucp_Tibetan,
    ucp_Myanmar,
    ucp_Georgian,
    ucp_Hangul,
    ucp_Ethiopic,
    ucp_Cherokee,
    ucp_Runic,
    ucp_Mongolian,
    ucp_Hiragana,
    ucp_Katakana,
    ucp_Bopomofo,
    ucp_Han,
    ucp_Yi,
    ucp_Gothic,
    ucp_Tagalog,
    ucp_Hanunoo,
    ucp_Buhid,
    ucp_Tagbanwa,
    ucp_Limbu,
    ucp_Tai_Le,
    ucp_Linear_B,
    ucp_Shavian,
    ucp_Cypriot,
    ucp_Buginese,
    ucp_Coptic,
    ucp_Glagolitic,
    ucp_Tifinagh,
    ucp_Syloti_Nagri,
    ucp_Phags_Pa,
    ucp_Nko,
    ucp_Kayah_Li,
    ucp_Lycian,
    ucp_Carian,
    ucp_Lydian,
    ucp_Avestan,
    ucp_Samaritan,
    ucp_Lisu,
    ucp_Javanese,
    ucp_Old_Turkic,
    ucp_Kaithi,
    ucp_Mandaic,
    ucp_Chakma,
    ucp_Meroitic_Hieroglyphs,
    ucp_Sharada,
    ucp_Takri,
    ucp_Caucasian_Albanian,
    ucp_Duployan,
    ucp_Elbasan,
    ucp_Grantha,
    ucp_Khojki,
    ucp_Linear_A,
    ucp_Mahajani,
    ucp_Manichaean,
    ucp_Modi,
    ucp_Old_Permic,
    ucp_Psalter_Pahlavi,
    ucp_Khudawadi,
    ucp_Tirhuta,
    ucp_Multani,
    ucp_Old_Hungarian,
    ucp_Adlam,
    ucp_Newa,
    ucp_Osage,
    ucp_Tangut,
    ucp_Masaram_Gondi,
    ucp_Dogra,
    ucp_Gunjala_Gondi,
    ucp_Hanifi_Rohingya,
    ucp_Sogdian,
    ucp_Nandinagari,
    ucp_Yezidi,
    ucp_Cypro_Minoan,
    ucp_Old_Uyghur,
    ucp_Toto,
    ucp_Garay,
    ucp_Gurung_Khema,
    ucp_Ol_Onal,
    ucp_Sunuwar,
    ucp_Todhri,
    ucp_Tulu_Tigalari,

    ucp_Unknown,
    ucp_Common,
    ucp_Lao,
    ucp_Canadian_Aboriginal,
    ucp_Ogham,
    ucp_Khmer,
    ucp_Old_Italic,
    ucp_Deseret,
    ucp_Inherited,
    ucp_Ugaritic,
    ucp_Osmanya,
    ucp_Braille,
    ucp_New_Tai_Lue,
    ucp_Old_Persian,
    ucp_Kharoshthi,
    ucp_Balinese,
    ucp_Cuneiform,
    ucp_Phoenician,
    ucp_Sundanese,
    ucp_Lepcha,
    ucp_Ol_Chiki,
    ucp_Vai,
    ucp_Saurashtra,
    ucp_Rejang,
    ucp_Cham,
    ucp_Tai_Tham,
    ucp_Tai_Viet,
    ucp_Egyptian_Hieroglyphs,
    ucp_Bamum,
    ucp_Meetei_Mayek,
    ucp_Imperial_Aramaic,
    ucp_Old_South_Arabian,
    ucp_Inscriptional_Parthian,
    ucp_Inscriptional_Pahlavi,
    ucp_Batak,
    ucp_Brahmi,
    ucp_Meroitic_Cursive,
    ucp_Miao,
    ucp_Sora_Sompeng,
    ucp_Bassa_Vah,
    ucp_Pahawh_Hmong,
    ucp_Mende_Kikakui,
    ucp_Mro,
    ucp_Old_North_Arabian,
    ucp_Nabataean,
    ucp_Palmyrene,
    ucp_Pau_Cin_Hau,
    ucp_Siddham,
    ucp_Warang_Citi,
    ucp_Ahom,
    ucp_Anatolian_Hieroglyphs,
    ucp_Hatran,
    ucp_SignWriting,
    ucp_Bhaiksuki,
    ucp_Marchen,
    ucp_Nushu,
    ucp_Soyombo,
    ucp_Zanabazar_Square,
    ucp_Makasar,
    ucp_Medefaidrin,
    ucp_Old_Sogdian,
    ucp_Elymaic,
    ucp_Nyiakeng_Puachue_Hmong,
    ucp_Wancho,
    ucp_Chorasmian,
    ucp_Dives_Akuru,
    ucp_Khitan_Small_Script,
    ucp_Tangsa,
    ucp_Vithkuqi,
    ucp_Kawi,
    ucp_Nag_Mundari,
    ucp_Kirat_Rai,
    ucp_Sidetic,
    ucp_Tai_Yo,
    ucp_Tolong_Siki,
    ucp_Beria_Erfe,

    ucp_Script_Count
};

/*
 * src/pcre2_ucp.h:409
 */
#define ucd_script_sets_item_size 4

/*
 * src/pcre2_internal.h:401-404
 */
#define HSPACE_BYTE_CASES                                                      \
    case CHAR_HT:                                                              \
    case CHAR_SPACE:                                                           \
    case CHAR_NBSP

/*
 * src/pcre2_internal.h:417-422
 */
#define VSPACE_BYTE_CASES                                                      \
    case CHAR_LF:                                                              \
    case CHAR_VT:                                                              \
    case CHAR_FF:                                                              \
    case CHAR_CR:                                                              \
    case CHAR_NEL

/*
 * src/pcre2_internal.h:1134-1242
 */
#define CHAR_HT '\011'
#define CHAR_VT '\013'
#define CHAR_FF '\014'
#define CHAR_CR '\015'
#define CHAR_LF '\012'
#define CHAR_NL CHAR_LF
#define CHAR_NEL ((unsigned char)'\x85')
#define CHAR_SPACE '\040'
#define CHAR_DOLLAR_SIGN '\044'
#define CHAR_0 '\060'
#define CHAR_9 '\071'
#define CHAR_COMMERCIAL_AT '\100'
#define CHAR_A '\101'
#define CHAR_F '\106'
#define CHAR_GRAVE_ACCENT '\140'
#define CHAR_a '\141'
#define CHAR_f '\146'
#define CHAR_NBSP ((unsigned char)'\xa0')

/*
 * src/pcre2_internal.h:1445-1471
 */
#define PT_LAMP 0
#define PT_GC 1
#define PT_PC 2
#define PT_SC 3
#define PT_SCX 4
#define PT_ALNUM 5
#define PT_SPACE 6
#define PT_PXSPACE 7
#define PT_WORD 8
#define PT_CLIST 9
#define PT_UCNC 10
#define PT_BIDICL 11
#define PT_BOOL 12
#define PT_ANY 13
#define PT_TABSIZE PT_ANY
#define PT_PXGRAPH 14
#define PT_PXPRINT 15
#define PT_PXPUNCT 16
#define PT_PXXDIGIT 17

/*
 * src/pcre2_internal.h:2108-2118
 */
typedef struct {
    uint8_t script;
    uint8_t chartype;
    uint8_t gbprop;
    uint8_t caseset;
    int32_t other_case;
    uint16_t scriptx_bidiclass;
    uint16_t bprops;
} ucd_record;

/*
 * src/pcre2_internal.h:2122-2140
 */
#define UCD_BLOCK_SIZE 128
#define REAL_GET_UCD(ch)                                                       \
    (PRIV(ucd_records) +                                                       \
     PRIV(ucd_stage2)[PRIV(ucd_stage1)[(int)(ch) / UCD_BLOCK_SIZE] *           \
                          UCD_BLOCK_SIZE +                                     \
                      (int)(ch) % UCD_BLOCK_SIZE])

#define GET_UCD(ch) REAL_GET_UCD(ch)

#define UCD_SCRIPTX_MASK 0x3ff
#define UCD_BIDICLASS_SHIFT 11
#define UCD_BPROPS_MASK 0xfff

#define UCD_SCRIPTX_PROP(prop) ((prop)->scriptx_bidiclass & UCD_SCRIPTX_MASK)
#define UCD_BIDICLASS_PROP(prop)                                               \
    ((prop)->scriptx_bidiclass >> UCD_BIDICLASS_SHIFT)
#define UCD_BPROPS_PROP(prop) ((prop)->bprops & UCD_BPROPS_MASK)

/*
 * src/pcre2_internal.h:2164
 */
#define MAPBIT(map, n) ((map)[(n) / 32] & (1u << ((n) % 32)))

/*
 * src/pcre2_util.h:116-118
 */
#ifndef PCRE2_ASSERT
#define PCRE2_ASSERT(x)                                                        \
    do {                                                                       \
    } while (0)
#endif

/*
 * src/pcre2_internal.h:2251-2265
 */
extern const uint32_t PRIV(ucd_boolprop_sets)[];
extern const uint32_t PRIV(ucd_script_sets)[];
extern const ucd_record PRIV(ucd_records)[];
extern const uint16_t PRIV(ucd_stage1)[];
extern const uint16_t PRIV(ucd_stage2)[];
extern const uint32_t PRIV(ucp_gentype)[];

/*
 * src/pcre2_compile.h:303-304
 */
void PRIV(update_classbits)(uint32_t ptype, uint32_t pdata, BOOL negated,
                            uint8_t *classbits);

#endif // KERNELS_86_UPDATE_CLASSBITS_INCLUDE_KERNEL_H_
