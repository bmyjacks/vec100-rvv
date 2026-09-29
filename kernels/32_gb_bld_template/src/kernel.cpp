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
 *    FactoryKernels/GB_bld__plus_fp64.c
 *    Source/builder/template/GB_bld_template.c
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
 * FactoryKernels/GB_bld__plus_fp64.c
 *
 *   GB_bld:  hard-coded functions for builder methods
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
 */

#include "kernel.h"

#include <cstdio>
#include <cstdlib>

/* Source/global/GB_Global.h:17-20 */
using GB_malloc_function_t = void *(*)(size_t);
using GB_calloc_function_t = void *(*)(size_t, size_t);
using GB_realloc_function_t = void *(*)(void *, size_t);
using GB_free_function_t = void (*)(void *);

/* Source/global/GB_Global.h:122-123 */
using GB_flush_function_t = int (*)(void);
using GB_printf_function_t = int (*)(const char *restrict format, ...);

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

/* Source/omp/include/GB_omp_kernels.h:43-50 (non-OpenMP selection) */
#define GB_OPENMP_LOCK_T int

/* Source/global/GB_Global.c:31-257: same field order and configuration
 * branches as upstream. C++17 aggregate initialization follows that order.
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0 */
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

#define GB_BITMAP_SWITCH_1 ((float)0.04)
#define GB_BITMAP_SWITCH_2 ((float)0.05)
#define GB_BITMAP_SWITCH_3_to_4 ((float)0.06)
#define GB_BITMAP_SWITCH_5_to_8 ((float)0.08)
#define GB_BITMAP_SWITCH_9_to_16 ((float)0.10)
#define GB_BITMAP_SWITCH_17_to_32 ((float)0.20)
#define GB_BITMAP_SWITCH_33_to_64 ((float)0.30)
#define GB_BITMAP_SWITCH_gt_than_64 ((float)0.40)

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

/* Source/global/GB_Global.c:529-537; abort callback accessors.
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0 */
void GB_Global_abort_set(void (*abort_function)(void)) {
    GB_Global.abort_function = abort_function;
}

void GB_Global_abort(void) { GB_Global.abort_function(); }

/* Source/global/GB_Global.c:1020-1038; diagnostic callback accessors.
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0 */
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

/* Source/print/GB_printf.h:13-42; GBDUMP with configurable print/flush.
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0 */
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

/* Source/ok/GB_abort.c:10-22; assertion failure reporting.
 * SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2025, All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0 */
void GB_abort(const char *file, int line) {
    GBDUMP("\nGraphBLAS assertion failed: [ %s ]: line %d\n", file, line);
    GB_Global_abort();
}

/*
 * FactoryKernels/GB_bld__plus_fp64.c:48-76
 * The factory's GB(_bld__plus_fp64) mangling resolves to GB_bld__plus_fp64.
 */
GrB_Info GB_bld__plus_fp64(double *restrict Tx, void *restrict Ti,
                           bool Ti_is_32, const double *restrict Sx,
                           int64_t nvals, int64_t ndupl,
                           const void *restrict I_work, bool I_is_32,
                           const void *restrict K_work, bool K_is_32,
                           const int64_t duplicate_entry,
                           const int64_t *restrict tstart_slice,
                           const int64_t *restrict tnz_slice, int nthreads) {
    GB_IDECL(I_work, const, u);
    GB_IPTR(I_work, I_is_32);
    GB_IDECL(K_work, const, u);
    GB_IPTR(K_work, K_is_32);
    GB_IDECL(Ti, , );
    GB_IPTR(Ti, Ti_is_32);
/*
 * FactoryKernels/GB_bld__plus_fp64.c:72
 */
#define GB_K_WORK(t) (K_work ? GB_IGET(K_work, t) : (t))
    /*
     * Source/builder/template/GB_bld_template.c:33-143
     */
    {
        if (GB_KNOWN_NO_DUPLICATES) {
            if (GB_K_IS_NULL) {
                int tid;
#pragma omp parallel for num_threads(nthreads) schedule(static)
                for (tid = 0; tid < nthreads; tid++) {
                    int64_t tstart = tstart_slice[tid];
                    int64_t tend = tstart_slice[tid + 1];
                    for (int64_t t = tstart; t < tend; t++) {
                        GB_BLD_COPY(Tx, t, Sx, t);
                    }
                }
            } else {
                int tid;
#pragma omp parallel for num_threads(nthreads) schedule(static)
                for (tid = 0; tid < nthreads; tid++) {
                    int64_t tstart = tstart_slice[tid];
                    int64_t tend = tstart_slice[tid + 1];
                    for (int64_t t = tstart; t < tend; t++) {
                        int64_t k = GB_IGET(K_work, t);
                        GB_BLD_COPY(Tx, t, Sx, k);
                    }
                }
            }
        } else {
            int tid;
#pragma omp parallel for num_threads(nthreads) schedule(static)
            for (tid = 0; tid < nthreads; tid++) {
                int64_t my_tnz = tnz_slice[tid];
                int64_t tstart = tstart_slice[tid];
                int64_t tend = tstart_slice[tid + 1];

                int64_t t;
                for (t = tstart; t < tend; t++) {
                    int64_t i = GB_IGET(I_work, t);
                    if (i != duplicate_entry)
                        break;
                }

                for (; t < tend; t++) {
                    int64_t i = GB_IGET(I_work, t);
                    ASSERT(i != duplicate_entry);
                    int64_t k = GB_K_WORK(t);
                    GB_BLD_COPY(Tx, my_tnz, Sx, k);
                    GB_ISET(Ti, my_tnz, i);
                    for (; t + 1 < nvals &&
                           GB_IGET(I_work, t + 1) == duplicate_entry;
                         t++) {
                        int64_t k = GB_K_WORK(t + 1);
                        GB_BLD_DUP(Tx, my_tnz, Sx, k);
                    }
                    my_tnz++;
                }
            }
        }
    }
    return GrB_SUCCESS;
}

/*
 * Wrapper for invoking the extracted kernel's no-duplicates branch.
 */
void GB_bld_template(const double *restrict Sx, double *restrict Tx,
                     const void *restrict K_work,
                     const uint32_t *restrict K_work32,
                     const uint64_t *restrict K_work64,
                     const int64_t *restrict tstart_slice, int nthreads) {
    const void *permutation =
        K_work ? K_work
               : (K_work32 ? static_cast<const void *>(K_work32)
                           : static_cast<const void *>(K_work64));
    GB_bld__plus_fp64(Tx, nullptr, false, Sx, tstart_slice[nthreads], 0,
                      nullptr, false, permutation, K_work32 != nullptr, 0,
                      tstart_slice, nullptr, nthreads);
}
