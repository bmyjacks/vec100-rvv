/****************************************************************************
 * Project: PCRE2 10.48
 * Source files: src/pcre2_match.c, src/pcre2_internal.h
 * src/pcre2_match.c:5036-5202,623-632;
 * src/pcre2_internal.h:401-422,585-589,679-682.
 *
 * src/pcre2_match.c
 */
/*************************************************
*      Perl-Compatible Regular Expressions       *
*************************************************/

/* PCRE is a library of functions to support regular expressions whose syntax
and semantics are as close as possible to those of the Perl 5 language.

                       Written by Philip Hazel
     Original API code Copyright (c) 1997-2012 University of Cambridge
          New API code Copyright (c) 2015-2024 University of Cambridge

-----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the University of Cambridge nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------
*/
#include "kernel.h"

// PCRE2 ASCII configuration, 8-bit code units; original case lists.
#define HSPACE_BYTE_CASES case 0x09: case 0x20: case 0xa0
#define VSPACE_BYTE_CASES case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x85

ClassRunResult pcre2_class_repeat(const uint8_t *subject, size_t length,
    size_t start, uint32_t min, uint32_t max, ClassType type,
    const uint8_t *ctypes, int partial, size_t start_used,
    int allowemptypartial, int hitend) {
    const uint8_t *Feptr = subject + start;
    const uint8_t *end_subject = subject + length;
    const uint8_t *start_used_ptr = subject + start_used;
    uint32_t i;
    int status = 0;
#define SCHECK_PARTIAL() do { \
    if (partial != 0 && (Feptr > start_used_ptr || allowemptypartial)) { \
        hitend = 1; \
        if (partial > 1) { status = -2; goto DONE; } \
    } \
} while (0)
    switch (type) {
    case OP_NOT_HSPACE:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            switch (*Feptr) {
            default: Feptr++; break;
            HSPACE_BYTE_CASES: goto ENDLOOP00;
            }
        }
    ENDLOOP00: break;
    case OP_HSPACE:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            switch (*Feptr) {
            default: goto ENDLOOP01;
            HSPACE_BYTE_CASES: Feptr++; break;
            }
        }
    ENDLOOP01: break;
    case OP_NOT_VSPACE:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            switch (*Feptr) {
            default: Feptr++; break;
            VSPACE_BYTE_CASES: goto ENDLOOP02;
            }
        }
    ENDLOOP02: break;
    case OP_VSPACE:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            switch (*Feptr) {
            default: goto ENDLOOP03;
            VSPACE_BYTE_CASES: Feptr++; break;
            }
        }
    ENDLOOP03: break;
    case OP_NOT_DIGIT:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            if ((ctypes[*Feptr] & ctype_digit) != 0) break;
            Feptr++;
        }
        break;
    case OP_DIGIT:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            if ((ctypes[*Feptr] & ctype_digit) == 0) break;
            Feptr++;
        }
        break;
    case OP_NOT_WHITESPACE:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            if ((ctypes[*Feptr] & ctype_space) != 0) break;
            Feptr++;
        }
        break;
    case OP_WHITESPACE:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            if ((ctypes[*Feptr] & ctype_space) == 0) break;
            Feptr++;
        }
        break;
    case OP_NOT_WORDCHAR:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            if ((ctypes[*Feptr] & ctype_word) != 0) break;
            Feptr++;
        }
        break;
    case OP_WORDCHAR:
        for (i = min; i < max; i++) {
            if (Feptr >= end_subject) { SCHECK_PARTIAL(); break; }
            if ((ctypes[*Feptr] & ctype_word) == 0) break;
            Feptr++;
        }
        break;
    default: return {start, hitend, -1};
    }
DONE:
    return {static_cast<size_t>(Feptr - subject), hitend, status};
#undef SCHECK_PARTIAL
}
