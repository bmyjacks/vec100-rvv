/* auto-generated on 2026-09-04 16:04:31 -0400. version 4.6.11 Do not edit! */
/****************************************************************************
 * Project: simdjson 4.6.11 (single-file amalgamation)
 * Source files: simdjson-4.6.11/simdjson.h,
 *               simdjson-4.6.11/simdjson.cpp
 *
 * Original files have the same auto-generated banner above, but no
 * copyright/license notice in their headers. The frozen extraction records
 * the simdjson project as Apache-2.0.
 ****************************************************************************/

#ifndef KERNELS_77_SIMDJSON_EIGHT_DIGITS_INCLUDE_KERNEL_H_
#define KERNELS_77_SIMDJSON_EIGHT_DIGITS_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

namespace simdjson {

/* simdjson-4.6.11/simdjson.h:3283 */
constexpr size_t SIMDJSON_PADDING = 64;

namespace fallback {
namespace numberparsing {

// Exposed from the upstream unnamed namespace; both functions read exactly
// eight bytes at chars (which need not be aligned) and return true iff all
// eight are ASCII '0'..'9'. The caller must supply eight readable bytes,
// including padding if the logical input ends before them. No NUL check.
bool is_made_of_eight_digits_fast(const uint8_t *chars);
bool is_made_of_eight_digits_fast_rvv(const uint8_t *chars);

} // namespace numberparsing
} // namespace fallback
} // namespace simdjson

#endif // KERNELS_77_SIMDJSON_EIGHT_DIGITS_INCLUDE_KERNEL_H_
