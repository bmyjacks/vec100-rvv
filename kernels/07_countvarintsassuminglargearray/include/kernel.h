/****************************************************************************
 *
 *
 *  Project: Protocol Buffers (protobuf) 36.2
 *  Source files:
 *    src/google/protobuf/parse_context.h
 *    src/google/protobuf/endian.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/google/protobuf/parse_context.h
 *
 *   Protocol Buffers - Google's data interchange format
 *
 * Copyright 2008 Google Inc.  All rights reserved.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file or at
 * https://developers.google.com/open-source/licenses/bsd
 *
 *
 * src/google/protobuf/endian.h
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

#ifndef KERNELS_07_COUNTVARINTSASSUMINGLARGEARRAY_INCLUDE_KERNEL_H_
#define KERNELS_07_COUNTVARINTSASSUMINGLARGEARRAY_INCLUDE_KERNEL_H_

#include <cstdint>
#include <cstring>

/*
 * src/google/protobuf/endian.h:70
 */
namespace little_endian {

/*
 * src/google/protobuf/endian.h:118-124
 */
inline uint64_t ToHost(uint64_t value) { return value; }

} // namespace little_endian

/*
 * src/google/protobuf/parse_context.h:70-73
 */
int CountVarintsAssumingLargeArray(const char *ptr, const char *end);

/*
 * src/google/protobuf/parse_context.h:866-867
 */
template <int> struct EndianHelper;

/*
 * src/google/protobuf/parse_context.h:892-899
 */
template <> struct EndianHelper<8> {
    static uint64_t Load(const void *p) {
        uint64_t tmp;
        std::memcpy(&tmp, p, 8);
        return little_endian::ToHost(tmp);
    }
};

#endif // KERNELS_07_COUNTVARINTSASSUMINGLARGEARRAY_INCLUDE_KERNEL_H_
