/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/builtin/include/GB_opaque.h
 *    Source/include/GB_abort.h
 *    FactoryKernels/GB_sel__gt_thunk_fp64.c
 *    Source/math/include/GB_math_macros.h
 *    Source/slice/include/GB_ek_slice_kernels.h
 *    Source/builtin/include/GB_Matrix_content.h
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
 * Source/include/GB_abort.h
 *
 *   GB_abort.h: assertions for all of GraphBLAS, including JIT kernels
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * FactoryKernels/GB_sel__gt_thunk_fp64.c
 *
 *   GB_sel:  hard-coded functions for selection operators
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
 * Source/builtin/include/GB_Matrix_content.h
 *
 *   GB_Matrix_content.h: content of GrB_Matrix, GrB_Vector, and GrB_Scalar
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

#ifndef KERNELS_34_GB_SELECT_PHASE2_ENTRY_INCLUDE_KERNEL_H_
#define KERNELS_34_GB_SELECT_PHASE2_ENTRY_INCLUDE_KERNEL_H_

#include <cstddef>
#include <cstdint>
#include <cstring>
#ifdef __cplusplus
#define restrict __restrict
#endif

/*
 * Source/builtin/include/GB_opaque.h:608-611
 */
#define GBh(Ah, k) ((Ah##32) ? Ah##32 [k] : ((Ah##64) ? Ah##64 [k] : (k)))

/* Source/builtin/include/GB_opaque.h:594-603 */
#define GB_MDECL(I, const, u)                                                  \
    const void *I = NULL;                                                      \
    const u##int32_t *restrict I##32 = NULL;                                   \
    const u##int64_t *restrict I##64 = NULL
#define GB_IPTR(I, is_32)                                                      \
    I##32 = (is_32) ? (decltype(I##32))I : NULL;                               \
    I##64 = (is_32) ? NULL : (decltype(I##64))I

/* Source/builtin/include/GB_opaque.h:19 */
typedef unsigned char GB_void;

/* Source/builtin/include/GB_opaque.h:361-371; standalone type view. */
struct GB_Type_opaque {
    uint64_t size;
};
/* Source/builtin/include/GB_Matrix_content.h:34-49 */
/* Source/builtin/include/GB_Matrix_content.h:215-232 */
/* Source/builtin/include/GB_Matrix_content.h:554-566 */
/* Standalone view of accessed fields, not the upstream GraphBLAS ABI.
 * GB_select_phase2_entry constructs these views from its array arguments. */
struct GB_Matrix_opaque {
    GB_Type_opaque *type;
    void *p;
    void *h;
    void *i;
    int8_t *b;
    GB_void *x;
    int64_t vlen;
    int64_t vdim;
    bool p_is_32;
    bool j_is_32;
    bool i_is_32;
    bool iso;
};
/* Include/GraphBLAS.h:462 */
using GrB_Matrix = GB_Matrix_opaque *;

/* Source/builtin/include/GB_opaque.h:540-552 */
#define GB_IS_BITMAP(A) ((A) != NULL && ((A)->b != NULL))
#define GB_IS_FULL(A)                                                          \
    ((A) != NULL && (A)->h == NULL && (A)->p == NULL && (A)->i == NULL &&      \
     (A)->b == NULL)

/* Include/GraphBLAS.h:373-376; only the result emitted by this caller */
enum GrB_Info { GrB_SUCCESS = 0 };

/* Source/builtin/include/GB_opaque.h:613-971 */
#ifndef GB_JIT_KERNEL
#define GB_IGET(I, k) (I##32 ? I##32 [k] : I##64 [k])
#define GB_ISET(I, k, i)                                                       \
    {                                                                          \
        if (I##64) {                                                           \
            I##64 [k] = (i);                                                   \
        } else {                                                               \
            I##32 [k] = (i);                                                   \
        }                                                                      \
    }
#define GB_IINC(I, k, i)                                                       \
    {                                                                          \
        if (I##64) {                                                           \
            I##64 [k] += (i);                                                  \
        } else {                                                               \
            I##32 [k] += (i);                                                  \
        }                                                                      \
    }
#define GB_IADDR(I, k) (I##32 ? ((void *)(I##32 + k)) : ((void *)(I##64 + k)))
#define GB_IDECL(I, const, u)                                                  \
    const u##int32_t *restrict I##32 = NULL;                                   \
    const u##int64_t *restrict I##64 = NULL
