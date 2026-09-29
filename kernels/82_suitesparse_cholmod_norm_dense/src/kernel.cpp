/****************************************************************************
 *
 *  Project: SuiteSparse 7.14.1 (CHOLMOD)
 *  Source files:
 *    CHOLMOD/MatrixOps/t_cholmod_norm_worker.c
 *    CHOLMOD/Include/cholmod_template.h
 *    CHOLMOD/Include/cholmod_internal.h
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
 */

#include "kernel.h"

#include <cmath>

/* CHOLMOD/Include/cholmod_internal.h:237, via
 * CHOLMOD/Include/cholmod_template.h:174 (REAL, DOUBLE). */
#define ABS(x, z, p) std::fabs((double)(x[p]))

namespace suitesparse {

/* Wrapper supplying the inputs and initial value of the selected
 * CHOLMOD/MatrixOps/t_cholmod_norm_worker.c:96-116 branch (norm == 1). */
double cholmod_norm_dense(Int nrow, Int ncol, Int d, const double *Xx) {
    using std::isnan;
    double xnorm = 0;

    /* CHOLMOD/MatrixOps/t_cholmod_norm_worker.c:103-114 */
    for (Int j = 0; j < ncol; j++) {
        double s = 0;
        for (Int i = 0; i < nrow; i++) {
            s += ABS(Xx, nullptr, i + j * d);
        }
        if ((isnan(s) || s > xnorm) && !isnan(xnorm)) {
            xnorm = s;
        }
    }
    return xnorm;
}

} // namespace suitesparse

#undef ABS
