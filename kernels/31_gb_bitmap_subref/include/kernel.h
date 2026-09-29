/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/builtin/include/GB_opaque.h
 *    Source/ij/include/GB_ijlist.h
 *    Include/GraphBLAS.h
 *    Source/extract/GB_bitmap_subref.c
 *    Source/extract/template/GB_bitmap_subref_template.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * Source/builtin/include/GB_opaque.h
 *
 *   GB_opaque.h: definitions of opaque objects
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/ij/include/GB_ijlist.h
 *
 *   GB_ijlist.h: return kth item, i = I [k], in an index list
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Include/GraphBLAS.h
 *
 *   GraphBLAS.h: definitions for the GraphBLAS package
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2026, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/extract/GB_bitmap_subref.c
 *
 *   GB_bitmap_subref: C = A(I,J) where A is bitmap or full
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/extract/template/GB_bitmap_subref_template.c
 *
 *   GB_bitmap_subref_template: C = A(I,J) where A is bitmap/full
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef KERNELS_31_GB_BITMAP_SUBREF_INCLUDE_KERNEL_H_
#define KERNELS_31_GB_BITMAP_SUBREF_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>
#include <cstring>
#define restrict __restrict

/* Source/builtin/include/GB_opaque.h:19 */
typedef unsigned char GB_void;

/* Source/builtin/include/GB_opaque.h:619-620 */
#define GB_IGET(I, k) (I##32 ? I##32 [k] : I##64 [k])

/* Source/ij/include/GB_ijlist.h:17-20 */
#define GB_ALL 0
#define GB_RANGE 1
#define GB_STRIDE 2
#define GB_LIST 3

/* Source/ij/include/GB_ijlist.h:31-37 */
#define GB_IJLIST(I, k, Ikind, Icolon)                                         \
    ((Ikind == GB_ALL)                                                         \
         ? (k)                                                                 \
         : ((Ikind == GB_RANGE)                                                \
                ? (Icolon[GxB_BEGIN] + (k))                                    \
                : ((Ikind == GB_STRIDE)                                        \
                       ? (Icolon[GxB_BEGIN] + (k) * Icolon[GxB_INC])           \
                       : (GB_IGET(I, k)))))

/* Include/GraphBLAS.h:1844-1846 */
#define GxB_BEGIN (0)
#define GxB_INC (2)

/* Source/extract/GB_bitmap_subref.c:114 */
#define GB_I_KIND Ikind

/* Source/extract/GB_bitmap_subref.c:243-244 */
#define GB_COPY_ENTRY(pC, pA)                                                  \
    memcpy(Cx + (pC) * csize, Ax + (pA) * csize, csize);

/* Source/extract/template/GB_bitmap_subref_template.c:50-55 */
#define GB_IXJ_WORK(pA, pC)                                                    \
    {                                                                          \
                                                                               \
        GB_COPY_ENTRY(pC, pA)}

/*
 * Wrapper for invoking the extracted full-matrix IxJ loop.
 * pC0 addresses the source A, and pA0 addresses the result C, as in
 * GB_bitmap_subref.c:138-144 and its swapped GB_IXJ_WORK invocation.
 */
void GB_bitmap_subref(GB_void *restrict Cx, const GB_void *restrict Ax,
                      size_t csize, const uint32_t *restrict I32,
                      const uint64_t *restrict I64, int Ikind,
                      const int64_t *restrict Icolon, int64_t iA_start,
                      int64_t iA_end, int64_t pC0, int64_t pA0);

void GB_bitmap_subref_rvv(GB_void *Cx, const GB_void *Ax, size_t csize,
                          const uint32_t *I32, const uint64_t *I64, int Ikind,
                          const int64_t *Icolon, int64_t iA_start,
                          int64_t iA_end, int64_t pC0, int64_t pA0);

#endif // KERNELS_31_GB_BITMAP_SUBREF_INCLUDE_KERNEL_H_
