/****************************************************************************
 *
 *
 *  Project: BLIS 2.1
 *  Source files:
 *    frame/include/bli_type_defs.h
 *    ref_kernels/1m/bli_packm_cxk_1er_ref.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * frame/include/bli_type_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2016, Hewlett Packard Enterprise Development LP
 *    Copyright (C) 2020, Advanced Micro Devices, Inc.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ref_kernels/1m/bli_packm_cxk_1er_ref.c
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef KERNELS_02_BLI_ZZPACKM_1ER_GENERIC_REF_INCLUDE_KERNEL_H_
#define KERNELS_02_BLI_ZZPACKM_1ER_GENERIC_REF_INCLUDE_KERNEL_H_

#include <cstdint>

/*
 * frame/include/bli_type_defs.h:83
 */
typedef int64_t gint_t;

/*
 * frame/include/bli_type_defs.h:110-115
 */
typedef gint_t dim_t;
typedef gint_t inc_t;

/*
 * frame/include/bli_type_defs.h:200-207
 */
typedef struct dcomplex {
    double real;
    double imag;
} dcomplex;

/*
 * frame/include/bli_type_defs.h:257-259
 */
#define BLIS_DATATYPE_NUM_BITS (BLIS_DOMAIN_NUM_BITS + BLIS_PRECISION_NUM_BITS)
#define BLIS_DOMAIN_NUM_BITS 1
#define BLIS_PRECISION_NUM_BITS 2

/*
 * frame/include/bli_type_defs.h:260-262
 */
#define BLIS_CONJTRANS_NUM_BITS (BLIS_TRANS_NUM_BITS + BLIS_CONJ_NUM_BITS)
#define BLIS_TRANS_NUM_BITS 1
#define BLIS_CONJ_NUM_BITS 1

/*
 * frame/include/bli_type_defs.h:263-268
 */
#define BLIS_UPLO_NUM_BITS                                                     \
    (BLIS_UPPER_NUM_BITS + BLIS_DIAG_NUM_BITS + BLIS_LOWER_NUM_BITS)
#define BLIS_UPPER_NUM_BITS 1
#define BLIS_DIAG_NUM_BITS 1
#define BLIS_LOWER_NUM_BITS 1
#define BLIS_UNIT_DIAG_NUM_BITS 1
#define BLIS_INVERT_DIAG_NUM_BITS 1

/*
 * frame/include/bli_type_defs.h:270-272
 */
#define BLIS_PACK_PANEL_NUM_BITS 1
#define BLIS_PACK_FORMAT_NUM_BITS 4
#define BLIS_PACK_NUM_BITS 1

/*
 * frame/include/bli_type_defs.h:283
 */
#define BLIS_DATATYPE_SHIFT 0

/*
 * frame/include/bli_type_defs.h:286-288
 */
#define BLIS_CONJTRANS_SHIFT (BLIS_DATATYPE_SHIFT + BLIS_DATATYPE_NUM_BITS)
#define BLIS_TRANS_SHIFT (BLIS_CONJTRANS_SHIFT)
#define BLIS_CONJ_SHIFT (BLIS_TRANS_SHIFT + BLIS_TRANS_NUM_BITS)

/*
 * frame/include/bli_type_defs.h:289
 */
#define BLIS_UPLO_SHIFT (BLIS_CONJTRANS_SHIFT + BLIS_CONJTRANS_NUM_BITS)

/*
 * frame/include/bli_type_defs.h:293-298
 */
#define BLIS_UNIT_DIAG_SHIFT (BLIS_UPLO_SHIFT + BLIS_UPLO_NUM_BITS)
#define BLIS_INVERT_DIAG_SHIFT (BLIS_UNIT_DIAG_SHIFT + BLIS_UNIT_DIAG_NUM_BITS)
#define BLIS_PACK_SCHEMA_SHIFT                                                 \
    (BLIS_INVERT_DIAG_SHIFT + BLIS_INVERT_DIAG_NUM_BITS)
