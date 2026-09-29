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

#ifndef KERNELS_60_MINIFY_INCLUDE_KERNEL_H_
#define KERNELS_60_MINIFY_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>

namespace simdjson {
/*
 * simdjson.cpp:2531-2566 (selected return values)
 */
enum error_code { SUCCESS = 0, UNCLOSED_STRING = 15 };

namespace fallback {
/*
 * simdjson.cpp:63507-63521 (only the selected method is required here)
 */
class implementation {
  public:
    error_code minify(const uint8_t *buf, size_t len, uint8_t *dst,
                      size_t &dst_len) const noexcept;
};
} // namespace fallback

/*
 * Wrapper for invoking the extracted kernel.
 */
error_code minify_isolated(const uint8_t *buf, size_t len, uint8_t *dst,
                           size_t &dst_len) noexcept;
} // namespace simdjson

#endif // KERNELS_60_MINIFY_INCLUDE_KERNEL_H_