#define GB_GET_MATRIX_PTR(I, A, is_32, component)                              \
    I = (A) ? A->component : NULL;                                             \
    I##32 = (A) ? (A->is_32 ? (decltype(I##32))I : NULL) : NULL;               \
    I##64 = (A) ? (A->is_32 ? NULL : (decltype(I##64))I) : NULL
#define GB_GET_HYPER_PTR(I, A, pix)                                            \
    I = (A && A->Y) ? A->Y->pix : NULL;                                        \
    I##32 = (A && A->Y) ? (A->j_is_32 ? A->Y->pix : NULL) : NULL;              \
    I##64 = (A && A->Y) ? (A->j_is_32 ? NULL : A->Y->pix) : NULL
#define GB_GET_PENDINGi_PTR(I, A)                                              \
    I = A->Pending->i;                                                         \
    I##32 = (A->i_is_32 ? A->Pending->i : NULL);                               \
    I##64 = (A->i_is_32 ? NULL : A->Pending->i)
#define GB_GET_PENDINGj_PTR(I, A)                                              \
    I = A->Pending->j;                                                         \
    I##32 = (A->j_is_32 ? A->Pending->j : NULL);                               \
    I##64 = (A->j_is_32 ? NULL : A->Pending->j)
#define GBp(Ap, k, vlen)                                                       \
    ((Ap##32) ? Ap##32 [k] : ((Ap##64) ? Ap##64 [k] : ((k) * (vlen))))
#define GBi(Ai, p, vlen)                                                       \
    ((Ai##32) ? Ai##32 [p] : ((Ai##64) ? Ai##64 [p] : ((p) % (vlen))))
#define GBb(Ab, p) ((Ab) ? Ab[p] : 1)
#define GB_Cp_DECLARE(Cp, const) GB_MDECL(Cp, const, u)
#define GB_Ch_DECLARE(Ch, const) GB_MDECL(Ch, const, u)
#define GB_Ci_DECLARE(Ci, const) GB_MDECL(Ci, const, )
#define GB_Ci_DECLARE_U(Ci, const) GB_MDECL(Ci, const, u)
#define GB_CPendingi_DECLARE(Pending_i) GB_MDECL(Pending_i, , u)
#define GB_CPendingj_DECLARE(Pending_j) GB_MDECL(Pending_j, , u)
#define GB_Mp_DECLARE(Mp, const) GB_MDECL(Mp, const, u)
#define GB_Mh_DECLARE(Mh, const) GB_MDECL(Mh, const, u)
#define GB_Mi_DECLARE(Mi, const) GB_MDECL(Mi, const, )
#define GB_Mi_DECLARE_U(Mi, const) GB_MDECL(Mi, const, u)
#define GB_Ap_DECLARE(Ap, const) GB_MDECL(Ap, const, u)
#define GB_Ah_DECLARE(Ah, const) GB_MDECL(Ah, const, u)
#define GB_Ai_DECLARE(Ai, const) GB_MDECL(Ai, const, )
#define GB_Ai_DECLARE_U(Ai, const) GB_MDECL(Ai, const, u)
#define GB_Bp_DECLARE(Bp, const) GB_MDECL(Bp, const, u)
#define GB_Bh_DECLARE(Bh, const) GB_MDECL(Bh, const, u)
#define GB_Bi_DECLARE(Bi, const) GB_MDECL(Bi, const, )
#define GB_Bi_DECLARE_U(Bi, const) GB_MDECL(Bi, const, u)
#define GB_Sp_DECLARE(Sp, const) GB_MDECL(Sp, const, u)
#define GB_Sh_DECLARE(Sh, const) GB_MDECL(Sh, const, u)
#define GB_Si_DECLARE(Si, const) GB_MDECL(Si, const, )
#define GB_Si_DECLARE_U(Si, const) GB_MDECL(Si, const, u)
#define GB_Rp_DECLARE(Rp, const) GB_MDECL(Rp, const, u)
#define GB_Rh_DECLARE(Rh, const) GB_MDECL(Rh, const, u)
#define GB_Ri_DECLARE(Ri, const) GB_MDECL(Ri, const, )
#define GB_Ri_DECLARE_U(Ri, const) GB_MDECL(Ri, const, u)
#define GB_Zp_DECLARE(Zp, const) GB_MDECL(Zp, const, u)
#define GB_Zh_DECLARE(Zh, const) GB_MDECL(Zh, const, u)
#define GB_Zi_DECLARE(Zi, const) GB_MDECL(Zi, const, )
#define GB_Zi_DECLARE_U(Zi, const) GB_MDECL(Zi, const, u)
#define GB_Cp_PTR(Cp, C) GB_GET_MATRIX_PTR(Cp, C, p_is_32, p)
#define GB_Ch_PTR(Ch, C) GB_GET_MATRIX_PTR(Ch, C, j_is_32, h)
#define GB_Ci_PTR(Ci, C) GB_GET_MATRIX_PTR(Ci, C, i_is_32, i)
#define GB_CPendingi_PTR(Pending_i, C) GB_GET_PENDINGi_PTR(Pending_i, C)
#define GB_CPendingj_PTR(Pending_j, C) GB_GET_PENDINGj_PTR(Pending_j, C)
#define GB_Mp_PTR(Mp, M) GB_GET_MATRIX_PTR(Mp, M, p_is_32, p)
#define GB_Mh_PTR(Mh, M) GB_GET_MATRIX_PTR(Mh, M, j_is_32, h)
#define GB_Mi_PTR(Mi, M) GB_GET_MATRIX_PTR(Mi, M, i_is_32, i)
#define GB_Ap_PTR(Ap, A) GB_GET_MATRIX_PTR(Ap, A, p_is_32, p)
#define GB_Ah_PTR(Ah, A) GB_GET_MATRIX_PTR(Ah, A, j_is_32, h)
#define GB_Ai_PTR(Ai, A) GB_GET_MATRIX_PTR(Ai, A, i_is_32, i)
#define GB_Bp_PTR(Bp, B) GB_GET_MATRIX_PTR(Bp, B, p_is_32, p)
#define GB_Bh_PTR(Bh, B) GB_GET_MATRIX_PTR(Bh, B, j_is_32, h)
#define GB_Bi_PTR(Bi, B) GB_GET_MATRIX_PTR(Bi, B, i_is_32, i)
#define GB_Sp_PTR(Sp, S) GB_GET_MATRIX_PTR(Sp, S, p_is_32, p)
#define GB_Sh_PTR(Sh, S) GB_GET_MATRIX_PTR(Sh, S, j_is_32, h)
#define GB_Si_PTR(Si, S) GB_GET_MATRIX_PTR(Si, S, i_is_32, i)
#define GB_Rp_PTR(Rp, R) GB_GET_MATRIX_PTR(Rp, R, p_is_32, p)
#define GB_Rh_PTR(Rh, R) GB_GET_MATRIX_PTR(Rh, R, j_is_32, h)
#define GB_Ri_PTR(Ri, R) GB_GET_MATRIX_PTR(Ri, R, i_is_32, i)
#define GB_Zp_PTR(Zp, Z) GB_GET_MATRIX_PTR(Zp, Z, p_is_32, p)
#define GB_Zh_PTR(Zh, Z) GB_GET_MATRIX_PTR(Zh, Z, j_is_32, h)
#define GB_Zi_PTR(Zi, Z) GB_GET_MATRIX_PTR(Zi, Z, i_is_32, i)
#define GBp_C(Cp, k, vlen) GBp(Cp, k, vlen)
#define GBh_C(Ch, k) GBh(Ch, k)
#define GBi_C(Ci, p, vlen) GBi(Ci, p, vlen)
#define GBb_C(Cb, p) GBb(Cb, p)
#define GB_C_NVALS(e) int64_t e = GB_nnz(C)
#define GB_C_NHELD(e) int64_t e = GB_nnz_held(C)
#define GBp_M(Mp, k, vlen) GBp(Mp, k, vlen)
#define GBh_M(Mh, k) GBh(Mh, k)
#define GBi_M(Mi, p, vlen) GBi(Mi, p, vlen)
#define GBb_M(Mb, p) GBb(Mb, p)
#define GB_M_NVALS(e) int64_t e = GB_nnz(M)
#define GB_M_NHELD(e) int64_t e = GB_nnz_held(M)
#define GBp_A(Ap, k, vlen) GBp(Ap, k, vlen)
#define GBh_A(Ah, k) GBh(Ah, k)
#define GBi_A(Ai, p, vlen) GBi(Ai, p, vlen)
#define GBb_A(Ab, p) GBb(Ab, p)
#define GB_A_NVALS(e) int64_t e = GB_nnz(A)
#define GB_A_NHELD(e) int64_t e = GB_nnz_held(A)
#define GBp_B(Bp, k, vlen) GBp(Bp, k, vlen)
#define GBh_B(Bh, k) GBh(Bh, k)
#define GBi_B(Bi, p, vlen) GBi(Bi, p, vlen)
#define GBb_B(Bb, p) GBb(Bb, p)
#define GB_B_NVALS(e) int64_t e = GB_nnz(B)
#define GB_B_NHELD(e) int64_t e = GB_nnz_held(B)
#define GBp_S(Sp, k, vlen) GBp(Sp, k, vlen)
#define GBh_S(Sh, k) GBh(Sh, k)
#define GBi_S(Si, p, vlen) GBi(Si, p, vlen)
#define GBb_S(Sb, p) GBb(Sb, p)
#define GB_S_NVALS(e) int64_t e = GB_nnz(S)
#define GB_S_NHELD(e) int64_t e = GB_nnz_held(S)
#define GBp_R(Rp, k, vlen) GBp(Rp, k, vlen)
#define GBh_R(Rh, k) GBh(Rh, k)
#define GBi_R(Ri, p, vlen) GBi(Ri, p, vlen)
#define GBb_R(Rb, p) GBb(Rb, p)
#define GB_R_NVALS(e) int64_t e = GB_nnz(R)
#define GB_R_NHELD(e) int64_t e = GB_nnz_held(R)
#define GBp_Z(Zp, k, vlen) GBp(Zp, k, vlen)
#define GBh_Z(Zh, k) GBh(Zh, k)
#define GBi_Z(Zi, p, vlen) GBi(Zi, p, vlen)
#define GBb_Z(Zb, p) GBb(Zb, p)
#define GB_Z_NVALS(e) int64_t e = GB_nnz(Z)
#define GB_Z_NHELD(e) int64_t e = GB_nnz_held(Z)
#else
#define GB_IGET(I, k) I[k]
#define GB_ISET(I, k, i) I[k] = (i)
#define GB_IINC(I, k, i) I[k] += (i)
#ifdef GB_CUDA_KERNEL
#define GB_JDECL(I, const, u, bits)                                            \
    const GB_EVAL4(u, int, bits, _t) *__restrict__ I = NULL
#else
#define GB_JDECL(I, const, u, bits)                                            \
    const GB_EVAL4(u, int, bits, _t) *restrict I = NULL
#endif
#define GB_GET_MATRIX_PTR(I, A, component) I = (A) ? (A->component) : NULL
#define GB_GET_HYPER_PTR(I, A, component)                                      \
    I = (A && A->Y) ? (A->Y->component) : NULL
#define GB_Cp_DECLARE(Cp, const) GB_JDECL(Cp, const, u, GB_Cp_BITS)
#define GB_Ch_DECLARE(Ch, const) GB_JDECL(Ch, const, u, GB_Cj_BITS)
#define GB_Ci_DECLARE(Ci, const) GB_JDECL(Ci, const, , GB_Ci_BITS)
#define GB_Ci_DECLARE_U(Ci, const) GB_JDECL(Ci, const, u, GB_Ci_BITS)
#define GB_CPendingi_DECLARE(Pending_i) GB_JDECL(Pending_i, , u, GB_Ci_BITS)
#define GB_CPendingj_DECLARE(Pending_j) GB_JDECL(Pending_j, , u, GB_Cj_BITS)
#define GB_Cp_IS_32 (GB_Cp_BITS == 32)
#define GB_Cj_IS_32 (GB_Cj_BITS == 32)
#define GB_Ci_IS_32 (GB_Ci_BITS == 32)
#define GB_Mp_DECLARE(Mp, const) GB_JDECL(Mp, const, u, GB_Mp_BITS)
#define GB_Mh_DECLARE(Mh, const) GB_JDECL(Mh, const, u, GB_Mj_BITS)
#define GB_Mi_DECLARE(Mi, const) GB_JDECL(Mi, const, , GB_Mi_BITS)
#define GB_Mi_DECLARE_U(Mi, const) GB_JDECL(Mi, const, u, GB_Mi_BITS)
#define GB_Mp_IS_32 (GB_Mp_BITS == 32)
#define GB_Mj_IS_32 (GB_Mj_BITS == 32)
#define GB_Mi_IS_32 (GB_Mi_BITS == 32)
#define GB_Ap_DECLARE(Ap, const) GB_JDECL(Ap, const, u, GB_Ap_BITS)
#define GB_Ah_DECLARE(Ah, const) GB_JDECL(Ah, const, u, GB_Aj_BITS)
#define GB_Ai_DECLARE(Ai, const) GB_JDECL(Ai, const, , GB_Ai_BITS)
#define GB_Ai_DECLARE_U(Ai, const) GB_JDECL(Ai, const, u, GB_Ai_BITS)
#define GB_Ap_IS_32 (GB_Ap_BITS == 32)
#define GB_Aj_IS_32 (GB_Aj_BITS == 32)
#define GB_Ai_IS_32 (GB_Ai_BITS == 32)
#define GB_Bp_DECLARE(Bp, const) GB_JDECL(Bp, const, u, GB_Bp_BITS)
#define GB_Bh_DECLARE(Bh, const) GB_JDECL(Bh, const, u, GB_Bj_BITS)
#define GB_Bi_DECLARE(Bi, const) GB_JDECL(Bi, const, , GB_Bi_BITS)
#define GB_Bi_DECLARE_U(Bi, const) GB_JDECL(Bi, const, u, GB_Bi_BITS)
#define GB_Bp_IS_32 (GB_Bp_BITS == 32)
#define GB_Bj_IS_32 (GB_Bj_BITS == 32)
#define GB_Bi_IS_32 (GB_Bi_BITS == 32)
#define GB_Sp_DECLARE(Sp, const) GB_JDECL(Sp, const, u, GB_Sp_BITS)
#define GB_Sh_DECLARE(Sh, const) GB_JDECL(Sh, const, u, GB_Sj_BITS)
#define GB_Si_DECLARE(Si, const) GB_JDECL(Si, const, , GB_Si_BITS)
#define GB_Si_DECLARE_U(Si, const) GB_JDECL(Si, const, u, GB_Si_BITS)
#define GB_Sp_IS_32 (GB_Sp_BITS == 32)
#define GB_Sj_IS_32 (GB_Sj_BITS == 32)
#define GB_Si_IS_32 (GB_Si_BITS == 32)
#define GB_Rp_DECLARE(Rp, const) GB_JDECL(Rp, const, u, GB_Rp_BITS)
#define GB_Rh_DECLARE(Rh, const) GB_JDECL(Rh, const, u, GB_Rj_BITS)
#define GB_Ri_DECLARE(Ri, const) GB_JDECL(Ri, const, , GB_Ri_BITS)
#define GB_Ri_DECLARE_U(Ri, const) GB_JDECL(Ri, const, u, GB_Ri_BITS)
#define GB_Rp_IS_32 (GB_Rp_BITS == 32)
#define GB_Rj_IS_32 (GB_Rj_BITS == 32)
#define GB_Ri_IS_32 (GB_Ri_BITS == 32)
#define GB_Zp_DECLARE(Zp, const) GB_JDECL(Zp, const, u, GB_Zp_BITS)
#define GB_Zh_DECLARE(Zh, const) GB_JDECL(Zh, const, u, GB_Zj_BITS)
#define GB_Zi_DECLARE(Zi, const) GB_JDECL(Zi, const, , GB_Zi_BITS)
#define GB_Zi_DECLARE_U(Zi, const) GB_JDECL(Zi, const, u, GB_Zi_BITS)
#define GB_Zp_IS_32 (GB_Zp_BITS == 32)
#define GB_Zj_IS_32 (GB_Zj_BITS == 32)
#define GB_Zi_IS_32 (GB_Zi_BITS == 32)
#define GB_Cp_PTR(Cp, C) GB_GET_MATRIX_PTR(Cp, C, p)
#define GB_Ch_PTR(Ch, C) GB_GET_MATRIX_PTR(Ch, C, h)
#define GB_Ci_PTR(Ci, C) GB_GET_MATRIX_PTR(Ci, C, i)
#define GB_CPendingi_PTR(Pending_i, C) Pending_i = C->Pending->i
#define GB_CPendingj_PTR(Pending_j, C) Pending_j = C->Pending->j
#define GB_Mp_PTR(Mp, M) GB_GET_MATRIX_PTR(Mp, M, p)
#define GB_Mh_PTR(Mh, M) GB_GET_MATRIX_PTR(Mh, M, h)
#define GB_Mi_PTR(Mi, M) GB_GET_MATRIX_PTR(Mi, M, i)
#define GB_Ap_PTR(Ap, A) GB_GET_MATRIX_PTR(Ap, A, p)
#define GB_Ah_PTR(Ah, A) GB_GET_MATRIX_PTR(Ah, A, h)
#define GB_Ai_PTR(Ai, A) GB_GET_MATRIX_PTR(Ai, A, i)
#define GB_Bp_PTR(Bp, B) GB_GET_MATRIX_PTR(Bp, B, p)
#define GB_Bh_PTR(Bh, B) GB_GET_MATRIX_PTR(Bh, B, h)
#define GB_Bi_PTR(Bi, B) GB_GET_MATRIX_PTR(Bi, B, i)
#define GB_Sp_PTR(Sp, S) GB_GET_MATRIX_PTR(Sp, S, p)
#define GB_Sh_PTR(Sh, S) GB_GET_MATRIX_PTR(Sh, S, h)
#define GB_Si_PTR(Si, S) GB_GET_MATRIX_PTR(Si, S, i)
#define GB_Rp_PTR(Rp, R) GB_GET_MATRIX_PTR(Rp, R, p)
#define GB_Rh_PTR(Rh, R) GB_GET_MATRIX_PTR(Rh, R, h)
#define GB_Ri_PTR(Ri, R) GB_GET_MATRIX_PTR(Ri, R, i)
#define GB_Zp_PTR(Zp, Z) GB_GET_MATRIX_PTR(Zp, Z, p)
#define GB_Zh_PTR(Zh, Z) GB_GET_MATRIX_PTR(Zh, Z, h)
#define GB_Zi_PTR(Zi, Z) GB_GET_MATRIX_PTR(Zi, Z, i)
#endif

/* Source/include/GB_abort.h:24-35 */
void GB_abort(const char *file, int line);
#define GB_assert(X)                                                           \
    {                                                                          \
        if (!(X)) {                                                            \
            GB_abort(__FILE__, __LINE__);                                      \
        }                                                                      \
    }

/* Source/include/GB_abort.h:37-43 */
#ifdef GB_DEBUG
#define ASSERT(X) GB_assert(X)
#else
#define ASSERT(X)
#endif

/* FactoryKernels/GB_sel__gt_thunk_fp64.c:16-17 */
#define GB_TEST_VALUE_OF_ENTRY(keep, p) bool keep = (Ax[p] > y)
#define GB_SELECT_ENTRY(Cx, pC, Ax, pA) Cx[pC] = Ax[pA]

/*
 * FactoryKernels/GB_sel__gt_thunk_fp64.c:13-15
 */
#define GB_A_TYPE double
#define GB_Y_TYPE double

/*
 * Source/math/include/GB_math_macros.h:37-38
 */
#define GB_IMAX(x, y) (((x) > (y)) ? (x) : (y))
#define GB_IMIN(x, y) (((x) < (y)) ? (x) : (y))

/*
 * Source/slice/include/GB_ek_slice_kernels.h:104-126
 */
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

/* FactoryKernels/GB_sel__gt_thunk_fp64.c:46-60 */
GrB_Info GB_sel_phase2__gt_thunk_fp64(GrB_Matrix C,
                                      const uint64_t *restrict Cp_kfirst,
                                      const GrB_Matrix A,
                                      const GB_void *restrict ythunk,
                                      const int64_t *A_ek_slicing,
                                      const int A_ntasks, const int A_nthreads);

/*
 * Wrapper for invoking the extracted entry selector with sparse FP64 arrays.
 */
GrB_Info GB_select_phase2_entry(const uint64_t *Ap, const int64_t *Ai,
                                const double *Ax, const uint64_t *Cp,
                                int64_t *Ci, double *Cx, int64_t vlen,
                                const int64_t *A_ek_slicing,
                                const uint64_t *Cp_kfirst, double threshold,
                                int A_ntasks, int A_nthreads);

GrB_Info GB_select_phase2_entry_rvv(const uint64_t *Ap, const int64_t *Ai,
                                    const double *Ax, const uint64_t *Cp,
                                    int64_t *Ci, double *Cx, int64_t vlen,
                                    const int64_t *A_ek_slicing,
                                    const uint64_t *Cp_kfirst, double threshold,
                                    int A_ntasks, int A_nthreads);

#endif // KERNELS_34_GB_SELECT_PHASE2_ENTRY_INCLUDE_KERNEL_H_
