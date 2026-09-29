/****************************************************************************
 *
 *
 *  Project: PCRE2 10.48
 *  Source files:
 *    src/pcre2_match.c
 *    src/pcre2_dfa_match.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/pcre2_match.c
 *
 *   PCRE is a library of functions to support regular expressions whose syntax
 *   and semantics are as close as possible to those of the Perl 5 language.
 *
 * Written by Philip Hazel
 * Original API code Copyright (c) 1997-2012 University of Cambridge
 * New API code Copyright (c) 2015-2024 University of Cambridge
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
 * src/pcre2_dfa_match.c
 *
 *   This module contains the external function pcre2_dfa_match(), which is an
 *   alternative matching function that uses a sort of DFA algorithm (not a true
 *   FSM). This is NOT Perl-compatible, but it has advantages in certain
 *   applications.
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
 */

#include "kernel.h"

/*
 * Wrapper for invoking the extracted kernel.
 */
PCRE2_SPTR
pcre2_match_start_bits_scan(PCRE2_SPTR start_match, PCRE2_SPTR end_subject,
                            PCRE2_SPTR mb_end_subject,
                            const uint8_t *start_bits, uint16_t partial,
                            BOOL *nomatch) {
    *nomatch = 0;

    /*
     * src/pcre2_match.c:7796-7815
     */
    if (start_bits != NULL) {
        while (start_match < end_subject) {
            uint32_t c = *start_match;
            if ((start_bits[c / 8] & (1u << (c & 7))) != 0)
                break;
            start_match++;
        }

        if (partial == 0 && start_match >= mb_end_subject) {
            *nomatch = 1;
        }
    }

    return start_match;
}

/*
 * Wrapper for invoking the extracted kernel.
 */
PCRE2_SPTR
pcre2_dfa_match_start_bits_scan(PCRE2_SPTR start_match, PCRE2_SPTR end_subject,
                                PCRE2_SPTR mb_end_subject,
                                const uint8_t *start_bits, uint32_t moptions,
                                BOOL *nomatch) {
    *nomatch = 0;

    /*
     * src/pcre2_dfa_match.c:3928-3945
     */
    if (start_bits != NULL) {
        while (start_match < end_subject) {
            uint32_t c = *start_match;
            if ((start_bits[c / 8] & (1u << (c & 7))) != 0)
                break;
            start_match++;
        }

        if ((moptions & (PCRE2_PARTIAL_HARD | PCRE2_PARTIAL_SOFT)) == 0 &&
            start_match >= mb_end_subject)
            *nomatch = 1;
    }

    return start_match;
}
