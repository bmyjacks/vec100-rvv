/****************************************************************************
 *
 *
 *  Project: Protocol Buffers (protobuf) 36.2
 *  Source files:
 *    src/google/protobuf/parse_context.cc
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/google/protobuf/parse_context.cc
 *
 *   Protocol Buffers - Google's data interchange format
 *
 * Copyright 2008 Google Inc.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file or at
 * https://developers.google.com/open-source/licenses/bsd
 *
 */

#include "kernel.h"

#include <cassert>

/*
 * src/google/protobuf/parse_context.cc:809-828
 */
int CountVarintsAssumingLargeArray(const char *ptr, const char *end) {
    int num_varints = end - ptr;
    assert(num_varints >= int{sizeof(uint64_t)});

    const char *const limit = end - sizeof(uint64_t);
    while (ptr < limit) {
        num_varints -= __builtin_popcountll(EndianHelper<8>::Load(ptr) &
                                            0x8080808080808080);
        ptr += sizeof(uint64_t);
    }

    return num_varints -
           __builtin_popcountll(EndianHelper<8>::Load(limit) &
                                (0x8080808080808080 << ((ptr - limit) * 8)));
}
