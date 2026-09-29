/****************************************************************************
 *
 *
 *  Project: LZ4 1.10.0
 *  Source files:
 *    lib/lz4.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 * lib/lz4.c
 *
 *    LZ4 - Fast LZ compression algorithm
 *    Copyright (C) 2011-2023, Yann Collet.
 *
 *    BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *
 *        * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *        * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following disclaimer
 *    in the documentation and/or other materials provided with the
 *    distribution.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *    You can contact the author at :
 *     - LZ4 homepage : http://www.lz4.org
 *     - LZ4 source repository : https://github.com/lz4/lz4
 */

#include "kernel.h"

/* Wrapper for the lib/lz4.c:1696-1699 hash-table rescale loop. */
void lz4_renorm_hash_table(LZ4_hash_table_view *LZ4_dict, U32 delta) {
    int i;
    for (i = 0; i < LZ4_HASH_SIZE_U32; i++) {
        if (LZ4_dict->hashTable[i] < delta)
            LZ4_dict->hashTable[i] = 0;
        else
            LZ4_dict->hashTable[i] -= delta;
    }
}