#define BLIS_PACK_PANEL_SHIFT (BLIS_PACK_SCHEMA_SHIFT)
#define BLIS_PACK_FORMAT_SHIFT                                                 \
    (BLIS_PACK_PANEL_SHIFT + BLIS_PACK_PANEL_NUM_BITS)
#define BLIS_PACK_SHIFT (BLIS_PACK_FORMAT_SHIFT + BLIS_PACK_FORMAT_NUM_BITS)

/*
 * frame/include/bli_type_defs.h:319
 */
#define BLIS_CONJ_BIT (((1 << BLIS_CONJ_NUM_BITS) - 1) << BLIS_CONJ_SHIFT)

/*
 * frame/include/bli_type_defs.h:327-329
 */
#define BLIS_PACK_PANEL_BIT                                                    \
    (((1 << BLIS_PACK_PANEL_NUM_BITS) - 1) << BLIS_PACK_PANEL_SHIFT)
#define BLIS_PACK_FORMAT_BITS                                                  \
    (((1 << BLIS_PACK_FORMAT_NUM_BITS) - 1) << BLIS_PACK_FORMAT_SHIFT)
#define BLIS_PACK_BIT (((1 << BLIS_PACK_NUM_BITS) - 1) << BLIS_PACK_SHIFT)

/*
 * frame/include/bli_type_defs.h:357
 */
#define BLIS_BITVAL_CONJ BLIS_CONJ_BIT

/*
 * frame/include/bli_type_defs.h:367-373
 */
#define BLIS_BITVAL_1E (0x1 << BLIS_PACK_FORMAT_SHIFT)
#define BLIS_BITVAL_1R (0x2 << BLIS_PACK_FORMAT_SHIFT)
#define BLIS_BITVAL_PACKED_PANELS_1E                                           \
    (BLIS_PACK_BIT | BLIS_BITVAL_1E | BLIS_PACK_PANEL_BIT)
#define BLIS_BITVAL_PACKED_PANELS_1R                                           \
    (BLIS_PACK_BIT | BLIS_BITVAL_1R | BLIS_PACK_PANEL_BIT)

/*
 * frame/include/bli_type_defs.h:403-407
 */
typedef enum conj_e {
    BLIS_NO_CONJUGATE = 0x0,
    BLIS_CONJUGATE = BLIS_BITVAL_CONJ
} conj_t;

/*
 * frame/include/bli_type_defs.h:473-488
 */
typedef enum pack_e {
    BLIS_PACKED_PANELS_1E = BLIS_BITVAL_PACKED_PANELS_1E,
    BLIS_PACKED_PANELS_1R = BLIS_BITVAL_PACKED_PANELS_1R
} pack_t;

/*
 * frame/include/bli_type_defs.h:1457
 */
typedef struct cntx_s cntx_t;

/*
 * ref_kernels/1m/bli_packm_cxk_1er_ref.c:94-108
 */
/*
 * ref_kernels/1m/bli_packm_cxk_1er_ref.c:225
 */
void bli_zzpackm_1er_generic_ref(conj_t conja, pack_t schema, dim_t cdim,
                                 dim_t cdim_max, dim_t cdim_bcast, dim_t n,
                                 dim_t n_max, const void *kappa, const void *a,
                                 inc_t inca, inc_t lda, void *p, inc_t ldp,
                                 const void *params, const cntx_t *cntx);

/*
 * Wrapper for invoking the extracted kernel.
 */
void bli_zzpackm_1er_generic_ref_wrapper(
    conj_t conja, pack_t schema, dim_t cdim, dim_t cdim_max, dim_t cdim_bcast,
    dim_t n, dim_t n_max, const void *kappa, const void *a, inc_t inca,
    inc_t lda, void *p, inc_t ldp, const void *params, const cntx_t *cntx);

#endif // KERNELS_02_BLI_ZZPACKM_1ER_GENERIC_REF_INCLUDE_KERNEL_H_
