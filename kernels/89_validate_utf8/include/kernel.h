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

#ifndef KERNELS_89_VALIDATE_UTF8_INCLUDE_KERNEL_H_
#define KERNELS_89_VALIDATE_UTF8_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

namespace simdjson {
namespace fallback {

/*
 * simdjson.cpp:63507-63521 (isolated class: the unrelated parser/minifier
 * virtual interface is not needed to call this member function)
 */
class implementation final {
  public:
    bool validate_utf8(const char *buf, size_t len) const noexcept;
};

} // namespace fallback
} // namespace simdjson

/*
 * Wrapper for invoking the extracted kernel.
 */
bool validate_utf8_isolated(const char *buf, size_t len);

#endif // KERNELS_89_VALIDATE_UTF8_INCLUDE_KERNEL_H_
