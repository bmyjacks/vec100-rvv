/* auto-generated on 2026-09-04 16:04:31 -0400. version 4.6.11 Do not edit! */
/****************************************************************************
 * Project: simdjson 4.6.11 (single-file amalgamation)
 * Source file: simdjson-4.6.11/simdjson.cpp
 * Original file banner above; no copyright/license notice in its header.
 * The frozen extraction records the simdjson project as Apache-2.0.
 ****************************************************************************/

#include "kernel.h"

#include <cstring>

namespace simdjson {
namespace fallback {
namespace numberparsing {

/* simdjson-4.6.11/simdjson.cpp:62362-62375; only the upstream
 * simdjson_inline keyword and unnamed namespace are removed to expose the
 * original function for standalone linking. Implementation comments removed. */
bool is_made_of_eight_digits_fast(const uint8_t *chars) {
    uint64_t val;
    static_assert(7 <= SIMDJSON_PADDING,
                  "SIMDJSON_PADDING must be bigger than 7");
    std::memcpy(&val, chars, 8);
    return (((val & 0xF0F0F0F0F0F0F0F0) |
             (((val + 0x0606060606060606) & 0xF0F0F0F0F0F0F0F0) >> 4)) ==
            0x3333333333333333);
}

} // namespace numberparsing
} // namespace fallback
} // namespace simdjson
