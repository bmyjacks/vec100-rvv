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

#include "kernel.h"

#include <cstring>

namespace simdjson {
namespace fallback {

/*
 * simdjson.cpp:65375-65435
 */
bool implementation::validate_utf8(const char *buf, size_t len) const noexcept {
    const uint8_t *data = reinterpret_cast<const uint8_t *>(buf);
    uint64_t pos = 0;
    uint32_t code_point = 0;
    while (pos < len) {
        uint64_t next_pos = pos + 16;
        if (next_pos <= len) {
            uint64_t v1;
            memcpy(&v1, data + pos, sizeof(uint64_t));
            uint64_t v2;
            memcpy(&v2, data + pos + sizeof(uint64_t), sizeof(uint64_t));
            uint64_t v{v1 | v2};
            if ((v & 0x8080808080808080) == 0) {
                pos = next_pos;
                continue;
            }
        }
        unsigned char byte = data[pos];
        if (byte < 0x80) {
            pos++;
            continue;
        } else if ((byte & 0xe0) == 0xc0) {
            next_pos = pos + 2;
            if (next_pos > len) {
                return false;
            }
            if ((data[pos + 1] & 0xc0) != 0x80) {
                return false;
            }
            code_point = (byte & 0x1f) << 6 | (data[pos + 1] & 0x3f);
            if (code_point < 0x80 || 0x7ff < code_point) {
                return false;
            }
        } else if ((byte & 0xf0) == 0xe0) {
            next_pos = pos + 3;
            if (next_pos > len) {
                return false;
            }
            if ((data[pos + 1] & 0xc0) != 0x80) {
                return false;
            }
            if ((data[pos + 2] & 0xc0) != 0x80) {
                return false;
            }
            code_point = (byte & 0x0f) << 12 | (data[pos + 1] & 0x3f) << 6 |
                         (data[pos + 2] & 0x3f);
            if (code_point < 0x800 || 0xffff < code_point ||
                (0xd7ff < code_point && code_point < 0xe000)) {
                return false;
            }
        } else if ((byte & 0xf8) == 0xf0) {
            next_pos = pos + 4;
            if (next_pos > len) {
                return false;
            }
            if ((data[pos + 1] & 0xc0) != 0x80) {
                return false;
            }
            if ((data[pos + 2] & 0xc0) != 0x80) {
                return false;
            }
            if ((data[pos + 3] & 0xc0) != 0x80) {
                return false;
            }
            code_point = (byte & 0x07) << 18 | (data[pos + 1] & 0x3f) << 12 |
                         (data[pos + 2] & 0x3f) << 6 | (data[pos + 3] & 0x3f);
            if (code_point <= 0xffff || 0x10ffff < code_point) {
                return false;
            }
        } else {
            return false;
        }
        pos = next_pos;
    }
    return true;
}

} // namespace fallback
} // namespace simdjson

/*
 * Wrapper for invoking the extracted kernel.
 */
bool validate_utf8_isolated(const char *buf, size_t len) {
    return simdjson::fallback::implementation{}.validate_utf8(buf, len);
}
