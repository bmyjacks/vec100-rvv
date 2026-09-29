#include "kernel.h"
#include <riscv_vector.h>

GrB_Info GB_bld__plus_fp64_rvv(double *Tx, void *Ti, bool Ti_is_32,
                                const double *Sx, int64_t nvals,
                                int64_t ndupl, const void *I_work,
                                bool I_is_32, const void *K_work,
                                bool K_is_32, int64_t duplicate_entry,
                                const int64_t *tstart, const int64_t *tnz,
                                int nthreads) {
    auto key = [&](const void *p, bool wide32, int64_t i) -> int64_t {
        return wide32 ? static_cast<const uint32_t *>(p)[i]
                      : static_cast<const uint64_t *>(p)[i];
    };
    if (ndupl != 0) {
        // Duplicate chains can cross task ends. Preserve each scalar update,
        // including FP addition order and the final partial output slots.
        for (int tid = 0; tid < nthreads; ++tid) {
            int64_t t = tstart[tid], end = tstart[tid + 1];
            int64_t out = tnz[tid];
            while (t < end && key(I_work, I_is_32, t) == duplicate_entry) ++t;
            for (; t < end; ++t) {
                int64_t i = key(I_work, I_is_32, t);
                int64_t k = K_work ? key(K_work, K_is_32, t) : t;
                Tx[out] = Sx[k];
                if (Ti_is_32) static_cast<uint32_t *>(Ti)[out] = i;
                else static_cast<uint64_t *>(Ti)[out] = i;
                for (; t + 1 < nvals &&
                       key(I_work, I_is_32, t + 1) == duplicate_entry; ++t)
                    Tx[out] += Sx[K_work ? key(K_work, K_is_32, t + 1) : t + 1];
                ++out;
            }
        }
        return GrB_SUCCESS;
    }
    for (int tid = 0; tid < nthreads; ++tid) {
        for (int64_t t = tstart[tid]; t < tstart[tid + 1];) {
            size_t vl = __riscv_vsetvl_e64m1(tstart[tid + 1] - t);
            vfloat64m1_t values;
            if (K_work) {
                vuint64m1_t indices = K_is_32
                    ? __riscv_vwcvtu_x_x_v_u64m1(
                          __riscv_vle32_v_u32mf2(
                              static_cast<const uint32_t *>(K_work) + t, vl), vl)
                    : __riscv_vle64_v_u64m1(
                          static_cast<const uint64_t *>(K_work) + t, vl);
                values = __riscv_vluxei64_v_f64m1(Sx,
                    __riscv_vsll_vx_u64m1(indices, 3, vl), vl);
            } else values = __riscv_vle64_v_f64m1(Sx + t, vl);
            __riscv_vse64_v_f64m1(Tx + t, values, vl);
            t += vl;
        }
    }
    return GrB_SUCCESS;
}

void GB_bld_template_rvv(const double *Sx, double *Tx,
                         const void *K_work, const uint32_t *K_work32,
                         const uint64_t *K_work64, const int64_t *tstart,
                         int nthreads) {
    const void *perm = K_work ? K_work : (K_work32 ?
        static_cast<const void *>(K_work32) : static_cast<const void *>(K_work64));
    GB_bld__plus_fp64_rvv(Tx, nullptr, false, Sx, tstart[nthreads], 0,
                          nullptr, false, perm, K_work32 != nullptr, 0,
                          tstart, nullptr, nthreads);
}
