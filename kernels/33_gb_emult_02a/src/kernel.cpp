/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/emult/template/GB_emult_02a.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * Source/emult/template/GB_emult_02a.c
 *
 *   GB_emult_02a: C = A.*B when A is sparse/hyper and B is bitmap
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "kernel.h"

/*
 * Wrapper for invoking the selected task-sliced template.
 */
void GB_emult_02a_template(
    const int32_t *restrict Ai32, const int64_t *restrict Ai64,
    const uint32_t *restrict Ap32, const uint64_t *restrict Ap64,
    const uint32_t *restrict Ah32, const uint64_t *restrict Ah64,
    const uint32_t *restrict Cp32, const uint64_t *restrict Cp64,
    const double *restrict Ax, const double *restrict Bx,
    const int8_t *restrict Bb, double *restrict Cx, int32_t *restrict Ci32,
    int64_t *restrict Ci64, const int64_t *restrict kfirst_Aslice,
    const int64_t *restrict klast_Aslice, const int64_t *restrict pstart_Aslice,
    const int64_t *restrict Cp_kfirst, int64_t vlen, int A_ntasks,
    int A_nthreads, const bool A_iso, const bool B_iso) {
    /*
     * Source/emult/template/GB_emult_02a.c:13-50
     */
    {
        int tid;
#pragma omp parallel for num_threads(A_nthreads) schedule(dynamic, 1)
        for (tid = 0; tid < A_ntasks; tid++) {
            int64_t kfirst = kfirst_Aslice[tid];
            int64_t klast = klast_Aslice[tid];
            for (int64_t k = kfirst; k <= klast; k++) {
                int64_t j = GBh_A(Ah, k);
                int64_t pB_start = j * vlen;
                GB_GET_PA_AND_PC(pA, pA_end, pC, tid, k, kfirst, klast,
                                 pstart_Aslice, Cp_kfirst, GB_IGET(Ap, k),
                                 GB_IGET(Ap, k + 1), GB_IGET(Cp, k));
                for (; pA < pA_end; pA++) {
                    int64_t i = GB_IGET(Ai, pA);
                    int64_t pB = pB_start + i;
                    if (!Bb[pB])
                        continue;
                    GB_ISET(Ci, pC, i);
                    GB_DECLAREA(aij);
                    GB_GETA(aij, Ax, pA, A_iso);
                    GB_DECLAREB(bij);
                    GB_GETB(bij, Bx, pB, B_iso);
                    GB_EWISEOP(Cx, pC, aij, bij, i, j);
                    pC++;
                }
            }
        }
    }
}
