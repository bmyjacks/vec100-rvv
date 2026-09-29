/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/add/template/GB_add_template.c
 *    Source/add/template/GB_add_full_32.c
 *  Source function: GB_add_full_32 (method 32 of the add template:
 *                   GB_ADD_PHASE 2, C and A full, B sparse/hyper,
 *                   plus_fp64), with the int taskid declaration from
 *                   GB_add_template.c:33
 *  Region: Source/add/template/GB_add_full_32.c:10-61 (Method32 full
 *          initialization and B scatter) and
 *          Source/add/template/GB_add_template.c:33 (int taskid ;)
 *
 *
 *  Below are the copyright notices of original files
 *
 *
 * Source/add/template/GB_add_template.c
 *
 *   GB_add_template:  phase1 and phase2 for C=A+B, C<M>=A+B, C<!M>=A+B
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/add/template/GB_add_full_32.c
 *
 *   GB_add_full_32:  C=A+B, C and A are full, B is sparse/hyper
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "kernel.h"

/*
 * Wrapper for invoking the extracted template fragment.
 */
void GB_add_full_32(const uint32_t *restrict Bp32,
                    const uint64_t *restrict Bp64, const int32_t *restrict Bi32,
                    const int64_t *restrict Bi64, const uint32_t *restrict Bh32,
                    const uint64_t *restrict Bh64, const double *restrict Bx,
                    const double *restrict Ax, double *restrict Cx,
                    const int64_t *restrict B_ek_slicing, const int B_ntasks,
                    const int B_nthreads, const int64_t vlen, const bool A_iso,
                    const bool B_iso, const int64_t cnz, const int C_nthreads,
                    const double beta_scalar) {
    /*
     * Source/add/template/GB_add_template.c:33
     */
    int taskid;

    /*
     * Source/add/template/GB_add_full_32.c:10-31
     */
#pragma omp parallel for num_threads(C_nthreads) schedule(static)
    for (int64_t p = 0; p < cnz; p++) {
        {
            GB_COPY_A_to_C(Cx, p, Ax, p, A_iso);
        }
    }

    /*
     * Source/add/template/GB_add_full_32.c:33-60
     */
    const int64_t *kfirst_Bslice = B_ek_slicing;
    const int64_t *klast_Bslice = B_ek_slicing + B_ntasks;
    const int64_t *pstart_Bslice = B_ek_slicing + B_ntasks * 2;
#pragma omp parallel for num_threads(B_nthreads) schedule(dynamic, 1)
    for (taskid = 0; taskid < B_ntasks; taskid++) {
        int64_t kfirst = kfirst_Bslice[taskid];
        int64_t klast = klast_Bslice[taskid];
        for (int64_t k = kfirst; k <= klast; k++) {
            int64_t j = GBh_B(Bh, k);
            GB_GET_PA(pB_start, pB_end, taskid, k, kfirst, klast, pstart_Bslice,
                      GB_IGET(Bp, k), GB_IGET(Bp, k + 1));
            int64_t pC_start = j * vlen;
            for (int64_t pB = pB_start; pB < pB_end; pB++) {
                int64_t i = GB_IGET(Bi, pB);
                int64_t p = pC_start + i;
                GB_LOAD_A(aij, Ax, p, A_iso);
                GB_LOAD_B(bij, Bx, pB, B_iso);
                GB_EWISEOP(Cx, p, aij, bij, i, j);
            }
        }
    }
}
