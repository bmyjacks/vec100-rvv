/****************************************************************************
 *
 *  Project: SuiteSparse 7.14.1 (CHOLMOD)
 *  Source files:
 *    CHOLMOD/MatrixOps/t_cholmod_norm_worker.c
 *    CHOLMOD/Include/cholmod_template.h
 *    CHOLMOD/Include/cholmod_internal.h
 *    CHOLMOD/Include/cholmod_types.h
 *
 *  The original file copyright and license notices follow.
 *
 * CHOLMOD/MatrixOps/t_cholmod_norm_worker.c
 *  CHOLMOD/MatrixOps Module.  Copyright (C) 2005-2023, Timothy A. Davis.
 *  All Rights Reserved.
 *  SPDX-License-Identifier: GPL-2.0+
 *
 * CHOLMOD/Include/cholmod_template.h
 *  CHOLMOD/Include/cholmod_internal.h. Copyright (C) 2005-2023,
 *  Timothy A. Davis.  All Rights Reserved.
 *  SPDX-License-Identifier: Apache-2.0
 *
 * CHOLMOD/Include/cholmod_internal.h
 *  CHOLMOD/Include/cholmod_internal.h. Copyright (C) 2005-2023,
 *  Timothy A. Davis.  All Rights Reserved.
 *  SPDX-License-Identifier: Apache-2.0
 *
 * CHOLMOD/Include/cholmod_types.h
 *  CHOLMOD/Include/cholmod_types.h. Copyright (C) 2005-2023,
 *  Timothy A. Davis.  All Rights Reserved.
 *  SPDX-License-Identifier: Apache-2.0
 */

#ifndef KERNELS_82_SUITESPARSE_CHOLMOD_NORM_DENSE_INCLUDE_KERNEL_H_
#define KERNELS_82_SUITESPARSE_CHOLMOD_NORM_DENSE_INCLUDE_KERNEL_H_

#include <cstdint>

namespace suitesparse {

/* CHOLMOD/Include/cholmod_types.h:39 (CHOLMOD_INT64 configuration). */
using Int = std::int64_t;

/* Wrapper for the double-real dense 1-norm in
 * CHOLMOD/MatrixOps/t_cholmod_norm_worker.c:103-114. */
double cholmod_norm_dense(Int nrow, Int ncol, Int d, const double *Xx);
double cholmod_norm_dense_rvv(Int nrow, Int ncol, Int d, const double *Xx);

} // namespace suitesparse

#endif // KERNELS_82_SUITESPARSE_CHOLMOD_NORM_DENSE_INCLUDE_KERNEL_H_
