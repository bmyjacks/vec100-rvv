/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/builtin/include/GB_opaque.h
 *    Source/builder/template/GB_bld_template.c
 *    FactoryKernels/GB_bld__plus_fp64.c
 *    Source/include/GB_abort.h
 *    Include/GraphBLAS.h
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
 * Source/builder/template/GB_bld_template.c
 *
 *   GB_bld_template.c: Tx=build(Sx), and assemble any duplicate tuples
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * FactoryKernels/GB_bld__plus_fp64.c
 *
 *   GB_bld:  hard-coded functions for builder methods
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/include/GB_abort.h
 *
 *   GB_abort.h: assertions for all of GraphBLAS, including JIT kernels
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
 */

#ifndef KERNELS_32_GB_BLD_TEMPLATE_INCLUDE_KERNEL_H_
#define KERNELS_32_GB_BLD_TEMPLATE_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>
#define restrict __restrict

/* Include/GraphBLAS.h:373-376 (selected success result) */
typedef enum { GrB_SUCCESS = 0 } GrB_Info;

/* Source/builtin/include/GB_opaque.h:601-603; C++ void-pointer casts */
#define GB_IPTR(I, is_32)                                                      \
    I##32 = (is_32) ? reinterpret_cast<decltype(I##32)>(I) : NULL;             \
    I##64 = (is_32) ? NULL : reinterpret_cast<decltype(I##64)>(I)

/* Source/builtin/include/GB_opaque.h:620-624 */
#define GB_IGET(I, k) (I##32 ? I##32 [k] : I##64 [k])
#define GB_ISET(I, k, i)                                                       \
    {                                                                          \
        if (I##64) {                                                           \
            I##64 [k] = (i);                                                   \
        } else {                                                               \
            I##32 [k] = (i);                                                   \
        }                                                                      \
    }

/* Source/builtin/include/GB_opaque.h:636-638 */
#define GB_IDECL(I, const, u)                                                  \
    const u##int32_t *restrict I##32 = NULL;                                   \
    const u##int64_t *restrict I##64 = NULL

/* Source/include/GB_abort.h:24-43 */
void GB_abort(const char *file, int line);
#define GB_assert(X)                                                           \
    {                                                                          \
        if (!(X)) {                                                            \
            GB_abort(__FILE__, __LINE__);                                      \
        }                                                                      \
    }
#ifdef GB_DEBUG
#define ASSERT(X) GB_assert(X)
#else
#define ASSERT(X)
#endif

/* Source/builder/template/GB_bld_template.c:25-27 */
#define GB_KNOWN_NO_DUPLICATES (ndupl == 0)

/* Source/builder/template/GB_bld_template.c:29-31 */
#define GB_K_IS_NULL (K_work == NULL)

/* FactoryKernels/GB_bld__plus_fp64.c:22 */
#define GB_BLD_DUP(Tx, k, Sx, i) Tx[k] += Sx[i]

/* FactoryKernels/GB_bld__plus_fp64.c:24 */
#define GB_BLD_COPY(Tx, k, Sx, i) Tx[k] = Sx[i]

/* FactoryKernels/GB_bld__plus_fp64.c:48-64 */
GrB_Info GB_bld__plus_fp64(double *restrict Tx, void *restrict Ti,
                           bool Ti_is_32, const double *restrict Sx,
                           int64_t nvals, int64_t ndupl,
                           const void *restrict I_work, bool I_is_32,
                           const void *restrict K_work, bool K_is_32,
                           const int64_t duplicate_entry,
                           const int64_t *restrict tstart_slice,
                           const int64_t *restrict tnz_slice, int nthreads);

/*
 * Wrapper for invoking the extracted kernel.
 */
void GB_bld_template(const double *restrict Sx, double *restrict Tx,
                     const void *restrict K_work,
                     const uint32_t *restrict K_work32,
                     const uint64_t *restrict K_work64,
                     const int64_t *restrict tstart_slice, int nthreads);

GrB_Info GB_bld__plus_fp64_rvv(double *Tx, void *Ti, bool Ti_is_32,
                                const double *Sx, int64_t nvals,
                                int64_t ndupl, const void *I_work,
                                bool I_is_32, const void *K_work,
                                bool K_is_32, int64_t duplicate_entry,
                                const int64_t *tstart_slice,
                                const int64_t *tnz_slice, int nthreads);
void GB_bld_template_rvv(const double *Sx, double *Tx, const void *K_work,
                         const uint32_t *K_work32, const uint64_t *K_work64,
                         const int64_t *tstart_slice, int nthreads);

#endif // KERNELS_32_GB_BLD_TEMPLATE_INCLUDE_KERNEL_H_
