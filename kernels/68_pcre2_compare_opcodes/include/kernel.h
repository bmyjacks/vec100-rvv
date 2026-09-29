/****************************************************************************
 *
 *
 *  Project: PCRE2 10.48
 *  Source files:
 *    src/pcre2_internal.h
 *
 *
 *  The original file copyright and license notices follow.
 *
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

#ifndef KERNELS_68_PCRE2_COMPARE_OPCODES_INCLUDE_KERNEL_H_
#define KERNELS_68_PCRE2_COMPARE_OPCODES_INCLUDE_KERNEL_H_

#include <stdint.h>

/*
 * src/pcre2_internal.h:93-97
 */
typedef int BOOL;
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

/*
 * Wrapper for the bitmap comparison in compare_opcodes(). Both inputs must
 * provide 32 readable bytes. FALSE is the original early return on a nonzero
 * AND (or AND-NOT); TRUE means the loops completed without that early return.
 * In the enclosing original function, completion then tested list[1] and
 * either returned TRUE or continued scanning opcodes.
 */
BOOL pcre2_009_class_intersect(const uint8_t *set1, const uint8_t *set2,
                               BOOL invert_bits);

#endif // KERNELS_68_PCRE2_COMPARE_OPCODES_INCLUDE_KERNEL_H_
