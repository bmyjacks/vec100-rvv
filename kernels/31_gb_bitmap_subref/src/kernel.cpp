/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/assign/template/GB_bitmap_assign_IxJ_template.c
 *    Source/extract/template/GB_bitmap_subref_template.c
 *    Source/extract/GB_bitmap_subref.c
 *  Source function: GB_bitmap_subref (the numeric non-iso full
 *                   instantiation of the IxJ template; the subref caller
 *                   defines GB_IXJ_WORK(pA,pC)
 *                   { GB_COPY_ENTRY (pC, pA) })
 *  Region: Source/assign/template/GB_bitmap_assign_IxJ_template.c:93-103 (the
 *          I(iA_start,iA_end-1) loop); GB_bitmap_subref_template.c:50-55
 *          (GB_IXJ_WORK); GB_bitmap_subref.c:243-244 (GB_COPY_ENTRY)
 *
 *
 *  Below are the copyright notice of original file
 *
 *
 * Source/assign/template/GB_bitmap_assign_IxJ_template.c
 *
 *   GB_bitmap_assign_IxJ_template: iterate over all of C(I,J)
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
 * Source/extract/GB_bitmap_subref.c
 *
 *   GB_bitmap_subref: C = A(I,J) where A is bitmap or full
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "kernel.h"

/*
 * Wrapper for invoking the extracted template fragment.
 * Here pC0 is the base in upstream A and pA0 is the base in result C:
 * the upstream subref caller swaps these in GB_IXJ_WORK (pC, pA).
 */
void GB_bitmap_subref(GB_void *restrict Cx, const GB_void *restrict Ax,
                      size_t csize, const uint32_t *restrict I32,
                      const uint64_t *restrict I64, int Ikind,
                      const int64_t *restrict Icolon, int64_t iA_start,
                      int64_t iA_end, int64_t pC0, int64_t pA0) {
    /*
     * Source/assign/template/GB_bitmap_assign_IxJ_template.c:93-103
     */
    for (int64_t iA = iA_start; iA < iA_end; iA++) {
        int64_t iC = GB_IJLIST(I, iA, GB_I_KIND, Icolon);
        int64_t pC = iC + pC0;
        int64_t pA = iA + pA0;
        GB_IXJ_WORK(pC, pA);
    }
}
