/****************************************************************************
 *
 *
 *  Project: simdjson 4.6.11
 *  Source files:
 *    simdjson.cpp
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * simdjson.cpp
 *
 * Copyright 2018-2025 The simdjson authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef KERNELS_63_NUMBER_OF_DIGITS_DECIMAL_LEFT_SHIFT_INCLUDE_KERNEL_H_
#define KERNELS_63_NUMBER_OF_DIGITS_DECIMAL_LEFT_SHIFT_INCLUDE_KERNEL_H_

#include <cstdint>

namespace simdjson {
namespace internal {

/*
 * simdjson.cpp:4237-4238
 */
namespace {
constexpr uint32_t max_digits = 768;
} // namespace

/*
 * simdjson.cpp:4248-4254
 */
struct decimal {
    uint32_t num_digits;
    int32_t decimal_point;
    bool negative;
    bool truncated;
    uint8_t digits[max_digits];
};

/*
 * Wrapper for invoking the extracted kernel.
 */
uint32_t number_of_digits_decimal_left_shift_isolated(decimal &h,
                                                      uint32_t shift);

} // namespace internal
} // namespace simdjson

#endif // KERNELS_63_NUMBER_OF_DIGITS_DECIMAL_LEFT_SHIFT_INCLUDE_KERNEL_H_
