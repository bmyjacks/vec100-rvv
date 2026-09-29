/****************************************************************************
 *
 *
 *  Project: SuiteSparse:GraphBLAS 10.5.1
 *  Source files:
 *    Source/global/GB_Global.h
 *    Include/GraphBLAS.h
 *    Source/include/GB_defaults.h
 *    Source/gateway/GB_cuda_gateway.h
 *    Source/omp/include/GB_omp_kernels.h
 *    Source/global/GB_Global.c
 *    Source/print/GB_printf.h
 *    Source/ok/GB_abort.c
 *    Source/select/template/GB_select_phase2_template.c
 *    FactoryKernels/GB_sel__gt_thunk_fp64.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * Source/global/GB_Global.h
 *
 *   GB_Global.h: definitions for global data
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
 * Source/include/GB_defaults.h
 *
 *   GB_defaults.h: default parameter settings
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/gateway/GB_cuda_gateway.h
 *
 *   GB_cuda_gateway.h: definitions for interface to GB_cuda_* functions
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/omp/include/GB_omp_kernels.h
 *
 *   GB_omp_kernels.h: definitions using OpenMP in SuiteSparse:GraphBLAS
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/global/GB_Global.c
 *
 *   GB_Global: global values in GraphBLAS
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/print/GB_printf.h
 *
 *   GB_printf.h: definitions for printing from GraphBLAS
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/ok/GB_abort.c
 *
 *   GB_abort.c: hard assertions for all of GraphBLAS, including JIT kernels
 *
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Source/select/template/GB_select_phase2_template.c
 *
 *   GB_select_phase2: C=select(A,thunk)
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
 */

#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#ifdef _OPENMP
#include <omp.h>
#endif

/* Source/global/GB_Global.h:17-20 */
using GB_malloc_function_t = void *(*)(size_t);
using GB_calloc_function_t = void *(*)(size_t, size_t);
using GB_realloc_function_t = void *(*)(void *, size_t);
using GB_free_function_t = void (*)(void *);

/* Source/global/GB_Global.h:122-129 */
using GB_printf_function_t = int (*)(const char *restrict format, ...);
using GB_flush_function_t = int (*)(void);

/* Include/GraphBLAS.h:436 */
#define GrB_NONBLOCKING 0

/* Include/GraphBLAS.h:1683 */
#define GxB_NBITMAP_SWITCH 8

/* Include/GraphBLAS.h:2988 */
#define GxB_NARENAS 8

/* Source/include/GB_defaults.h:20-21 */
#define GB_HYPER_SWITCH_DEFAULT (0.0625)
#define GB_HYPER_HASH_DEFAULT (1024)

/* Source/gateway/GB_cuda_gateway.h:26 */
#define GB_CUDA_MAX_GPUS 32

/* Source/gateway/GB_cuda_gateway.h:36-49 */
typedef struct {
    char name[256];
    size_t total_global_memory;
    int number_of_sms;
    int compute_capability_major;
    int compute_capability_minor;
    bool use_memory_pool;
    size_t pool_memsize;
    size_t max_pool_memsize;
    void *memory_resource;
} GB_cuda_device;

/* Source/omp/include/GB_omp_kernels.h:43-50 */
#if defined(_OPENMP)
#define GB_OPENMP_LOCK_T omp_lock_t
#else
#define GB_OPENMP_LOCK_T int
#endif

/* Source/global/GB_Global.c:31-257 */
typedef struct {
    int mode;
    bool init_called;
    float bitmap_switch[GxB_NBITMAP_SWITCH];
    float hyper_switch;
    bool is_csc;
    int64_t hyper_hash;
    void (*abort_function)(void);
    GB_malloc_function_t malloc_function[GxB_NARENAS];
    GB_calloc_function_t calloc_function[GxB_NARENAS];
    GB_realloc_function_t realloc_function[GxB_NARENAS];
    GB_free_function_t free_function[GxB_NARENAS];
    bool malloc_tracking;
    int64_t nmalloc;
    bool malloc_debug;
    int64_t malloc_debug_count;
    int64_t hack[8];
    bool burble;
    GB_printf_function_t printf_func;
    GB_flush_function_t flush_func;
    bool print_one_based;
    bool stats_mem_shallow;
    double timing[40];
#ifdef GB_MEMTABLE_DEBUG
#define GB_MEMTABLE_SIZE 10000
    GB_void *memtable_p[GB_MEMTABLE_SIZE];
    uint64_t memtable_memsize[GB_MEMTABLE_SIZE];
    int memtable_arena[GB_MEMTABLE_SIZE];
#endif
    int nmemtable;
    bool cpu_features_avx2;
    bool cpu_features_avx512f;
    bool cpu_features_rvv_1_0;
    int8_t p_control;
    int8_t j_control;
    int8_t i_control;
    int gpu_count;
    GB_cuda_device gpu_properties[GB_CUDA_MAX_GPUS];
#define GB_GLOBAL_NLOCKS 8
    GB_OPENMP_LOCK_T lock[GB_GLOBAL_NLOCKS];
    bool lock_is_created[GB_GLOBAL_NLOCKS];
} GB_Global_struct;

