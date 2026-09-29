/****************************************************************************
 * Project: PCRE2 10.48
 * Source files: src/pcre2_internal.h
 *
 * src/pcre2_internal.h
 */
/*************************************************
*      Perl-Compatible Regular Expressions       *
*************************************************/

/* PCRE2 is a library of functions to support regular expressions whose syntax
and semantics are as close as possible to those of the Perl 5 language.

                       Written by Philip Hazel
     Original API code Copyright (c) 1997-2012 University of Cambridge
          New API code Copyright (c) 2016-2024 University of Cambridge

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

#ifndef KERNELS_110_PCRE2_CLASS_REPEAT_KERNEL_H_
#define KERNELS_110_PCRE2_CLASS_REPEAT_KERNEL_H_

#include <stddef.h>
#include <stdint.h>

// PCRE2 10.48, src/pcre2_match.c:5036-5202 (8-bit, non-UTF,
// maximizing repeat). The ten symbolic opcodes below correspond to the
// Lctype cases in that switch; their numeric encoding is local to this API.
enum ClassType {
    OP_NOT_HSPACE, OP_HSPACE, OP_NOT_VSPACE, OP_VSPACE,
    OP_NOT_DIGIT, OP_DIGIT, OP_NOT_WHITESPACE, OP_WHITESPACE,
    OP_NOT_WORDCHAR, OP_WORDCHAR
};

// src/pcre2_internal.h:585-589, ASCII 8-bit configuration.
enum { ctype_space = 0x01, ctype_digit = 0x08, ctype_word = 0x10 };

struct ClassRunResult {
    size_t offset;      // Feptr - subject; failing byte remains unconsumed
    int hitend;         // mb->hitend after this scan
    int status;         // 0 or PCRE2_ERROR_PARTIAL (-2) on hard partial
};

// subject[0..length) and ctypes[0..256) readable; start <= length;
// min <= max (the upstream Lmin/Lmax state after minimum matching).
// start_used is mb->start_used_ptr - subject; hitend is its incoming value.
// partial: 0=none, 1=soft, 2=hard; allowemptypartial as in match block.
ClassRunResult pcre2_class_repeat(const uint8_t *subject, size_t length,
    size_t start, uint32_t min, uint32_t max, ClassType type,
    const uint8_t *ctypes, int partial, size_t start_used,
    int allowemptypartial, int hitend);
ClassRunResult pcre2_class_repeat_rvv(const uint8_t *subject, size_t length,
    size_t start, uint32_t min, uint32_t max, ClassType type,
    const uint8_t *ctypes, int partial, size_t start_used,
    int allowemptypartial, int hitend);

#endif
