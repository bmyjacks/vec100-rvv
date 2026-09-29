/****************************************************************************
 *
 *
 *  Project: Protocol Buffers (protobuf) 36.2
 *  Source files:
 *    third_party/utf8_range/utf8_range.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * third_party/utf8_range/utf8_range.h
 *
 */

#ifndef KERNELS_87_UTF8_RANGE_VALIDATEUTF8NAIVE_INCLUDE_KERNEL_H_
#define KERNELS_87_UTF8_RANGE_VALIDATEUTF8NAIVE_INCLUDE_KERNEL_H_

#include <stdbool.h>
#include <stddef.h>

/*
 * third_party/utf8_range/utf8_range.h:7-9
 */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * third_party/utf8_range/utf8_range.h:11-17
 */
bool utf8_range_IsValid(const char *data, size_t len);
size_t utf8_range_ValidPrefix(const char *data, size_t len);

/*
 * third_party/utf8_range/utf8_range.h:19-21
 */
#ifdef __cplusplus
}
#endif

#endif // KERNELS_87_UTF8_RANGE_VALIDATEUTF8NAIVE_INCLUDE_KERNEL_H_
