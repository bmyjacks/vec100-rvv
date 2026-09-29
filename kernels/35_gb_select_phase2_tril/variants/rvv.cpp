#include "kernel.h"
#include <riscv_vector.h>

GrB_Info GB_select_phase2_tril_rvv(const uint64_t *Ap, const int64_t *Ai,
                                   const double *Ax, const uint64_t *Cp,
                                   int64_t *Ci, double *Cx, const uint64_t *Zp,
                                   const int64_t *slice,
                                   const uint64_t *Cp_kfirst, int64_t,
                                   bool triu, int ntasks, int) {
    for (int tid = 0; tid < ntasks; ++tid)
        for (int64_t k = slice[tid]; k <= slice[ntasks + tid]; ++k) {
            int64_t begin = k == slice[tid] ? slice[2 * ntasks + tid] : Ap[k];
            int64_t end = k == slice[ntasks + tid] ? slice[2 * ntasks + tid + 1] : Ap[k + 1];
            if (k == slice[tid] && end > int64_t(Ap[k + 1])) end = Ap[k + 1];
            int64_t out = k == slice[tid] ? Cp_kfirst[tid] : Cp[k];
            int64_t lo = triu ? begin : (begin > int64_t(Zp[k]) ? begin : Zp[k]);
            int64_t hi = triu ? (end < int64_t(Zp[k]) ? end : Zp[k]) : end;
            for (int64_t p = lo; p < hi;) {
                size_t vl = __riscv_vsetvl_e64m1(hi - p);
                __riscv_vse64_v_u64m1(reinterpret_cast<uint64_t *>(Ci + out),
                    __riscv_vle64_v_u64m1(
                        reinterpret_cast<const uint64_t *>(Ai + p), vl), vl);
                __riscv_vse64_v_f64m1(Cx + out,
                    __riscv_vle64_v_f64m1(Ax + p, vl), vl);
                p += vl;
                out += vl;
            }
        }
    return GrB_SUCCESS;
}
