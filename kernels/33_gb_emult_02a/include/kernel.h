/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/builtin/include/GB_opaque.h
 *    Source/ewise/include/GB_ewise_shared_definitions.h
 *    FactoryKernels/GB_ew__times_fp64.c
 *    Source/math/include/GB_math_macros.h
 *    Source/slice/include/GB_ek_slice_kernels.h
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
 * Source/ewise/include/GB_ewise_shared_definitions.h
 *
 *   GB_ewise_shared_definitions.h: common macros for ewise kernels
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * FactoryKernels/GB_ew__times_fp64.c
 *
 *   GB_ew: ewise kernels for each built-in binary operator
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/math/include/GB_math_macros.h
 *
 *   GB_math_macros.h: simple math macros
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/slice/include/GB_ek_slice_kernels.h
 *
 *   GB_ek_slice_kernels.h: slice the entries and vectors of a matrix
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef KERNELS_33_GB_EMULT_02A_INCLUDE_KERNEL_H_
#define KERNELS_33_GB_EMULT_02A_INCLUDE_KERNEL_H_

#include <cstdint>
#define restrict __restrict

/* Source/builtin/include/GB_opaque.h:608-611 */
#define GBh(Ah, k) ((Ah##32) ? Ah##32 [k] : ((Ah##64) ? Ah##64 [k] : (k)))

/* Source/builtin/include/GB_opaque.h:619-624 */
#define GB_IGET(I, k) (I##32 ? I##32 [k] : I##64 [k])
#define GB_ISET(I, k, i)                                                       \
    {                                                                          \
        if (I##64) {                                                           \
            I##64 [k] = (i);                                                   \
        } else {                                                               \
            I##32 [k] = (i);                                                   \
        }                                                                      \
    }

/* Source/builtin/include/GB_opaque.h:785 */
#define GBh_A(Ah, k) GBh(Ah, k)

/* Source/ewise/include/GB_ewise_shared_definitions.h:21-23 */
#define GB_EWISEOP(Cx, p, aij, bij, i, j) GB_BINOP(Cx[p], aij, bij, i, j)

/* FactoryKernels/GB_ew__times_fp64.c:24 */
#define GB_BINOP(z, x, y, i, j) z = (x) * (y)

/* FactoryKernels/GB_ew__times_fp64.c:32-33 */
#define GB_DECLAREA(aij) double aij
#define GB_GETA(aij, Ax, pA, A_iso) aij = Ax[(A_iso) ? 0 : (pA)]

/* FactoryKernels/GB_ew__times_fp64.c:38-39 */
#define GB_DECLAREB(bij) double bij
#define GB_GETB(bij, Bx, pB, B_iso) bij = Bx[(B_iso) ? 0 : (pB)]

/* Source/math/include/GB_math_macros.h:38 */
#define GB_IMIN(x, y) (((x) < (y)) ? (x) : (y))

/* Source/slice/include/GB_ek_slice_kernels.h:104-126 */
#define GB_GET_PA_AND_PC(pA_start, pA_end, pC, tid, k, kfirst, klast,          \
                         pstart_slice, Cp_kfirst, p0, p1, p2)                  \
    int64_t pA_start, pA_end, pC;                                              \
    if (k == kfirst) {                                                         \
        pA_start = pstart_slice[tid];                                          \
        pA_end = GB_IMIN(p1, pstart_slice[tid + 1]);                           \
        pC = Cp_kfirst[tid];                                                   \
    } else if (k == klast) {                                                   \
        pA_start = p0;                                                         \
        pA_end = pstart_slice[tid + 1];                                        \
        pC = p2;                                                               \
    } else {                                                                   \
        pA_start = p0;                                                         \
        pA_end = p1;                                                           \
        pC = p2;                                                               \
    }

/*
 * Wrapper for invoking Source/emult/template/GB_emult_02a.c:13-50.
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
    int A_nthreads, const bool A_iso, const bool B_iso);

void GB_emult_02a_template_rvv(
    const int32_t *Ai32, const int64_t *Ai64,
    const uint32_t *Ap32, const uint64_t *Ap64,
    const uint32_t *Ah32, const uint64_t *Ah64,
    const uint32_t *Cp32, const uint64_t *Cp64,
    const double *Ax, const double *Bx, const int8_t *Bb,
    double *Cx, int32_t *Ci32, int64_t *Ci64,
    const int64_t *kfirst_Aslice, const int64_t *klast_Aslice,
    const int64_t *pstart_Aslice, const int64_t *Cp_kfirst,
    int64_t vlen, int A_ntasks, int A_nthreads, bool A_iso, bool B_iso);

#endif // KERNELS_33_GB_EMULT_02A_INCLUDE_KERNEL_H_
