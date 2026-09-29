/****************************************************************************
 *
 *
 *  Project: PCRE2 10.48
 *  Source files:
 *    src/pcre2_internal.h
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

#ifndef KERNELS_69_PCRE2_MATCH_INCLUDE_KERNEL_H_
#define KERNELS_69_PCRE2_MATCH_INCLUDE_KERNEL_H_

#include <stddef.h>
#include <stdint.h>

/*
 * src/pcre2.h.generic:523-527
 */
typedef uint8_t PCRE2_UCHAR8;
typedef const PCRE2_UCHAR8 *PCRE2_SPTR8;

/*
 * src/pcre2.h.generic:886-887
 */
#define PCRE2_SPTR PCRE2_SPTR8

/*
 * src/pcre2_internal.h:93-97
 */
typedef int BOOL;

/*
 * src/pcre2.h.generic:183-184
 */
#define PCRE2_PARTIAL_SOFT 0x00000010u
#define PCRE2_PARTIAL_HARD 0x00000020u

/*
 * Wrapper for invoking the extracted kernel.
 */
PCRE2_SPTR pcre2_match_start_bits_scan(PCRE2_SPTR start_match,
                                       PCRE2_SPTR end_subject,
                                       PCRE2_SPTR mb_end_subject,
                                       const uint8_t *start_bits,
                                       uint16_t partial, BOOL *nomatch);

/*
 * Wrapper for invoking the extracted kernel.
 */
PCRE2_SPTR pcre2_dfa_match_start_bits_scan(PCRE2_SPTR start_match,
                                           PCRE2_SPTR end_subject,
                                           PCRE2_SPTR mb_end_subject,
                                           const uint8_t *start_bits,
                                           uint32_t moptions, BOOL *nomatch);

#endif // KERNELS_69_PCRE2_MATCH_INCLUDE_KERNEL_H_
