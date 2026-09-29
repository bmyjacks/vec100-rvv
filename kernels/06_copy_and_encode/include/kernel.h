/****************************************************************************
 *
 *
 *  Project: XZ Utils (xz) 5.8.4
 *  Source files:
 *    src/liblzma/api/lzma.h
 *    src/liblzma/api/lzma/vli.h
 *    src/liblzma/api/lzma/check.h
 *    src/liblzma/api/lzma/base.h
 *    src/liblzma/api/lzma/filter.h
 *    src/liblzma/api/lzma/delta.h
 *    src/liblzma/common/common.h
 *    src/liblzma/delta/delta_private.h
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * src/liblzma/api/lzma.h
 *
 *   The public API of liblzma data compression library
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author: Lasse Collin
 *
 *
 * src/liblzma/api/lzma/vli.h
 *
 *   Variable-length integer handling
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author: Lasse Collin
 *
 *
 * src/liblzma/api/lzma/check.h
 *
 *   Integrity checks
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author: Lasse Collin
 *
 *
 * src/liblzma/api/lzma/base.h
 *
 *   Data types and functions used in many places in liblzma API
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author: Lasse Collin
 *
 *
 * src/liblzma/api/lzma/filter.h
 *
 *   Common filter related types and functions
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author: Lasse Collin
 *
 *
 * src/liblzma/api/lzma/delta.h
 *
 *   Delta filter
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author: Lasse Collin
 *
 *
 * src/liblzma/common/common.h
 *
 *   Definitions common to the whole liblzma library
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author:     Lasse Collin
 *
 *
 * src/liblzma/delta/delta_private.h
 *
 *   Private common stuff for Delta encoder and decoder
 *
 * SPDX-License-Identifier: 0BSD
 *
 * Author:     Lasse Collin
 *
 */

#ifndef KERNELS_06_COPY_AND_ENCODE_INCLUDE_KERNEL_H_
#define KERNELS_06_COPY_AND_ENCODE_INCLUDE_KERNEL_H_

#include <stddef.h>
#include <stdint.h>

/*
 * src/liblzma/api/lzma.h:202
 */
#define LZMA_API_CALL

/*
 * src/liblzma/api/lzma/vli.h:61
 */
typedef uint64_t lzma_vli;

/*
 * src/liblzma/api/lzma/check.h:25-53
 */
typedef enum {
    LZMA_CHECK_NONE = 0,
    LZMA_CHECK_CRC32 = 1,
    LZMA_CHECK_CRC64 = 4,
    LZMA_CHECK_SHA256 = 10
} lzma_check;

/*
 * src/liblzma/api/lzma/base.h:55-271
 */
typedef enum {
    LZMA_OK = 0,
    LZMA_STREAM_END = 1,
    LZMA_NO_CHECK = 2,
    LZMA_UNSUPPORTED_CHECK = 3,
    LZMA_GET_CHECK = 4,
    LZMA_MEM_ERROR = 5,
    LZMA_MEMLIMIT_ERROR = 6,
    LZMA_FORMAT_ERROR = 7,
    LZMA_OPTIONS_ERROR = 8,
    LZMA_DATA_ERROR = 9,
    LZMA_BUF_ERROR = 10,
    LZMA_PROG_ERROR = 11,
    LZMA_SEEK_NEEDED = 12,
    LZMA_RET_INTERNAL1 = 101,
    LZMA_RET_INTERNAL2 = 102,
    LZMA_RET_INTERNAL3 = 103,
    LZMA_RET_INTERNAL4 = 104,
    LZMA_RET_INTERNAL5 = 105,
    LZMA_RET_INTERNAL6 = 106,
    LZMA_RET_INTERNAL7 = 107,
    LZMA_RET_INTERNAL8 = 108
} lzma_ret;

/*
 * src/liblzma/api/lzma/base.h:284-379
 */
typedef enum {
    LZMA_RUN = 0,
    LZMA_SYNC_FLUSH = 1,
    LZMA_FULL_FLUSH = 2,
    LZMA_FULL_BARRIER = 4,
    LZMA_FINISH = 3
} lzma_action;

/*
 * src/liblzma/api/lzma/base.h:406-470
 */
typedef struct {
    void *(LZMA_API_CALL *alloc)(void *opaque, size_t nmemb, size_t size);
    void(LZMA_API_CALL *free)(void *opaque, void *ptr);
    void *opaque;
} lzma_allocator;

/*
 * src/liblzma/api/lzma/filter.h:41-63
 */
typedef struct {
    lzma_vli id;
    void *options;
} lzma_filter;

/*
 * src/liblzma/common/common.h:175
 */
typedef struct lzma_next_coder_s lzma_next_coder;

/*
 * src/liblzma/common/common.h:189-198
 */
typedef lzma_ret (*lzma_code_function)(
    void *coder, const lzma_allocator *allocator,
    const uint8_t *__restrict__ in, size_t *__restrict__ in_pos, size_t in_size,
    uint8_t *__restrict__ out, size_t *__restrict__ out_pos, size_t out_size,
    lzma_action action);

typedef void (*lzma_end_function)(void *coder, const lzma_allocator *allocator);

/*
 * src/liblzma/common/common.h:222-273
 */
struct lzma_next_coder_s {
    void *coder;
    lzma_vli id;
    uintptr_t init;
    lzma_code_function code;
    lzma_end_function end;
    void (*get_progress)(void *coder, uint64_t *progress_in,
                         uint64_t *progress_out);
    lzma_check (*get_check)(const void *coder);
    lzma_ret (*memconfig)(void *coder, uint64_t *memusage,
                          uint64_t *old_memlimit, uint64_t new_memlimit);
    lzma_ret (*update)(void *coder, const lzma_allocator *allocator,
                       const lzma_filter *filters,
                       const lzma_filter *reversed_filters);
    lzma_ret (*set_out_limit)(void *coder, uint64_t *uncomp_size,
                              uint64_t out_limit);
};

/*
 * src/liblzma/api/lzma/delta.h:67
 */
#define LZMA_DELTA_DIST_MAX 256

/*
 * src/liblzma/delta/delta_private.h:17-29
 */
typedef struct {
    lzma_next_coder next;
    size_t distance;
    uint8_t pos;
    uint8_t history[LZMA_DELTA_DIST_MAX];
} lzma_delta_coder;

/*
 * Wrapper for invoking the extracted static function.
 */
void copy_and_encode_isolated(lzma_delta_coder *coder,
                              const uint8_t *__restrict__ in,
                              uint8_t *__restrict__ out, size_t size);

/*
 * Wrapper for invoking the extracted static function.
 */
void encode_in_place_isolated(lzma_delta_coder *coder, uint8_t *buffer,
                              size_t size);

#endif // KERNELS_06_COPY_AND_ENCODE_INCLUDE_KERNEL_H_