/* Source/global/GB_Global.c:31-257 */
#define GB_BITMAP_SWITCH_1 ((float)0.04)
#define GB_BITMAP_SWITCH_2 ((float)0.05)
#define GB_BITMAP_SWITCH_3_to_4 ((float)0.06)
#define GB_BITMAP_SWITCH_5_to_8 ((float)0.08)
#define GB_BITMAP_SWITCH_9_to_16 ((float)0.10)
#define GB_BITMAP_SWITCH_17_to_32 ((float)0.20)
#define GB_BITMAP_SWITCH_33_to_64 ((float)0.30)
#define GB_BITMAP_SWITCH_gt_than_64 ((float)0.40)

/* Source/global/GB_Global.c:31-257 */
static GB_Global_struct GB_Global = {
    GrB_NONBLOCKING,
    false,
    {GB_BITMAP_SWITCH_1, GB_BITMAP_SWITCH_2, GB_BITMAP_SWITCH_3_to_4,
     GB_BITMAP_SWITCH_5_to_8, GB_BITMAP_SWITCH_9_to_16,
     GB_BITMAP_SWITCH_17_to_32, GB_BITMAP_SWITCH_33_to_64,
     GB_BITMAP_SWITCH_gt_than_64},
    GB_HYPER_SWITCH_DEFAULT,
    false,
    GB_HYPER_HASH_DEFAULT,
    std::abort,
    {std::malloc, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {std::calloc, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {std::realloc, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {std::free, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    false,
    0,
    false,
    0,
    {0, 0, 0, 0, 0, 0, 0, 0},
    false,
    NULL,
    NULL,
    false,
    false,
    {0},
#ifdef GB_MEMTABLE_DEBUG
    {},
    {},
    {},
#endif
    0,
    false,
    false,
    false,
    (int8_t)32,
    (int8_t)32,
    (int8_t)32,
    0,
    {},
    {},
    {0, 0, 0, 0, 0, 0, 0, 0}};

/* Source/global/GB_Global.c:529-537 */
void GB_Global_abort_set(void (*abort_function)(void)) {
    GB_Global.abort_function = abort_function;
}

void GB_Global_abort(void) { GB_Global.abort_function(); }

/* Source/global/GB_Global.c:1020-1038 */
GB_printf_function_t GB_Global_printf_get(void) {
    return GB_Global.printf_func;
}

GB_flush_function_t GB_Global_flush_get(void) { return GB_Global.flush_func; }

void GB_Global_printf_set(GB_printf_function_t pr_func) {
    GB_Global.printf_func = pr_func;
}

void GB_Global_flush_set(GB_flush_function_t fl_func) {
    GB_Global.flush_func = fl_func;
}

/* Source/print/GB_printf.h:13-42 */
#define GBDUMP(...)                                                            \
    {                                                                          \
        GB_printf_function_t printf_func = GB_Global_printf_get();             \
        if (printf_func != NULL) {                                             \
            printf_func(__VA_ARGS__);                                          \
        } else {                                                               \
            printf(__VA_ARGS__); /* printf_func is NULL; use libc */           \
        }                                                                      \
        GB_flush_function_t flush_func = GB_Global_flush_get();                \
        if (flush_func != NULL) {                                              \
            flush_func();                                                      \
        } else {                                                               \
            fflush(stdout); /* flush_func is NULL; use libc */                 \
        }                                                                      \
    }

/* Source/ok/GB_abort.c:10-22 */
void GB_abort(const char *file, int line) {
    GBDUMP("\nGraphBLAS assertion failed: [ %s ]: line %d\n", file, line);
    GB_Global_abort();
}

/* FactoryKernels/GB_sel__gt_thunk_fp64.c:46-58 */
GrB_Info
GB_sel_phase2__gt_thunk_fp64(GrB_Matrix C, const uint64_t *restrict Cp_kfirst,
                             const GrB_Matrix A, const GB_void *restrict ythunk,
                             const int64_t *A_ek_slicing, const int A_ntasks,
                             const int A_nthreads) {
    GB_Y_TYPE y = *((GB_Y_TYPE *)ythunk);
    /* Source/select/template/GB_select_phase2_template.c:14-53 */
    {
        const int64_t *restrict kfirst_Aslice = A_ek_slicing;
        const int64_t *restrict klast_Aslice = A_ek_slicing + A_ntasks;
        const int64_t *restrict pstart_Aslice = A_ek_slicing + A_ntasks * 2;

        GB_Ap_DECLARE(Ap, const);
        GB_Ap_PTR(Ap, A);
        GB_Ah_DECLARE(Ah, const);
        GB_Ah_PTR(Ah, A);
        GB_Ai_DECLARE(Ai, const);
        GB_Ai_PTR(Ai, A);

        const GB_A_TYPE *restrict Ax = (GB_A_TYPE *)A->x;
        size_t asize = A->type->size;
        int64_t avlen = A->vlen;
        int64_t avdim = A->vdim;
        ASSERT(!GB_IS_BITMAP(A));
        ASSERT(!GB_IS_FULL(A));
        GB_Cp_DECLARE(Cp, const);
        GB_Cp_PTR(Cp, C);
        GB_Ci_DECLARE(Ci, );
        GB_Ci_PTR(Ci, C);
        GB_A_TYPE *restrict Cx = (GB_A_TYPE *)C->x;

        /* Source/select/template/GB_select_phase2_template.c:54-99 */
        int tid;
#pragma omp parallel for num_threads(A_nthreads) schedule(dynamic, 1)
        for (tid = 0; tid < A_ntasks; tid++) {
            int64_t kfirst = kfirst_Aslice[tid];
            int64_t klast = klast_Aslice[tid];
            for (int64_t k = kfirst; k <= klast; k++) {
                GB_GET_PA_AND_PC(pA_start, pA_end, pC, tid, k, kfirst, klast,
                                 pstart_Aslice, Cp_kfirst, GBp_A(Ap, k, avlen),
                                 GBp_A(Ap, k + 1, avlen), GB_IGET(Cp, k));
                int64_t j = GBh_A(Ah, k);
                for (int64_t pA = pA_start; pA < pA_end; pA++) {
                    ASSERT(Ai != NULL);
                    int64_t i = GB_IGET(Ai, pA);
                    GB_TEST_VALUE_OF_ENTRY(keep, pA);
                    if (keep) {
                        ASSERT(pC >= GB_IGET(Cp, k));
                        ASSERT(pC < GB_IGET(Cp, k + 1));
                        GB_ISET(Ci, pC, i);
                        GB_SELECT_ENTRY(Cx, pC, Ax, pA);
                        pC++;
                    }
                }
            }
        }
    }
    /* FactoryKernels/GB_sel__gt_thunk_fp64.c:59 */
    return (GrB_SUCCESS);
}

/*
 * Wrapper for invoking the extracted entry selector with sparse FP64 arrays.
 */
GrB_Info GB_select_phase2_entry(const uint64_t *Ap, const int64_t *Ai,
                                const double *Ax, const uint64_t *Cp,
                                int64_t *Ci, double *Cx, int64_t vlen,
                                const int64_t *A_ek_slicing,
                                const uint64_t *Cp_kfirst, double threshold,
                                int A_ntasks, int A_nthreads) {
    GB_Type_opaque type{sizeof(double)};
    GB_Matrix_opaque A{};
    A.type = &type;
    A.p = const_cast<uint64_t *>(Ap);
    A.i = const_cast<int64_t *>(Ai);
    A.x = reinterpret_cast<GB_void *>(const_cast<double *>(Ax));
    A.vlen = vlen;
    A.vdim = 1;
    GB_Matrix_opaque C{};
    C.type = &type;
    C.p = const_cast<uint64_t *>(Cp);
    C.i = Ci;
    C.x = reinterpret_cast<GB_void *>(Cx);
    C.vlen = vlen;
    C.vdim = 1;
    return GB_sel_phase2__gt_thunk_fp64(
        &C, Cp_kfirst, &A, reinterpret_cast<const GB_void *>(&threshold),
        A_ek_slicing, A_ntasks, A_nthreads);
}
