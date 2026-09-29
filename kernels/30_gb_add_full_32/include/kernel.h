/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/builtin/include/GB_opaque.h
 *    Source/slice/include/GB_ek_slice_kernels.h
 *    Source/math/include/GB_math_macros.h
 *    Source/add/template/GB_add_template.c
 *    Source/ewise/include/GB_ewise_shared_definitions.h
 *    FactoryKernels/GB_ew__plus_fp64.c
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
 * Source/slice/include/GB_ek_slice_kernels.h
 *
 *   GB_ek_slice_kernels.h: slice the entries and vectors of a matrix
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
 * Source/add/template/GB_add_template.c
 *
 *   GB_add_template:  phase1 and phase2 for C=A+B, C<M>=A+B, C<!M>=A+B
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
 * FactoryKernels/GB_ew__plus_fp64.c
 *
 *   GB_ew: ewise kernels for each built-in binary operator
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef KERNELS_30_GB_ADD_FULL_32_INCLUDE_KERNEL_H_
#define KERNELS_30_GB_ADD_FULL_32_INCLUDE_KERNEL_H_

#include <cstdint>
#define restrict __restrict

/* Source/builtin/include/GB_opaque.h:608-611 */
#define GBh(Ah, k) ((Ah##32) ? Ah##32 [k] : ((Ah##64) ? Ah##64 [k] : (k)))

/* Source/builtin/include/GB_opaque.h:619-620 */
#define GB_IGET(I, k) (I##32 ? I##32 [k] : I##64 [k])

/* Source/builtin/include/GB_opaque.h:793 */
#define GBh_B(Bh, k) GBh(Bh, k)

/* Source/math/include/GB_math_macros.h:38 */
#define GB_IMIN(x, y) (((x) < (y)) ? (x) : (y))

/* Source/slice/include/GB_ek_slice_kernels.h:138-157 */
#define GB_GET_PA(pA_start, pA_end, tid, k, kfirst, klast, pstart_slice, p0,   \
                  p1)                                                          \
    int64_t pA_start, pA_end;                                                  \
    if (k == kfirst) {                                                         \
        pA_start = pstart_slice[tid];                                          \
        pA_end = GB_IMIN(p1, pstart_slice[tid + 1]);                           \
    } else if (k == klast) {                                                   \
        pA_start = p0;                                                         \
        pA_end = pstart_slice[tid + 1];                                        \
    } else {                                                                   \
        pA_start = p0;                                                         \
        pA_end = p1;                                                           \
    }

/* Source/add/template/GB_add_template.c:158-163 (non-positional selection) */
#define GB_LOAD_A(aij, Ax, pA, A_iso)                                          \
    GB_DECLAREA(aij);                                                          \
    GB_GETA(aij, Ax, pA, A_iso)
#define GB_LOAD_B(bij, Bx, pB, B_iso)                                          \
    GB_DECLAREB(bij);                                                          \
    GB_GETB(bij, Bx, pB, B_iso)

/* Source/ewise/include/GB_ewise_shared_definitions.h:21-23 */
#define GB_EWISEOP(Cx, p, aij, bij, i, j) GB_BINOP(Cx[p], aij, bij, i, j)

/* Source/ewise/include/GB_ewise_shared_definitions.h:35-38 */
#define GB_COPY_A_to_C(Cx, pC, Ax, pA, A_iso) Cx[pC] = Ax[(A_iso) ? 0 : (pA)]

/* FactoryKernels/GB_ew__plus_fp64.c:24 */
#define GB_BINOP(z, x, y, i, j) z = (x) + (y)

/* FactoryKernels/GB_ew__plus_fp64.c:32-33 */
#define GB_DECLAREA(aij) double aij
#define GB_GETA(aij, Ax, pA, A_iso) aij = Ax[(A_iso) ? 0 : (pA)]

/* FactoryKernels/GB_ew__plus_fp64.c:38-39 */
#define GB_DECLAREB(bij) double bij
#define GB_GETB(bij, Bx, pB, B_iso) bij = Bx[(B_iso) ? 0 : (pB)]

/*
 * Wrapper for invoking the extracted template method.
 */
void GB_add_full_32(const uint32_t *restrict Bp32,
                    const uint64_t *restrict Bp64, const int32_t *restrict Bi32,
                    const int64_t *restrict Bi64, const uint32_t *restrict Bh32,
                    const uint64_t *restrict Bh64, const double *restrict Bx,
                    const double *restrict Ax, double *restrict Cx,
                    const int64_t *restrict B_ek_slicing, int B_ntasks,
                    int B_nthreads, int64_t vlen, bool A_iso, bool B_iso,
                    int64_t cnz, int C_nthreads, double beta_scalar);

void GB_add_full_32_rvv(const uint32_t *Bp32, const uint64_t *Bp64,
                        const int32_t *Bi32, const int64_t *Bi64,
                        const uint32_t *Bh32, const uint64_t *Bh64,
                        const double *Bx, const double *Ax, double *Cx,
                        const int64_t *B_ek_slicing, int B_ntasks,
                        int B_nthreads, int64_t vlen, bool A_iso, bool B_iso,
                        int64_t cnz, int C_nthreads, double beta_scalar);

#endif // KERNELS_30_GB_ADD_FULL_32_INCLUDE_KERNEL_H_
